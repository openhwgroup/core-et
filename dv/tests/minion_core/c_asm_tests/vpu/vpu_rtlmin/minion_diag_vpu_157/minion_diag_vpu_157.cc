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
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00000
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0000c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00018
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0001c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00020
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00024
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00028
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0002c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00030
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00034
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00038
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0003c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00040
			 0x80000000,                                                  // -zero                                       /// 00044
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c780000,                                                  // Leading 1s:                                 /// 0004c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00050
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00058
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00064
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00068
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0006c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00080
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00084
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00088
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00090
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00094
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00098
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a8
			 0x3f028f5c,                                                  // 0.51                                        /// 000ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000cc
			 0x7fc00001,                                                  // signaling NaN                               /// 000d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00000000,                                                  // zero                                        /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00100
			 0x0c400000,                                                  // Leading 1s:                                 /// 00104
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00108
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00118
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00128
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0012c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0013c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00144
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00148
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00150
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00154
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00164
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00168
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0016c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0017c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00180
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00188
			 0x0c400000,                                                  // Leading 1s:                                 /// 0018c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00190
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00194
			 0x55555555,                                                  // 4 random values                             /// 00198
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0019c
			 0xbf028f5c,                                                  // -0.51                                       /// 001a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00200
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00208
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00210
			 0x55555555,                                                  // 4 random values                             /// 00214
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00220
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00224
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00228
			 0x0e000007,                                                  // Trailing 1s:                                /// 0022c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00230
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00234
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00238
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0023c
			 0x4b000000,                                                  // 8388608.0                                   /// 00240
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0024c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00250
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00254
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00258
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0025c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00260
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00264
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0026c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00274
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0027c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00280
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00284
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00288
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00294
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00298
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b0
			 0x7fc00001,                                                  // signaling NaN                               /// 002b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x7fc00001,                                                  // signaling NaN                               /// 002fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00300
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00304
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00308
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00310
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00314
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00318
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0031c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00320
			 0x80000000,                                                  // -zero                                       /// 00324
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0xbf028f5c,                                                  // -0.51                                       /// 0032c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00330
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00338
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0033c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00340
			 0x0e000007,                                                  // Trailing 1s:                                /// 00344
			 0xbf028f5c,                                                  // -0.51                                       /// 00348
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00354
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0035c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00360
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00368
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0036c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xbf028f5c,                                                  // -0.51                                       /// 00378
			 0x0c700000,                                                  // Leading 1s:                                 /// 0037c
			 0x80000000,                                                  // -zero                                       /// 00380
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00384
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00388
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00394
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00400
			 0x0c600000,                                                  // Leading 1s:                                 /// 00404
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00408
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00410
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0041c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00420
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0042c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00430
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00438
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0043c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00440
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00444
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0044c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0045c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00460
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00464
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00468
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00470
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00474
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0047c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00480
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x0c700000,                                                  // Leading 1s:                                 /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x00011111,                                                  // 9.7958E-41                                  /// 00498
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0049c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0xffc00001,                                                  // -signaling NaN                              /// 004c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xff800000,                                                  // -inf                                        /// 004f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00504
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00508
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0050c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00510
			 0x7fc00001,                                                  // signaling NaN                               /// 00514
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00518
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0051c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00530
			 0x0e000001,                                                  // Trailing 1s:                                /// 00534
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00538
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00540
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00544
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x33333333,                                                  // 4 random values                             /// 0054c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0x0c600000,                                                  // Leading 1s:                                 /// 00558
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00560
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00564
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00568
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0056c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x3f028f5c,                                                  // 0.51                                        /// 00580
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00588
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0058c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00598
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0059c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a4
			 0x7fc00001,                                                  // signaling NaN                               /// 005a8
			 0x4b000000,                                                  // 8388608.0                                   /// 005ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 005b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005c4
			 0xff800000,                                                  // -inf                                        /// 005c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x00000000,                                                  // zero                                        /// 00604
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00610
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00624
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00628
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0xff800000,                                                  // -inf                                        /// 00634
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0063c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00640
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00644
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00654
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00658
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0065c
			 0x3f028f5c,                                                  // 0.51                                        /// 00660
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0066c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00670
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00678
			 0x80011111,                                                  // -9.7958E-41                                 /// 0067c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00688
			 0x0e000003,                                                  // Trailing 1s:                                /// 0068c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00690
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00698
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0069c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d8
			 0x00000000,                                                  // zero                                        /// 006dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00704
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00708
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0070c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00718
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00720
			 0x0c700000,                                                  // Leading 1s:                                 /// 00724
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0072c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00730
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00734
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00738
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00740
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00748
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0074c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00750
			 0x00000000,                                                  // zero                                        /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0075c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x7f800000,                                                  // inf                                         /// 0076c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x33333333,                                                  // 4 random values                             /// 00774
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00778
			 0xbf028f5c,                                                  // -0.51                                       /// 0077c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00790
			 0x0e000001,                                                  // Trailing 1s:                                /// 00794
			 0x0e000007,                                                  // Trailing 1s:                                /// 00798
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0079c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00800
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e000001,                                                  // Trailing 1s:                                /// 00808
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0080c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00810
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00818
			 0x0e000007,                                                  // Trailing 1s:                                /// 0081c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00820
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00828
			 0x33333333,                                                  // 4 random values                             /// 0082c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00830
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00840
			 0x0e000003,                                                  // Trailing 1s:                                /// 00844
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00848
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0084c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00850
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00854
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00858
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0085c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00860
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00864
			 0x55555555,                                                  // 4 random values                             /// 00868
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00870
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00878
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0087c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00880
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00888
			 0x80011111,                                                  // -9.7958E-41                                 /// 0088c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00890
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00894
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f8
			 0xcb000000,                                                  // -8388608.0                                  /// 008fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00900
			 0x0c780000,                                                  // Leading 1s:                                 /// 00904
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00908
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0090c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00910
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00914
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00918
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00924
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00928
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0092c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00934
			 0x0c400000,                                                  // Leading 1s:                                 /// 00938
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0093c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0094c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00950
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00954
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00958
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0095c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00968
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00970
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00974
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00978
			 0x0e000003,                                                  // Trailing 1s:                                /// 0097c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00980
			 0x55555555,                                                  // 4 random values                             /// 00984
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00988
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00990
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00994
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00998
			 0xffc00001,                                                  // -signaling NaN                              /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00000000,                                                  // zero                                        /// 009e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a6c
			 0x33333333,                                                  // 4 random values                             /// 00a70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aa0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ab4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ab8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00acc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ad8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00adc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00afc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0xbf028f5c,                                                  // -0.51                                       /// 00b0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b44
			 0x7f800000,                                                  // inf                                         /// 00b48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ba0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd0
			 0x33333333,                                                  // 4 random values                             /// 00bd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00be0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00be4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00be8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c14
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c28
			 0xbf028f5c,                                                  // -0.51                                       /// 00c2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ca4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ccc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cdc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cec
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d14
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d28
			 0x3f028f5c,                                                  // 0.51                                        /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d30
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db4
			 0x80000000,                                                  // -zero                                       /// 00db8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dc8
			 0x7f800000,                                                  // inf                                         /// 00dcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x4b000000,                                                  // 8388608.0                                   /// 00df8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x7f800000,                                                  // inf                                         /// 00e08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x3f028f5c,                                                  // 0.51                                        /// 00e18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e30
			 0x33333333,                                                  // 4 random values                             /// 00e34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea4
			 0x00000000,                                                  // zero                                        /// 00ea8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00eb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ebc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ec8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00edc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ef0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ef8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f2c
			 0x80000000,                                                  // -zero                                       /// 00f30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f34
			 0x55555555,                                                  // 4 random values                             /// 00f38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0xcb000000,                                                  // -8388608.0                                  /// 00f4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0xbf028f5c,                                                  // -0.51                                       /// 00f58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa4
			 0x7f800000,                                                  // inf                                         /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fbc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0xff800000,                                                  // -inf                                        /// 00fe4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ff4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ff8
			 0x00800000                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00004
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00008
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0000c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00014
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c600000,                                                  // Leading 1s:                                 /// 0001c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00020
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00011111,                                                  // 9.7958E-41                                  /// 0002c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00030
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00034
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00040
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00044
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00054
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00058
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0005c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x0e000001,                                                  // Trailing 1s:                                /// 00064
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00068
			 0x0c400000,                                                  // Leading 1s:                                 /// 0006c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00070
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0007c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00080
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00088
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00090
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00094
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00098
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0009c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a0
			 0xcb000000,                                                  // -8388608.0                                  /// 000a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x33333333,                                                  // 4 random values                             /// 000b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00100
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00104
			 0x0c400000,                                                  // Leading 1s:                                 /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00110
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00114
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00118
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xff800000,                                                  // -inf                                        /// 00124
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0012c
			 0x55555555,                                                  // 4 random values                             /// 00130
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00138
			 0x3f028f5c,                                                  // 0.51                                        /// 0013c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00140
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0014c
			 0x7f800000,                                                  // inf                                         /// 00150
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00154
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00158
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0015c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00160
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00164
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00170
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00174
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00178
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0017c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00180
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00184
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00188
			 0xffc00001,                                                  // -signaling NaN                              /// 0018c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00198
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0019c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00204
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00214
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00220
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00224
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00228
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0022c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00230
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00238
			 0x0e000007,                                                  // Trailing 1s:                                /// 0023c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00240
			 0xff800000,                                                  // -inf                                        /// 00244
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00254
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00260
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00274
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00278
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c400000,                                                  // Leading 1s:                                 /// 00288
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00290
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00298
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0e000007,                                                  // Trailing 1s:                                /// 002a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b8
			 0x7fc00001,                                                  // signaling NaN                               /// 002bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x7fc00001,                                                  // signaling NaN                               /// 002f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00300
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00304
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00308
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0030c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00310
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00314
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00318
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0031c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00320
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00334
			 0x0e000003,                                                  // Trailing 1s:                                /// 00338
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00340
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00348
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0034c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00354
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00358
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0035c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00360
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00364
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00368
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0036c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00370
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00374
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00384
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00390
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x7fc00001,                                                  // signaling NaN                               /// 003a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c0
			 0xff800000,                                                  // -inf                                        /// 003c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x80000000,                                                  // -zero                                       /// 003dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xff800000,                                                  // -inf                                        /// 00400
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00408
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0040c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00410
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0041c
			 0xcb000000,                                                  // -8388608.0                                  /// 00420
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00424
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0042c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00430
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0043c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00444
			 0x80000000,                                                  // -zero                                       /// 00448
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0044c
			 0xbf028f5c,                                                  // -0.51                                       /// 00450
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00458
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0045c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00460
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00468
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00470
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00478
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00484
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0048c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00494
			 0x0e000001,                                                  // Trailing 1s:                                /// 00498
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004c4
			 0x55555555,                                                  // 4 random values                             /// 004c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xcb000000,                                                  // -8388608.0                                  /// 00504
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0050c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00510
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0051c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00520
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00524
			 0xcb000000,                                                  // -8388608.0                                  /// 00528
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0052c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00534
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0c780000,                                                  // Leading 1s:                                 /// 0053c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x33333333,                                                  // 4 random values                             /// 00544
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0054c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00558
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00564
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00568
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00570
			 0x0e000001,                                                  // Trailing 1s:                                /// 00574
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00578
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0057c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00580
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00584
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00598
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c8
			 0xbf028f5c,                                                  // -0.51                                       /// 005cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00600
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00610
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00614
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00618
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0061c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x0e000007,                                                  // Trailing 1s:                                /// 00628
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0062c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00630
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00638
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0063c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00640
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0064c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00650
			 0x3f028f5c,                                                  // 0.51                                        /// 00654
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00660
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00664
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00670
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00674
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0067c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00680
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00688
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00690
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00698
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00011111,                                                  // 9.7958E-41                                  /// 006a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b8
			 0xbf028f5c,                                                  // -0.51                                       /// 006bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d8
			 0x7f800000,                                                  // inf                                         /// 006dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000000,                                                  // -zero                                       /// 006e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000000,                                                  // -zero                                       /// 006f4
			 0xcb000000,                                                  // -8388608.0                                  /// 006f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00704
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00708
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00710
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00714
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0071c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00720
			 0x0c400000,                                                  // Leading 1s:                                 /// 00724
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00730
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00734
			 0x0c700000,                                                  // Leading 1s:                                 /// 00738
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00740
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00744
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00750
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00754
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0075c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00760
			 0x55555555,                                                  // 4 random values                             /// 00764
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0076c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00770
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00774
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0077c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00780
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00790
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00798
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0079c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0xff800000,                                                  // -inf                                        /// 007b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00800
			 0x00011111,                                                  // 9.7958E-41                                  /// 00804
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00808
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e000001,                                                  // Trailing 1s:                                /// 00824
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00828
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0082c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00830
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00840
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x3f028f5c,                                                  // 0.51                                        /// 0084c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00850
			 0xcb000000,                                                  // -8388608.0                                  /// 00854
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00858
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0085c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00860
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00864
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0086c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00870
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00874
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0087c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00884
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00888
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00890
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008bc
			 0x3f028f5c,                                                  // 0.51                                        /// 008c0
			 0xcb000000,                                                  // -8388608.0                                  /// 008c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008dc
			 0x55555555,                                                  // 4 random values                             /// 008e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x4b000000,                                                  // 8388608.0                                   /// 0090c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00011111,                                                  // 9.7958E-41                                  /// 00918
			 0xff800000,                                                  // -inf                                        /// 0091c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00920
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00924
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00928
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0092c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00930
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00938
			 0x00011111,                                                  // 9.7958E-41                                  /// 0093c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00940
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00954
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00958
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0096c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0e000003,                                                  // Trailing 1s:                                /// 00974
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00978
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0097c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00980
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00984
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00988
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0098c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00998
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00000000,                                                  // zero                                        /// 009e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xff800000,                                                  // -inf                                        /// 009f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x00000000,                                                  // zero                                        /// 00a08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x7f800000,                                                  // inf                                         /// 00a60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0xbf028f5c,                                                  // -0.51                                       /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a74
			 0x3f028f5c,                                                  // 0.51                                        /// 00a78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x55555555,                                                  // 4 random values                             /// 00a8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab4
			 0x55555555,                                                  // 4 random values                             /// 00ab8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00abc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ac4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00acc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ad4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00adc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ae8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00af8
			 0xbf028f5c,                                                  // -0.51                                       /// 00afc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b58
			 0xff800000,                                                  // -inf                                        /// 00b5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b70
			 0xff800000,                                                  // -inf                                        /// 00b74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ba8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bd4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00be4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c44
			 0xff800000,                                                  // -inf                                        /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ca4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cbc
			 0x00000000,                                                  // zero                                        /// 00cc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ccc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0xff800000,                                                  // -inf                                        /// 00cdc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0xff800000,                                                  // -inf                                        /// 00ce4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cf4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d08
			 0xffc00001,                                                  // -signaling NaN                              /// 00d0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d74
			 0xffc00001,                                                  // -signaling NaN                              /// 00d78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dcc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00df0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00df4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dfc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e44
			 0xcb000000,                                                  // -8388608.0                                  /// 00e48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000000,                                                  // -zero                                       /// 00e8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x3f028f5c,                                                  // 0.51                                        /// 00e94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ea0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00edc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f60
			 0xff800000,                                                  // -inf                                        /// 00f64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f74
			 0x00000000,                                                  // zero                                        /// 00f78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0xff800000,                                                  // -inf                                        /// 00ff8
			 0x7f7ffffe // max norm - 2ulp                               // max norm +ve                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x3d17e9c5, /// 0x0
			 0x39303608, /// 0x4
			 0x8c6e7dc0, /// 0x8
			 0x8a7a37b0, /// 0xc
			 0x30800485, /// 0x10
			 0xd8ccf853, /// 0x14
			 0xcb53c577, /// 0x18
			 0xc10eb529, /// 0x1c
			 0x543df066, /// 0x20
			 0x6d3fe411, /// 0x24
			 0xcc95ca55, /// 0x28
			 0x1d3872c4, /// 0x2c
			 0x322f1c2b, /// 0x30
			 0xfa1220dd, /// 0x34
			 0x7e501791, /// 0x38
			 0x056be5b2, /// 0x3c
			 0x3af3094f, /// 0x40
			 0x7ec61d0d, /// 0x44
			 0xf2c9594b, /// 0x48
			 0x554529b0, /// 0x4c
			 0x759df431, /// 0x50
			 0x5d078c13, /// 0x54
			 0x37b0b7fd, /// 0x58
			 0x7c95821a, /// 0x5c
			 0x70fd3d2f, /// 0x60
			 0x52e7154f, /// 0x64
			 0x4275a3b8, /// 0x68
			 0x86204d07, /// 0x6c
			 0xa262b478, /// 0x70
			 0xdf3bb652, /// 0x74
			 0x9dedf78b, /// 0x78
			 0xe3cb9285, /// 0x7c
			 0xe7038e73, /// 0x80
			 0x4c196b48, /// 0x84
			 0x72f11ce2, /// 0x88
			 0x1b9673d6, /// 0x8c
			 0xf10bd3ef, /// 0x90
			 0xcfb2fdff, /// 0x94
			 0x65b5fe12, /// 0x98
			 0x7cec2238, /// 0x9c
			 0x4e3fab04, /// 0xa0
			 0xe797aace, /// 0xa4
			 0x75473a35, /// 0xa8
			 0xce1f3072, /// 0xac
			 0x145e533e, /// 0xb0
			 0x04defee1, /// 0xb4
			 0x5e913c1a, /// 0xb8
			 0x04db20f0, /// 0xbc
			 0xde25a431, /// 0xc0
			 0xc353e482, /// 0xc4
			 0x3d21f8a6, /// 0xc8
			 0x9bf507a0, /// 0xcc
			 0x9f24687b, /// 0xd0
			 0xbf62ab96, /// 0xd4
			 0xf9c28b1e, /// 0xd8
			 0x3f405b94, /// 0xdc
			 0x48c2300d, /// 0xe0
			 0x80b0e816, /// 0xe4
			 0x7643db2c, /// 0xe8
			 0x1653e505, /// 0xec
			 0x4e8f9796, /// 0xf0
			 0xd206679f, /// 0xf4
			 0x966bc1d2, /// 0xf8
			 0x84c4367b, /// 0xfc
			 0xe0061c4c, /// 0x100
			 0xad67bc51, /// 0x104
			 0xe518db11, /// 0x108
			 0x3826c636, /// 0x10c
			 0x8c68d4c3, /// 0x110
			 0xc444abd0, /// 0x114
			 0xee0efff1, /// 0x118
			 0x26dc0649, /// 0x11c
			 0x03664223, /// 0x120
			 0x167ed7f4, /// 0x124
			 0x5066c348, /// 0x128
			 0xff0a0c19, /// 0x12c
			 0xdef49be3, /// 0x130
			 0xd190735e, /// 0x134
			 0x8b3eca05, /// 0x138
			 0xff0fc1ef, /// 0x13c
			 0x51262884, /// 0x140
			 0x9841c699, /// 0x144
			 0x993d6e80, /// 0x148
			 0x3d318a3d, /// 0x14c
			 0x6b1ba801, /// 0x150
			 0x2a6af65d, /// 0x154
			 0xa59f0003, /// 0x158
			 0xadbf2c4b, /// 0x15c
			 0xdedc6671, /// 0x160
			 0x0937851f, /// 0x164
			 0x3624b076, /// 0x168
			 0x90b63e8e, /// 0x16c
			 0x76866e5a, /// 0x170
			 0xb0cec9ca, /// 0x174
			 0x9a269d0c, /// 0x178
			 0xac11f67f, /// 0x17c
			 0x0bce0da4, /// 0x180
			 0x84e45a63, /// 0x184
			 0xd3bf2b8e, /// 0x188
			 0x2de6be08, /// 0x18c
			 0x76c2c371, /// 0x190
			 0x4294ea6c, /// 0x194
			 0x1a62ea33, /// 0x198
			 0x690ae8f8, /// 0x19c
			 0xc300d4c8, /// 0x1a0
			 0x6610044c, /// 0x1a4
			 0x26c7890e, /// 0x1a8
			 0x1fe70eb3, /// 0x1ac
			 0x5dff5a94, /// 0x1b0
			 0xb8b7b6d3, /// 0x1b4
			 0xdb46c08f, /// 0x1b8
			 0x6c1fdc73, /// 0x1bc
			 0x9f5dc4db, /// 0x1c0
			 0xff0c7bd3, /// 0x1c4
			 0xa08d2de7, /// 0x1c8
			 0x3112fca8, /// 0x1cc
			 0x99fb8984, /// 0x1d0
			 0xbc68420e, /// 0x1d4
			 0x3a24ea01, /// 0x1d8
			 0x4fb2d1e9, /// 0x1dc
			 0x19fc9455, /// 0x1e0
			 0x8be2d57b, /// 0x1e4
			 0xa2abb65d, /// 0x1e8
			 0x41f31a7c, /// 0x1ec
			 0x9660b2e7, /// 0x1f0
			 0x36c1e046, /// 0x1f4
			 0x70f0bf2c, /// 0x1f8
			 0xecaebe6f, /// 0x1fc
			 0x48748ac5, /// 0x200
			 0x5fb26122, /// 0x204
			 0x70ff09fa, /// 0x208
			 0x119f868c, /// 0x20c
			 0xcb6eb722, /// 0x210
			 0x072382d4, /// 0x214
			 0x385392c4, /// 0x218
			 0x8e7d5e5b, /// 0x21c
			 0x6e6818c3, /// 0x220
			 0xc6c61337, /// 0x224
			 0x8906c7cb, /// 0x228
			 0x240aab19, /// 0x22c
			 0x5ffc02d8, /// 0x230
			 0xa19b8d9b, /// 0x234
			 0xe5773237, /// 0x238
			 0x1828fca3, /// 0x23c
			 0x1dfa9126, /// 0x240
			 0x0a1ba39e, /// 0x244
			 0x1435bc8b, /// 0x248
			 0x109c49f7, /// 0x24c
			 0x80dcabea, /// 0x250
			 0x83623a32, /// 0x254
			 0xb5210649, /// 0x258
			 0xc8a4ca5e, /// 0x25c
			 0xc27db495, /// 0x260
			 0x71d87af2, /// 0x264
			 0x4da4397a, /// 0x268
			 0x5d6226a6, /// 0x26c
			 0x21b2dfea, /// 0x270
			 0xef097d92, /// 0x274
			 0xb04cd2f2, /// 0x278
			 0x8ebca60c, /// 0x27c
			 0xbb60b6a0, /// 0x280
			 0xbe1cb405, /// 0x284
			 0xa6ccbed8, /// 0x288
			 0x8de9ac1f, /// 0x28c
			 0xce42d4a1, /// 0x290
			 0x9ecb6207, /// 0x294
			 0x2f6bcf1b, /// 0x298
			 0xbc052a49, /// 0x29c
			 0xe46653cd, /// 0x2a0
			 0xed9c6d72, /// 0x2a4
			 0x7acb5293, /// 0x2a8
			 0x778d2140, /// 0x2ac
			 0x75fe04e3, /// 0x2b0
			 0xc10f4dcc, /// 0x2b4
			 0xebfc63f3, /// 0x2b8
			 0x481782ae, /// 0x2bc
			 0xe0965571, /// 0x2c0
			 0x41772f15, /// 0x2c4
			 0x37ef718e, /// 0x2c8
			 0x7b7308ea, /// 0x2cc
			 0xe0959340, /// 0x2d0
			 0xee1a0a35, /// 0x2d4
			 0xb66b8639, /// 0x2d8
			 0x7b10dacf, /// 0x2dc
			 0x601475ee, /// 0x2e0
			 0x38dba3b0, /// 0x2e4
			 0xd1324ff3, /// 0x2e8
			 0x58675476, /// 0x2ec
			 0x5dceab8c, /// 0x2f0
			 0x0a7e19cd, /// 0x2f4
			 0x1b8092ad, /// 0x2f8
			 0xc5579c8c, /// 0x2fc
			 0xa98e46ec, /// 0x300
			 0x28c1dfda, /// 0x304
			 0xda5f0889, /// 0x308
			 0xa256651b, /// 0x30c
			 0x16cd8a7c, /// 0x310
			 0xd2b73f38, /// 0x314
			 0x09c32224, /// 0x318
			 0x1ca9cc4c, /// 0x31c
			 0xf4567d84, /// 0x320
			 0x0bbb4b0e, /// 0x324
			 0xc22b3fb2, /// 0x328
			 0x4958aa35, /// 0x32c
			 0xe6ae978e, /// 0x330
			 0xaf9b4c2d, /// 0x334
			 0x7eb40c5c, /// 0x338
			 0xa45095a3, /// 0x33c
			 0x247461c3, /// 0x340
			 0x56dadc26, /// 0x344
			 0xb6b41512, /// 0x348
			 0xb0c2d861, /// 0x34c
			 0xd386cae8, /// 0x350
			 0x6a0b0198, /// 0x354
			 0x9bf2480a, /// 0x358
			 0x810d6be2, /// 0x35c
			 0x3a47bea4, /// 0x360
			 0x9ed23eec, /// 0x364
			 0xc87a6901, /// 0x368
			 0x5cda7a89, /// 0x36c
			 0x1275be0c, /// 0x370
			 0x7b822567, /// 0x374
			 0x2448e59a, /// 0x378
			 0x58700e22, /// 0x37c
			 0xb6fbaea0, /// 0x380
			 0x5ebe1a89, /// 0x384
			 0xe7d472d1, /// 0x388
			 0x94327837, /// 0x38c
			 0xfb66b7f8, /// 0x390
			 0x41fb1129, /// 0x394
			 0x585c1d0a, /// 0x398
			 0xf00595e3, /// 0x39c
			 0xe8aa31fc, /// 0x3a0
			 0xb3e981ff, /// 0x3a4
			 0x350b8d99, /// 0x3a8
			 0x0d9c4bf2, /// 0x3ac
			 0x7382e8ce, /// 0x3b0
			 0xb1eed042, /// 0x3b4
			 0x9f9b8f2c, /// 0x3b8
			 0x8c546025, /// 0x3bc
			 0xda46e6c4, /// 0x3c0
			 0x83698ccd, /// 0x3c4
			 0x3ca83caf, /// 0x3c8
			 0x3a12eb8b, /// 0x3cc
			 0xee06cc3b, /// 0x3d0
			 0x0f463513, /// 0x3d4
			 0x28c4e122, /// 0x3d8
			 0xe1d9befd, /// 0x3dc
			 0x9f97d884, /// 0x3e0
			 0x8b074685, /// 0x3e4
			 0xec0ca07f, /// 0x3e8
			 0xb69b8845, /// 0x3ec
			 0x503f2a59, /// 0x3f0
			 0xea5bfb7a, /// 0x3f4
			 0xd185c925, /// 0x3f8
			 0x389857fe, /// 0x3fc
			 0x454b7a3f, /// 0x400
			 0x3838ea0a, /// 0x404
			 0x638b9754, /// 0x408
			 0x3fe1fd1f, /// 0x40c
			 0xf284cae2, /// 0x410
			 0x34472082, /// 0x414
			 0x045c45af, /// 0x418
			 0x02d8cb70, /// 0x41c
			 0xda82df64, /// 0x420
			 0x330cd9f3, /// 0x424
			 0xd75905e6, /// 0x428
			 0xf6b05624, /// 0x42c
			 0x978f2fdd, /// 0x430
			 0x574dc89a, /// 0x434
			 0xab2ebe64, /// 0x438
			 0x566f694e, /// 0x43c
			 0x45e5bc6a, /// 0x440
			 0xd2cefb39, /// 0x444
			 0xcd38be36, /// 0x448
			 0x5fbdd96d, /// 0x44c
			 0x575378f8, /// 0x450
			 0xa29532b6, /// 0x454
			 0x8a2de26e, /// 0x458
			 0x0c73a9a2, /// 0x45c
			 0xac9ff4ac, /// 0x460
			 0x1e6eb2f6, /// 0x464
			 0xaca3df24, /// 0x468
			 0x5c41f0b6, /// 0x46c
			 0x77402e6d, /// 0x470
			 0x3369d3e2, /// 0x474
			 0x5aced9e3, /// 0x478
			 0x0653347b, /// 0x47c
			 0x854cfcb0, /// 0x480
			 0x27052ab2, /// 0x484
			 0xf0fc9d56, /// 0x488
			 0x05ac7f07, /// 0x48c
			 0xb62952df, /// 0x490
			 0xfe35a3a8, /// 0x494
			 0xc6357362, /// 0x498
			 0x1098bc3f, /// 0x49c
			 0x9f328ffd, /// 0x4a0
			 0xf25b7ea1, /// 0x4a4
			 0xf205dae9, /// 0x4a8
			 0x640fbeda, /// 0x4ac
			 0x527ba6d9, /// 0x4b0
			 0xfc10be28, /// 0x4b4
			 0xbd699642, /// 0x4b8
			 0xf910fb07, /// 0x4bc
			 0xdfa6389a, /// 0x4c0
			 0x2dbb4ce1, /// 0x4c4
			 0xd0b85e9b, /// 0x4c8
			 0x057ef659, /// 0x4cc
			 0xde213293, /// 0x4d0
			 0x70bda663, /// 0x4d4
			 0xa7812b53, /// 0x4d8
			 0xd611b0c6, /// 0x4dc
			 0x6817cba3, /// 0x4e0
			 0xe3a8ccba, /// 0x4e4
			 0x1c464f3d, /// 0x4e8
			 0xf5d9103e, /// 0x4ec
			 0x2738bd24, /// 0x4f0
			 0x5fd53ae5, /// 0x4f4
			 0x124ff680, /// 0x4f8
			 0xf742aef4, /// 0x4fc
			 0x87d13b95, /// 0x500
			 0x83264b23, /// 0x504
			 0x53d92770, /// 0x508
			 0x7bcf6a11, /// 0x50c
			 0x9c3b3bc0, /// 0x510
			 0xe17822ec, /// 0x514
			 0x7aa2273d, /// 0x518
			 0xe831d66b, /// 0x51c
			 0x65830fc8, /// 0x520
			 0xed5a501d, /// 0x524
			 0xf6ebb3b1, /// 0x528
			 0xbacae732, /// 0x52c
			 0xa7854856, /// 0x530
			 0xdebbb4a3, /// 0x534
			 0x146411cc, /// 0x538
			 0x9a4a11a7, /// 0x53c
			 0x34f062c3, /// 0x540
			 0x911754bd, /// 0x544
			 0xfd9c6f34, /// 0x548
			 0x29638d60, /// 0x54c
			 0x7a649ce0, /// 0x550
			 0x8385fe28, /// 0x554
			 0x24ebde3b, /// 0x558
			 0xe6ed2ecc, /// 0x55c
			 0x75d62dd2, /// 0x560
			 0xf9add0a2, /// 0x564
			 0xf1c23b7f, /// 0x568
			 0xf4998489, /// 0x56c
			 0x57b1bd7b, /// 0x570
			 0x449c4b92, /// 0x574
			 0xc7c682a0, /// 0x578
			 0x32f960d9, /// 0x57c
			 0x21d9c9cd, /// 0x580
			 0x4ea89358, /// 0x584
			 0x00fac3fa, /// 0x588
			 0x0c62a5d1, /// 0x58c
			 0xac993426, /// 0x590
			 0x12b5a2ff, /// 0x594
			 0x583e0f71, /// 0x598
			 0x2b47ae8b, /// 0x59c
			 0xae764e5f, /// 0x5a0
			 0xcb1a283b, /// 0x5a4
			 0xdeb49706, /// 0x5a8
			 0xb8e40dbb, /// 0x5ac
			 0xce3c2aac, /// 0x5b0
			 0x70583b84, /// 0x5b4
			 0x87760365, /// 0x5b8
			 0x2d89a864, /// 0x5bc
			 0x1269ccf1, /// 0x5c0
			 0x6b280845, /// 0x5c4
			 0xc9fe91e0, /// 0x5c8
			 0x5d474514, /// 0x5cc
			 0x0b164dff, /// 0x5d0
			 0x415fa008, /// 0x5d4
			 0xebb0b20a, /// 0x5d8
			 0x0b1b5687, /// 0x5dc
			 0xc6eb6e4b, /// 0x5e0
			 0xd5a19ab4, /// 0x5e4
			 0x41610a97, /// 0x5e8
			 0x5402f1c8, /// 0x5ec
			 0xcc147578, /// 0x5f0
			 0x5ab357ca, /// 0x5f4
			 0xa29eb23b, /// 0x5f8
			 0xf3fcf079, /// 0x5fc
			 0x82f1a852, /// 0x600
			 0x8308e796, /// 0x604
			 0x1b98c559, /// 0x608
			 0x29b74bef, /// 0x60c
			 0xe77fce2b, /// 0x610
			 0x678d5a6c, /// 0x614
			 0x1e072ba2, /// 0x618
			 0xc9f1f43e, /// 0x61c
			 0x779cf628, /// 0x620
			 0x45c5adb6, /// 0x624
			 0xd2f776dc, /// 0x628
			 0x88b518bf, /// 0x62c
			 0xd1907a40, /// 0x630
			 0xd7c51050, /// 0x634
			 0xc1378199, /// 0x638
			 0x1ce2fc50, /// 0x63c
			 0x7cceb368, /// 0x640
			 0xca06bba5, /// 0x644
			 0xf076dfc3, /// 0x648
			 0x06676e62, /// 0x64c
			 0x7c02c30b, /// 0x650
			 0x21fb846a, /// 0x654
			 0xd234b739, /// 0x658
			 0x62cce144, /// 0x65c
			 0x6c33d5b5, /// 0x660
			 0x02e85164, /// 0x664
			 0x0ccf0b74, /// 0x668
			 0xec70bd1b, /// 0x66c
			 0x08918d43, /// 0x670
			 0x53553a9c, /// 0x674
			 0xbe500cb6, /// 0x678
			 0x3a5eb2de, /// 0x67c
			 0x819f6777, /// 0x680
			 0x4143914d, /// 0x684
			 0xbac332a2, /// 0x688
			 0xddb24585, /// 0x68c
			 0xd0e6ce79, /// 0x690
			 0x8029d521, /// 0x694
			 0x32085750, /// 0x698
			 0x717b19ab, /// 0x69c
			 0xcff365b6, /// 0x6a0
			 0xb2028bd0, /// 0x6a4
			 0xe46b43e7, /// 0x6a8
			 0xec9d5e47, /// 0x6ac
			 0x58072959, /// 0x6b0
			 0x3be60d20, /// 0x6b4
			 0x568d2106, /// 0x6b8
			 0xd6fa2c21, /// 0x6bc
			 0xea055a7a, /// 0x6c0
			 0xbb365778, /// 0x6c4
			 0x25925f8d, /// 0x6c8
			 0xfa68ab05, /// 0x6cc
			 0x4b6d9ec7, /// 0x6d0
			 0x55fb94db, /// 0x6d4
			 0x37fdfc9f, /// 0x6d8
			 0x1f014a7b, /// 0x6dc
			 0xaea79086, /// 0x6e0
			 0xc104331a, /// 0x6e4
			 0x16217bcc, /// 0x6e8
			 0x16e76e7f, /// 0x6ec
			 0xd922c4d0, /// 0x6f0
			 0x8537cc32, /// 0x6f4
			 0x478a34f1, /// 0x6f8
			 0x3e55d9f2, /// 0x6fc
			 0xf930e13c, /// 0x700
			 0xe4d384f7, /// 0x704
			 0x7ea57260, /// 0x708
			 0xf86083f3, /// 0x70c
			 0x052559f4, /// 0x710
			 0xf4a3a7dd, /// 0x714
			 0x5a63bb7d, /// 0x718
			 0x3f5dd42f, /// 0x71c
			 0xa2978065, /// 0x720
			 0x3aed87e4, /// 0x724
			 0x7ee064d0, /// 0x728
			 0xc07a75ea, /// 0x72c
			 0x2977e0ea, /// 0x730
			 0x549873b1, /// 0x734
			 0x32a280d2, /// 0x738
			 0x5bd78419, /// 0x73c
			 0xc1f2bdb3, /// 0x740
			 0xfd2296b5, /// 0x744
			 0xef6b486c, /// 0x748
			 0x55d58433, /// 0x74c
			 0x27ce9ea7, /// 0x750
			 0xf195db22, /// 0x754
			 0x2e582b79, /// 0x758
			 0xd1195238, /// 0x75c
			 0x16cd1420, /// 0x760
			 0x41e6853a, /// 0x764
			 0x788cb846, /// 0x768
			 0xda37c1f4, /// 0x76c
			 0x42c38333, /// 0x770
			 0x45de4dd0, /// 0x774
			 0x9a247fdb, /// 0x778
			 0x533bd1ce, /// 0x77c
			 0x8fecb519, /// 0x780
			 0x0e48ee74, /// 0x784
			 0x8fe0a806, /// 0x788
			 0x35256cc4, /// 0x78c
			 0xce428c31, /// 0x790
			 0xfde6d5eb, /// 0x794
			 0x5bd47f52, /// 0x798
			 0xeae14178, /// 0x79c
			 0x0fed3d4e, /// 0x7a0
			 0x65e4d643, /// 0x7a4
			 0x97c42a8a, /// 0x7a8
			 0xe5c6c655, /// 0x7ac
			 0x275c9308, /// 0x7b0
			 0xce14e986, /// 0x7b4
			 0x1f8c0f93, /// 0x7b8
			 0x8d9af153, /// 0x7bc
			 0x15ebdb94, /// 0x7c0
			 0xc956d77f, /// 0x7c4
			 0x43f312ab, /// 0x7c8
			 0x428e5db1, /// 0x7cc
			 0x7c5688c2, /// 0x7d0
			 0x07474bce, /// 0x7d4
			 0x3bf13b59, /// 0x7d8
			 0xd7748bf5, /// 0x7dc
			 0xaa392267, /// 0x7e0
			 0x1e9b78bd, /// 0x7e4
			 0x056e1ea6, /// 0x7e8
			 0xc6bf11ef, /// 0x7ec
			 0x2c8238f4, /// 0x7f0
			 0x07ec51ec, /// 0x7f4
			 0xa161ed3d, /// 0x7f8
			 0xdb046840, /// 0x7fc
			 0x8b32507d, /// 0x800
			 0x09ac16f2, /// 0x804
			 0xc8edf96a, /// 0x808
			 0xd2b46b68, /// 0x80c
			 0x35d575ee, /// 0x810
			 0x0ec39486, /// 0x814
			 0xfb4e1195, /// 0x818
			 0x871ed2ba, /// 0x81c
			 0x349a7922, /// 0x820
			 0x9a52fc53, /// 0x824
			 0x747376e1, /// 0x828
			 0xee95505b, /// 0x82c
			 0x37f846d3, /// 0x830
			 0x9e707450, /// 0x834
			 0xf56ae060, /// 0x838
			 0x00687c87, /// 0x83c
			 0x2f403998, /// 0x840
			 0xcd227879, /// 0x844
			 0x1c511f61, /// 0x848
			 0x91a33209, /// 0x84c
			 0x52fb5296, /// 0x850
			 0x4878b82c, /// 0x854
			 0x40256e0d, /// 0x858
			 0x78249013, /// 0x85c
			 0xa1bc3d80, /// 0x860
			 0x9066c43c, /// 0x864
			 0x2341f2a4, /// 0x868
			 0xd8420997, /// 0x86c
			 0xaa180548, /// 0x870
			 0x45c3e0a6, /// 0x874
			 0x1bc1e63b, /// 0x878
			 0xd3460d8b, /// 0x87c
			 0xa4bee4af, /// 0x880
			 0x873c0e2b, /// 0x884
			 0x5ef9af03, /// 0x888
			 0xb7de9798, /// 0x88c
			 0x1e0724b3, /// 0x890
			 0x3288a1d2, /// 0x894
			 0x90e5d282, /// 0x898
			 0x37a00c46, /// 0x89c
			 0xb35631b9, /// 0x8a0
			 0x7f3211a7, /// 0x8a4
			 0x716fd444, /// 0x8a8
			 0x3d15285c, /// 0x8ac
			 0xef3534c1, /// 0x8b0
			 0xb56026d9, /// 0x8b4
			 0x43a0de14, /// 0x8b8
			 0x9d30cd3c, /// 0x8bc
			 0xe3f8258d, /// 0x8c0
			 0x71ffc525, /// 0x8c4
			 0x8664a206, /// 0x8c8
			 0x55399a38, /// 0x8cc
			 0xd950a876, /// 0x8d0
			 0xf8c96b1c, /// 0x8d4
			 0x4e406816, /// 0x8d8
			 0x9758bb5e, /// 0x8dc
			 0x9589004f, /// 0x8e0
			 0xee4aa855, /// 0x8e4
			 0xd7a3a562, /// 0x8e8
			 0x87d7d020, /// 0x8ec
			 0x24eb28bb, /// 0x8f0
			 0xd3ac0453, /// 0x8f4
			 0xa23de3c4, /// 0x8f8
			 0xc35fc2ea, /// 0x8fc
			 0x795b1bac, /// 0x900
			 0x740d395f, /// 0x904
			 0x63eaa9ae, /// 0x908
			 0xb60fc132, /// 0x90c
			 0x61618ece, /// 0x910
			 0xcfd74543, /// 0x914
			 0xc2f70bd4, /// 0x918
			 0xdc3adb42, /// 0x91c
			 0x9d289162, /// 0x920
			 0x10c11682, /// 0x924
			 0x1b7ca24b, /// 0x928
			 0x9d40b333, /// 0x92c
			 0x612957fe, /// 0x930
			 0x05ca19c3, /// 0x934
			 0xf72d54c0, /// 0x938
			 0x9957a986, /// 0x93c
			 0x5078f092, /// 0x940
			 0xa4670682, /// 0x944
			 0x5e915cbd, /// 0x948
			 0x20943d4a, /// 0x94c
			 0x8fff6c45, /// 0x950
			 0x5b0d572c, /// 0x954
			 0x5d790d20, /// 0x958
			 0x6d945a19, /// 0x95c
			 0x366e03cb, /// 0x960
			 0x2c58f3ac, /// 0x964
			 0xb950622a, /// 0x968
			 0x59684e85, /// 0x96c
			 0x539eccb3, /// 0x970
			 0x14c6ac3a, /// 0x974
			 0xf1a8920c, /// 0x978
			 0x41882212, /// 0x97c
			 0xe4eeb586, /// 0x980
			 0x65f50e34, /// 0x984
			 0xc565e080, /// 0x988
			 0xdcd2ec01, /// 0x98c
			 0xe17ca0aa, /// 0x990
			 0x8cfc68a8, /// 0x994
			 0x489d4066, /// 0x998
			 0x5f270843, /// 0x99c
			 0xb55cb92a, /// 0x9a0
			 0xf041475f, /// 0x9a4
			 0x28a0ff30, /// 0x9a8
			 0xc0f986d1, /// 0x9ac
			 0xcc957a3c, /// 0x9b0
			 0x1d2472d0, /// 0x9b4
			 0x380acaa6, /// 0x9b8
			 0x90947273, /// 0x9bc
			 0x893b3406, /// 0x9c0
			 0x43b12a6c, /// 0x9c4
			 0xc879fac3, /// 0x9c8
			 0xef9458c7, /// 0x9cc
			 0x88ef6e81, /// 0x9d0
			 0x03db3887, /// 0x9d4
			 0x65a3de11, /// 0x9d8
			 0x690e26c5, /// 0x9dc
			 0x3265491f, /// 0x9e0
			 0x34de93fd, /// 0x9e4
			 0x9d045d17, /// 0x9e8
			 0x55957389, /// 0x9ec
			 0x7cf959e1, /// 0x9f0
			 0xab469c15, /// 0x9f4
			 0x5604a644, /// 0x9f8
			 0x040b062f, /// 0x9fc
			 0x67247b00, /// 0xa00
			 0xb005a52d, /// 0xa04
			 0x533d5f7f, /// 0xa08
			 0x614e54e6, /// 0xa0c
			 0xb4b01c8c, /// 0xa10
			 0x50d5cd58, /// 0xa14
			 0x82605893, /// 0xa18
			 0x7493b331, /// 0xa1c
			 0x9702ef3e, /// 0xa20
			 0xe7eefe2d, /// 0xa24
			 0x9a61fd09, /// 0xa28
			 0x7411290c, /// 0xa2c
			 0xf0c41ae4, /// 0xa30
			 0xbef9b3db, /// 0xa34
			 0xf7a9fea7, /// 0xa38
			 0xcb2ffa95, /// 0xa3c
			 0x548973f6, /// 0xa40
			 0x9c7a963c, /// 0xa44
			 0xcb35db36, /// 0xa48
			 0x8ebaeaac, /// 0xa4c
			 0x7b5d310e, /// 0xa50
			 0x2b2e2c1e, /// 0xa54
			 0xccf9fe2b, /// 0xa58
			 0x59067083, /// 0xa5c
			 0xe1b2cad5, /// 0xa60
			 0x77e98896, /// 0xa64
			 0x73856a77, /// 0xa68
			 0xbbedd29d, /// 0xa6c
			 0x7c58c18a, /// 0xa70
			 0x61534a42, /// 0xa74
			 0xdca3c6a3, /// 0xa78
			 0xe19577ea, /// 0xa7c
			 0x29c5fae6, /// 0xa80
			 0x0e62990e, /// 0xa84
			 0x91814786, /// 0xa88
			 0xf388aadf, /// 0xa8c
			 0x96ed0ca6, /// 0xa90
			 0x62b05fba, /// 0xa94
			 0xeb154d0d, /// 0xa98
			 0x12c23b0c, /// 0xa9c
			 0x01a8b221, /// 0xaa0
			 0x4c1b2e35, /// 0xaa4
			 0xcfe2cebd, /// 0xaa8
			 0xbf348e7a, /// 0xaac
			 0x172daa4d, /// 0xab0
			 0x2dd9f9c9, /// 0xab4
			 0xa6aaddd1, /// 0xab8
			 0x4c4c291e, /// 0xabc
			 0x1b1e33ee, /// 0xac0
			 0x8f6fc3a2, /// 0xac4
			 0x99821b67, /// 0xac8
			 0xf4041e3b, /// 0xacc
			 0x77eb1468, /// 0xad0
			 0x826c5f99, /// 0xad4
			 0x46da77bd, /// 0xad8
			 0x07da1d36, /// 0xadc
			 0x40503a19, /// 0xae0
			 0x18909bb5, /// 0xae4
			 0x3186080a, /// 0xae8
			 0xc3313b48, /// 0xaec
			 0x5aa80f21, /// 0xaf0
			 0x88d921d7, /// 0xaf4
			 0xffc2b52e, /// 0xaf8
			 0x183b983c, /// 0xafc
			 0x6133c9b3, /// 0xb00
			 0xacd11924, /// 0xb04
			 0x04b6032d, /// 0xb08
			 0x5d85d811, /// 0xb0c
			 0x058e906a, /// 0xb10
			 0x187a4229, /// 0xb14
			 0x4f38ab48, /// 0xb18
			 0xb84a4f98, /// 0xb1c
			 0xdaa308be, /// 0xb20
			 0x7f0aa9ad, /// 0xb24
			 0x2d0ee579, /// 0xb28
			 0x49032a3f, /// 0xb2c
			 0x627170f2, /// 0xb30
			 0x49879442, /// 0xb34
			 0x42042a7b, /// 0xb38
			 0x9b914893, /// 0xb3c
			 0x94de73b2, /// 0xb40
			 0x8a733c47, /// 0xb44
			 0x3976b143, /// 0xb48
			 0xc00c9904, /// 0xb4c
			 0x146dc054, /// 0xb50
			 0x7dc6c1d2, /// 0xb54
			 0x8571ed88, /// 0xb58
			 0x7d98ac68, /// 0xb5c
			 0xcd1de86e, /// 0xb60
			 0x5a26b02e, /// 0xb64
			 0x039ed3be, /// 0xb68
			 0xda45bdf1, /// 0xb6c
			 0xb9c77afb, /// 0xb70
			 0x201bb038, /// 0xb74
			 0x281c1a7a, /// 0xb78
			 0x15844dcd, /// 0xb7c
			 0x861676c6, /// 0xb80
			 0x91de600a, /// 0xb84
			 0x60abd25f, /// 0xb88
			 0x712d6f65, /// 0xb8c
			 0xa61df8e5, /// 0xb90
			 0x8b43980d, /// 0xb94
			 0x6fac1cf0, /// 0xb98
			 0x23fc3f2c, /// 0xb9c
			 0x117635f6, /// 0xba0
			 0x3cbf698f, /// 0xba4
			 0x30f723c5, /// 0xba8
			 0x35625187, /// 0xbac
			 0x599cc891, /// 0xbb0
			 0x2b2608ec, /// 0xbb4
			 0x82fac570, /// 0xbb8
			 0xee4fb870, /// 0xbbc
			 0xe3484001, /// 0xbc0
			 0xef22f65f, /// 0xbc4
			 0xc449379b, /// 0xbc8
			 0x607497ab, /// 0xbcc
			 0x0e4194b2, /// 0xbd0
			 0x5db4e9f6, /// 0xbd4
			 0x2976de70, /// 0xbd8
			 0x53827e10, /// 0xbdc
			 0x97d1e61a, /// 0xbe0
			 0x81e23e0a, /// 0xbe4
			 0xf44136d2, /// 0xbe8
			 0xcfb7d475, /// 0xbec
			 0xd899bd8d, /// 0xbf0
			 0x530ea191, /// 0xbf4
			 0xd1d6d226, /// 0xbf8
			 0xe3b04e98, /// 0xbfc
			 0xddf14198, /// 0xc00
			 0x18ea9dfd, /// 0xc04
			 0xf48109f4, /// 0xc08
			 0x1373919a, /// 0xc0c
			 0x7502f776, /// 0xc10
			 0x6f860c60, /// 0xc14
			 0x682a9556, /// 0xc18
			 0x1c3e6eb9, /// 0xc1c
			 0x4e95c699, /// 0xc20
			 0x87dd5428, /// 0xc24
			 0x0bbe24b8, /// 0xc28
			 0x2568d509, /// 0xc2c
			 0x709999dd, /// 0xc30
			 0x1f1b760f, /// 0xc34
			 0xd9b6bc59, /// 0xc38
			 0x0be575e4, /// 0xc3c
			 0x25344961, /// 0xc40
			 0xfb3e7224, /// 0xc44
			 0x2541e3ae, /// 0xc48
			 0x901de33c, /// 0xc4c
			 0x5f1a332d, /// 0xc50
			 0xf2b2ddf8, /// 0xc54
			 0x21719b86, /// 0xc58
			 0xc14b1996, /// 0xc5c
			 0xa6cda18a, /// 0xc60
			 0x7c9199c6, /// 0xc64
			 0x666e8b13, /// 0xc68
			 0x61d730f0, /// 0xc6c
			 0x3d2da03e, /// 0xc70
			 0x1742449c, /// 0xc74
			 0x3980fc1b, /// 0xc78
			 0xb010eeb2, /// 0xc7c
			 0xe3f4ffcb, /// 0xc80
			 0xbf09d7d2, /// 0xc84
			 0xb30b7a00, /// 0xc88
			 0x55a2b5f3, /// 0xc8c
			 0xfa1b90fe, /// 0xc90
			 0x0201c70c, /// 0xc94
			 0xafa44d50, /// 0xc98
			 0xffca0163, /// 0xc9c
			 0x8d659d08, /// 0xca0
			 0x706233ee, /// 0xca4
			 0x718a5c65, /// 0xca8
			 0x0d209145, /// 0xcac
			 0x0c456145, /// 0xcb0
			 0x2ff25713, /// 0xcb4
			 0x2c39fa08, /// 0xcb8
			 0x038c3bde, /// 0xcbc
			 0xd974805b, /// 0xcc0
			 0xe53e8aea, /// 0xcc4
			 0x62ac5018, /// 0xcc8
			 0x28eebf59, /// 0xccc
			 0xe2b9b209, /// 0xcd0
			 0x02cce954, /// 0xcd4
			 0x59d7af83, /// 0xcd8
			 0xd47ae9bc, /// 0xcdc
			 0x985653e4, /// 0xce0
			 0x716d83af, /// 0xce4
			 0x7fcae851, /// 0xce8
			 0xb72934bd, /// 0xcec
			 0x6acb6a82, /// 0xcf0
			 0x5aa93a83, /// 0xcf4
			 0xa82d900b, /// 0xcf8
			 0xf9ac6b49, /// 0xcfc
			 0x2fbaae78, /// 0xd00
			 0x9e79543e, /// 0xd04
			 0x1e078fe8, /// 0xd08
			 0x17a5273f, /// 0xd0c
			 0xc7c2b6a9, /// 0xd10
			 0xab373012, /// 0xd14
			 0x69a074a2, /// 0xd18
			 0xd63e1bc2, /// 0xd1c
			 0x3d9d0fde, /// 0xd20
			 0x68386bd8, /// 0xd24
			 0xcf1da48c, /// 0xd28
			 0xf390a449, /// 0xd2c
			 0x34f246e9, /// 0xd30
			 0x54de61a1, /// 0xd34
			 0xb4dcb3d7, /// 0xd38
			 0x37d9cb1c, /// 0xd3c
			 0x9acdba92, /// 0xd40
			 0xf81d8522, /// 0xd44
			 0x353c7956, /// 0xd48
			 0x880701d4, /// 0xd4c
			 0xa9a8fcf4, /// 0xd50
			 0x49261617, /// 0xd54
			 0xf5de8a0a, /// 0xd58
			 0xf608c929, /// 0xd5c
			 0xc8b4fa0f, /// 0xd60
			 0x2b8a4a7e, /// 0xd64
			 0x55a5c1e8, /// 0xd68
			 0xa651592d, /// 0xd6c
			 0x7b371478, /// 0xd70
			 0x055ecf65, /// 0xd74
			 0x6e9f2a42, /// 0xd78
			 0xbabcc513, /// 0xd7c
			 0x4b02b677, /// 0xd80
			 0xcf5dbae2, /// 0xd84
			 0x4b06db8e, /// 0xd88
			 0x43010e9d, /// 0xd8c
			 0x700bdc37, /// 0xd90
			 0xccde8a3a, /// 0xd94
			 0x2ca53bab, /// 0xd98
			 0xa8011b24, /// 0xd9c
			 0xaaeaf865, /// 0xda0
			 0x142eb1cc, /// 0xda4
			 0x671957f1, /// 0xda8
			 0xa85a63d4, /// 0xdac
			 0xb03815e1, /// 0xdb0
			 0xcaef8635, /// 0xdb4
			 0x08cb4e9d, /// 0xdb8
			 0x1796fcb9, /// 0xdbc
			 0x4f9d12d9, /// 0xdc0
			 0x9744c688, /// 0xdc4
			 0xc46a1d72, /// 0xdc8
			 0x2878e13b, /// 0xdcc
			 0xb2bdd53f, /// 0xdd0
			 0xf0a26a48, /// 0xdd4
			 0x612b3931, /// 0xdd8
			 0x6aa007df, /// 0xddc
			 0x975ecc53, /// 0xde0
			 0x01dbe458, /// 0xde4
			 0xcc4788db, /// 0xde8
			 0x35bee708, /// 0xdec
			 0xb9dbda47, /// 0xdf0
			 0xf7c10507, /// 0xdf4
			 0x86256ca5, /// 0xdf8
			 0xc75dad91, /// 0xdfc
			 0x37bf975d, /// 0xe00
			 0x3e609dad, /// 0xe04
			 0x1289afa1, /// 0xe08
			 0x9d4efc6e, /// 0xe0c
			 0xb2d0d906, /// 0xe10
			 0x66f8ed67, /// 0xe14
			 0x153fb98a, /// 0xe18
			 0xcafdb514, /// 0xe1c
			 0x432e32a7, /// 0xe20
			 0xd8f0f326, /// 0xe24
			 0xf0b99ac3, /// 0xe28
			 0x2e37722e, /// 0xe2c
			 0x72dab85a, /// 0xe30
			 0xd8f0c940, /// 0xe34
			 0x20e667f3, /// 0xe38
			 0xbd353c70, /// 0xe3c
			 0x47758f78, /// 0xe40
			 0x33665c61, /// 0xe44
			 0x7ab26cae, /// 0xe48
			 0xc17c648a, /// 0xe4c
			 0x080fc27e, /// 0xe50
			 0xb08c5aa4, /// 0xe54
			 0x023439a7, /// 0xe58
			 0xbd7bd5f0, /// 0xe5c
			 0x3c9d23c1, /// 0xe60
			 0x36cae31a, /// 0xe64
			 0x2708a6e9, /// 0xe68
			 0x4b7e4460, /// 0xe6c
			 0xcd6ba5f2, /// 0xe70
			 0xc2543fdb, /// 0xe74
			 0x2658f1d8, /// 0xe78
			 0xeb9f9c3a, /// 0xe7c
			 0x61dfcc70, /// 0xe80
			 0x25c6c84c, /// 0xe84
			 0x6fe531cc, /// 0xe88
			 0x87b47cd2, /// 0xe8c
			 0xfbaad4f4, /// 0xe90
			 0x57870384, /// 0xe94
			 0x3e8dd350, /// 0xe98
			 0xe5ce2cd9, /// 0xe9c
			 0xb425ea42, /// 0xea0
			 0xd838ff6c, /// 0xea4
			 0x8ab325c4, /// 0xea8
			 0xc218aa3a, /// 0xeac
			 0x623032f8, /// 0xeb0
			 0xa97ca020, /// 0xeb4
			 0x4ce3ae7b, /// 0xeb8
			 0x00e64843, /// 0xebc
			 0x731f8365, /// 0xec0
			 0xeadf18fd, /// 0xec4
			 0x56dbbf92, /// 0xec8
			 0xfe03397e, /// 0xecc
			 0xe5732ec6, /// 0xed0
			 0xa152cb39, /// 0xed4
			 0xc34f9419, /// 0xed8
			 0x2cb902df, /// 0xedc
			 0xc921cd80, /// 0xee0
			 0xd353ca69, /// 0xee4
			 0xa4b94db7, /// 0xee8
			 0x8c5ac349, /// 0xeec
			 0x56440bf9, /// 0xef0
			 0x39fcef18, /// 0xef4
			 0xe8f746a3, /// 0xef8
			 0x720a90be, /// 0xefc
			 0x888eeae0, /// 0xf00
			 0x78d437cf, /// 0xf04
			 0xb6d37ae5, /// 0xf08
			 0xa2f62e26, /// 0xf0c
			 0x0768f221, /// 0xf10
			 0x470c56ba, /// 0xf14
			 0xd791d874, /// 0xf18
			 0xd6f42c07, /// 0xf1c
			 0xd606f5ca, /// 0xf20
			 0x04336f9a, /// 0xf24
			 0x8840b690, /// 0xf28
			 0x31921688, /// 0xf2c
			 0xd40be675, /// 0xf30
			 0x52be69bd, /// 0xf34
			 0xd15cb06e, /// 0xf38
			 0x1f926fd6, /// 0xf3c
			 0x76181d7a, /// 0xf40
			 0x59f1c25f, /// 0xf44
			 0xf92fae60, /// 0xf48
			 0x16b54f2b, /// 0xf4c
			 0x0bb56373, /// 0xf50
			 0x0148188f, /// 0xf54
			 0x2c0f4544, /// 0xf58
			 0xa38b0365, /// 0xf5c
			 0x292f0866, /// 0xf60
			 0xd7772014, /// 0xf64
			 0x22482e82, /// 0xf68
			 0x4c62a681, /// 0xf6c
			 0xd63110fc, /// 0xf70
			 0x1204fa28, /// 0xf74
			 0xde3c20d1, /// 0xf78
			 0xe6e49c89, /// 0xf7c
			 0xfc973b06, /// 0xf80
			 0x217c3297, /// 0xf84
			 0xa3eb4447, /// 0xf88
			 0x8700476e, /// 0xf8c
			 0x1cf33535, /// 0xf90
			 0x64d38760, /// 0xf94
			 0x92294351, /// 0xf98
			 0x6e8faf87, /// 0xf9c
			 0x9a666f3a, /// 0xfa0
			 0x8818aad9, /// 0xfa4
			 0x9dd6f617, /// 0xfa8
			 0x2dfa15ec, /// 0xfac
			 0xbb407de7, /// 0xfb0
			 0x37600065, /// 0xfb4
			 0x5ad619a7, /// 0xfb8
			 0xc36ddeab, /// 0xfbc
			 0xa763facd, /// 0xfc0
			 0x2cbb3ef0, /// 0xfc4
			 0x990958e3, /// 0xfc8
			 0xc20c0c55, /// 0xfcc
			 0x52c991ee, /// 0xfd0
			 0x9e803788, /// 0xfd4
			 0xd13631cb, /// 0xfd8
			 0x3786df6c, /// 0xfdc
			 0x602e4e5e, /// 0xfe0
			 0x8cbd092c, /// 0xfe4
			 0xbe697789, /// 0xfe8
			 0x67a1cf9d, /// 0xfec
			 0xba8c8093, /// 0xff0
			 0x7412bdc0, /// 0xff4
			 0x16aa2132, /// 0xff8
			 0x27668ba4 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00000
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x0c400000,                                                  // Leading 1s:                                 /// 0000c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0001c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00020
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00024
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00028
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00030
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00038
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00040
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00048
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x4b000000,                                                  // 8388608.0                                   /// 00054
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00060
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00064
			 0x80011111,                                                  // -9.7958E-41                                 /// 00068
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00074
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0c780000,                                                  // Leading 1s:                                 /// 0007c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00080
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00084
			 0x0c400000,                                                  // Leading 1s:                                 /// 00088
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0009c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000bc
			 0x3f028f5c,                                                  // 0.51                                        /// 000c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x55555555,                                                  // 4 random values                             /// 000f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00100
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00118
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0011c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00120
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00124
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00128
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0012c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00130
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00134
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00138
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0013c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00140
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00148
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00150
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00154
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0015c
			 0x33333333,                                                  // 4 random values                             /// 00160
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00164
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00168
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00174
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0017c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00180
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00184
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0018c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00190
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001dc
			 0x33333333,                                                  // 4 random values                             /// 001e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001f0
			 0x7f800000,                                                  // inf                                         /// 001f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00200
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00204
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00208
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00214
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00218
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0021c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00220
			 0x3f028f5c,                                                  // 0.51                                        /// 00224
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00228
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00234
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00238
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00244
			 0xcb000000,                                                  // -8388608.0                                  /// 00248
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00250
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0025c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00260
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00264
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00268
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0026c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00270
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0027c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00284
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00288
			 0x7f800000,                                                  // inf                                         /// 0028c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00290
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0029c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c4
			 0x7f800000,                                                  // inf                                         /// 002c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00308
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00310
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00318
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00324
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00330
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00338
			 0x80000000,                                                  // -zero                                       /// 0033c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00340
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00344
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00348
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x55555555,                                                  // 4 random values                             /// 00354
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00358
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0035c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00360
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00364
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00368
			 0xff800000,                                                  // -inf                                        /// 0036c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00378
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0037c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00380
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00384
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00388
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00390
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00394
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00398
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0039c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00400
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00404
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00408
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0040c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00410
			 0x3f028f5c,                                                  // 0.51                                        /// 00414
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00418
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00420
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00424
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00428
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0042c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00430
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0c700000,                                                  // Leading 1s:                                 /// 00438
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00448
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0044c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00450
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00468
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0xbf028f5c,                                                  // -0.51                                       /// 00470
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00474
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00478
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0047c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0e000001,                                                  // Trailing 1s:                                /// 00488
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0048c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004bc
			 0xcb000000,                                                  // -8388608.0                                  /// 004c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004e4
			 0x00000000,                                                  // zero                                        /// 004e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00500
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00504
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00508
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00514
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00518
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00528
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00538
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0053c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00540
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00544
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00548
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0054c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00554
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00558
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0055c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00564
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00568
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0056c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x7f800000,                                                  // inf                                         /// 00574
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00580
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00584
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00588
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0058c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x33333333,                                                  // 4 random values                             /// 00594
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00598
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0059c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xff800000,                                                  // -inf                                        /// 005b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d8
			 0xcb000000,                                                  // -8388608.0                                  /// 005dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00600
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00604
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00608
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00614
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00620
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00628
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0062c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00630
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00634
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00638
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00644
			 0x7fc00001,                                                  // signaling NaN                               /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00650
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0065c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00664
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00668
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0066c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00674
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00678
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0067c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x80000000,                                                  // -zero                                       /// 00690
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00698
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b4
			 0xff800000,                                                  // -inf                                        /// 006b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c8
			 0x55555555,                                                  // 4 random values                             /// 006cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00700
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00704
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0070c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00710
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00714
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00718
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0071c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00728
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00730
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00738
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00744
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00748
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0074c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00750
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0075c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00764
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00768
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0076c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00770
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00774
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00780
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00790
			 0x00000000,                                                  // zero                                        /// 00794
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00798
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0079c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007cc
			 0x00000000,                                                  // zero                                        /// 007d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00800
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00804
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x55555555,                                                  // 4 random values                             /// 0080c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00810
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00814
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0081c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00824
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0082c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00830
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00838
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00850
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00858
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0085c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00860
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00864
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00868
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0086c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00870
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00874
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00878
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0087c
			 0xffc00001,                                                  // -signaling NaN                              /// 00880
			 0x7f800000,                                                  // inf                                         /// 00884
			 0x0e000001,                                                  // Trailing 1s:                                /// 00888
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0088c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00890
			 0x0c400000,                                                  // Leading 1s:                                 /// 00894
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0c400000,                                                  // Leading 1s:                                 /// 008a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d4
			 0x00000000,                                                  // zero                                        /// 008d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00900
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80011111,                                                  // -9.7958E-41                                 /// 00908
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00910
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00914
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00918
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0091c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00920
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00928
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00934
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00938
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0093c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00940
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0xcb000000,                                                  // -8388608.0                                  /// 0094c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00950
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xbf028f5c,                                                  // -0.51                                       /// 00958
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0095c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00960
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00964
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00968
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0096c
			 0x55555555,                                                  // 4 random values                             /// 00970
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00974
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0097c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00980
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00984
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0098c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00990
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00994
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0099c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a0
			 0x00000000,                                                  // zero                                        /// 009a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c8
			 0x80000000,                                                  // -zero                                       /// 009cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a34
			 0x4b000000,                                                  // 8388608.0                                   /// 00a38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a54
			 0x00000000,                                                  // zero                                        /// 00a58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a84
			 0xffc00001,                                                  // -signaling NaN                              /// 00a88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aa0
			 0x7fc00001,                                                  // signaling NaN                               /// 00aa4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ab8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00abc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00acc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad0
			 0x7f800000,                                                  // inf                                         /// 00ad4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00adc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ae4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00afc
			 0xff800000,                                                  // -inf                                        /// 00b00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x33333333,                                                  // 4 random values                             /// 00b24
			 0x4b000000,                                                  // 8388608.0                                   /// 00b28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b38
			 0x33333333,                                                  // 4 random values                             /// 00b3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xcb000000,                                                  // -8388608.0                                  /// 00b88
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ba8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bb0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00be0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00be4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c14
			 0xff800000,                                                  // -inf                                        /// 00c18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c40
			 0x7fc00001,                                                  // signaling NaN                               /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cac
			 0x80000000,                                                  // -zero                                       /// 00cb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cd8
			 0xff800000,                                                  // -inf                                        /// 00cdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ce0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cf0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d24
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d40
			 0x33333333,                                                  // 4 random values                             /// 00d44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00da0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc8
			 0x33333333,                                                  // 4 random values                             /// 00dcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ddc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00df4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x3f028f5c,                                                  // 0.51                                        /// 00e00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x80000000,                                                  // -zero                                       /// 00e28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e78
			 0xff800000,                                                  // -inf                                        /// 00e7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e90
			 0x80000000,                                                  // -zero                                       /// 00e94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ea4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ec0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ecc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef4
			 0x7f800000,                                                  // inf                                         /// 00ef8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0xff800000,                                                  // -inf                                        /// 00f54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fa8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ff0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80000004                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xe8dc5750, /// 0x0
			 0x0087ef8b, /// 0x4
			 0xfa3b9faf, /// 0x8
			 0x35736ebf, /// 0xc
			 0xd37d4426, /// 0x10
			 0xecff2ddf, /// 0x14
			 0xc149ec31, /// 0x18
			 0x284705cf, /// 0x1c
			 0x1ee9497b, /// 0x20
			 0xdedfa3cc, /// 0x24
			 0xee402e8e, /// 0x28
			 0x07d65372, /// 0x2c
			 0xe3739f1f, /// 0x30
			 0xbb4203af, /// 0x34
			 0x405a57cb, /// 0x38
			 0x43154a4d, /// 0x3c
			 0x923ee194, /// 0x40
			 0x0eb0d2a5, /// 0x44
			 0x11978af7, /// 0x48
			 0x80b508af, /// 0x4c
			 0xeb95113d, /// 0x50
			 0xab10e613, /// 0x54
			 0x95728056, /// 0x58
			 0x8069f3a9, /// 0x5c
			 0x617f12b2, /// 0x60
			 0x60ce4ba2, /// 0x64
			 0x1a196885, /// 0x68
			 0xe7ac9b1b, /// 0x6c
			 0x7f79a89c, /// 0x70
			 0x45dcf2ce, /// 0x74
			 0x05b7f9c9, /// 0x78
			 0x91b61502, /// 0x7c
			 0x80f6d11b, /// 0x80
			 0x0d166834, /// 0x84
			 0x1bb604c1, /// 0x88
			 0xbb0820c3, /// 0x8c
			 0xe6a74860, /// 0x90
			 0x8174bcad, /// 0x94
			 0x832fd0ac, /// 0x98
			 0x014a285d, /// 0x9c
			 0x120e56ef, /// 0xa0
			 0x69afa6de, /// 0xa4
			 0x955eb0a4, /// 0xa8
			 0x0d859f2f, /// 0xac
			 0x065cb6f6, /// 0xb0
			 0x9f6e9e92, /// 0xb4
			 0xf7aad8be, /// 0xb8
			 0x875dd1ee, /// 0xbc
			 0x6c7f2b45, /// 0xc0
			 0xb5195548, /// 0xc4
			 0x111ab1b5, /// 0xc8
			 0xf38c6534, /// 0xcc
			 0x5f3daf57, /// 0xd0
			 0xbe9ea3b6, /// 0xd4
			 0x1170c5df, /// 0xd8
			 0xb772b2e8, /// 0xdc
			 0x57de65f0, /// 0xe0
			 0xf266c5a8, /// 0xe4
			 0xd3296e5e, /// 0xe8
			 0x1f5d3978, /// 0xec
			 0x9ad6cdd0, /// 0xf0
			 0xbba1250a, /// 0xf4
			 0x0ea10259, /// 0xf8
			 0x0dfed330, /// 0xfc
			 0x347508cb, /// 0x100
			 0xa116d13b, /// 0x104
			 0xdaf8f9ee, /// 0x108
			 0x2b4a6ea2, /// 0x10c
			 0xa4f37da5, /// 0x110
			 0x416a6176, /// 0x114
			 0x85942607, /// 0x118
			 0xcf93ad35, /// 0x11c
			 0x03d48974, /// 0x120
			 0xa43fb512, /// 0x124
			 0xcd29b56b, /// 0x128
			 0xc49d6c35, /// 0x12c
			 0xe29577e0, /// 0x130
			 0xf96b212b, /// 0x134
			 0xb3cf8795, /// 0x138
			 0x1bf6304c, /// 0x13c
			 0x68c5c56f, /// 0x140
			 0x5c812b1f, /// 0x144
			 0xa12059ed, /// 0x148
			 0x3befbbbf, /// 0x14c
			 0xdd6f3b48, /// 0x150
			 0x9f0cf223, /// 0x154
			 0x3883ac1a, /// 0x158
			 0xfaa75c4d, /// 0x15c
			 0x1fb4ae3c, /// 0x160
			 0x1b3eb5f2, /// 0x164
			 0xedab3c40, /// 0x168
			 0x3ff5c0a1, /// 0x16c
			 0x8d33a9a6, /// 0x170
			 0xbb45f2ed, /// 0x174
			 0x4b6c7cee, /// 0x178
			 0x217335c0, /// 0x17c
			 0xef853d98, /// 0x180
			 0x231f83a5, /// 0x184
			 0x27a4f4ed, /// 0x188
			 0x9e9a871e, /// 0x18c
			 0xfa72c1ce, /// 0x190
			 0xc5895407, /// 0x194
			 0x3522c43d, /// 0x198
			 0x33b1de14, /// 0x19c
			 0x72b5a645, /// 0x1a0
			 0x08bb994f, /// 0x1a4
			 0x26c0c4a8, /// 0x1a8
			 0x37ea4ed5, /// 0x1ac
			 0x2297142f, /// 0x1b0
			 0xd3e6ac02, /// 0x1b4
			 0x69ad94fd, /// 0x1b8
			 0x81bb7841, /// 0x1bc
			 0xccb5b569, /// 0x1c0
			 0x11332170, /// 0x1c4
			 0xcf56b340, /// 0x1c8
			 0x9d3d1d5b, /// 0x1cc
			 0xefcade0f, /// 0x1d0
			 0x6abaeaa5, /// 0x1d4
			 0x4d0eba44, /// 0x1d8
			 0xbc500416, /// 0x1dc
			 0x21cd00f0, /// 0x1e0
			 0x4584e47c, /// 0x1e4
			 0x8751baba, /// 0x1e8
			 0x436c3cc6, /// 0x1ec
			 0xe5a30186, /// 0x1f0
			 0x3e0fbef5, /// 0x1f4
			 0xab50d22b, /// 0x1f8
			 0x098c68f0, /// 0x1fc
			 0x2a012a43, /// 0x200
			 0x8cf9b385, /// 0x204
			 0x9c9794ed, /// 0x208
			 0x8b00a696, /// 0x20c
			 0xe598effb, /// 0x210
			 0xce5b4dea, /// 0x214
			 0x2f997615, /// 0x218
			 0xbcf5bf65, /// 0x21c
			 0xa2dd296b, /// 0x220
			 0x4a081f2a, /// 0x224
			 0x95dc3e52, /// 0x228
			 0x7156f204, /// 0x22c
			 0x76c6005d, /// 0x230
			 0xd3e7a2eb, /// 0x234
			 0xbf364564, /// 0x238
			 0xb15720d6, /// 0x23c
			 0x0797b68c, /// 0x240
			 0xd1ee7447, /// 0x244
			 0xb7f48fa6, /// 0x248
			 0xa1aa2f0d, /// 0x24c
			 0x5cdd0ebb, /// 0x250
			 0x5e82fe8c, /// 0x254
			 0xe1c9c389, /// 0x258
			 0x11bd23bd, /// 0x25c
			 0x2b146443, /// 0x260
			 0xa0f28b4f, /// 0x264
			 0x06894198, /// 0x268
			 0xaa57c3be, /// 0x26c
			 0x612767e8, /// 0x270
			 0xa678ddc0, /// 0x274
			 0xc31b6c03, /// 0x278
			 0x3e4cd697, /// 0x27c
			 0xe81d35b6, /// 0x280
			 0x2256422c, /// 0x284
			 0xe9903f6b, /// 0x288
			 0x3f3dc336, /// 0x28c
			 0x46747a09, /// 0x290
			 0x00be774e, /// 0x294
			 0x05cdd2e1, /// 0x298
			 0xbd8dffb6, /// 0x29c
			 0x83aa71b6, /// 0x2a0
			 0x5b8b8ff3, /// 0x2a4
			 0x35fc74c9, /// 0x2a8
			 0xc9203c4e, /// 0x2ac
			 0xa39f7fcd, /// 0x2b0
			 0x9b7d9152, /// 0x2b4
			 0x63ef0511, /// 0x2b8
			 0x9c74821c, /// 0x2bc
			 0xb106e860, /// 0x2c0
			 0x19443d2e, /// 0x2c4
			 0x0551fa07, /// 0x2c8
			 0xfb949a53, /// 0x2cc
			 0x118568d5, /// 0x2d0
			 0x1ccaa84c, /// 0x2d4
			 0xb4f5231c, /// 0x2d8
			 0x5262fe9f, /// 0x2dc
			 0x508a9faa, /// 0x2e0
			 0xda473fd7, /// 0x2e4
			 0xc9b9d03f, /// 0x2e8
			 0x36304f98, /// 0x2ec
			 0x0167169a, /// 0x2f0
			 0x80e1e63a, /// 0x2f4
			 0x8df405c0, /// 0x2f8
			 0xe4451365, /// 0x2fc
			 0xba93d624, /// 0x300
			 0x5e0bd5bc, /// 0x304
			 0xbe67ade3, /// 0x308
			 0x98e61f22, /// 0x30c
			 0x7fe62515, /// 0x310
			 0x9d9b0cd7, /// 0x314
			 0xd119161a, /// 0x318
			 0x6504e6b1, /// 0x31c
			 0x49d2d3f9, /// 0x320
			 0xb48002a1, /// 0x324
			 0x97e1c0db, /// 0x328
			 0x094d9061, /// 0x32c
			 0x478c0626, /// 0x330
			 0x45f961ce, /// 0x334
			 0xf660e4b3, /// 0x338
			 0x0de80a9c, /// 0x33c
			 0xc929c6d5, /// 0x340
			 0x979a9cc1, /// 0x344
			 0x509d2ae0, /// 0x348
			 0x5bc4f5b7, /// 0x34c
			 0x20087cfb, /// 0x350
			 0xb2db50ee, /// 0x354
			 0x5defbe5d, /// 0x358
			 0xf082b5f1, /// 0x35c
			 0xff0e50fa, /// 0x360
			 0x01beb5c9, /// 0x364
			 0x4dc91f2c, /// 0x368
			 0xa365362c, /// 0x36c
			 0x648844dd, /// 0x370
			 0x0fc6bd52, /// 0x374
			 0xf38c2eb3, /// 0x378
			 0x6b370a0b, /// 0x37c
			 0x7d8c9695, /// 0x380
			 0x597ca6fc, /// 0x384
			 0xafe607e1, /// 0x388
			 0x5ab6b9c5, /// 0x38c
			 0x5edd2199, /// 0x390
			 0x068da3ad, /// 0x394
			 0x82e44608, /// 0x398
			 0x0c31e831, /// 0x39c
			 0xe9b49703, /// 0x3a0
			 0x02b4a149, /// 0x3a4
			 0xd8c1b91d, /// 0x3a8
			 0xd89cd7dd, /// 0x3ac
			 0x050c576e, /// 0x3b0
			 0x8618c141, /// 0x3b4
			 0x6fc800ad, /// 0x3b8
			 0x7d60e28d, /// 0x3bc
			 0x261e79a3, /// 0x3c0
			 0x08e02c03, /// 0x3c4
			 0x22ff2259, /// 0x3c8
			 0xc9ac8b0d, /// 0x3cc
			 0x0b0322c0, /// 0x3d0
			 0x4f029c23, /// 0x3d4
			 0x7438b3cb, /// 0x3d8
			 0xa47000dd, /// 0x3dc
			 0xd2c57196, /// 0x3e0
			 0xb26b2c44, /// 0x3e4
			 0x07f6e728, /// 0x3e8
			 0xd2af780d, /// 0x3ec
			 0xc5fa787c, /// 0x3f0
			 0xcd34b37f, /// 0x3f4
			 0x22e65e28, /// 0x3f8
			 0xd048661e, /// 0x3fc
			 0x7c7778c7, /// 0x400
			 0x516ea56a, /// 0x404
			 0xe8cb06f3, /// 0x408
			 0x7fa92113, /// 0x40c
			 0xc4fe1cee, /// 0x410
			 0x50f34b9f, /// 0x414
			 0x9e14b4ec, /// 0x418
			 0x03bb0042, /// 0x41c
			 0x6ba8d3f8, /// 0x420
			 0x47ef82fc, /// 0x424
			 0x2ca52e57, /// 0x428
			 0xd4861044, /// 0x42c
			 0x8e12bc12, /// 0x430
			 0x57adea30, /// 0x434
			 0xc8b6b4c2, /// 0x438
			 0x9ab3386a, /// 0x43c
			 0xbf6f5917, /// 0x440
			 0x9fd9c888, /// 0x444
			 0x21c77ccd, /// 0x448
			 0x3692c661, /// 0x44c
			 0x0d20c9df, /// 0x450
			 0x8b9ec17d, /// 0x454
			 0x8344bc0a, /// 0x458
			 0x7eefc4b4, /// 0x45c
			 0xe5bbd1fa, /// 0x460
			 0x58874aee, /// 0x464
			 0x1eabeaf7, /// 0x468
			 0x39ac8ae4, /// 0x46c
			 0x9edbeb31, /// 0x470
			 0x1794379d, /// 0x474
			 0xd9024d13, /// 0x478
			 0xc889ea54, /// 0x47c
			 0x709b4052, /// 0x480
			 0x82dc8af1, /// 0x484
			 0xf97d342b, /// 0x488
			 0x813aa0a9, /// 0x48c
			 0x767b8cda, /// 0x490
			 0x4765775b, /// 0x494
			 0x1f810fc6, /// 0x498
			 0x693579d5, /// 0x49c
			 0x9560c07b, /// 0x4a0
			 0x8a76f077, /// 0x4a4
			 0xc8aee8d1, /// 0x4a8
			 0x41705199, /// 0x4ac
			 0x5ccf0046, /// 0x4b0
			 0x36877c4b, /// 0x4b4
			 0x5e046d9d, /// 0x4b8
			 0x621ee982, /// 0x4bc
			 0xcae785f5, /// 0x4c0
			 0x932f8e78, /// 0x4c4
			 0xfacce259, /// 0x4c8
			 0xf9bd272d, /// 0x4cc
			 0x366dff60, /// 0x4d0
			 0x7728aabb, /// 0x4d4
			 0xfa0999f2, /// 0x4d8
			 0xec0dbcd6, /// 0x4dc
			 0xa16951f3, /// 0x4e0
			 0x29fd65e6, /// 0x4e4
			 0x8d98d26a, /// 0x4e8
			 0xdd4518a0, /// 0x4ec
			 0x05176583, /// 0x4f0
			 0xd2695c3e, /// 0x4f4
			 0x605c99ec, /// 0x4f8
			 0x447ad5b9, /// 0x4fc
			 0xca9f10ce, /// 0x500
			 0xe21ce7b1, /// 0x504
			 0x1a041ec4, /// 0x508
			 0x44bcc488, /// 0x50c
			 0xd36582c2, /// 0x510
			 0x0adca07f, /// 0x514
			 0x73423445, /// 0x518
			 0xf39a8c1b, /// 0x51c
			 0x7d08dc01, /// 0x520
			 0xfcb960a3, /// 0x524
			 0x97c800cc, /// 0x528
			 0xb42ef04f, /// 0x52c
			 0xe59a4e00, /// 0x530
			 0x57e40b63, /// 0x534
			 0x4ecacc9a, /// 0x538
			 0x4df69d17, /// 0x53c
			 0xe07134ef, /// 0x540
			 0x1c0112cd, /// 0x544
			 0x96f23ce4, /// 0x548
			 0xd4df1ab7, /// 0x54c
			 0x1084a15a, /// 0x550
			 0x1da88ab6, /// 0x554
			 0xa0cdbe22, /// 0x558
			 0xe8d1b7c4, /// 0x55c
			 0xf91884b7, /// 0x560
			 0x5bac1713, /// 0x564
			 0x5ec8b986, /// 0x568
			 0xef416d76, /// 0x56c
			 0xb4276dc3, /// 0x570
			 0x70fdc01a, /// 0x574
			 0x07334ce4, /// 0x578
			 0x0d9685f3, /// 0x57c
			 0x1ce6a429, /// 0x580
			 0xe9e40df2, /// 0x584
			 0xbfb5d1ca, /// 0x588
			 0xfecb87dc, /// 0x58c
			 0xcb8be93c, /// 0x590
			 0x78cf7c9d, /// 0x594
			 0x2531a269, /// 0x598
			 0xe13b244b, /// 0x59c
			 0x1caf573b, /// 0x5a0
			 0x57f70a64, /// 0x5a4
			 0xaf9a731b, /// 0x5a8
			 0x7d891942, /// 0x5ac
			 0x546d65d7, /// 0x5b0
			 0x79eed1df, /// 0x5b4
			 0x22a4f13a, /// 0x5b8
			 0xbf480cba, /// 0x5bc
			 0x1693686c, /// 0x5c0
			 0x8b1a02fd, /// 0x5c4
			 0x843ae78c, /// 0x5c8
			 0x59aec421, /// 0x5cc
			 0x965e4d7e, /// 0x5d0
			 0x07559a57, /// 0x5d4
			 0x6b9f87cd, /// 0x5d8
			 0x0976d35d, /// 0x5dc
			 0x3495eb39, /// 0x5e0
			 0xe21dd715, /// 0x5e4
			 0xd41ffda6, /// 0x5e8
			 0x3e7615cd, /// 0x5ec
			 0xb9ebbb98, /// 0x5f0
			 0x33065810, /// 0x5f4
			 0x136fc24c, /// 0x5f8
			 0x02070a8e, /// 0x5fc
			 0x87ade404, /// 0x600
			 0x51dbeaeb, /// 0x604
			 0x313a59b6, /// 0x608
			 0x8129d309, /// 0x60c
			 0x34533aa9, /// 0x610
			 0x36c8d514, /// 0x614
			 0x1e1df1fc, /// 0x618
			 0xff0b4cef, /// 0x61c
			 0xb400b7ee, /// 0x620
			 0x6d17cffa, /// 0x624
			 0xa4927739, /// 0x628
			 0xc097165b, /// 0x62c
			 0xe2ce742b, /// 0x630
			 0xcc5e9bc9, /// 0x634
			 0xdda9ccf4, /// 0x638
			 0x9aa37d4e, /// 0x63c
			 0x6c8828db, /// 0x640
			 0x5f937395, /// 0x644
			 0x503950f1, /// 0x648
			 0x958de26a, /// 0x64c
			 0xc700e6d1, /// 0x650
			 0x003e6292, /// 0x654
			 0x216a19f4, /// 0x658
			 0x7d8b9d9f, /// 0x65c
			 0x27c4cdfe, /// 0x660
			 0x7f937c69, /// 0x664
			 0xa6d21edb, /// 0x668
			 0x7ddbdbf9, /// 0x66c
			 0x1aa88b59, /// 0x670
			 0xbe0e547c, /// 0x674
			 0x9e63d937, /// 0x678
			 0xbdf2855d, /// 0x67c
			 0x2215d0ce, /// 0x680
			 0x81cadd00, /// 0x684
			 0xddc8baa8, /// 0x688
			 0x0e1342fe, /// 0x68c
			 0xa8f84306, /// 0x690
			 0x479edb05, /// 0x694
			 0xe7572764, /// 0x698
			 0xb78e920e, /// 0x69c
			 0xe3c1492b, /// 0x6a0
			 0x7cf3a3f6, /// 0x6a4
			 0xe8c4f175, /// 0x6a8
			 0x40adc487, /// 0x6ac
			 0x1e61e094, /// 0x6b0
			 0xd25a330f, /// 0x6b4
			 0x288128b1, /// 0x6b8
			 0xa1cd7e8b, /// 0x6bc
			 0x8145baab, /// 0x6c0
			 0x2c7bff22, /// 0x6c4
			 0xde125225, /// 0x6c8
			 0x1123fcf5, /// 0x6cc
			 0xa0dc2800, /// 0x6d0
			 0x2f50e95b, /// 0x6d4
			 0x82c5d9a0, /// 0x6d8
			 0x62b68157, /// 0x6dc
			 0xadb3e1ca, /// 0x6e0
			 0xb477145f, /// 0x6e4
			 0xe3a551da, /// 0x6e8
			 0xecaa9278, /// 0x6ec
			 0xe1cda4e7, /// 0x6f0
			 0x1b23e979, /// 0x6f4
			 0xb15a14eb, /// 0x6f8
			 0x39243671, /// 0x6fc
			 0xb0ae374b, /// 0x700
			 0xcf7ce226, /// 0x704
			 0x92c6b6f6, /// 0x708
			 0x0e5d70ca, /// 0x70c
			 0x9e9c12bb, /// 0x710
			 0x67bbe83f, /// 0x714
			 0xacabd9f1, /// 0x718
			 0x22004ccd, /// 0x71c
			 0x99de3f2a, /// 0x720
			 0xa6d625e1, /// 0x724
			 0xca3fc7ef, /// 0x728
			 0xa9c1b664, /// 0x72c
			 0xf8b20507, /// 0x730
			 0xb9759a8e, /// 0x734
			 0x06e274cc, /// 0x738
			 0x6f969663, /// 0x73c
			 0x6a691ea5, /// 0x740
			 0xd9ee7542, /// 0x744
			 0xf16e96e3, /// 0x748
			 0x40dfb8b5, /// 0x74c
			 0x18df465c, /// 0x750
			 0xd7218d65, /// 0x754
			 0x5fd45e89, /// 0x758
			 0x6c72d9dd, /// 0x75c
			 0x03768248, /// 0x760
			 0xfc279d9c, /// 0x764
			 0xbc5cbe58, /// 0x768
			 0x393dd079, /// 0x76c
			 0xe7ddb9b8, /// 0x770
			 0x487592f1, /// 0x774
			 0x2d5cfca8, /// 0x778
			 0x7ee9e06e, /// 0x77c
			 0x6edf5bbd, /// 0x780
			 0xd729aff4, /// 0x784
			 0x6e90f019, /// 0x788
			 0xdb76c4a0, /// 0x78c
			 0x076acee3, /// 0x790
			 0xe9a484ce, /// 0x794
			 0x25120f5d, /// 0x798
			 0x22d5f1d1, /// 0x79c
			 0x96229d07, /// 0x7a0
			 0x1c3fb1ed, /// 0x7a4
			 0x03604ea0, /// 0x7a8
			 0x1eb697d9, /// 0x7ac
			 0x03b185f7, /// 0x7b0
			 0x246613ac, /// 0x7b4
			 0x0cdb139f, /// 0x7b8
			 0x10a64814, /// 0x7bc
			 0x828bf974, /// 0x7c0
			 0x15fb443f, /// 0x7c4
			 0xbf8e871a, /// 0x7c8
			 0xc6bd6686, /// 0x7cc
			 0x351ac23e, /// 0x7d0
			 0xb59422f8, /// 0x7d4
			 0xb10aefcd, /// 0x7d8
			 0x4fa8a6ec, /// 0x7dc
			 0xfcb730a8, /// 0x7e0
			 0x1e0d86bf, /// 0x7e4
			 0x73d544e7, /// 0x7e8
			 0x7634a3a2, /// 0x7ec
			 0x0769129f, /// 0x7f0
			 0x28e4b395, /// 0x7f4
			 0x5d016de8, /// 0x7f8
			 0xab26d336, /// 0x7fc
			 0x98e54dbc, /// 0x800
			 0x325824e6, /// 0x804
			 0xe02aad88, /// 0x808
			 0x8c6f7251, /// 0x80c
			 0x0f5256b1, /// 0x810
			 0x0a6d8dca, /// 0x814
			 0xbfa2b021, /// 0x818
			 0x34b9756d, /// 0x81c
			 0x40d4f813, /// 0x820
			 0x8240111f, /// 0x824
			 0x3e73a8f6, /// 0x828
			 0xe08e578c, /// 0x82c
			 0x20d1e91c, /// 0x830
			 0xf5d77e16, /// 0x834
			 0x0271e2c7, /// 0x838
			 0x70b9c20f, /// 0x83c
			 0x218d04a0, /// 0x840
			 0x6f704c96, /// 0x844
			 0xc1b08df3, /// 0x848
			 0xd7abd84a, /// 0x84c
			 0x5aae01d1, /// 0x850
			 0x23efca7a, /// 0x854
			 0xe24f9903, /// 0x858
			 0x4755a76a, /// 0x85c
			 0x1e32cda9, /// 0x860
			 0x5063803a, /// 0x864
			 0xf51cae64, /// 0x868
			 0x50060c47, /// 0x86c
			 0x471ada45, /// 0x870
			 0x464b8e93, /// 0x874
			 0xe3d67cb8, /// 0x878
			 0xb74b1a21, /// 0x87c
			 0x74c8acdb, /// 0x880
			 0x7b72bb02, /// 0x884
			 0x42698396, /// 0x888
			 0xb4479b08, /// 0x88c
			 0xbf3e0ece, /// 0x890
			 0xb0e6db91, /// 0x894
			 0x13fb8653, /// 0x898
			 0xd7330e1c, /// 0x89c
			 0xbb5de31c, /// 0x8a0
			 0x69c47876, /// 0x8a4
			 0x5967dc64, /// 0x8a8
			 0x477a18db, /// 0x8ac
			 0xe6caa472, /// 0x8b0
			 0xfb8b5269, /// 0x8b4
			 0x0bcca934, /// 0x8b8
			 0x1aef770a, /// 0x8bc
			 0xd244eeb4, /// 0x8c0
			 0xecb5e6c2, /// 0x8c4
			 0x2641b105, /// 0x8c8
			 0x438158a9, /// 0x8cc
			 0xc7bc7247, /// 0x8d0
			 0x22e2bdd8, /// 0x8d4
			 0x7e6ae44b, /// 0x8d8
			 0xf700093b, /// 0x8dc
			 0x9b7d3576, /// 0x8e0
			 0xb3408fb0, /// 0x8e4
			 0x7b9271a6, /// 0x8e8
			 0x391760c9, /// 0x8ec
			 0xcc6dedd6, /// 0x8f0
			 0xdd4a7af5, /// 0x8f4
			 0x931ae4ca, /// 0x8f8
			 0xc7b466c0, /// 0x8fc
			 0x15ebb000, /// 0x900
			 0x34902916, /// 0x904
			 0xad727da1, /// 0x908
			 0xfbc23463, /// 0x90c
			 0xeb1fb7b0, /// 0x910
			 0xb72e4763, /// 0x914
			 0xcc10072e, /// 0x918
			 0x123dff48, /// 0x91c
			 0x45bbf77f, /// 0x920
			 0xb9745045, /// 0x924
			 0x4c6aae2a, /// 0x928
			 0x00d64d1b, /// 0x92c
			 0xe6993a0d, /// 0x930
			 0xf5f01315, /// 0x934
			 0x34d8d912, /// 0x938
			 0x5892d9f7, /// 0x93c
			 0xc126be3e, /// 0x940
			 0x56b0d288, /// 0x944
			 0x120b6f33, /// 0x948
			 0x4931247c, /// 0x94c
			 0x517b6864, /// 0x950
			 0x8e520b28, /// 0x954
			 0x245b4f3f, /// 0x958
			 0x7f7bca9a, /// 0x95c
			 0xf7b726a5, /// 0x960
			 0x46467b92, /// 0x964
			 0x879695d3, /// 0x968
			 0xf134c40d, /// 0x96c
			 0x518f3af5, /// 0x970
			 0xbee8fa2d, /// 0x974
			 0x91c69384, /// 0x978
			 0xf29a25eb, /// 0x97c
			 0xac26661d, /// 0x980
			 0x99570fd4, /// 0x984
			 0x89c94930, /// 0x988
			 0xd3aeabf1, /// 0x98c
			 0x9a482389, /// 0x990
			 0x06cedcf3, /// 0x994
			 0xf6e48b41, /// 0x998
			 0xb5b78291, /// 0x99c
			 0x3ab7e813, /// 0x9a0
			 0x6f1142ed, /// 0x9a4
			 0x090b8f15, /// 0x9a8
			 0x433a72b6, /// 0x9ac
			 0x1f79cf63, /// 0x9b0
			 0xb0ebc552, /// 0x9b4
			 0x84059926, /// 0x9b8
			 0xcd85697c, /// 0x9bc
			 0x0adf8e10, /// 0x9c0
			 0x3d4174d9, /// 0x9c4
			 0xaaa802a6, /// 0x9c8
			 0xfcb9f3c3, /// 0x9cc
			 0x59bf1893, /// 0x9d0
			 0x42b064a2, /// 0x9d4
			 0xe9f507c2, /// 0x9d8
			 0x1f41fb33, /// 0x9dc
			 0xb7631cff, /// 0x9e0
			 0x00e1545e, /// 0x9e4
			 0xad3ec37f, /// 0x9e8
			 0xa46a511b, /// 0x9ec
			 0x4e8a3800, /// 0x9f0
			 0xe5c33dc3, /// 0x9f4
			 0xd7082d47, /// 0x9f8
			 0x142ef400, /// 0x9fc
			 0x7585c8bb, /// 0xa00
			 0x5298edbd, /// 0xa04
			 0x56e5d632, /// 0xa08
			 0xd05fdd39, /// 0xa0c
			 0xf42dc1b2, /// 0xa10
			 0x84d88a08, /// 0xa14
			 0xa8233036, /// 0xa18
			 0xb203e1e4, /// 0xa1c
			 0xe0bb7b3c, /// 0xa20
			 0xa7cf451a, /// 0xa24
			 0xc1517244, /// 0xa28
			 0x8b87b7fe, /// 0xa2c
			 0x344c298a, /// 0xa30
			 0x05dfaf7c, /// 0xa34
			 0x4fe78625, /// 0xa38
			 0xe2744f54, /// 0xa3c
			 0x191af69e, /// 0xa40
			 0x614c59b2, /// 0xa44
			 0x1ced0e9b, /// 0xa48
			 0x53a46978, /// 0xa4c
			 0x48504aa5, /// 0xa50
			 0xfbb8e9b4, /// 0xa54
			 0x270cacc2, /// 0xa58
			 0x0f9393b0, /// 0xa5c
			 0x77312bd2, /// 0xa60
			 0x28202dd2, /// 0xa64
			 0xbccaa22e, /// 0xa68
			 0x616942da, /// 0xa6c
			 0x0972badb, /// 0xa70
			 0x7e6da86c, /// 0xa74
			 0xd2f086cb, /// 0xa78
			 0xd0aa5794, /// 0xa7c
			 0x5e6ebb2c, /// 0xa80
			 0xf9492b41, /// 0xa84
			 0x022a327e, /// 0xa88
			 0x4994c0fd, /// 0xa8c
			 0xdfe97e82, /// 0xa90
			 0x66364f06, /// 0xa94
			 0x2e7e8c7e, /// 0xa98
			 0x1145e78d, /// 0xa9c
			 0x328a002c, /// 0xaa0
			 0xd7002167, /// 0xaa4
			 0x1f90a827, /// 0xaa8
			 0xabd4661c, /// 0xaac
			 0x99b65dd1, /// 0xab0
			 0x39e7a627, /// 0xab4
			 0x85c020e8, /// 0xab8
			 0x9351b1c2, /// 0xabc
			 0x65358f84, /// 0xac0
			 0xd9f06f6c, /// 0xac4
			 0xed52e521, /// 0xac8
			 0xc64ce6af, /// 0xacc
			 0xeacd2b91, /// 0xad0
			 0x461939f0, /// 0xad4
			 0x71e8e6f5, /// 0xad8
			 0xab66aeb4, /// 0xadc
			 0x5358e9d1, /// 0xae0
			 0x301eb463, /// 0xae4
			 0xfc7709a8, /// 0xae8
			 0xcd335aa6, /// 0xaec
			 0x3fc11735, /// 0xaf0
			 0x59c2f313, /// 0xaf4
			 0x1bd1ae81, /// 0xaf8
			 0xf8e7cc1e, /// 0xafc
			 0x641968e5, /// 0xb00
			 0x15ebad93, /// 0xb04
			 0x2d933a90, /// 0xb08
			 0xe4b666aa, /// 0xb0c
			 0x08d613e1, /// 0xb10
			 0x8c6bdac6, /// 0xb14
			 0x759fd0ff, /// 0xb18
			 0xd7ec0bde, /// 0xb1c
			 0x6b56ebdd, /// 0xb20
			 0xfd741e33, /// 0xb24
			 0x0ac763dc, /// 0xb28
			 0x1e1daf7f, /// 0xb2c
			 0x92c98213, /// 0xb30
			 0x9680f8a2, /// 0xb34
			 0x9d18d376, /// 0xb38
			 0xf4ca334f, /// 0xb3c
			 0x790fc7a4, /// 0xb40
			 0x0d0c1d53, /// 0xb44
			 0x805b9fe8, /// 0xb48
			 0xbb87782a, /// 0xb4c
			 0xcbe16ea1, /// 0xb50
			 0xa4b89262, /// 0xb54
			 0xa263ef89, /// 0xb58
			 0x16cf371d, /// 0xb5c
			 0x831195bb, /// 0xb60
			 0x1c6cddf5, /// 0xb64
			 0x2fe4b67e, /// 0xb68
			 0x439123f8, /// 0xb6c
			 0x860e9bd0, /// 0xb70
			 0xef1e207a, /// 0xb74
			 0x8b89609f, /// 0xb78
			 0x16e41a38, /// 0xb7c
			 0xa2c5d0e0, /// 0xb80
			 0x913f065f, /// 0xb84
			 0xf9791738, /// 0xb88
			 0xb120c226, /// 0xb8c
			 0xbfbc0b09, /// 0xb90
			 0x84828dd1, /// 0xb94
			 0x6f5f14cd, /// 0xb98
			 0x680d7d4d, /// 0xb9c
			 0x759c4e46, /// 0xba0
			 0x7328a502, /// 0xba4
			 0xe4bd0dde, /// 0xba8
			 0x570406aa, /// 0xbac
			 0x52c02c8f, /// 0xbb0
			 0x702f6197, /// 0xbb4
			 0x62792737, /// 0xbb8
			 0xc6a1500f, /// 0xbbc
			 0xa64ddc22, /// 0xbc0
			 0xc872207b, /// 0xbc4
			 0xfd2665da, /// 0xbc8
			 0x9b1f2291, /// 0xbcc
			 0xb09d1abb, /// 0xbd0
			 0xb615e186, /// 0xbd4
			 0x8d78ffe7, /// 0xbd8
			 0xf234debf, /// 0xbdc
			 0x04a3aec2, /// 0xbe0
			 0x266764fa, /// 0xbe4
			 0xb1675f52, /// 0xbe8
			 0x96375d6c, /// 0xbec
			 0xbeb34f23, /// 0xbf0
			 0x1d89e276, /// 0xbf4
			 0xcc1ca9e0, /// 0xbf8
			 0x973fdcce, /// 0xbfc
			 0x0d0abd31, /// 0xc00
			 0x78125750, /// 0xc04
			 0x5af7482c, /// 0xc08
			 0x369c9938, /// 0xc0c
			 0xef4bdec7, /// 0xc10
			 0x11ed8fc6, /// 0xc14
			 0x61af97c8, /// 0xc18
			 0xfd82c4c1, /// 0xc1c
			 0xe10625fa, /// 0xc20
			 0x2cea6a53, /// 0xc24
			 0xe11f62ea, /// 0xc28
			 0x6c0dfa54, /// 0xc2c
			 0xd1f7c2ec, /// 0xc30
			 0x0a4fecca, /// 0xc34
			 0x45829dc3, /// 0xc38
			 0x61efe34d, /// 0xc3c
			 0x9ff316a6, /// 0xc40
			 0x838691d4, /// 0xc44
			 0xf0935828, /// 0xc48
			 0xbeff8205, /// 0xc4c
			 0xdcb270c7, /// 0xc50
			 0x26936e7e, /// 0xc54
			 0x54406910, /// 0xc58
			 0x38835b2d, /// 0xc5c
			 0x6e6bba8a, /// 0xc60
			 0x32a73d7d, /// 0xc64
			 0x7a69bd38, /// 0xc68
			 0x4dad9020, /// 0xc6c
			 0x0c6ba707, /// 0xc70
			 0xdf694c92, /// 0xc74
			 0xe842120b, /// 0xc78
			 0x631178c7, /// 0xc7c
			 0x8d3458c7, /// 0xc80
			 0x56c9346a, /// 0xc84
			 0x5c2a1afa, /// 0xc88
			 0xb4e98821, /// 0xc8c
			 0xa5914796, /// 0xc90
			 0x329c0cff, /// 0xc94
			 0x1d7af014, /// 0xc98
			 0x932de3f0, /// 0xc9c
			 0x5c9d57fc, /// 0xca0
			 0x1e76e1ad, /// 0xca4
			 0x04311040, /// 0xca8
			 0x77cf4dc3, /// 0xcac
			 0x37f57095, /// 0xcb0
			 0xe664bed4, /// 0xcb4
			 0x70fa57b9, /// 0xcb8
			 0x4c55f36e, /// 0xcbc
			 0xe4015180, /// 0xcc0
			 0x00918401, /// 0xcc4
			 0xa66ce717, /// 0xcc8
			 0x30c017f6, /// 0xccc
			 0xb317a200, /// 0xcd0
			 0x313397e5, /// 0xcd4
			 0x187834ab, /// 0xcd8
			 0xe6fb05bd, /// 0xcdc
			 0xf07e2afa, /// 0xce0
			 0x73790194, /// 0xce4
			 0x549619ee, /// 0xce8
			 0x718ed755, /// 0xcec
			 0xc6a353c2, /// 0xcf0
			 0x9478144e, /// 0xcf4
			 0x26f74d9c, /// 0xcf8
			 0xccc3fa90, /// 0xcfc
			 0x742f2375, /// 0xd00
			 0xf88ff8b8, /// 0xd04
			 0x2590e07b, /// 0xd08
			 0x4f664f97, /// 0xd0c
			 0x9d0de9f9, /// 0xd10
			 0x5e8aeb8a, /// 0xd14
			 0xb013494f, /// 0xd18
			 0x04c33299, /// 0xd1c
			 0x373beaf8, /// 0xd20
			 0x2b2a8e58, /// 0xd24
			 0x5acc2afe, /// 0xd28
			 0xa5653a18, /// 0xd2c
			 0x32df6d1e, /// 0xd30
			 0xde4f2256, /// 0xd34
			 0xa925a657, /// 0xd38
			 0x01ce7bbc, /// 0xd3c
			 0xaafac5fe, /// 0xd40
			 0x9abe57bd, /// 0xd44
			 0x58df3c26, /// 0xd48
			 0xad247f1d, /// 0xd4c
			 0x90c87cc9, /// 0xd50
			 0xc7e5f0a9, /// 0xd54
			 0x9641e453, /// 0xd58
			 0x000191d0, /// 0xd5c
			 0xc859d2ea, /// 0xd60
			 0xa6c1fbcc, /// 0xd64
			 0x59270556, /// 0xd68
			 0xc7bd2963, /// 0xd6c
			 0x3e128f7d, /// 0xd70
			 0x67fa56a4, /// 0xd74
			 0xd9fa947e, /// 0xd78
			 0x5f700fdc, /// 0xd7c
			 0xb8c7978a, /// 0xd80
			 0xf563f2c9, /// 0xd84
			 0x22f92ec8, /// 0xd88
			 0xf6e97cb5, /// 0xd8c
			 0xd9f92acb, /// 0xd90
			 0x97980b30, /// 0xd94
			 0x3dde10b5, /// 0xd98
			 0xc40af4db, /// 0xd9c
			 0x77da168a, /// 0xda0
			 0x9c6a6af2, /// 0xda4
			 0x3c9e979e, /// 0xda8
			 0xa1f5a396, /// 0xdac
			 0x4fe2f90e, /// 0xdb0
			 0x7491a13d, /// 0xdb4
			 0xe1ce469f, /// 0xdb8
			 0x33f52339, /// 0xdbc
			 0x6102141d, /// 0xdc0
			 0xa245af06, /// 0xdc4
			 0x8e5b4a19, /// 0xdc8
			 0x9ace7a25, /// 0xdcc
			 0x1c363a66, /// 0xdd0
			 0x9b29a8e3, /// 0xdd4
			 0xd8ac0793, /// 0xdd8
			 0xf8691cbf, /// 0xddc
			 0x0105baa0, /// 0xde0
			 0xcc7f7e2d, /// 0xde4
			 0x6170d3ba, /// 0xde8
			 0x76d420bc, /// 0xdec
			 0x88f4d882, /// 0xdf0
			 0x485b9394, /// 0xdf4
			 0xeacaf628, /// 0xdf8
			 0x108380bc, /// 0xdfc
			 0x23481ec5, /// 0xe00
			 0x8d59d521, /// 0xe04
			 0x2d0bc798, /// 0xe08
			 0x082e6317, /// 0xe0c
			 0x9dd8a54c, /// 0xe10
			 0x6e387f0c, /// 0xe14
			 0x6624d910, /// 0xe18
			 0xdc9656d3, /// 0xe1c
			 0x0e9675f3, /// 0xe20
			 0x3688db16, /// 0xe24
			 0x35c71cd9, /// 0xe28
			 0x29527d38, /// 0xe2c
			 0x69257458, /// 0xe30
			 0x1c2a8e93, /// 0xe34
			 0x0c7c440e, /// 0xe38
			 0x46210b89, /// 0xe3c
			 0x2468c509, /// 0xe40
			 0x9bcf178a, /// 0xe44
			 0xb464119c, /// 0xe48
			 0x9fbefe22, /// 0xe4c
			 0x1bda6775, /// 0xe50
			 0x610ccaab, /// 0xe54
			 0xbd555834, /// 0xe58
			 0x21bb858b, /// 0xe5c
			 0x72a0327e, /// 0xe60
			 0xa17bb7ec, /// 0xe64
			 0xaf74a28b, /// 0xe68
			 0x7d14c674, /// 0xe6c
			 0x1da46570, /// 0xe70
			 0x80137715, /// 0xe74
			 0xbfa22908, /// 0xe78
			 0x305cd0a3, /// 0xe7c
			 0xd0e1081f, /// 0xe80
			 0x8e0e066b, /// 0xe84
			 0xfbb7be3a, /// 0xe88
			 0x6f6624e6, /// 0xe8c
			 0xaf3c4bd7, /// 0xe90
			 0x3364308a, /// 0xe94
			 0x0c7ab261, /// 0xe98
			 0xa2bd8c91, /// 0xe9c
			 0x03afe2db, /// 0xea0
			 0x18d74bc4, /// 0xea4
			 0x9234be81, /// 0xea8
			 0x669ba0f9, /// 0xeac
			 0x72a2edad, /// 0xeb0
			 0x0c885693, /// 0xeb4
			 0xe11c52fa, /// 0xeb8
			 0x619c0eb6, /// 0xebc
			 0x45564692, /// 0xec0
			 0x81009ea3, /// 0xec4
			 0x78ac51a5, /// 0xec8
			 0x53d7d966, /// 0xecc
			 0x7e9faca9, /// 0xed0
			 0xff650114, /// 0xed4
			 0x03879180, /// 0xed8
			 0x91eb70e2, /// 0xedc
			 0x6086b0f7, /// 0xee0
			 0x299e3e2c, /// 0xee4
			 0x7e680bd9, /// 0xee8
			 0xe25f5826, /// 0xeec
			 0x2591164f, /// 0xef0
			 0x0ba4a4bc, /// 0xef4
			 0x05616d63, /// 0xef8
			 0x43c4d691, /// 0xefc
			 0x32f30742, /// 0xf00
			 0x0710791e, /// 0xf04
			 0x907616e7, /// 0xf08
			 0xfee87296, /// 0xf0c
			 0xdecce592, /// 0xf10
			 0x8c1d70b8, /// 0xf14
			 0x093e552c, /// 0xf18
			 0xa2877fd4, /// 0xf1c
			 0x59461871, /// 0xf20
			 0x2b89a70e, /// 0xf24
			 0xdf87c4d1, /// 0xf28
			 0x539e3251, /// 0xf2c
			 0x408501e7, /// 0xf30
			 0x96aeb95f, /// 0xf34
			 0x61a826ba, /// 0xf38
			 0x8ecf9ebb, /// 0xf3c
			 0x6f16cf44, /// 0xf40
			 0xfb1ccec2, /// 0xf44
			 0xee329812, /// 0xf48
			 0x8b3b9e91, /// 0xf4c
			 0xe89c6988, /// 0xf50
			 0xd41d892b, /// 0xf54
			 0xcd00a516, /// 0xf58
			 0x2d539d8e, /// 0xf5c
			 0x1f1c1d49, /// 0xf60
			 0x4836e227, /// 0xf64
			 0x43133b91, /// 0xf68
			 0xcf668fb6, /// 0xf6c
			 0x0097f065, /// 0xf70
			 0xd9e98f4d, /// 0xf74
			 0xb801c9aa, /// 0xf78
			 0x12d21559, /// 0xf7c
			 0x0d596d80, /// 0xf80
			 0x230cfe3b, /// 0xf84
			 0xb62dcf77, /// 0xf88
			 0x6d0aabfb, /// 0xf8c
			 0x521fa97d, /// 0xf90
			 0xc04789e2, /// 0xf94
			 0xabca9413, /// 0xf98
			 0x71a10c5a, /// 0xf9c
			 0xce10ab6e, /// 0xfa0
			 0x31cd1160, /// 0xfa4
			 0x0b9a7b8c, /// 0xfa8
			 0x1aa249ff, /// 0xfac
			 0x0bbf5472, /// 0xfb0
			 0x2330b5b5, /// 0xfb4
			 0xea981fb9, /// 0xfb8
			 0x000fa09f, /// 0xfbc
			 0x5459ccf9, /// 0xfc0
			 0x151a0228, /// 0xfc4
			 0x6d09da46, /// 0xfc8
			 0xcbe8bfc3, /// 0xfcc
			 0x1fdda150, /// 0xfd0
			 0x0300c68c, /// 0xfd4
			 0xb367de6b, /// 0xfd8
			 0x2e3b45fb, /// 0xfdc
			 0x7deea689, /// 0xfe0
			 0xd8757fb6, /// 0xfe4
			 0xa82f861e, /// 0xfe8
			 0x5f42a6d2, /// 0xfec
			 0xa2cae42b, /// 0xff0
			 0x7c64cb49, /// 0xff4
			 0xc524a1af, /// 0xff8
			 0x2f6a1629 /// last
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
			 0x00000094,
			 0x000001a4,
			 0x000006f4,
			 0x000005cc,
			 0x000003dc,
			 0x000001d0,
			 0x0000077c,
			 0x000005f4,

			 /// vpu register f2
			 0x41c00000,
			 0x41a00000,
			 0x41500000,
			 0x41b80000,
			 0x41980000,
			 0x41f80000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f3
			 0x42000000,
			 0x41d00000,
			 0x41c80000,
			 0x41980000,
			 0x40800000,
			 0x41980000,
			 0x40a00000,
			 0x41e00000,

			 /// vpu register f4
			 0x41880000,
			 0x41a00000,
			 0x41a80000,
			 0x41a00000,
			 0x41f80000,
			 0x41d80000,
			 0x40800000,
			 0x41400000,

			 /// vpu register f5
			 0x41b80000,
			 0x41e80000,
			 0x40400000,
			 0x41300000,
			 0x41b00000,
			 0x40c00000,
			 0x41800000,
			 0x41c80000,

			 /// vpu register f6
			 0x41980000,
			 0x41d80000,
			 0x41200000,
			 0x41600000,
			 0x41300000,
			 0x41900000,
			 0x41d00000,
			 0x41100000,

			 /// vpu register f7
			 0x41980000,
			 0x41d80000,
			 0x40000000,
			 0x40e00000,
			 0x42000000,
			 0x41100000,
			 0x41700000,
			 0x41000000,

			 /// vpu register f8
			 0x41b80000,
			 0x41e00000,
			 0x40c00000,
			 0x41900000,
			 0x41000000,
			 0x41100000,
			 0x41e00000,
			 0x41000000,

			 /// vpu register f9
			 0x41200000,
			 0x42000000,
			 0x41a00000,
			 0x41600000,
			 0x41880000,
			 0x41a80000,
			 0x41c80000,
			 0x41e00000,

			 /// vpu register f10
			 0x41400000,
			 0x41500000,
			 0x41a00000,
			 0x41b00000,
			 0x41f80000,
			 0x41e80000,
			 0x3f800000,
			 0x41300000,

			 /// vpu register f11
			 0x41f80000,
			 0x41200000,
			 0x41880000,
			 0x41f80000,
			 0x40c00000,
			 0x40c00000,
			 0x41c80000,
			 0x41b80000,

			 /// vpu register f12
			 0x41c00000,
			 0x41f00000,
			 0x41100000,
			 0x41f80000,
			 0x41d00000,
			 0x41100000,
			 0x40800000,
			 0x41000000,

			 /// vpu register f13
			 0x40a00000,
			 0x41100000,
			 0x3f800000,
			 0x41400000,
			 0x41100000,
			 0x41d00000,
			 0x41880000,
			 0x41c80000,

			 /// vpu register f14
			 0x41c00000,
			 0x41b80000,
			 0x41880000,
			 0x41b80000,
			 0x41b80000,
			 0x41f80000,
			 0x40400000,
			 0x41d00000,

			 /// vpu register f15
			 0x41a00000,
			 0x3f800000,
			 0x41d80000,
			 0x41c80000,
			 0x40800000,
			 0x41b80000,
			 0x41e80000,
			 0x41e00000,

			 /// vpu register f16
			 0x41a80000,
			 0x40800000,
			 0x3f800000,
			 0x41f80000,
			 0x40e00000,
			 0x40000000,
			 0x41a80000,
			 0x41500000,

			 /// vpu register f17
			 0x41200000,
			 0x40000000,
			 0x41e80000,
			 0x41e00000,
			 0x41b80000,
			 0x41d00000,
			 0x41e00000,
			 0x41600000,

			 /// vpu register f18
			 0x41f80000,
			 0x41d80000,
			 0x40000000,
			 0x41d80000,
			 0x41000000,
			 0x41200000,
			 0x41400000,
			 0x41f80000,

			 /// vpu register f19
			 0x41f00000,
			 0x41700000,
			 0x41600000,
			 0x41d00000,
			 0x40e00000,
			 0x41300000,
			 0x41800000,
			 0x41600000,

			 /// vpu register f20
			 0x41880000,
			 0x41880000,
			 0x3f800000,
			 0x41e80000,
			 0x40400000,
			 0x41400000,
			 0x40c00000,
			 0x41500000,

			 /// vpu register f21
			 0x41200000,
			 0x41c80000,
			 0x41e80000,
			 0x41600000,
			 0x3f800000,
			 0x41500000,
			 0x41600000,
			 0x41d80000,

			 /// vpu register f22
			 0x41300000,
			 0x41d00000,
			 0x41700000,
			 0x40a00000,
			 0x40400000,
			 0x41a80000,
			 0x41a80000,
			 0x41000000,

			 /// vpu register f23
			 0x41a80000,
			 0x41b00000,
			 0x41500000,
			 0x41e00000,
			 0x41400000,
			 0x41b80000,
			 0x41500000,
			 0x41d00000,

			 /// vpu register f24
			 0x41800000,
			 0x41e80000,
			 0x41e80000,
			 0x41100000,
			 0x41000000,
			 0x40400000,
			 0x41c80000,
			 0x41800000,

			 /// vpu register f25
			 0x40a00000,
			 0x41000000,
			 0x41100000,
			 0x41000000,
			 0x41e80000,
			 0x41880000,
			 0x41980000,
			 0x41c80000,

			 /// vpu register f26
			 0x41100000,
			 0x41a00000,
			 0x41b00000,
			 0x41000000,
			 0x41f00000,
			 0x40e00000,
			 0x41f80000,
			 0x41600000,

			 /// vpu register f27
			 0x41e80000,
			 0x40000000,
			 0x41980000,
			 0x3f800000,
			 0x41200000,
			 0x41980000,
			 0x41700000,
			 0x41700000,

			 /// vpu register f28
			 0x41000000,
			 0x40800000,
			 0x41600000,
			 0x40400000,
			 0x41300000,
			 0x41b00000,
			 0x41d80000,
			 0x41c80000,

			 /// vpu register f29
			 0x41a80000,
			 0x41100000,
			 0x40e00000,
			 0x41980000,
			 0x40a00000,
			 0x40c00000,
			 0x41400000,
			 0x40c00000,

			 /// vpu register f30
			 0x42000000,
			 0x41a80000,
			 0x41b00000,
			 0x41b00000,
			 0x41c80000,
			 0x40c00000,
			 0x40e00000,
			 0x41980000,

			 /// vpu register f31
			 0x41f00000,
			 0x41400000,
			 0x41a00000,
			 0x41100000,
			 0x40800000,
			 0x41c00000,
			 0x41300000,
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
		"fle.pi f4, f30, f1\n"                                ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f10, f29, f8\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f10, f10, f13\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f11, f23, f17\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f8, f21, f19\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f17, f8, f27\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f16, f8, f6\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f12, f9, f15\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f7, f3, f9\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f21, f0, f15\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f21, f30, f13\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f3, f2, f11\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f27, f15, f2\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f8, f15, f15\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f16, f13, f25\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f28, f30, f12\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f22, f1, f0\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f7, f3, f11\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f23, f24, f26\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f22, f13, f7\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f28, f11, f6\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f12, f17\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f15, f14, f4\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f27, f4, f28\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f7, f22\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f27, f11, f5\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f5, f26, f12\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f28, f19, f28\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f7, f20, f11\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f10, f19, f13\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f16, f0\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f28, f0, f3\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f23, f1, f21\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f2, f9, f14\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f7, f17, f1\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f30, f15\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f29, f22, f24\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f21, f26, f26\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f22, f28, f16\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f11, f25, f23\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f12, f20, f7\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f9, f24, f13\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f12, f13, f1\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f20, f30, f17\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f15, f24, f0\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f5, f0, f12\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f18, f22, f4\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f12, f12, f29\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f17, f23, f26\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f3, f13, f30\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f10, f27\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f16, f7, f23\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f25, f26, f6\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f11, f7, f22\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f5, f24, f10\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f21, f24, f7\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f26, f28, f28\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f26, f8, f9\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f24, f19, f4\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f2, f16, f12\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f16, f12, f0\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f6, f6, f11\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f5, f21, f25\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f15, f0, f28\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f23, f25, f20\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f24, f27, f3\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f12, f23, f8\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f6, f18, f0\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f4, f6\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f29, f15, f0\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f25, f21, f28\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f5, f1, f0\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f22, f6, f2\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f6, f10, f26\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f28, f18, f30\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f19, f13, f10\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f2, f1, f8\n"                                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f12, f7, f22\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f19, f22, f12\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f20, f23, f5\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f7, f16, f19\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f21, f15, f30\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f23, f2, f20\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f11, f29, f14\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f4, f18, f5\n"                                ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f9, f19, f30\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f4, f2, f24\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f23, f6, f29\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f14, f16, f28\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f17, f5, f6\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f9, f25, f28\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f16, f1, f4\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f30, f8, f26\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f16, f13, f20\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f26, f17, f11\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f9, f4, f21\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f19, f19, f0\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f29, f17, f15\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.pi f13, f5, f11\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
