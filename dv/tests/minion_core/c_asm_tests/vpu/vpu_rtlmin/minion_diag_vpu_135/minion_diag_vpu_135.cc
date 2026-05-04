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
			 0x7fc00001,                                                  // signaling NaN                               /// 00000
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00004
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00008
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x7f800000,                                                  // inf                                         /// 00010
			 0x00011111,                                                  // 9.7958E-41                                  /// 00014
			 0x33333333,                                                  // 4 random values                             /// 00018
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0001c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00020
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00028
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0002c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00030
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00034
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00038
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0003c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00048
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0004c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00050
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00054
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00058
			 0x80000000,                                                  // -zero                                       /// 0005c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00064
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00068
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0006c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00070
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00088
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00090
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00098
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0009c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00100
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00104
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00110
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0011c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00128
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00130
			 0x7f800000,                                                  // inf                                         /// 00134
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00138
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0013c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00150
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00158
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00160
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00164
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00170
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00174
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0017c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00180
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00184
			 0x80000000,                                                  // -zero                                       /// 00188
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0018c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00190
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00194
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00198
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x55555555,                                                  // 4 random values                             /// 001e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00204
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00210
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0021c
			 0xbf028f5c,                                                  // -0.51                                       /// 00220
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00224
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0022c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00230
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00240
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00244
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00248
			 0x00000000,                                                  // zero                                        /// 0024c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00250
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00254
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00258
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x00000000,                                                  // zero                                        /// 00260
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00268
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0026c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00274
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0027c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00280
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00288
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x0c400000,                                                  // Leading 1s:                                 /// 00294
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00298
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0029c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00000000,                                                  // zero                                        /// 002b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0xffc00001,                                                  // -signaling NaN                              /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00304
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00308
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0030c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00314
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00318
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00320
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00324
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0032c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00330
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00334
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0033c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00340
			 0xff800000,                                                  // -inf                                        /// 00344
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00348
			 0x80011111,                                                  // -9.7958E-41                                 /// 0034c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00354
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00358
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00011111,                                                  // 9.7958E-41                                  /// 00364
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00378
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0037c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00388
			 0x0e000001,                                                  // Trailing 1s:                                /// 0038c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00394
			 0x7fc00001,                                                  // signaling NaN                               /// 00398
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0039c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0xffc00001,                                                  // -signaling NaN                              /// 003e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00404
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00408
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00414
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00420
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00428
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0042c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0044c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00450
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00454
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00458
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0045c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00464
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00468
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00470
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00474
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0047c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00480
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00488
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0049c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00500
			 0xcb000000,                                                  // -8388608.0                                  /// 00504
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00508
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0050c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00510
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00514
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00518
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0051c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00520
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00524
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00528
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0052c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00534
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00538
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0053c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00540
			 0x80000000,                                                  // -zero                                       /// 00544
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x0e000007,                                                  // Trailing 1s:                                /// 0054c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00550
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00554
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0056c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00570
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00574
			 0x4b000000,                                                  // 8388608.0                                   /// 00578
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00580
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00590
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00594
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00598
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0059c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005c4
			 0xbf028f5c,                                                  // -0.51                                       /// 005c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00600
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0060c
			 0xcb000000,                                                  // -8388608.0                                  /// 00610
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00614
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00620
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x4b000000,                                                  // 8388608.0                                   /// 00628
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0062c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00630
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00634
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00638
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0063c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00640
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00648
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0064c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x33333333,                                                  // 4 random values                             /// 00658
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0065c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0xbf028f5c,                                                  // -0.51                                       /// 00664
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00674
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00684
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00688
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0068c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00694
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00698
			 0x0c780000,                                                  // Leading 1s:                                 /// 0069c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b4
			 0x3f028f5c,                                                  // 0.51                                        /// 006b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x55555555,                                                  // 4 random values                             /// 006c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00700
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00704
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0070c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00710
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00714
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00718
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00724
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00734
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0073c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00740
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00744
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00748
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00750
			 0x80011111,                                                  // -9.7958E-41                                 /// 00754
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00758
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00764
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00770
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0xffc00001,                                                  // -signaling NaN                              /// 00778
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0077c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00784
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00788
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00790
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00794
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00798
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0xcb000000,                                                  // -8388608.0                                  /// 007b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x3f028f5c,                                                  // 0.51                                        /// 007f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00804
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00810
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00814
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x4b000000,                                                  // 8388608.0                                   /// 0081c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00820
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00824
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00828
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00830
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00834
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80011111,                                                  // -9.7958E-41                                 /// 0083c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00840
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00850
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00854
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0085c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00860
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00870
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00874
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00878
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0087c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00880
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00888
			 0x7f800000,                                                  // inf                                         /// 0088c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00898
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0089c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00904
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00910
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00914
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00924
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0xffc00001,                                                  // -signaling NaN                              /// 00934
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xbf028f5c,                                                  // -0.51                                       /// 00944
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0094c
			 0xbf028f5c,                                                  // -0.51                                       /// 00950
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0095c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00960
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00964
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0096c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00970
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00974
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00978
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00980
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0098c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0099c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009d0
			 0xff800000,                                                  // -inf                                        /// 009d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x7fc00001,                                                  // signaling NaN                               /// 009e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0xffc00001,                                                  // -signaling NaN                              /// 00a44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x7fc00001,                                                  // signaling NaN                               /// 00a84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x33333333,                                                  // 4 random values                             /// 00aa4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ab0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ab4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ac4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00acc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00adc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00afc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b44
			 0xcb000000,                                                  // -8388608.0                                  /// 00b48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b58
			 0x00000000,                                                  // zero                                        /// 00b5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b80
			 0xcb000000,                                                  // -8388608.0                                  /// 00b84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xcb000000,                                                  // -8388608.0                                  /// 00bc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00be4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00be8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c04
			 0xbf028f5c,                                                  // -0.51                                       /// 00c08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c4c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x3f028f5c,                                                  // 0.51                                        /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00ca0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ca8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cb0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ccc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x33333333,                                                  // 4 random values                             /// 00cdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf0
			 0x33333333,                                                  // 4 random values                             /// 00cf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d0c
			 0x55555555,                                                  // 4 random values                             /// 00d10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d14
			 0x3f028f5c,                                                  // 0.51                                        /// 00d18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d20
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d2c
			 0x55555555,                                                  // 4 random values                             /// 00d30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7f800000,                                                  // inf                                         /// 00d94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00da4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00de4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00df0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e5c
			 0x00000000,                                                  // zero                                        /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xbf028f5c,                                                  // -0.51                                       /// 00e8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ea8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ecc
			 0x7f800000,                                                  // inf                                         /// 00ed0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ed4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00edc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ef0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xbf028f5c,                                                  // -0.51                                       /// 00f24
			 0x4b000000,                                                  // 8388608.0                                   /// 00f28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x7f800000,                                                  // inf                                         /// 00f54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0xff800000,                                                  // -inf                                        /// 00f60
			 0xff800000,                                                  // -inf                                        /// 00f64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fbc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fcc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff8
			 0xff7ffffe // max norm - 3ulp                               // max norm -ve                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00004
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c400000,                                                  // Leading 1s:                                 /// 0000c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00010
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00018
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0001c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00024
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c700000,                                                  // Leading 1s:                                 /// 0002c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00038
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00044
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0004c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00050
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00054
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00058
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0005c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0006c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00070
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00078
			 0x0c600000,                                                  // Leading 1s:                                 /// 0007c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00084
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00088
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00098
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0xffc00001,                                                  // -signaling NaN                              /// 000e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00100
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00104
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00108
			 0x80000000,                                                  // -zero                                       /// 0010c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00114
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0011c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00120
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00124
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00128
			 0xff800000,                                                  // -inf                                        /// 0012c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00130
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00011111,                                                  // 9.7958E-41                                  /// 00138
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00144
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0014c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00150
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00154
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00160
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00164
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00168
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0017c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00188
			 0x80011111,                                                  // -9.7958E-41                                 /// 0018c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0019c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00200
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0020c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00210
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00218
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0021c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00220
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00224
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00228
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00234
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00238
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0023c
			 0xff800000,                                                  // -inf                                        /// 00240
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00244
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00248
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00250
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00258
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00260
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00264
			 0x33333333,                                                  // 4 random values                             /// 00268
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0xffc00001,                                                  // -signaling NaN                              /// 00270
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00274
			 0xffc00001,                                                  // -signaling NaN                              /// 00278
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0027c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00294
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00298
			 0xff800000,                                                  // -inf                                        /// 0029c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002bc
			 0xffc00001,                                                  // -signaling NaN                              /// 002c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00308
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0030c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00318
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0xcb000000,                                                  // -8388608.0                                  /// 00324
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00328
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0032c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00330
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00334
			 0x00000000,                                                  // zero                                        /// 00338
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0033c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00340
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00344
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00348
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0c600000,                                                  // Leading 1s:                                 /// 0036c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00370
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00374
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00378
			 0x33333333,                                                  // 4 random values                             /// 0037c
			 0x4b000000,                                                  // 8388608.0                                   /// 00380
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00384
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00390
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00394
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00398
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0039c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80011111,                                                  // -9.7958E-41                                 /// 00404
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0040c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00410
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00418
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0041c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00420
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00424
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00428
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0042c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00430
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00434
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0043c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0044c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00450
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00454
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00458
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00460
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00464
			 0xbf028f5c,                                                  // -0.51                                       /// 00468
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0046c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00470
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00478
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0047c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00480
			 0xffc00001,                                                  // -signaling NaN                              /// 00484
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0048c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00490
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00494
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00498
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x7f800000,                                                  // inf                                         /// 004a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00500
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00508
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00510
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00518
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00520
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00524
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00528
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0052c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00530
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00538
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0053c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00544
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00548
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0xbf028f5c,                                                  // -0.51                                       /// 00550
			 0x0c700000,                                                  // Leading 1s:                                 /// 00554
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0055c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00560
			 0x00011111,                                                  // 9.7958E-41                                  /// 00564
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00568
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00570
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00574
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00578
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00584
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xffc00001,                                                  // -signaling NaN                              /// 0058c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00590
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00594
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00598
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0059c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00600
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00604
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00608
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0060c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00614
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0061c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00628
			 0x0c700000,                                                  // Leading 1s:                                 /// 0062c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00630
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00634
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00638
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0c780000,                                                  // Leading 1s:                                 /// 00644
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0064c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0xffc00001,                                                  // -signaling NaN                              /// 00658
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00660
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00664
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0066c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00674
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00678
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0067c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00688
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0068c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x55555555,                                                  // 4 random values                             /// 00698
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00700
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00704
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00708
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00718
			 0x0c400000,                                                  // Leading 1s:                                 /// 0071c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00720
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00724
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00728
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0072c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e000007,                                                  // Trailing 1s:                                /// 00734
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00738
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00740
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00744
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00748
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0074c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00750
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00754
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0075c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00760
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00764
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00770
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00778
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0077c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00780
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00784
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00788
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00790
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0079c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x7fc00001,                                                  // signaling NaN                               /// 007b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007e0
			 0xcb000000,                                                  // -8388608.0                                  /// 007e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00804
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00808
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00810
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00814
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00818
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0081c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00820
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00824
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00828
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0082c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00830
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00838
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00840
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00844
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00848
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0084c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00860
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00864
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00868
			 0x80011111,                                                  // -9.7958E-41                                 /// 0086c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00870
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00874
			 0x80000000,                                                  // -zero                                       /// 00878
			 0x0c600000,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00880
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00884
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00888
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0088c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00890
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00894
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0089c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x55555555,                                                  // 4 random values                             /// 008b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008dc
			 0x80000000,                                                  // -zero                                       /// 008e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x7f800000,                                                  // inf                                         /// 00908
			 0xffc00001,                                                  // -signaling NaN                              /// 0090c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00914
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00918
			 0x80000000,                                                  // -zero                                       /// 0091c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00920
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xbf028f5c,                                                  // -0.51                                       /// 00928
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00934
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00938
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0093c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00944
			 0x4b000000,                                                  // 8388608.0                                   /// 00948
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0094c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00954
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00958
			 0x33333333,                                                  // 4 random values                             /// 0095c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00960
			 0x80011111,                                                  // -9.7958E-41                                 /// 00964
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00968
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0096c
			 0x4b000000,                                                  // 8388608.0                                   /// 00970
			 0x7f800000,                                                  // inf                                         /// 00974
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00978
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00988
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0098c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00994
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0099c
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b4
			 0x3f028f5c,                                                  // 0.51                                        /// 009b8
			 0xffc00001,                                                  // -signaling NaN                              /// 009bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0xffc00001,                                                  // -signaling NaN                              /// 00a08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x55555555,                                                  // 4 random values                             /// 00a3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x4b000000,                                                  // 8388608.0                                   /// 00a74
			 0x4b000000,                                                  // 8388608.0                                   /// 00a78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00aa4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ab0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00abc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ac4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ac8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00acc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00adc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ae0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ae8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00af4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7fc00001,                                                  // signaling NaN                               /// 00b08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b14
			 0x7fc00001,                                                  // signaling NaN                               /// 00b18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000000,                                                  // zero                                        /// 00b88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x80000000,                                                  // -zero                                       /// 00b94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb0
			 0x80000000,                                                  // -zero                                       /// 00bb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb8
			 0x7f800000,                                                  // inf                                         /// 00bbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0xff800000,                                                  // -inf                                        /// 00be0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c10
			 0x00000000,                                                  // zero                                        /// 00c14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c48
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c70
			 0x7fc00001,                                                  // signaling NaN                               /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x3f028f5c,                                                  // 0.51                                        /// 00c94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cc4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cc8
			 0x33333333,                                                  // 4 random values                             /// 00ccc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x55555555,                                                  // 4 random values                             /// 00cd4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ce8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d58
			 0xbf028f5c,                                                  // -0.51                                       /// 00d5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d80
			 0x3f028f5c,                                                  // 0.51                                        /// 00d84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00da4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da8
			 0x7f800000,                                                  // inf                                         /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ddc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dfc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x7f800000,                                                  // inf                                         /// 00e0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0xff800000,                                                  // -inf                                        /// 00e14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x3f028f5c,                                                  // 0.51                                        /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ea0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00eac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ebc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ecc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ef8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f00
			 0x3f028f5c,                                                  // 0.51                                        /// 00f04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x7f800000,                                                  // inf                                         /// 00f10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x55555555,                                                  // 4 random values                             /// 00f3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f44
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f54
			 0xcb000000,                                                  // -8388608.0                                  /// 00f58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80000000,                                                  // -zero                                       /// 00fa8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fcc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff8
			 0x0e0000ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00000
			 0x0e000001,                                                  // Trailing 1s:                                /// 00004
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00010
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00014
			 0x0e000007,                                                  // Trailing 1s:                                /// 00018
			 0x0e000007,                                                  // Trailing 1s:                                /// 0001c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00024
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00028
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00011111,                                                  // 9.7958E-41                                  /// 00034
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00040
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00044
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00048
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00050
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00054
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00058
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0005c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00060
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00068
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00070
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00074
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00080
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00084
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00090
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00094
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00098
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0009c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00100
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00104
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00108
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0010c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00110
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00120
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00124
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x33333333,                                                  // 4 random values                             /// 0012c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00134
			 0xff800000,                                                  // -inf                                        /// 00138
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0013c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00144
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00148
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00158
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00160
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00164
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00168
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0016c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00170
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00174
			 0x80000000,                                                  // -zero                                       /// 00178
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00180
			 0xff800000,                                                  // -inf                                        /// 00184
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00188
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0018c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00198
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0019c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x7f800000,                                                  // inf                                         /// 001b4
			 0x80000000,                                                  // -zero                                       /// 001b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001f4
			 0x33333333,                                                  // 4 random values                             /// 001f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00200
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0020c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0021c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00220
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00224
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00228
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0022c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00234
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0023c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00244
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0024c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00250
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00254
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00264
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00268
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00278
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0027c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00280
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00284
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00290
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00298
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0029c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 002b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c8
			 0x7fc00001,                                                  // signaling NaN                               /// 002cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00300
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0030c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00310
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00318
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0031c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00324
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00328
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0032c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00330
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00334
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00338
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0033c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00344
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00350
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0035c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00360
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00364
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00378
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0xff800000,                                                  // -inf                                        /// 00388
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0038c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00390
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00394
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00398
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003a4
			 0x7f800000,                                                  // inf                                         /// 003a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003cc
			 0xcb000000,                                                  // -8388608.0                                  /// 003d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e0
			 0x80000000,                                                  // -zero                                       /// 003e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00400
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00408
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0040c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00410
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00424
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00430
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0043c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00440
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00448
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0044c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00450
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0045c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00460
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00468
			 0x3f028f5c,                                                  // 0.51                                        /// 0046c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00474
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00478
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0047c
			 0xcb000000,                                                  // -8388608.0                                  /// 00480
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00488
			 0xcb000000,                                                  // -8388608.0                                  /// 0048c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00490
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00498
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0049c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004a8
			 0x80000000,                                                  // -zero                                       /// 004ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00500
			 0x4b000000,                                                  // 8388608.0                                   /// 00504
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00508
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0050c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00514
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00518
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0051c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00520
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00524
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00528
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0052c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00530
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0053c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00544
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0054c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0055c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00560
			 0x00000000,                                                  // zero                                        /// 00564
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00568
			 0x00000000,                                                  // zero                                        /// 0056c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00570
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00584
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00588
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00590
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x0c600000,                                                  // Leading 1s:                                 /// 00598
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00604
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00610
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00618
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0061c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00620
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00624
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00630
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00634
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00638
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0063c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00640
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00644
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00648
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0064c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00654
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0065c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00660
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00668
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00670
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x0c780000,                                                  // Leading 1s:                                 /// 00678
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00680
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00684
			 0xff800000,                                                  // -inf                                        /// 00688
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0068c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00690
			 0xffc00001,                                                  // -signaling NaN                              /// 00694
			 0x00011111,                                                  // 9.7958E-41                                  /// 00698
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00700
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00704
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00718
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0071c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0073c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00744
			 0x4b000000,                                                  // 8388608.0                                   /// 00748
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0074c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00750
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00754
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00758
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0075c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00760
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00764
			 0x00011111,                                                  // 9.7958E-41                                  /// 00768
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00774
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0077c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00780
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00788
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0078c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00790
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xffc00001,                                                  // -signaling NaN                              /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xff800000,                                                  // -inf                                        /// 007d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00800
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00804
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00808
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0080c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00810
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00814
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00820
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0082c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0083c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00840
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00844
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00848
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0084c
			 0x00000000,                                                  // zero                                        /// 00850
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00854
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x80000000,                                                  // -zero                                       /// 00860
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00864
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00868
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0086c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0087c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00880
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00888
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0088c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00890
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00894
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00898
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0089c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x33333333,                                                  // 4 random values                             /// 008d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00900
			 0x7fc00001,                                                  // signaling NaN                               /// 00904
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0090c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00910
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0xcb000000,                                                  // -8388608.0                                  /// 00918
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0091c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00928
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0092c
			 0x55555555,                                                  // 4 random values                             /// 00930
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00934
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x33333333,                                                  // 4 random values                             /// 0093c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00940
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00944
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00948
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00954
			 0x0c400000,                                                  // Leading 1s:                                 /// 00958
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0095c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00960
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00964
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00970
			 0x7fc00001,                                                  // signaling NaN                               /// 00974
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00984
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0098c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00990
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00994
			 0x80000000,                                                  // -zero                                       /// 00998
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c0
			 0xff800000,                                                  // -inf                                        /// 009c4
			 0x80000000,                                                  // -zero                                       /// 009c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009cc
			 0x55555555,                                                  // 4 random values                             /// 009d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a70
			 0x33333333,                                                  // 4 random values                             /// 00a74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00adc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ae4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x7fc00001,                                                  // signaling NaN                               /// 00aec
			 0x55555555,                                                  // 4 random values                             /// 00af0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af4
			 0x3f028f5c,                                                  // 0.51                                        /// 00af8
			 0x4b000000,                                                  // 8388608.0                                   /// 00afc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b04
			 0xbf028f5c,                                                  // -0.51                                       /// 00b08
			 0xbf028f5c,                                                  // -0.51                                       /// 00b0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b48
			 0xff800000,                                                  // -inf                                        /// 00b4c
			 0x33333333,                                                  // 4 random values                             /// 00b50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0xcb000000,                                                  // -8388608.0                                  /// 00b94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bb4
			 0xff800000,                                                  // -inf                                        /// 00bb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bd4
			 0x00000000,                                                  // zero                                        /// 00bd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x33333333,                                                  // 4 random values                             /// 00bec
			 0x55555555,                                                  // 4 random values                             /// 00bf0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c34
			 0xff800000,                                                  // -inf                                        /// 00c38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c84
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0xff800000,                                                  // -inf                                        /// 00c8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ca8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x33333333,                                                  // 4 random values                             /// 00cc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ccc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ce4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cec
			 0xffc00001,                                                  // -signaling NaN                              /// 00cf0
			 0x33333333,                                                  // 4 random values                             /// 00cf4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x4b000000,                                                  // 8388608.0                                   /// 00d00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000000,                                                  // zero                                        /// 00d28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d30
			 0x7f800000,                                                  // inf                                         /// 00d34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d3c
			 0x00000000,                                                  // zero                                        /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d98
			 0x7f800000,                                                  // inf                                         /// 00d9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0xcb000000,                                                  // -8388608.0                                  /// 00de0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e08
			 0x7fc00001,                                                  // signaling NaN                               /// 00e0c
			 0x80000000,                                                  // -zero                                       /// 00e10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e18
			 0x33333333,                                                  // 4 random values                             /// 00e1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x3f028f5c,                                                  // 0.51                                        /// 00e38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xbf028f5c,                                                  // -0.51                                       /// 00e4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e74
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e80
			 0x7fc00001,                                                  // signaling NaN                               /// 00e84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ea0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x33333333,                                                  // 4 random values                             /// 00ed4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00edc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ef4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00efc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f84
			 0x7f800000,                                                  // inf                                         /// 00f88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f98
			 0x3f028f5c,                                                  // 0.51                                        /// 00f9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fa4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fd8
			 0xff800000,                                                  // -inf                                        /// 00fdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fe4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fec
			 0xff800000,                                                  // -inf                                        /// 00ff0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ff8
			 0x0e3fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00008
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0000c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00018
			 0xcb000000,                                                  // -8388608.0                                  /// 0001c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00020
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00024
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0002c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00030
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00034
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00038
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00040
			 0x0c400000,                                                  // Leading 1s:                                 /// 00044
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00048
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x7f800000,                                                  // inf                                         /// 00050
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00054
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00058
			 0x0c700000,                                                  // Leading 1s:                                 /// 0005c
			 0x7f800000,                                                  // inf                                         /// 00060
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00068
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0006c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00070
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00074
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00078
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0008c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00098
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0009c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xffc00001,                                                  // -signaling NaN                              /// 000f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x33333333,                                                  // 4 random values                             /// 00100
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00104
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00108
			 0x80011111,                                                  // -9.7958E-41                                 /// 0010c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00110
			 0xff800000,                                                  // -inf                                        /// 00114
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00118
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0011c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00120
			 0xbf028f5c,                                                  // -0.51                                       /// 00124
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00128
			 0x7f800000,                                                  // inf                                         /// 0012c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0013c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00140
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00144
			 0x33333333,                                                  // 4 random values                             /// 00148
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0014c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00150
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00154
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00158
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00160
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00168
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0xff800000,                                                  // -inf                                        /// 00178
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0018c
			 0x4b000000,                                                  // 8388608.0                                   /// 00190
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00194
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00198
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0019c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001d8
			 0x00000000,                                                  // zero                                        /// 001dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00200
			 0xbf028f5c,                                                  // -0.51                                       /// 00204
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00208
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0020c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00210
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0021c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00220
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00228
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0022c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00230
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00234
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00238
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x33333333,                                                  // 4 random values                             /// 00244
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00250
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00254
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x4b000000,                                                  // 8388608.0                                   /// 00260
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0026c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00270
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00274
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0c700000,                                                  // Leading 1s:                                 /// 0027c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00280
			 0xffc00001,                                                  // -signaling NaN                              /// 00284
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0028c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7f800000,                                                  // inf                                         /// 00294
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0029c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002bc
			 0xffc00001,                                                  // -signaling NaN                              /// 002c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00304
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00308
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0030c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00314
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00318
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00320
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00324
			 0x55555555,                                                  // 4 random values                             /// 00328
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00334
			 0x7fc00001,                                                  // signaling NaN                               /// 00338
			 0xbf028f5c,                                                  // -0.51                                       /// 0033c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00340
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00348
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0034c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00350
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00358
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0035c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00360
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0036c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00374
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00378
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00388
			 0x00011111,                                                  // 9.7958E-41                                  /// 0038c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00390
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00394
			 0xcb000000,                                                  // -8388608.0                                  /// 00398
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0039c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a4
			 0x00000000,                                                  // zero                                        /// 003a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e0
			 0xff800000,                                                  // -inf                                        /// 003e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00404
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0040c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x7fc00001,                                                  // signaling NaN                               /// 00414
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00418
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0041c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00420
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00424
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00428
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xffc00001,                                                  // -signaling NaN                              /// 00430
			 0x00000000,                                                  // zero                                        /// 00434
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0043c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00440
			 0x0c700000,                                                  // Leading 1s:                                 /// 00444
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00448
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0044c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00450
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00454
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0045c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00460
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00464
			 0x0c600000,                                                  // Leading 1s:                                 /// 00468
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0046c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00474
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00480
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00484
			 0xcb000000,                                                  // -8388608.0                                  /// 00488
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0048c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00490
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00494
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00500
			 0xff800000,                                                  // -inf                                        /// 00504
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0050c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00510
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00514
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00518
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00524
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00528
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00538
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0053c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00544
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x55555555,                                                  // 4 random values                             /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00558
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00560
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00564
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00568
			 0x33333333,                                                  // 4 random values                             /// 0056c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00570
			 0x4b000000,                                                  // 8388608.0                                   /// 00574
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00578
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00580
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00588
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0058c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00598
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0059c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005bc
			 0x00000000,                                                  // zero                                        /// 005c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00608
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00610
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x0c600000,                                                  // Leading 1s:                                 /// 00618
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0061c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00620
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00624
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0062c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00630
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00634
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0063c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00640
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00644
			 0x0c400000,                                                  // Leading 1s:                                 /// 00648
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0064c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00660
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00668
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0066c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00670
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00674
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00678
			 0x0c400000,                                                  // Leading 1s:                                 /// 0067c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00680
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00684
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00688
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x7fc00001,                                                  // signaling NaN                               /// 00694
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0069c
			 0x0e000007,                                                  // Trailing 1s:                                /// 006a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c8
			 0x4b000000,                                                  // 8388608.0                                   /// 006cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00700
			 0x80000000,                                                  // -zero                                       /// 00704
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00708
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00724
			 0x0c600000,                                                  // Leading 1s:                                 /// 00728
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00734
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00738
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0073c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0xcb000000,                                                  // -8388608.0                                  /// 00744
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00748
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0074c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00754
			 0x80000000,                                                  // -zero                                       /// 00758
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0075c
			 0x80000000,                                                  // -zero                                       /// 00760
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00764
			 0x80000000,                                                  // -zero                                       /// 00768
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00770
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00774
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00778
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00780
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00790
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00794
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00798
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0079c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0xff800000,                                                  // -inf                                        /// 007ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00800
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00810
			 0xffc00001,                                                  // -signaling NaN                              /// 00814
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00818
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x3f028f5c,                                                  // 0.51                                        /// 0083c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00840
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00844
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00848
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0084c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00850
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00854
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00858
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0086c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0e000007,                                                  // Trailing 1s:                                /// 0087c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00884
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00888
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0088c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00890
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0e000007,                                                  // Trailing 1s:                                /// 00898
			 0x00000000,                                                  // zero                                        /// 0089c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c4
			 0x4b000000,                                                  // 8388608.0                                   /// 008c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00900
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00908
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0090c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00910
			 0xbf028f5c,                                                  // -0.51                                       /// 00914
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0091c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00924
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00928
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0092c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00934
			 0x80000000,                                                  // -zero                                       /// 00938
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0093c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0094c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00954
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00960
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00968
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00974
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00980
			 0xffc00001,                                                  // -signaling NaN                              /// 00984
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0098c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00994
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0xffc00001,                                                  // -signaling NaN                              /// 009b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80000000,                                                  // -zero                                       /// 009e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a68
			 0x7f800000,                                                  // inf                                         /// 00a6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x00000000,                                                  // zero                                        /// 00a84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab4
			 0xff800000,                                                  // -inf                                        /// 00ab8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00abc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00adc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af0
			 0x80000000,                                                  // -zero                                       /// 00af4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af8
			 0xcb000000,                                                  // -8388608.0                                  /// 00afc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x33333333,                                                  // 4 random values                             /// 00b1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x33333333,                                                  // 4 random values                             /// 00b9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ba0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bdc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00be0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00be8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c00
			 0xbf028f5c,                                                  // -0.51                                       /// 00c04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000000,                                                  // -zero                                       /// 00c5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00000000,                                                  // zero                                        /// 00c80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ca0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ce0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0xff800000,                                                  // -inf                                        /// 00cf0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d50
			 0xbf028f5c,                                                  // -0.51                                       /// 00d54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d74
			 0xcb000000,                                                  // -8388608.0                                  /// 00d78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0xcb000000,                                                  // -8388608.0                                  /// 00d8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00da0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00df0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00df8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dfc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e40
			 0x00000000,                                                  // zero                                        /// 00e44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eb0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00eb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00edc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00efc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f68
			 0xbf028f5c,                                                  // -0.51                                       /// 00f6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fa0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fc4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fdc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fe0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c7ffffc                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00000
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0000c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00010
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00018
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00020
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00024
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00028
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00030
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00034
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00038
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0003c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00040
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00044
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00048
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0004c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00050
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00054
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00058
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0005c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00060
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00064
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00068
			 0x80011111,                                                  // -9.7958E-41                                 /// 0006c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00070
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00074
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00078
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00080
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00088
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0008c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00090
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00094
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00098
			 0x0c780000,                                                  // Leading 1s:                                 /// 0009c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000b8
			 0x33333333,                                                  // 4 random values                             /// 000bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000e8
			 0x80000000,                                                  // -zero                                       /// 000ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00104
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0010c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00110
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00114
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00118
			 0x0c600000,                                                  // Leading 1s:                                 /// 0011c
			 0x80000000,                                                  // -zero                                       /// 00120
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00124
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00128
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0012c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00130
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00134
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00138
			 0xffc00001,                                                  // -signaling NaN                              /// 0013c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00140
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00148
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0014c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00150
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00154
			 0x7f800000,                                                  // inf                                         /// 00158
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0015c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00164
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0016c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00170
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00178
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0017c
			 0x7fc00001,                                                  // signaling NaN                               /// 00180
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0018c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00194
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00198
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0019c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00200
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0020c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00210
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0e000007,                                                  // Trailing 1s:                                /// 00218
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0021c
			 0xcb000000,                                                  // -8388608.0                                  /// 00220
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00228
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0022c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00234
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00238
			 0x4b000000,                                                  // 8388608.0                                   /// 0023c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00244
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0024c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00250
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00254
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00258
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00260
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00264
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0026c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00270
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00274
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00278
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00280
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00294
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00298
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002bc
			 0x55555555,                                                  // 4 random values                             /// 002c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7fc00001,                                                  // signaling NaN                               /// 002d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00308
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0030c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00310
			 0x7f800000,                                                  // inf                                         /// 00314
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0031c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00324
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00328
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x3f028f5c,                                                  // 0.51                                        /// 00330
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x3f028f5c,                                                  // 0.51                                        /// 00340
			 0x7f800000,                                                  // inf                                         /// 00344
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00348
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00354
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00360
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00364
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0036c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00370
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00374
			 0x7f800000,                                                  // inf                                         /// 00378
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0037c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00380
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00384
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00388
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00398
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b8
			 0x80000000,                                                  // -zero                                       /// 003bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00404
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00408
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0040c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00410
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0x00000000,                                                  // zero                                        /// 00420
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00424
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0042c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c600000,                                                  // Leading 1s:                                 /// 00444
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00448
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00450
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00454
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00458
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0045c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00460
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00464
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0046c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00478
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0047c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x7f800000,                                                  // inf                                         /// 00484
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00488
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00490
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00494
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0049c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 004c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x3f028f5c,                                                  // 0.51                                        /// 004dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00504
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00508
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0050c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00510
			 0x55555555,                                                  // 4 random values                             /// 00514
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0051c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0052c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00530
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00534
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00538
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00540
			 0x00011111,                                                  // 9.7958E-41                                  /// 00544
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00548
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00550
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00554
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00558
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00568
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000000,                                                  // -zero                                       /// 00570
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00574
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00578
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0057c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00584
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0058c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00011111,                                                  // 9.7958E-41                                  /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f0
			 0xbf028f5c,                                                  // -0.51                                       /// 005f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005fc
			 0x80000000,                                                  // -zero                                       /// 00600
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00604
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00614
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00618
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00620
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00628
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0062c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00630
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00634
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00638
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0063c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00640
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00644
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00648
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0064c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00650
			 0x55555555,                                                  // 4 random values                             /// 00654
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00658
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00660
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0066c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0067c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00680
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00684
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00688
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0068c
			 0xcb000000,                                                  // -8388608.0                                  /// 00690
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00694
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0069c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ac
			 0x80000000,                                                  // -zero                                       /// 006b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00700
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00704
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00708
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00710
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00718
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0071c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00728
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00734
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00738
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0073c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x7f800000,                                                  // inf                                         /// 00744
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00748
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0074c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00750
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0075c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00760
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00764
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00770
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00778
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0077c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00780
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00784
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00788
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00790
			 0x0c600000,                                                  // Leading 1s:                                 /// 00794
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00000000,                                                  // zero                                        /// 0079c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x80000000,                                                  // -zero                                       /// 007d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x55555555,                                                  // 4 random values                             /// 007e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x0c600000,                                                  // Leading 1s:                                 /// 00804
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00808
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0080c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00814
			 0x80000000,                                                  // -zero                                       /// 00818
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0081c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00820
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00824
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00828
			 0x33333333,                                                  // 4 random values                             /// 0082c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00834
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00838
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0083c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00840
			 0x80011111,                                                  // -9.7958E-41                                 /// 00844
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00848
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00850
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00854
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0xffc00001,                                                  // -signaling NaN                              /// 0085c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00860
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00868
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0086c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00870
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00874
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0087c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00884
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00890
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00894
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00898
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0089c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00904
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0090c
			 0x4b000000,                                                  // 8388608.0                                   /// 00910
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00914
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00918
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00920
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0092c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00930
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00934
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00938
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0093c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00940
			 0x7f800000,                                                  // inf                                         /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0094c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00954
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00958
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x55555555,                                                  // 4 random values                             /// 00960
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00964
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00974
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00978
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0097c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00980
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00984
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00988
			 0x0c780000,                                                  // Leading 1s:                                 /// 0098c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00990
			 0x00000000,                                                  // zero                                        /// 00994
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00998
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0099c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009dc
			 0x00000000,                                                  // zero                                        /// 009e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a04
			 0x7f800000,                                                  // inf                                         /// 00a08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aa8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00acc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ad8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00adc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00af8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00afc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b28
			 0xff800000,                                                  // -inf                                        /// 00b2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x3f028f5c,                                                  // 0.51                                        /// 00b84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0xcb000000,                                                  // -8388608.0                                  /// 00b98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xff800000,                                                  // -inf                                        /// 00bc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bf4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ccc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cfc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d54
			 0x7fc00001,                                                  // signaling NaN                               /// 00d58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0xbf028f5c,                                                  // -0.51                                       /// 00d74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ddc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00de0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de4
			 0xbf028f5c,                                                  // -0.51                                       /// 00de8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x3f028f5c,                                                  // 0.51                                        /// 00df4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x00000000,                                                  // zero                                        /// 00e14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x7fc00001,                                                  // signaling NaN                               /// 00e1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e40
			 0xffc00001,                                                  // -signaling NaN                              /// 00e44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e98
			 0x00000000,                                                  // zero                                        /// 00e9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ea4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ea8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x33333333,                                                  // 4 random values                             /// 00ebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ec8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ecc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00edc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ee4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eec
			 0xffc00001,                                                  // -signaling NaN                              /// 00ef0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0xbf028f5c,                                                  // -0.51                                       /// 00f0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f38
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x80000000,                                                  // -zero                                       /// 00f70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00fa0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x7f7ffffe // max norm - 1ulp                               // max norm +ve                                /// last
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
			 0x00000794,
			 0x000006a0,
			 0x000003d8,
			 0x0000054c,
			 0x0000011c,
			 0x0000050c,
			 0x000002b8,
			 0x00000134,

			 /// vpu register f2
			 0x41400000,
			 0x41f00000,
			 0x41200000,
			 0x40800000,
			 0x40a00000,
			 0x41300000,
			 0x41e00000,
			 0x41400000,

			 /// vpu register f3
			 0x40c00000,
			 0x41000000,
			 0x42000000,
			 0x41b80000,
			 0x41f00000,
			 0x41e80000,
			 0x41600000,
			 0x41f80000,

			 /// vpu register f4
			 0x40e00000,
			 0x41f00000,
			 0x41200000,
			 0x41d80000,
			 0x41700000,
			 0x41700000,
			 0x41f00000,
			 0x42000000,

			 /// vpu register f5
			 0x41a80000,
			 0x40e00000,
			 0x41c00000,
			 0x41c80000,
			 0x40000000,
			 0x41f00000,
			 0x41d80000,
			 0x41900000,

			 /// vpu register f6
			 0x40c00000,
			 0x41000000,
			 0x41b80000,
			 0x41980000,
			 0x41300000,
			 0x40800000,
			 0x41980000,
			 0x40400000,

			 /// vpu register f7
			 0x41900000,
			 0x41a80000,
			 0x41f00000,
			 0x40000000,
			 0x41300000,
			 0x41700000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f8
			 0x41e80000,
			 0x40c00000,
			 0x41600000,
			 0x41a80000,
			 0x40e00000,
			 0x40000000,
			 0x42000000,
			 0x41000000,

			 /// vpu register f9
			 0x41000000,
			 0x40800000,
			 0x41a80000,
			 0x41600000,
			 0x41f80000,
			 0x41f80000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f10
			 0x41c00000,
			 0x41e00000,
			 0x41400000,
			 0x41800000,
			 0x41400000,
			 0x41500000,
			 0x41880000,
			 0x40c00000,

			 /// vpu register f11
			 0x41a00000,
			 0x41f80000,
			 0x41700000,
			 0x40e00000,
			 0x41900000,
			 0x3f800000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f12
			 0x41300000,
			 0x41100000,
			 0x41d80000,
			 0x41b80000,
			 0x41700000,
			 0x3f800000,
			 0x40e00000,
			 0x41700000,

			 /// vpu register f13
			 0x41a80000,
			 0x41c80000,
			 0x41d80000,
			 0x41e00000,
			 0x3f800000,
			 0x41c00000,
			 0x3f800000,
			 0x40e00000,

			 /// vpu register f14
			 0x42000000,
			 0x40c00000,
			 0x41f00000,
			 0x41e00000,
			 0x40c00000,
			 0x41500000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f15
			 0x41400000,
			 0x41100000,
			 0x41400000,
			 0x41b00000,
			 0x41a00000,
			 0x41a00000,
			 0x41000000,
			 0x41980000,

			 /// vpu register f16
			 0x40400000,
			 0x41d00000,
			 0x40c00000,
			 0x41c00000,
			 0x41900000,
			 0x42000000,
			 0x40400000,
			 0x41880000,

			 /// vpu register f17
			 0x41600000,
			 0x41d00000,
			 0x42000000,
			 0x41000000,
			 0x41900000,
			 0x41a00000,
			 0x41200000,
			 0x41980000,

			 /// vpu register f18
			 0x41d00000,
			 0x41d00000,
			 0x41e00000,
			 0x41a00000,
			 0x41a80000,
			 0x41100000,
			 0x41980000,
			 0x41d00000,

			 /// vpu register f19
			 0x41880000,
			 0x41b80000,
			 0x41a00000,
			 0x41300000,
			 0x41f80000,
			 0x41b80000,
			 0x40a00000,
			 0x41b80000,

			 /// vpu register f20
			 0x41f80000,
			 0x41500000,
			 0x41100000,
			 0x40400000,
			 0x41600000,
			 0x41880000,
			 0x41200000,
			 0x3f800000,

			 /// vpu register f21
			 0x41f00000,
			 0x41700000,
			 0x41b80000,
			 0x3f800000,
			 0x40e00000,
			 0x41d80000,
			 0x41d80000,
			 0x41d00000,

			 /// vpu register f22
			 0x41000000,
			 0x40800000,
			 0x41400000,
			 0x41d00000,
			 0x41f80000,
			 0x41d00000,
			 0x41100000,
			 0x41d00000,

			 /// vpu register f23
			 0x41900000,
			 0x41500000,
			 0x41200000,
			 0x40800000,
			 0x41880000,
			 0x41300000,
			 0x41500000,
			 0x41e00000,

			 /// vpu register f24
			 0x40800000,
			 0x41c80000,
			 0x41200000,
			 0x40800000,
			 0x40e00000,
			 0x41c00000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f25
			 0x40c00000,
			 0x41200000,
			 0x41400000,
			 0x41500000,
			 0x41900000,
			 0x41100000,
			 0x41e80000,
			 0x40a00000,

			 /// vpu register f26
			 0x41f00000,
			 0x41400000,
			 0x41500000,
			 0x41f00000,
			 0x41980000,
			 0x41500000,
			 0x41100000,
			 0x41500000,

			 /// vpu register f27
			 0x41e00000,
			 0x41a80000,
			 0x41e00000,
			 0x41c00000,
			 0x40400000,
			 0x41e00000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f28
			 0x41200000,
			 0x41d00000,
			 0x40000000,
			 0x40800000,
			 0x40000000,
			 0x41d80000,
			 0x41a80000,
			 0x41d80000,

			 /// vpu register f29
			 0x41f00000,
			 0x41100000,
			 0x41d00000,
			 0x41200000,
			 0x40000000,
			 0x41300000,
			 0x41f00000,
			 0x41400000,

			 /// vpu register f30
			 0x41e00000,
			 0x41d00000,
			 0x41c80000,
			 0x41d80000,
			 0x41900000,
			 0x40c00000,
			 0x40a00000,
			 0x42000000,

			 /// vpu register f31
			 0x41f00000,
			 0x41400000,
			 0x40a00000,
			 0x41400000,
			 0x40e00000,
			 0x41500000,
			 0x41100000,
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
		"fsub.pi f29, f24, f19\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f16, f13, f10\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f15, f27\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f25, f8, f10\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f11, f5, f8\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f10, f19\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f25, f25, f4\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f26, f21, f20\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f25, f6, f21\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f14, f9, f12\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f7, f20, f27\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f2, f2, f25\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f5, f7, f6\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f3, f11, f22\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f30, f17, f8\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f3, f29, f7\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f21, f17\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f6, f10, f18\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f10, f10, f5\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f26, f19, f26\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f8, f1, f3\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f27, f17, f13\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f12, f13, f2\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f2, f24, f3\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f22, f15, f28\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f25, f30\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f20, f24, f27\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f22, f6, f21\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f30, f14, f7\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f17, f5, f10\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f17, f16\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f24, f27, f29\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f14, f0\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f7, f10, f19\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f29, f21, f21\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f20, f6\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f18, f0, f20\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f5, f18, f27\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f11, f8, f3\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f14, f21, f11\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f13, f27, f29\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f8, f14, f19\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f2, f27\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f14, f30, f1\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f10, f2\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f21, f15\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f5, f9, f14\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f19, f20, f5\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f13, f19, f6\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f17, f27\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f13, f6, f14\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f24, f1, f29\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f17, f14, f2\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f30, f24, f20\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f14, f15, f26\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f10, f16, f6\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f18, f17, f9\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f18, f25\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f10, f2, f27\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f30, f11, f16\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f27, f15, f13\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f8, f0\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f6, f20, f15\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f21, f10, f4\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f4, f14, f4\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f15, f17, f7\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f26, f12, f28\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f19, f6, f21\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f9, f12, f16\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f14, f3, f16\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f7, f15, f20\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f16, f15, f13\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f29, f19, f15\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f7, f19, f15\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f10, f16, f16\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f21, f14, f5\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f10, f4, f8\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f10, f16, f0\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f16, f21, f28\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f27, f18, f13\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f6, f22, f17\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f11, f22, f9\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f27, f13, f23\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f30, f15, f18\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f3, f7, f17\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f8, f2, f8\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f9, f23, f8\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f8, f17, f7\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f30, f0, f20\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f18, f25, f7\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f7, f29, f12\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f16, f26, f11\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f29, f24, f4\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f13, f3, f21\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f29, f21, f6\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f21, f22, f30\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f5, f15, f1\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f24, f1, f3\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsub.pi f23, f0, f3\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
