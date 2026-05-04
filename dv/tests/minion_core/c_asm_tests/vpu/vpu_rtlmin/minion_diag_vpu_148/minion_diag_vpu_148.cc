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
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00000
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00004
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00008
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0000c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0001c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00020
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00028
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00034
			 0x0e000001,                                                  // Trailing 1s:                                /// 00038
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0003c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00040
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00044
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00048
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00050
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00058
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0005c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00064
			 0x3f028f5c,                                                  // 0.51                                        /// 00068
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0006c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00070
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00074
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00078
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0007c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00080
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00088
			 0x7f800000,                                                  // inf                                         /// 0008c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0xff800000,                                                  // -inf                                        /// 00094
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00098
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0009c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a4
			 0x4b000000,                                                  // 8388608.0                                   /// 000a8
			 0x80000000,                                                  // -zero                                       /// 000ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000c8
			 0x33333333,                                                  // 4 random values                             /// 000cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00108
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0010c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00110
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00114
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00118
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00120
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00124
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00128
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0012c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00134
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00138
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0013c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00144
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00148
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0014c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00150
			 0x0c780000,                                                  // Leading 1s:                                 /// 00154
			 0x0c700000,                                                  // Leading 1s:                                 /// 00158
			 0x80011111,                                                  // -9.7958E-41                                 /// 0015c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00160
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00164
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00168
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00170
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00180
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00184
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0018c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00190
			 0x3f028f5c,                                                  // 0.51                                        /// 00194
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00198
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0019c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00200
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00204
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00208
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0020c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00218
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0021c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00228
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e000007,                                                  // Trailing 1s:                                /// 00234
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00238
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0023c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00240
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00248
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0024c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00254
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0025c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00260
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0026c
			 0x4b000000,                                                  // 8388608.0                                   /// 00270
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00274
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00280
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x0e000007,                                                  // Trailing 1s:                                /// 00288
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00294
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00298
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 002b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0xff800000,                                                  // -inf                                        /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00308
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0030c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00310
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00314
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00320
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00324
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0032c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00330
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00334
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00338
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0033c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00340
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00350
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00354
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x4b000000,                                                  // 8388608.0                                   /// 00360
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00368
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0036c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00370
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00374
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00378
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00380
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00384
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x55555555,                                                  // 4 random values                             /// 0038c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00390
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00394
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c8
			 0xffc00001,                                                  // -signaling NaN                              /// 003cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003f0
			 0xffc00001,                                                  // -signaling NaN                              /// 003f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x7f800000,                                                  // inf                                         /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00414
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00418
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00420
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0042c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00430
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00438
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0043c
			 0x80000000,                                                  // -zero                                       /// 00440
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00444
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00000000,                                                  // zero                                        /// 00450
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xbf028f5c,                                                  // -0.51                                       /// 00458
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0045c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00460
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00464
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7fc00001,                                                  // signaling NaN                               /// 00478
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00484
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00488
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0048c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00490
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00494
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00498
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00500
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00504
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00508
			 0x0c700000,                                                  // Leading 1s:                                 /// 0050c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00510
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00514
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00520
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00524
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00530
			 0x55555555,                                                  // 4 random values                             /// 00534
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00538
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0053c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00540
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00544
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00548
			 0x4b000000,                                                  // 8388608.0                                   /// 0054c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00558
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00560
			 0x0e000007,                                                  // Trailing 1s:                                /// 00564
			 0x4b000000,                                                  // 8388608.0                                   /// 00568
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0056c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00574
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00578
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0057c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00584
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00594
			 0x33333333,                                                  // 4 random values                             /// 00598
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005ac
			 0x3f028f5c,                                                  // 0.51                                        /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00600
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00604
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00608
			 0x0c700000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00610
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00614
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00618
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00624
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00630
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00634
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0063c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00640
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00648
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0064c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00650
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00654
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00658
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00660
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00664
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00668
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0066c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00670
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00678
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00680
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00688
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00694
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0069c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b0
			 0x7fc00001,                                                  // signaling NaN                               /// 006b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x7fc00001,                                                  // signaling NaN                               /// 006bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00700
			 0x55555555,                                                  // 4 random values                             /// 00704
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00720
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00724
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00728
			 0x0c600000,                                                  // Leading 1s:                                 /// 0072c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00730
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e000007,                                                  // Trailing 1s:                                /// 00738
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0073c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00740
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00744
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00750
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00754
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00758
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00764
			 0x00011111,                                                  // 9.7958E-41                                  /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0076c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00770
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00780
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00784
			 0xff800000,                                                  // -inf                                        /// 00788
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00794
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00798
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0079c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000000,                                                  // zero                                        /// 007a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007e8
			 0x80000000,                                                  // -zero                                       /// 007ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x7fc00001,                                                  // signaling NaN                               /// 007f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00804
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00810
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00814
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00824
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00828
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00840
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00848
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00850
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00858
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0085c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00860
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00868
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0086c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00870
			 0x0e000003,                                                  // Trailing 1s:                                /// 00874
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0087c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00884
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00890
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b4
			 0xff800000,                                                  // -inf                                        /// 008b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008bc
			 0x55555555,                                                  // 4 random values                             /// 008c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00900
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00904
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00908
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0090c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00910
			 0x0e000003,                                                  // Trailing 1s:                                /// 00914
			 0x0c400000,                                                  // Leading 1s:                                 /// 00918
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00920
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00924
			 0x00011111,                                                  // 9.7958E-41                                  /// 00928
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00934
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0093c
			 0x4b000000,                                                  // 8388608.0                                   /// 00940
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00950
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00954
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00958
			 0x80000000,                                                  // -zero                                       /// 0095c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00964
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00968
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0096c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00970
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00974
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00978
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00980
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00984
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0098c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00990
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00994
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00998
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0xff800000,                                                  // -inf                                        /// 009c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x3f028f5c,                                                  // 0.51                                        /// 00a74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a84
			 0x7f800000,                                                  // inf                                         /// 00a88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ab8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x33333333,                                                  // 4 random values                             /// 00ac4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00acc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ad8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b58
			 0xff800000,                                                  // -inf                                        /// 00b5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b70
			 0x33333333,                                                  // 4 random values                             /// 00b74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ba8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c3c
			 0x00000000,                                                  // zero                                        /// 00c40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00000000,                                                  // zero                                        /// 00c98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ca8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cb8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ce4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00da0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00da4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dac
			 0x00000000,                                                  // zero                                        /// 00db0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00db8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00de0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df0
			 0xff800000,                                                  // -inf                                        /// 00df4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e94
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ea4
			 0x00000000,                                                  // zero                                        /// 00ea8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eac
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ebc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ec4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ef0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00efc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f24
			 0x55555555,                                                  // 4 random values                             /// 00f28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f44
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f60
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f84
			 0x4b000000,                                                  // 8388608.0                                   /// 00f88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0xcb8c4b40                                                  // -18388608.0                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00000
			 0x55555555,                                                  // 4 random values                             /// 00004
			 0x0e000001,                                                  // Trailing 1s:                                /// 00008
			 0x0e000001,                                                  // Trailing 1s:                                /// 0000c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00010
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00014
			 0xcb000000,                                                  // -8388608.0                                  /// 00018
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0001c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00020
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0002c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00030
			 0xffc00001,                                                  // -signaling NaN                              /// 00034
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00038
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0004c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x7fc00001,                                                  // signaling NaN                               /// 00054
			 0x0c700000,                                                  // Leading 1s:                                 /// 00058
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0005c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00060
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00068
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00074
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00078
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0007c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00084
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000000,                                                  // -zero                                       /// 0008c
			 0x7f800000,                                                  // inf                                         /// 00090
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00098
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0009c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c0
			 0x80000000,                                                  // -zero                                       /// 000c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00100
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0010c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00110
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00114
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00124
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0012c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00130
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00138
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0013c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00144
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00148
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00150
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00158
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0015c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0xcb000000,                                                  // -8388608.0                                  /// 00168
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0016c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00170
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00174
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00178
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0017c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0018c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00190
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0019c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c4
			 0xffc00001,                                                  // -signaling NaN                              /// 001c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e8
			 0xffc00001,                                                  // -signaling NaN                              /// 001ec
			 0xbf028f5c,                                                  // -0.51                                       /// 001f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xffc00001,                                                  // -signaling NaN                              /// 001fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00200
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00204
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00208
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00214
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00218
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00224
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00228
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00230
			 0x7fc00001,                                                  // signaling NaN                               /// 00234
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00244
			 0x0e000001,                                                  // Trailing 1s:                                /// 00248
			 0x0c700000,                                                  // Leading 1s:                                 /// 0024c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00250
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00254
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00258
			 0x0c780000,                                                  // Leading 1s:                                 /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00264
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00268
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00270
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00278
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00284
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00288
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00294
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00298
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0029c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002bc
			 0x7f800000,                                                  // inf                                         /// 002c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00300
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00304
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00310
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00314
			 0x0c780000,                                                  // Leading 1s:                                 /// 00318
			 0x7fc00001,                                                  // signaling NaN                               /// 0031c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00320
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00324
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00328
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0032c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00330
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00338
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7fc00001,                                                  // signaling NaN                               /// 00340
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00344
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00350
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00354
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0035c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x0c400000,                                                  // Leading 1s:                                 /// 00364
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0036c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00370
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00374
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00378
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00380
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00388
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0038c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00390
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00394
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00398
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0039c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00404
			 0xcb000000,                                                  // -8388608.0                                  /// 00408
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00410
			 0x0c780000,                                                  // Leading 1s:                                 /// 00414
			 0x0c700000,                                                  // Leading 1s:                                 /// 00418
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00420
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00424
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0042c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00434
			 0x33333333,                                                  // 4 random values                             /// 00438
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00440
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0044c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00450
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00454
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x4b000000,                                                  // 8388608.0                                   /// 00460
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0046c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00470
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00474
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00478
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00480
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00484
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0048c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00490
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00504
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0050c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00518
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00528
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00530
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00538
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0053c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00544
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00548
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00550
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00554
			 0x0c700000,                                                  // Leading 1s:                                 /// 00558
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00560
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00568
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0056c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00570
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00574
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00580
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00588
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00590
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0059c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005dc
			 0x7f800000,                                                  // inf                                         /// 005e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00600
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00608
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0060c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00614
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00618
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00620
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00624
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0063c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00644
			 0x00000000,                                                  // zero                                        /// 00648
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0064c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00658
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0065c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00660
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00664
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00668
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0066c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00674
			 0x7fc00001,                                                  // signaling NaN                               /// 00678
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0067c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00684
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00688
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0068c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0xcb000000,                                                  // -8388608.0                                  /// 00698
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0069c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00000000,                                                  // zero                                        /// 006ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f4
			 0xcb000000,                                                  // -8388608.0                                  /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00700
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c700000,                                                  // Leading 1s:                                 /// 00708
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0070c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00710
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00714
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0xbf028f5c,                                                  // -0.51                                       /// 00728
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0072c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00734
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00754
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00758
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0075c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00764
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0076c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00770
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00774
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00778
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0077c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xcb000000,                                                  // -8388608.0                                  /// 00788
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00790
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00794
			 0x0e000001,                                                  // Trailing 1s:                                /// 00798
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0079c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a4
			 0x33333333,                                                  // 4 random values                             /// 007a8
			 0xffc00001,                                                  // -signaling NaN                              /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x55555555,                                                  // 4 random values                             /// 007b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x7fc00001,                                                  // signaling NaN                               /// 007cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x55555555,                                                  // 4 random values                             /// 007e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x4b000000,                                                  // 8388608.0                                   /// 00804
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e000001,                                                  // Trailing 1s:                                /// 0080c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00818
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x55555555,                                                  // 4 random values                             /// 00820
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00824
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00828
			 0x0c700000,                                                  // Leading 1s:                                 /// 0082c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00830
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00834
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00838
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0x0e000003,                                                  // Trailing 1s:                                /// 00844
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0084c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00850
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0085c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00864
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00868
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00870
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c780000,                                                  // Leading 1s:                                 /// 00884
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00888
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0088c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00890
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008cc
			 0xcb000000,                                                  // -8388608.0                                  /// 008d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00900
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00904
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0090c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00910
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00914
			 0xcb000000,                                                  // -8388608.0                                  /// 00918
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00924
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00928
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0092c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00930
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00934
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00938
			 0xff800000,                                                  // -inf                                        /// 0093c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00940
			 0x3f028f5c,                                                  // 0.51                                        /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80000000,                                                  // -zero                                       /// 00950
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00954
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00960
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00964
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00968
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0096c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00978
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0097c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00988
			 0x33333333,                                                  // 4 random values                             /// 0098c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00990
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xcb000000,                                                  // -8388608.0                                  /// 00998
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0099c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0xbf028f5c,                                                  // -0.51                                       /// 00a24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a68
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x7f800000,                                                  // inf                                         /// 00a9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aa0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aa8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00aac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x55555555,                                                  // 4 random values                             /// 00ab4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ad4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ad8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00adc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aec
			 0x7f800000,                                                  // inf                                         /// 00af0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00afc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b08
			 0x00000000,                                                  // zero                                        /// 00b0c
			 0x7f800000,                                                  // inf                                         /// 00b10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b24
			 0x4b000000,                                                  // 8388608.0                                   /// 00b28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b38
			 0xbf028f5c,                                                  // -0.51                                       /// 00b3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b60
			 0x33333333,                                                  // 4 random values                             /// 00b64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00bc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bcc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xffc00001,                                                  // -signaling NaN                              /// 00be8
			 0x80000000,                                                  // -zero                                       /// 00bec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bfc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xffc00001,                                                  // -signaling NaN                              /// 00c0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c80
			 0x00000000,                                                  // zero                                        /// 00c84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000000,                                                  // zero                                        /// 00c94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ca8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cac
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cbc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cc8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xff800000,                                                  // -inf                                        /// 00ce0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d38
			 0x55555555,                                                  // 4 random values                             /// 00d3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xbf028f5c,                                                  // -0.51                                       /// 00d8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00da0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00da4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00da8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dcc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00de4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e84
			 0x7fc00001,                                                  // signaling NaN                               /// 00e88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x55555555,                                                  // 4 random values                             /// 00e90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ec0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ecc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed4
			 0x55555555,                                                  // 4 random values                             /// 00ed8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ef8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f08
			 0xcb000000,                                                  // -8388608.0                                  /// 00f0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f80
			 0xffc00001,                                                  // -signaling NaN                              /// 00f84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80000000,                                                  // -zero                                       /// 00fe4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ff8
			 0x80000200                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0000c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00010
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00014
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00018
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0001c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0c780000,                                                  // Leading 1s:                                 /// 00024
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00030
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x7fc00001,                                                  // signaling NaN                               /// 00038
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0003c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00040
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00044
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00048
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0004c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00054
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00058
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0005c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00060
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00064
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00068
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00070
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00074
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0007c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00080
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00084
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00088
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0008c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00090
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00094
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00098
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0009c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a4
			 0x80000000,                                                  // -zero                                       /// 000a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00100
			 0x0e000003,                                                  // Trailing 1s:                                /// 00104
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x55555555,                                                  // 4 random values                             /// 0010c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00114
			 0x3f028f5c,                                                  // 0.51                                        /// 00118
			 0x33333333,                                                  // 4 random values                             /// 0011c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00120
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00128
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00130
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00140
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00148
			 0xffc00001,                                                  // -signaling NaN                              /// 0014c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00150
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00154
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00158
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00160
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00164
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0016c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00174
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0017c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c780000,                                                  // Leading 1s:                                 /// 00184
			 0x0c700000,                                                  // Leading 1s:                                 /// 00188
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00190
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x33333333,                                                  // 4 random values                             /// 00198
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0xcb000000,                                                  // -8388608.0                                  /// 001ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00208
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0020c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00218
			 0x0c780000,                                                  // Leading 1s:                                 /// 0021c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00220
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00224
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00234
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00238
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00244
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00250
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00258
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00260
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00268
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0026c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00270
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0027c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00284
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00290
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00298
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00300
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00304
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00308
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0031c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00320
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00324
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00328
			 0xbf028f5c,                                                  // -0.51                                       /// 0032c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00330
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00338
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00340
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00348
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0034c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00350
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00358
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0036c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x33333333,                                                  // 4 random values                             /// 0037c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0c600000,                                                  // Leading 1s:                                 /// 00384
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00388
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0038c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00390
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00394
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00398
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b0
			 0x00000000,                                                  // zero                                        /// 003b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003fc
			 0x80000000,                                                  // -zero                                       /// 00400
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00404
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00408
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00410
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00418
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00420
			 0x0e000007,                                                  // Trailing 1s:                                /// 00424
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00428
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00438
			 0x0e000001,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00440
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00444
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00448
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0044c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00450
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00458
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80011111,                                                  // -9.7958E-41                                 /// 00464
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0046c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00488
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0048c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00490
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00494
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00498
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0049c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00508
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00510
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00514
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00518
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00520
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00524
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00528
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00538
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0053c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00544
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00548
			 0xff800000,                                                  // -inf                                        /// 0054c
			 0x00000000,                                                  // zero                                        /// 00550
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00554
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00558
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00560
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00564
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00568
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00574
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00578
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00580
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00584
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00588
			 0xffc00001,                                                  // -signaling NaN                              /// 0058c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00590
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00594
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00598
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000000,                                                  // -zero                                       /// 005b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00608
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0060c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00610
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00618
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0061c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00620
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00630
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00634
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00638
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0063c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00640
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00644
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0064c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80000000,                                                  // -zero                                       /// 0065c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00664
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00668
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00670
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0067c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00690
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0xbf028f5c,                                                  // -0.51                                       /// 00698
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0069c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006e8
			 0x3f028f5c,                                                  // 0.51                                        /// 006ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x7f800000,                                                  // inf                                         /// 0070c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c700000,                                                  // Leading 1s:                                 /// 00718
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0071c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00720
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00724
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00728
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0072c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00730
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0073c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00740
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00748
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0074c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00758
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0c400000,                                                  // Leading 1s:                                 /// 0076c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xcb000000,                                                  // -8388608.0                                  /// 00774
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00778
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0077c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00784
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0078c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00790
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00794
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x3f028f5c,                                                  // 0.51                                        /// 0079c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x80000000,                                                  // -zero                                       /// 007bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007cc
			 0xff800000,                                                  // -inf                                        /// 007d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007ec
			 0xffc00001,                                                  // -signaling NaN                              /// 007f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00800
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00808
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00810
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00818
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00820
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00828
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00830
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00834
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0083c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00844
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00858
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0085c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00860
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00864
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00870
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00874
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80011111,                                                  // -9.7958E-41                                 /// 0087c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00880
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00884
			 0x0e000001,                                                  // Trailing 1s:                                /// 00888
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0088c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00890
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00898
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0089c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a0
			 0x80000000,                                                  // -zero                                       /// 008a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008ac
			 0xbf028f5c,                                                  // -0.51                                       /// 008b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00900
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00904
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00908
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00918
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0091c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00920
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00924
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00928
			 0x0e000007,                                                  // Trailing 1s:                                /// 0092c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00930
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00934
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0093c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00944
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0094c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00950
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00958
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000000,                                                  // zero                                        /// 00960
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00964
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00968
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0096c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00970
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xff800000,                                                  // -inf                                        /// 00978
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00980
			 0x00011111,                                                  // 9.7958E-41                                  /// 00984
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0098c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0099c
			 0x80000000,                                                  // -zero                                       /// 009a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a4c
			 0x7f800000,                                                  // inf                                         /// 00a50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a60
			 0x3f028f5c,                                                  // 0.51                                        /// 00a64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a80
			 0x4b000000,                                                  // 8388608.0                                   /// 00a84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x33333333,                                                  // 4 random values                             /// 00a98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ab4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00adc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ae8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00afc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b14
			 0x55555555,                                                  // 4 random values                             /// 00b18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b6c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bf8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c30
			 0x4b000000,                                                  // 8388608.0                                   /// 00c34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c84
			 0xcb000000,                                                  // -8388608.0                                  /// 00c88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ca4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb0
			 0xff800000,                                                  // -inf                                        /// 00cb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ce8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cf8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cfc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x3f028f5c,                                                  // 0.51                                        /// 00d54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d64
			 0x7f800000,                                                  // inf                                         /// 00d68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d94
			 0x80000000,                                                  // -zero                                       /// 00d98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00da0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc8
			 0x33333333,                                                  // 4 random values                             /// 00dcc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00de8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e40
			 0x7fc00001,                                                  // signaling NaN                               /// 00e44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e64
			 0x7f800000,                                                  // inf                                         /// 00e68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ea0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ea8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ecc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ed0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ed4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00edc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ef4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00efc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f10
			 0x4b000000,                                                  // 8388608.0                                   /// 00f14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f1c
			 0x80000000,                                                  // -zero                                       /// 00f20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00000000,                                                  // zero                                        /// 00f84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fa0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fe8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ff4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff8
			 0x80080000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00000
			 0xbf028f5c,                                                  // -0.51                                       /// 00004
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00008
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0000c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00014
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0001c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00020
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00024
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0002c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00030
			 0xbf028f5c,                                                  // -0.51                                       /// 00034
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0003c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00040
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x0e000001,                                                  // Trailing 1s:                                /// 00048
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0xcb000000,                                                  // -8388608.0                                  /// 00050
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00054
			 0x33333333,                                                  // 4 random values                             /// 00058
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00068
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0006c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00070
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00084
			 0x4b000000,                                                  // 8388608.0                                   /// 00088
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00090
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00094
			 0x7fc00001,                                                  // signaling NaN                               /// 00098
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0009c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xffc00001,                                                  // -signaling NaN                              /// 000e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00100
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00108
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0010c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00110
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00118
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0011c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0012c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00138
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0013c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00140
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x3f028f5c,                                                  // 0.51                                        /// 00148
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0014c
			 0xbf028f5c,                                                  // -0.51                                       /// 00150
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00154
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00158
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0015c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0xbf028f5c,                                                  // -0.51                                       /// 00164
			 0x0e000001,                                                  // Trailing 1s:                                /// 00168
			 0x00011111,                                                  // 9.7958E-41                                  /// 0016c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00170
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00174
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00190
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00194
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001bc
			 0x55555555,                                                  // 4 random values                             /// 001c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x80000000,                                                  // -zero                                       /// 001cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00208
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0020c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00214
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00218
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00224
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0022c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00234
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0023c
			 0x55555555,                                                  // 4 random values                             /// 00240
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00248
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00250
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0xcb000000,                                                  // -8388608.0                                  /// 00258
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0026c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00270
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00274
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00280
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00288
			 0x55555555,                                                  // 4 random values                             /// 0028c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00294
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00298
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0029c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002ec
			 0x33333333,                                                  // 4 random values                             /// 002f0
			 0x33333333,                                                  // 4 random values                             /// 002f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00304
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00310
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00314
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0031c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00320
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00324
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00328
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0032c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00330
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00338
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00344
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00348
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0034c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00350
			 0x0c600000,                                                  // Leading 1s:                                 /// 00354
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00358
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00360
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00364
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000000,                                                  // zero                                        /// 00374
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00378
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00380
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00384
			 0x0c400000,                                                  // Leading 1s:                                 /// 00388
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00390
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00394
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00398
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x7f800000,                                                  // inf                                         /// 003b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003bc
			 0xcb000000,                                                  // -8388608.0                                  /// 003c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003f0
			 0xffc00001,                                                  // -signaling NaN                              /// 003f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00400
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00404
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x7f800000,                                                  // inf                                         /// 0040c
			 0xbf028f5c,                                                  // -0.51                                       /// 00410
			 0x00011111,                                                  // 9.7958E-41                                  /// 00414
			 0x0c600000,                                                  // Leading 1s:                                 /// 00418
			 0x0e000001,                                                  // Trailing 1s:                                /// 0041c
			 0x00000000,                                                  // zero                                        /// 00420
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00424
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0042c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00438
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0043c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0044c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00450
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00454
			 0x00000000,                                                  // zero                                        /// 00458
			 0x0e000001,                                                  // Trailing 1s:                                /// 0045c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00464
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00468
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0046c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00470
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00474
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00478
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0047c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00480
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00484
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00488
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0048c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00490
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00498
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0049c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x7fc00001,                                                  // signaling NaN                               /// 004b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00500
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00508
			 0x0e000001,                                                  // Trailing 1s:                                /// 0050c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x33333333,                                                  // 4 random values                             /// 00518
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0051c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00524
			 0xcb000000,                                                  // -8388608.0                                  /// 00528
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00534
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00540
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00544
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00548
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00550
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00554
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00560
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00564
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00574
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0057c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00580
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0058c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00598
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0059c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x55555555,                                                  // 4 random values                             /// 005a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xff800000,                                                  // -inf                                        /// 005f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00608
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00610
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00614
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00618
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00620
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00624
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00628
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0062c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00630
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00634
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00640
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0064c
			 0x80000000,                                                  // -zero                                       /// 00650
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0065c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0xbf028f5c,                                                  // -0.51                                       /// 00668
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e000001,                                                  // Trailing 1s:                                /// 00674
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00678
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0067c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00694
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00698
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 006b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e8
			 0x80000000,                                                  // -zero                                       /// 006ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00704
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00708
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0070c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00718
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0071c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00724
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00728
			 0x0e000001,                                                  // Trailing 1s:                                /// 0072c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00730
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00738
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00740
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00744
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0074c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00750
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00754
			 0x7fc00001,                                                  // signaling NaN                               /// 00758
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0075c
			 0x7f800000,                                                  // inf                                         /// 00760
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00764
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00768
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00770
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0077c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00780
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00784
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00788
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0078c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00790
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00800
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00804
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00808
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0080c
			 0x55555555,                                                  // 4 random values                             /// 00810
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00818
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0081c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00820
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00828
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00830
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00838
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0083c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00840
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00848
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00850
			 0x4b000000,                                                  // 8388608.0                                   /// 00854
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00858
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0085c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00860
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00868
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00870
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00874
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0087c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00890
			 0x0c700000,                                                  // Leading 1s:                                 /// 00894
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00898
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0089c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x4b000000,                                                  // 8388608.0                                   /// 008a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00910
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00914
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00918
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0091c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00924
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00928
			 0xffc00001,                                                  // -signaling NaN                              /// 0092c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00934
			 0xcb000000,                                                  // -8388608.0                                  /// 00938
			 0x80000000,                                                  // -zero                                       /// 0093c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00944
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00948
			 0x80000000,                                                  // -zero                                       /// 0094c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00950
			 0x0c700000,                                                  // Leading 1s:                                 /// 00954
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0095c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00960
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00964
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0096c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00970
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00974
			 0x0c400000,                                                  // Leading 1s:                                 /// 00978
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0xffc00001,                                                  // -signaling NaN                              /// 00980
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00984
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00988
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0098c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x4b000000,                                                  // 8388608.0                                   /// 00998
			 0x80011111,                                                  // -9.7958E-41                                 /// 0099c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c8
			 0x80000000,                                                  // -zero                                       /// 009cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f8
			 0xffc00001,                                                  // -signaling NaN                              /// 009fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00a00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xcb000000,                                                  // -8388608.0                                  /// 00a28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a30
			 0x00000000,                                                  // zero                                        /// 00a34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa4
			 0x7f800000,                                                  // inf                                         /// 00aa8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00acc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ae0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00af4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00afc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b18
			 0xbf028f5c,                                                  // -0.51                                       /// 00b1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0xff800000,                                                  // -inf                                        /// 00b30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ba4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bcc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bdc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00be0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00be4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bf0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c00
			 0x3f028f5c,                                                  // 0.51                                        /// 00c04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc4
			 0x55555555,                                                  // 4 random values                             /// 00cc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ce4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ce8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80000000,                                                  // -zero                                       /// 00cf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x3f028f5c,                                                  // 0.51                                        /// 00d14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xff800000,                                                  // -inf                                        /// 00d90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00db8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00de0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00df4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00df8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e58
			 0xbf028f5c,                                                  // -0.51                                       /// 00e5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x55555555,                                                  // 4 random values                             /// 00e6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e80
			 0x3f028f5c,                                                  // 0.51                                        /// 00e84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e90
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x7f800000,                                                  // inf                                         /// 00e9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ebc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ecc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ed8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00edc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ef0
			 0x7f800000,                                                  // inf                                         /// 00ef4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f80
			 0x3f028f5c,                                                  // 0.51                                        /// 00f84
			 0x3f028f5c,                                                  // 0.51                                        /// 00f88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x3f028f5c,                                                  // 0.51                                        /// 00f94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fe0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fe8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ff0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ff4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0e7fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xfbcd05a7, /// 0x0
			 0xd61f8ad0, /// 0x4
			 0xf1273fce, /// 0x8
			 0x63a1e8c2, /// 0xc
			 0x7f7f7a1d, /// 0x10
			 0xf0927e23, /// 0x14
			 0xf7fb67b0, /// 0x18
			 0xae09a0ea, /// 0x1c
			 0x56f9ccad, /// 0x20
			 0x4fff3bc5, /// 0x24
			 0xaf0162b3, /// 0x28
			 0x927f4741, /// 0x2c
			 0x5e3bf348, /// 0x30
			 0x45759716, /// 0x34
			 0x9acd0b60, /// 0x38
			 0x1aab29bc, /// 0x3c
			 0xaeb1f047, /// 0x40
			 0x9d17178f, /// 0x44
			 0x227dab93, /// 0x48
			 0xabbe3c74, /// 0x4c
			 0xc7379e40, /// 0x50
			 0xb2a8222d, /// 0x54
			 0x50335493, /// 0x58
			 0xa033fd17, /// 0x5c
			 0xea30c8de, /// 0x60
			 0x6f4a872e, /// 0x64
			 0xf48b2d28, /// 0x68
			 0x96683e2c, /// 0x6c
			 0xd4adcd4a, /// 0x70
			 0x275f3e3f, /// 0x74
			 0xe01ac299, /// 0x78
			 0x0926e9d7, /// 0x7c
			 0x046bff8f, /// 0x80
			 0x7ea900ef, /// 0x84
			 0x9b97a444, /// 0x88
			 0x8c3bda3c, /// 0x8c
			 0x6dd5cd30, /// 0x90
			 0xbc7dbc3f, /// 0x94
			 0x397e9430, /// 0x98
			 0x2afe4b79, /// 0x9c
			 0xe72e1da5, /// 0xa0
			 0x8b70a6cb, /// 0xa4
			 0x221cb850, /// 0xa8
			 0xed5a41e1, /// 0xac
			 0x9b8ec184, /// 0xb0
			 0x20eff5bb, /// 0xb4
			 0xe4b352e7, /// 0xb8
			 0x651d5f7e, /// 0xbc
			 0x40c21248, /// 0xc0
			 0xcf891072, /// 0xc4
			 0xd7821272, /// 0xc8
			 0x7f827217, /// 0xcc
			 0x33b9d8e4, /// 0xd0
			 0xdd14481f, /// 0xd4
			 0x6d9f4ccf, /// 0xd8
			 0x6eaf2e24, /// 0xdc
			 0x2c3ae6b4, /// 0xe0
			 0xfa99ac87, /// 0xe4
			 0xd48b9449, /// 0xe8
			 0x846406cd, /// 0xec
			 0xd1454b07, /// 0xf0
			 0xbd24c2f7, /// 0xf4
			 0x5a42ba39, /// 0xf8
			 0x7e9452f0, /// 0xfc
			 0x126ff118, /// 0x100
			 0x114fddcd, /// 0x104
			 0x06ff7d3f, /// 0x108
			 0x726aa4cd, /// 0x10c
			 0xa0b4f87a, /// 0x110
			 0x40637162, /// 0x114
			 0x6a01a976, /// 0x118
			 0x7d79b898, /// 0x11c
			 0x6672b7ef, /// 0x120
			 0xaac81260, /// 0x124
			 0x09d6b863, /// 0x128
			 0xf5a4da37, /// 0x12c
			 0x9c8eccbc, /// 0x130
			 0xaf289e0e, /// 0x134
			 0xd1be8f91, /// 0x138
			 0xc5ddfd5a, /// 0x13c
			 0x433a7bf6, /// 0x140
			 0xd3023ba7, /// 0x144
			 0xc879553e, /// 0x148
			 0x43665992, /// 0x14c
			 0x00a69ebf, /// 0x150
			 0x38c7c141, /// 0x154
			 0xb6fa0140, /// 0x158
			 0xf78b79c5, /// 0x15c
			 0x957e4dc5, /// 0x160
			 0x59c0e02c, /// 0x164
			 0x6008fe71, /// 0x168
			 0xc0b134c6, /// 0x16c
			 0x65c1ebf8, /// 0x170
			 0xde48277c, /// 0x174
			 0xa31ed71c, /// 0x178
			 0x5e4c72d8, /// 0x17c
			 0x9e789520, /// 0x180
			 0xf431881a, /// 0x184
			 0x9d7b48d5, /// 0x188
			 0xd3e55004, /// 0x18c
			 0x339440f8, /// 0x190
			 0x6ed6d6c9, /// 0x194
			 0x36078c73, /// 0x198
			 0x17a0b7c0, /// 0x19c
			 0x912ae563, /// 0x1a0
			 0x33c3462e, /// 0x1a4
			 0x70396cb1, /// 0x1a8
			 0xabe1b2c5, /// 0x1ac
			 0x73a60631, /// 0x1b0
			 0x420a8d3a, /// 0x1b4
			 0x16cafe1e, /// 0x1b8
			 0x59c66e84, /// 0x1bc
			 0x47e4db32, /// 0x1c0
			 0x37c04b16, /// 0x1c4
			 0xba9398e2, /// 0x1c8
			 0xd2c2b17f, /// 0x1cc
			 0xe4ba7efc, /// 0x1d0
			 0xda9cd026, /// 0x1d4
			 0x97aca475, /// 0x1d8
			 0xa7045323, /// 0x1dc
			 0x989bd6bd, /// 0x1e0
			 0xcbd5a031, /// 0x1e4
			 0x07a1c14d, /// 0x1e8
			 0x08db7406, /// 0x1ec
			 0x38e036de, /// 0x1f0
			 0xca819013, /// 0x1f4
			 0x410618ab, /// 0x1f8
			 0x27dd40d2, /// 0x1fc
			 0xcef934b2, /// 0x200
			 0xa6f7567d, /// 0x204
			 0xbe7da2d7, /// 0x208
			 0xe2bcfa8d, /// 0x20c
			 0x3148b623, /// 0x210
			 0x7c11e29a, /// 0x214
			 0x9f0e1a8e, /// 0x218
			 0x501a3e66, /// 0x21c
			 0x49812f49, /// 0x220
			 0x7630f258, /// 0x224
			 0xe5390ff9, /// 0x228
			 0x84e4dd57, /// 0x22c
			 0xd1a8484f, /// 0x230
			 0xe1ce1b78, /// 0x234
			 0x70d11591, /// 0x238
			 0x45e22e41, /// 0x23c
			 0x22dacf0f, /// 0x240
			 0x9b1268ed, /// 0x244
			 0x3c4808da, /// 0x248
			 0xf7274969, /// 0x24c
			 0x29a35a42, /// 0x250
			 0x0e89b40b, /// 0x254
			 0x7d216b76, /// 0x258
			 0xc15362b6, /// 0x25c
			 0x11783726, /// 0x260
			 0xc28cb8e7, /// 0x264
			 0xf5598f70, /// 0x268
			 0x3a0ca86b, /// 0x26c
			 0x6f917389, /// 0x270
			 0x9d50da44, /// 0x274
			 0x278931e8, /// 0x278
			 0x50d95628, /// 0x27c
			 0xa7ba1ce8, /// 0x280
			 0xc95bec85, /// 0x284
			 0xb0dc7a11, /// 0x288
			 0x75b0f1ec, /// 0x28c
			 0x16810709, /// 0x290
			 0x36e03217, /// 0x294
			 0x655ebcd2, /// 0x298
			 0xb8938856, /// 0x29c
			 0xd40102bc, /// 0x2a0
			 0x2fc68b14, /// 0x2a4
			 0xeaf142ad, /// 0x2a8
			 0xb6c2a6a3, /// 0x2ac
			 0xfb0a9d80, /// 0x2b0
			 0x7f93d1a0, /// 0x2b4
			 0xfd83536b, /// 0x2b8
			 0x2fdc389c, /// 0x2bc
			 0x03e4aaaa, /// 0x2c0
			 0xea855e1a, /// 0x2c4
			 0xf4c6b6bf, /// 0x2c8
			 0xe7db41bd, /// 0x2cc
			 0xd7b583b0, /// 0x2d0
			 0x0a49c897, /// 0x2d4
			 0xa2c051ae, /// 0x2d8
			 0x1b4b8eb1, /// 0x2dc
			 0x20533ac6, /// 0x2e0
			 0x97c57682, /// 0x2e4
			 0xfa8b0fb5, /// 0x2e8
			 0x0b7a76e1, /// 0x2ec
			 0xc99f5a6a, /// 0x2f0
			 0xde950b1b, /// 0x2f4
			 0x90c4e90a, /// 0x2f8
			 0x50d7fde5, /// 0x2fc
			 0x5fa81d9c, /// 0x300
			 0x882b7e46, /// 0x304
			 0x58f0c6df, /// 0x308
			 0x492be9d3, /// 0x30c
			 0x473d4033, /// 0x310
			 0xaa5115ec, /// 0x314
			 0xfc3610c6, /// 0x318
			 0xf6d37589, /// 0x31c
			 0xbf7af630, /// 0x320
			 0xec8e6f42, /// 0x324
			 0x12ff2ec1, /// 0x328
			 0x854b3759, /// 0x32c
			 0x6ba6fbd8, /// 0x330
			 0xe5f0bfa4, /// 0x334
			 0x55b838f0, /// 0x338
			 0x766929f3, /// 0x33c
			 0xd07fccf3, /// 0x340
			 0x6d273593, /// 0x344
			 0x738b1cc0, /// 0x348
			 0x16495df9, /// 0x34c
			 0xb992fc71, /// 0x350
			 0xc430e42a, /// 0x354
			 0x05775c16, /// 0x358
			 0x4ec973ad, /// 0x35c
			 0xda73f104, /// 0x360
			 0x2a4c9de6, /// 0x364
			 0xede4d39f, /// 0x368
			 0xe938d82d, /// 0x36c
			 0xf1954ee9, /// 0x370
			 0xa3af6847, /// 0x374
			 0x48d90368, /// 0x378
			 0xa5f001e3, /// 0x37c
			 0x492d1a3d, /// 0x380
			 0x85562e52, /// 0x384
			 0x9ecc03e1, /// 0x388
			 0x3d9d63cb, /// 0x38c
			 0xe4c48553, /// 0x390
			 0x40a7219e, /// 0x394
			 0xb8a582f8, /// 0x398
			 0x551d9dfd, /// 0x39c
			 0x87624c1b, /// 0x3a0
			 0xb6948461, /// 0x3a4
			 0xe84de101, /// 0x3a8
			 0xfb55da28, /// 0x3ac
			 0x566c3b03, /// 0x3b0
			 0xf9327998, /// 0x3b4
			 0x724e80cc, /// 0x3b8
			 0x397fe1b4, /// 0x3bc
			 0xf8032d4f, /// 0x3c0
			 0x7e0242f8, /// 0x3c4
			 0xf4f68eec, /// 0x3c8
			 0x25704a2b, /// 0x3cc
			 0x7848b3c8, /// 0x3d0
			 0xf37e25b4, /// 0x3d4
			 0x9da7067f, /// 0x3d8
			 0xc28e9b57, /// 0x3dc
			 0xe2c55e51, /// 0x3e0
			 0x6e73a412, /// 0x3e4
			 0x5275c427, /// 0x3e8
			 0xa4f9a302, /// 0x3ec
			 0xf59b66c5, /// 0x3f0
			 0x684f836c, /// 0x3f4
			 0x9b95c1ea, /// 0x3f8
			 0x1166aa59, /// 0x3fc
			 0x000c664e, /// 0x400
			 0x97bde050, /// 0x404
			 0xed5632bf, /// 0x408
			 0x34610e98, /// 0x40c
			 0x81e98065, /// 0x410
			 0x2aa539a8, /// 0x414
			 0x0fb7f36b, /// 0x418
			 0xb417a91a, /// 0x41c
			 0x1fe3488e, /// 0x420
			 0x38377e3d, /// 0x424
			 0x7766a021, /// 0x428
			 0x5f2fc502, /// 0x42c
			 0x6debe386, /// 0x430
			 0x3852d84b, /// 0x434
			 0xd3ecf6f2, /// 0x438
			 0x8cf5b5c8, /// 0x43c
			 0x5f69e07f, /// 0x440
			 0x5b79e756, /// 0x444
			 0x9e52d73d, /// 0x448
			 0xb4258d88, /// 0x44c
			 0xd390b0de, /// 0x450
			 0xe1d6580b, /// 0x454
			 0xa5ef41b8, /// 0x458
			 0x98689b26, /// 0x45c
			 0x9cdbed2f, /// 0x460
			 0x8903c446, /// 0x464
			 0x4b9dc72e, /// 0x468
			 0xbadc85ba, /// 0x46c
			 0xf753cbcf, /// 0x470
			 0xbb9a122b, /// 0x474
			 0x65377a8a, /// 0x478
			 0x81b990db, /// 0x47c
			 0xcb402141, /// 0x480
			 0xc56c1237, /// 0x484
			 0xa1ae0c04, /// 0x488
			 0x4643e0f2, /// 0x48c
			 0xe25a11f6, /// 0x490
			 0xaeafd788, /// 0x494
			 0xfe19862d, /// 0x498
			 0x58d2a372, /// 0x49c
			 0x939705d0, /// 0x4a0
			 0xfd6e3293, /// 0x4a4
			 0xd654c1be, /// 0x4a8
			 0xacdb8322, /// 0x4ac
			 0xb19caa07, /// 0x4b0
			 0x8448cb4c, /// 0x4b4
			 0x8cebf185, /// 0x4b8
			 0x45d80064, /// 0x4bc
			 0xb1f38273, /// 0x4c0
			 0xc2dcbd1d, /// 0x4c4
			 0xf1639270, /// 0x4c8
			 0x9654b732, /// 0x4cc
			 0xa9bc77cd, /// 0x4d0
			 0xbf0c22c1, /// 0x4d4
			 0xea763e96, /// 0x4d8
			 0x9b0573ce, /// 0x4dc
			 0xaddf7edb, /// 0x4e0
			 0x3ae2d08e, /// 0x4e4
			 0x751122e4, /// 0x4e8
			 0x441ec860, /// 0x4ec
			 0x543d1402, /// 0x4f0
			 0xbf5f1504, /// 0x4f4
			 0x6a8ccee8, /// 0x4f8
			 0x3fd0332d, /// 0x4fc
			 0xb6799d37, /// 0x500
			 0x40d2ba2e, /// 0x504
			 0x1df73bca, /// 0x508
			 0x8561d09e, /// 0x50c
			 0xae363494, /// 0x510
			 0x779d3dfc, /// 0x514
			 0x3c2b733e, /// 0x518
			 0xfe8d362c, /// 0x51c
			 0x6c993fa5, /// 0x520
			 0x84e0b47a, /// 0x524
			 0xbd9b02c0, /// 0x528
			 0x5432fffe, /// 0x52c
			 0x5a3f53df, /// 0x530
			 0xd3a799a2, /// 0x534
			 0xb6a0a888, /// 0x538
			 0x280d14f1, /// 0x53c
			 0xc41b0318, /// 0x540
			 0x7201710f, /// 0x544
			 0xf5a64d9c, /// 0x548
			 0x130b17c3, /// 0x54c
			 0x299f345e, /// 0x550
			 0x016bc211, /// 0x554
			 0x573401b8, /// 0x558
			 0x7c282783, /// 0x55c
			 0x3d5e0b11, /// 0x560
			 0x52d32275, /// 0x564
			 0x1144ad80, /// 0x568
			 0x8633d262, /// 0x56c
			 0x69be4d06, /// 0x570
			 0x96923e58, /// 0x574
			 0x81dabecd, /// 0x578
			 0x7e4d091e, /// 0x57c
			 0xe7366bd7, /// 0x580
			 0x14e4a665, /// 0x584
			 0xe743a4e5, /// 0x588
			 0x46141724, /// 0x58c
			 0x59be9c41, /// 0x590
			 0xffdec410, /// 0x594
			 0x4b439ca3, /// 0x598
			 0x5a18637d, /// 0x59c
			 0x4b4672a0, /// 0x5a0
			 0x1ec60956, /// 0x5a4
			 0xa561eb11, /// 0x5a8
			 0x292abe9a, /// 0x5ac
			 0x1c18a31a, /// 0x5b0
			 0x981b5cea, /// 0x5b4
			 0x06e9fd75, /// 0x5b8
			 0xb5f996ce, /// 0x5bc
			 0x4b50524b, /// 0x5c0
			 0x69f3a2b5, /// 0x5c4
			 0xe07bf4bc, /// 0x5c8
			 0xcf5d64c2, /// 0x5cc
			 0xb818966a, /// 0x5d0
			 0x168359aa, /// 0x5d4
			 0x7ad1d2b4, /// 0x5d8
			 0x26e6819c, /// 0x5dc
			 0x6a0e83cd, /// 0x5e0
			 0x5a0daff0, /// 0x5e4
			 0x91a50395, /// 0x5e8
			 0x8a55be33, /// 0x5ec
			 0x0d36757f, /// 0x5f0
			 0x38c2dd8b, /// 0x5f4
			 0x543556e7, /// 0x5f8
			 0x01d84fa3, /// 0x5fc
			 0xfb8d8bb4, /// 0x600
			 0x7223485e, /// 0x604
			 0x73f26170, /// 0x608
			 0x53e90b7b, /// 0x60c
			 0xeab32781, /// 0x610
			 0x5d0436e1, /// 0x614
			 0xde0f6fb2, /// 0x618
			 0x07c137d9, /// 0x61c
			 0xafd080b6, /// 0x620
			 0x201f8407, /// 0x624
			 0x8b50fadc, /// 0x628
			 0x36113f93, /// 0x62c
			 0xbf6a42a5, /// 0x630
			 0x978dc57a, /// 0x634
			 0x15d20367, /// 0x638
			 0x066a2087, /// 0x63c
			 0xdd6c6dd4, /// 0x640
			 0xb5ecd94b, /// 0x644
			 0xea800673, /// 0x648
			 0x6c1c6705, /// 0x64c
			 0x17b13069, /// 0x650
			 0x2ba5283f, /// 0x654
			 0x74b27587, /// 0x658
			 0x723d9f1a, /// 0x65c
			 0x36e8371b, /// 0x660
			 0x4670b08a, /// 0x664
			 0x385a0c68, /// 0x668
			 0x75e29192, /// 0x66c
			 0xecf742e3, /// 0x670
			 0x82eb8c85, /// 0x674
			 0xb8eb0c8e, /// 0x678
			 0xf75333a1, /// 0x67c
			 0x089b393e, /// 0x680
			 0xea154acd, /// 0x684
			 0xc01be3b5, /// 0x688
			 0xbe83dcc8, /// 0x68c
			 0x58bc3b8d, /// 0x690
			 0x7fe6574e, /// 0x694
			 0xc4c2eda3, /// 0x698
			 0xf8116243, /// 0x69c
			 0x9fdd02c7, /// 0x6a0
			 0xd34527a6, /// 0x6a4
			 0xba2b12d2, /// 0x6a8
			 0x445d2fd0, /// 0x6ac
			 0xda9cb9c5, /// 0x6b0
			 0xcf5d519d, /// 0x6b4
			 0xebd267c7, /// 0x6b8
			 0xe43b0f84, /// 0x6bc
			 0x414d4044, /// 0x6c0
			 0x7ee50c07, /// 0x6c4
			 0x397a0665, /// 0x6c8
			 0x0f571065, /// 0x6cc
			 0x5f296826, /// 0x6d0
			 0xb9ccc445, /// 0x6d4
			 0xd1835161, /// 0x6d8
			 0x0351aaa5, /// 0x6dc
			 0x0276b213, /// 0x6e0
			 0x6e1cbde9, /// 0x6e4
			 0x35dfe640, /// 0x6e8
			 0x8349a11e, /// 0x6ec
			 0xf1014ed9, /// 0x6f0
			 0x33e958f6, /// 0x6f4
			 0x38a3b80f, /// 0x6f8
			 0x16a80b4a, /// 0x6fc
			 0xf9d9bba1, /// 0x700
			 0x988f222e, /// 0x704
			 0x71d82c96, /// 0x708
			 0x29e9a090, /// 0x70c
			 0xdaa0d0cc, /// 0x710
			 0x9334e540, /// 0x714
			 0x75766e6e, /// 0x718
			 0x5c1f8153, /// 0x71c
			 0x82cf84dd, /// 0x720
			 0xee64f72e, /// 0x724
			 0xc48e313d, /// 0x728
			 0x70c4c614, /// 0x72c
			 0x10ce6ec8, /// 0x730
			 0x09b153a5, /// 0x734
			 0x47503b27, /// 0x738
			 0xa502ea14, /// 0x73c
			 0x7c3e67e5, /// 0x740
			 0x5c8945f5, /// 0x744
			 0xa8f2c2fe, /// 0x748
			 0x0af81b1c, /// 0x74c
			 0x41af1751, /// 0x750
			 0xc5bdf042, /// 0x754
			 0xef7f27b3, /// 0x758
			 0x981deb9f, /// 0x75c
			 0x1197b7d5, /// 0x760
			 0xf6afa7e6, /// 0x764
			 0x67d14a94, /// 0x768
			 0x7540d90e, /// 0x76c
			 0x96d8206d, /// 0x770
			 0xc9f3d0e4, /// 0x774
			 0x2ecffe85, /// 0x778
			 0xea16d012, /// 0x77c
			 0xcb7ee027, /// 0x780
			 0x0d7d3a91, /// 0x784
			 0xed032d7f, /// 0x788
			 0xba8a1bd8, /// 0x78c
			 0xf1281c3b, /// 0x790
			 0x9fca9c2f, /// 0x794
			 0x7c37c9b9, /// 0x798
			 0x791079c3, /// 0x79c
			 0xb915189b, /// 0x7a0
			 0xb6e01f96, /// 0x7a4
			 0x8589bcde, /// 0x7a8
			 0xde8f7362, /// 0x7ac
			 0x519a1628, /// 0x7b0
			 0x030bff7e, /// 0x7b4
			 0x19751d53, /// 0x7b8
			 0x9bbe082d, /// 0x7bc
			 0xf2035a0a, /// 0x7c0
			 0xc41c5546, /// 0x7c4
			 0x97061071, /// 0x7c8
			 0xd05c6196, /// 0x7cc
			 0xc3409517, /// 0x7d0
			 0x03a575ef, /// 0x7d4
			 0x814eac03, /// 0x7d8
			 0xab5984e1, /// 0x7dc
			 0x759f938c, /// 0x7e0
			 0xb3edcfd8, /// 0x7e4
			 0x255e5de2, /// 0x7e8
			 0x5aab6d3c, /// 0x7ec
			 0xe8e20765, /// 0x7f0
			 0x49d1c61f, /// 0x7f4
			 0xab5d6f0e, /// 0x7f8
			 0x324b227c, /// 0x7fc
			 0x8d2fc573, /// 0x800
			 0x862650ef, /// 0x804
			 0x66243760, /// 0x808
			 0x5396e395, /// 0x80c
			 0xf528c74a, /// 0x810
			 0x508f48d5, /// 0x814
			 0xbe6f5c04, /// 0x818
			 0x4183337a, /// 0x81c
			 0x2c589388, /// 0x820
			 0xdfe213e9, /// 0x824
			 0x0a59137a, /// 0x828
			 0xce9c8506, /// 0x82c
			 0xa117bf67, /// 0x830
			 0x7816d5e6, /// 0x834
			 0x039623b4, /// 0x838
			 0x6e5b84c2, /// 0x83c
			 0x580b507d, /// 0x840
			 0x533d3707, /// 0x844
			 0xfb4a6011, /// 0x848
			 0xf8806e90, /// 0x84c
			 0xaee14878, /// 0x850
			 0x52dc01ad, /// 0x854
			 0x19399f05, /// 0x858
			 0x7def319d, /// 0x85c
			 0x29a5fc2a, /// 0x860
			 0xb1eb5f03, /// 0x864
			 0x3fa61297, /// 0x868
			 0x0fae7f44, /// 0x86c
			 0x3f5e2e4d, /// 0x870
			 0xebccea83, /// 0x874
			 0x6d5e0b15, /// 0x878
			 0x409074e9, /// 0x87c
			 0xa8e75022, /// 0x880
			 0xb4e823bb, /// 0x884
			 0xb5af2289, /// 0x888
			 0xe489a6a7, /// 0x88c
			 0x72927c2f, /// 0x890
			 0x0e66e168, /// 0x894
			 0x96f988ef, /// 0x898
			 0x4538fc56, /// 0x89c
			 0x16bd2049, /// 0x8a0
			 0x97195709, /// 0x8a4
			 0xaa90e231, /// 0x8a8
			 0xef0390c3, /// 0x8ac
			 0x63e680be, /// 0x8b0
			 0x5cff30a6, /// 0x8b4
			 0xdad413d2, /// 0x8b8
			 0x03582329, /// 0x8bc
			 0x67cee237, /// 0x8c0
			 0x8a30f98e, /// 0x8c4
			 0x81999916, /// 0x8c8
			 0x197ba95e, /// 0x8cc
			 0xddd0d82e, /// 0x8d0
			 0x7d20b7be, /// 0x8d4
			 0x10a026fb, /// 0x8d8
			 0xd743c57c, /// 0x8dc
			 0x71e38c8b, /// 0x8e0
			 0x75a236bd, /// 0x8e4
			 0x997d4751, /// 0x8e8
			 0x398277d0, /// 0x8ec
			 0xa7d35fde, /// 0x8f0
			 0x3fadd3b6, /// 0x8f4
			 0x70c944e6, /// 0x8f8
			 0xeb0e9d4f, /// 0x8fc
			 0x1ad472ce, /// 0x900
			 0xf63f6cd4, /// 0x904
			 0x6ca2e69e, /// 0x908
			 0xf9bab284, /// 0x90c
			 0xb3283c23, /// 0x910
			 0xdbf39d46, /// 0x914
			 0x9ef49290, /// 0x918
			 0x677ddb6e, /// 0x91c
			 0xf7ef3c46, /// 0x920
			 0xbbe20f88, /// 0x924
			 0x953427de, /// 0x928
			 0xa1ec24a7, /// 0x92c
			 0xa15241bc, /// 0x930
			 0x8156be05, /// 0x934
			 0xe549d2b5, /// 0x938
			 0x822d3d4c, /// 0x93c
			 0xcde5d939, /// 0x940
			 0xc8e1970b, /// 0x944
			 0xf21fdef5, /// 0x948
			 0xf9a570b3, /// 0x94c
			 0x4835de8e, /// 0x950
			 0x615f148e, /// 0x954
			 0xe7230936, /// 0x958
			 0x9c9227e1, /// 0x95c
			 0x7a5cee2b, /// 0x960
			 0xe35edc7f, /// 0x964
			 0x9dbdfbf4, /// 0x968
			 0x1602cac9, /// 0x96c
			 0x5f9f5853, /// 0x970
			 0x6b37cbc7, /// 0x974
			 0xd85499e7, /// 0x978
			 0xc90c3ae3, /// 0x97c
			 0x4c434731, /// 0x980
			 0x75b4464e, /// 0x984
			 0xa23cdf6b, /// 0x988
			 0xa6c5c103, /// 0x98c
			 0xedf46fd1, /// 0x990
			 0x5cad6da9, /// 0x994
			 0x6ea2b5c9, /// 0x998
			 0x6b6ad9b6, /// 0x99c
			 0x3b809f26, /// 0x9a0
			 0xf3017164, /// 0x9a4
			 0xf4a80bd4, /// 0x9a8
			 0xf53248b4, /// 0x9ac
			 0xa950f749, /// 0x9b0
			 0xe57e0ec8, /// 0x9b4
			 0x05bfe905, /// 0x9b8
			 0x8c1742db, /// 0x9bc
			 0x3b6d54f5, /// 0x9c0
			 0xd226e238, /// 0x9c4
			 0x529e4e50, /// 0x9c8
			 0x296ca8f8, /// 0x9cc
			 0x40a0c74b, /// 0x9d0
			 0x8c3eadd5, /// 0x9d4
			 0x49179d23, /// 0x9d8
			 0x772f5911, /// 0x9dc
			 0x89f08e22, /// 0x9e0
			 0xb7a4a90e, /// 0x9e4
			 0x84e363f9, /// 0x9e8
			 0x0b7e0f25, /// 0x9ec
			 0x4f899c4f, /// 0x9f0
			 0xc87c1f9c, /// 0x9f4
			 0xd66403f9, /// 0x9f8
			 0x91039583, /// 0x9fc
			 0xdf2b6654, /// 0xa00
			 0x244526af, /// 0xa04
			 0x7fa2ae3c, /// 0xa08
			 0xb8fcf774, /// 0xa0c
			 0xd4b5a64f, /// 0xa10
			 0x297e1e61, /// 0xa14
			 0x0e34bf6b, /// 0xa18
			 0x7984c0fd, /// 0xa1c
			 0x1aac0f17, /// 0xa20
			 0xf5b4627d, /// 0xa24
			 0x3ba9384f, /// 0xa28
			 0xafcc44b2, /// 0xa2c
			 0xe4b29b82, /// 0xa30
			 0xe477f13d, /// 0xa34
			 0x25cba633, /// 0xa38
			 0x244c5bd7, /// 0xa3c
			 0x9fa42711, /// 0xa40
			 0xf51159f3, /// 0xa44
			 0xec81dbaf, /// 0xa48
			 0x8fdefa11, /// 0xa4c
			 0xc80723ea, /// 0xa50
			 0x3b7bb66e, /// 0xa54
			 0x057cc53d, /// 0xa58
			 0xd5111093, /// 0xa5c
			 0xa5f4fd2e, /// 0xa60
			 0x22e6dd03, /// 0xa64
			 0x3a29c5e1, /// 0xa68
			 0x3bd8a2db, /// 0xa6c
			 0x2bead2a5, /// 0xa70
			 0xd17f1239, /// 0xa74
			 0xc2b170fa, /// 0xa78
			 0xbef37a55, /// 0xa7c
			 0x8c8b4e51, /// 0xa80
			 0x5864492c, /// 0xa84
			 0x9a7e4ff9, /// 0xa88
			 0xb06734ca, /// 0xa8c
			 0x79c9ac55, /// 0xa90
			 0xc3c6a937, /// 0xa94
			 0x85682363, /// 0xa98
			 0xa2d09d4e, /// 0xa9c
			 0x9588b022, /// 0xaa0
			 0x63688b3b, /// 0xaa4
			 0x73717f0a, /// 0xaa8
			 0x63f94bb4, /// 0xaac
			 0xbc1230da, /// 0xab0
			 0x1a079be8, /// 0xab4
			 0x2aceea52, /// 0xab8
			 0xbf5fecea, /// 0xabc
			 0xd3687541, /// 0xac0
			 0x734a0ea6, /// 0xac4
			 0x75dd1ae6, /// 0xac8
			 0xe80eb519, /// 0xacc
			 0x8fa8f248, /// 0xad0
			 0x2f6f346d, /// 0xad4
			 0x1b6ec1e6, /// 0xad8
			 0x88af62e4, /// 0xadc
			 0x2a9ecc64, /// 0xae0
			 0x335f6733, /// 0xae4
			 0x769df8d3, /// 0xae8
			 0x8ad959a5, /// 0xaec
			 0x851ea109, /// 0xaf0
			 0x6e1bcee7, /// 0xaf4
			 0x528b8b9a, /// 0xaf8
			 0x7cb52bc1, /// 0xafc
			 0x7f1b765e, /// 0xb00
			 0x8b49360c, /// 0xb04
			 0xd2d79dc1, /// 0xb08
			 0x98f399e4, /// 0xb0c
			 0xfe7c1d85, /// 0xb10
			 0x6d198dbf, /// 0xb14
			 0x42def7f7, /// 0xb18
			 0xf807ab09, /// 0xb1c
			 0x49dfbf9a, /// 0xb20
			 0x7aec1c2f, /// 0xb24
			 0x722a8f9c, /// 0xb28
			 0x420f6e0f, /// 0xb2c
			 0xcdabb959, /// 0xb30
			 0xc6a71beb, /// 0xb34
			 0x65454254, /// 0xb38
			 0xbe58aeed, /// 0xb3c
			 0x7fd62ad3, /// 0xb40
			 0x137f08d2, /// 0xb44
			 0x82d61c2f, /// 0xb48
			 0xce6372c7, /// 0xb4c
			 0x514967e8, /// 0xb50
			 0xb3ac732d, /// 0xb54
			 0x5bda0856, /// 0xb58
			 0x8829a1f5, /// 0xb5c
			 0x8eda7acd, /// 0xb60
			 0xc6900fbf, /// 0xb64
			 0x8dd106ff, /// 0xb68
			 0x1075f9ef, /// 0xb6c
			 0x062f543c, /// 0xb70
			 0xec0fa4e1, /// 0xb74
			 0x91fdeeb5, /// 0xb78
			 0x49b1b476, /// 0xb7c
			 0x6ede384c, /// 0xb80
			 0x1b1b7fda, /// 0xb84
			 0xd728c4e5, /// 0xb88
			 0xfa3f24b6, /// 0xb8c
			 0xa3764cbc, /// 0xb90
			 0x3cd696fc, /// 0xb94
			 0xf7b9c1f1, /// 0xb98
			 0xdc088113, /// 0xb9c
			 0x31e0a0b3, /// 0xba0
			 0x9e1edd01, /// 0xba4
			 0x77f99c06, /// 0xba8
			 0x02749648, /// 0xbac
			 0x31ea8188, /// 0xbb0
			 0x87f23aeb, /// 0xbb4
			 0xe08a26d6, /// 0xbb8
			 0x98a8cbdc, /// 0xbbc
			 0x1130e6f3, /// 0xbc0
			 0x67d17a49, /// 0xbc4
			 0x1285d493, /// 0xbc8
			 0xd472140d, /// 0xbcc
			 0xd686f24a, /// 0xbd0
			 0xd9d81ddb, /// 0xbd4
			 0x9b9133b3, /// 0xbd8
			 0x59ec277f, /// 0xbdc
			 0xecc3a116, /// 0xbe0
			 0xbd0476d0, /// 0xbe4
			 0xa5b4dfae, /// 0xbe8
			 0xe56aa37e, /// 0xbec
			 0xdda9fb2a, /// 0xbf0
			 0x474536cf, /// 0xbf4
			 0xfe5b2b55, /// 0xbf8
			 0xa8515aab, /// 0xbfc
			 0xbf09e19f, /// 0xc00
			 0x1c3e24f0, /// 0xc04
			 0x3f2cc16c, /// 0xc08
			 0x70dce7aa, /// 0xc0c
			 0x7cc3f79e, /// 0xc10
			 0xcc612a50, /// 0xc14
			 0x06755880, /// 0xc18
			 0x41c58c00, /// 0xc1c
			 0xeec5154f, /// 0xc20
			 0xbc3725b5, /// 0xc24
			 0x1101556a, /// 0xc28
			 0xf35efa11, /// 0xc2c
			 0x54e23c19, /// 0xc30
			 0x00425de9, /// 0xc34
			 0x3fa003d9, /// 0xc38
			 0xc6af9d25, /// 0xc3c
			 0xd4395913, /// 0xc40
			 0x065343a1, /// 0xc44
			 0xfe49eb25, /// 0xc48
			 0xe3997b21, /// 0xc4c
			 0xc1c6a6fd, /// 0xc50
			 0x0687f517, /// 0xc54
			 0xdba7b14e, /// 0xc58
			 0x0353912b, /// 0xc5c
			 0xb4140e93, /// 0xc60
			 0xc95bb6f0, /// 0xc64
			 0x934fee30, /// 0xc68
			 0xee9e58fc, /// 0xc6c
			 0xa474e03e, /// 0xc70
			 0xd8b88a31, /// 0xc74
			 0xd10bc8ee, /// 0xc78
			 0x27c4f135, /// 0xc7c
			 0x25bb6577, /// 0xc80
			 0xc56f6c98, /// 0xc84
			 0x9d57224d, /// 0xc88
			 0x27c48a4d, /// 0xc8c
			 0x070e1ad6, /// 0xc90
			 0x4787ef4f, /// 0xc94
			 0x6dc3a4c6, /// 0xc98
			 0xdaf6db3f, /// 0xc9c
			 0x4436d757, /// 0xca0
			 0x04c5bbc2, /// 0xca4
			 0x6a509698, /// 0xca8
			 0x943c3637, /// 0xcac
			 0xc6cdd88a, /// 0xcb0
			 0x06a6bfe2, /// 0xcb4
			 0x7e93e688, /// 0xcb8
			 0x236c4dae, /// 0xcbc
			 0x06a5cf93, /// 0xcc0
			 0xbe6c27b3, /// 0xcc4
			 0x4392ef24, /// 0xcc8
			 0x3eb1dc2c, /// 0xccc
			 0x9952bc9e, /// 0xcd0
			 0xa98f24cd, /// 0xcd4
			 0x90a11968, /// 0xcd8
			 0xdcc537e1, /// 0xcdc
			 0x390a3d93, /// 0xce0
			 0xfe8c227c, /// 0xce4
			 0x23ab9442, /// 0xce8
			 0xa03df41e, /// 0xcec
			 0x63914380, /// 0xcf0
			 0x6d07cd97, /// 0xcf4
			 0xef05a712, /// 0xcf8
			 0xaca10485, /// 0xcfc
			 0x98813edb, /// 0xd00
			 0x0e13b4ee, /// 0xd04
			 0xb69af619, /// 0xd08
			 0x40c363e7, /// 0xd0c
			 0x57282a6a, /// 0xd10
			 0x7f2d1ed4, /// 0xd14
			 0xf23279de, /// 0xd18
			 0xea74c36c, /// 0xd1c
			 0x3e1124d0, /// 0xd20
			 0xc431c578, /// 0xd24
			 0xb9fd8e4f, /// 0xd28
			 0xe2f5a546, /// 0xd2c
			 0x5dabc669, /// 0xd30
			 0x9a9f2357, /// 0xd34
			 0x7b62774f, /// 0xd38
			 0xbb0005df, /// 0xd3c
			 0x9f88aa37, /// 0xd40
			 0x73aba18d, /// 0xd44
			 0xdbf9c4d2, /// 0xd48
			 0x63b18c6c, /// 0xd4c
			 0xd2c2216c, /// 0xd50
			 0xdc4e2f6d, /// 0xd54
			 0x44e94976, /// 0xd58
			 0x13b2348c, /// 0xd5c
			 0x3075c8e7, /// 0xd60
			 0xa766b8a7, /// 0xd64
			 0xade0c3a1, /// 0xd68
			 0x3cff741c, /// 0xd6c
			 0x03d596ac, /// 0xd70
			 0x43f22793, /// 0xd74
			 0x7319847d, /// 0xd78
			 0x41600d95, /// 0xd7c
			 0x52cba6e5, /// 0xd80
			 0xf25af197, /// 0xd84
			 0x71521055, /// 0xd88
			 0x36781dd2, /// 0xd8c
			 0x3308c1f1, /// 0xd90
			 0xa15e978c, /// 0xd94
			 0xb4f3411a, /// 0xd98
			 0x4651a707, /// 0xd9c
			 0x2317b3ff, /// 0xda0
			 0x7eb0580b, /// 0xda4
			 0xdfc67a8f, /// 0xda8
			 0x30a6f76b, /// 0xdac
			 0xafbf26ed, /// 0xdb0
			 0x6c65b797, /// 0xdb4
			 0x00ac259f, /// 0xdb8
			 0x1ad14ea1, /// 0xdbc
			 0x5c0d1a62, /// 0xdc0
			 0x8621707a, /// 0xdc4
			 0xee12553f, /// 0xdc8
			 0x47081e31, /// 0xdcc
			 0x76190414, /// 0xdd0
			 0x0e77a1e0, /// 0xdd4
			 0x1ca0bf85, /// 0xdd8
			 0x67671cff, /// 0xddc
			 0x2387f8b9, /// 0xde0
			 0x04423f48, /// 0xde4
			 0x9da3251a, /// 0xde8
			 0xbd7f8e5e, /// 0xdec
			 0xd89d5d80, /// 0xdf0
			 0x1d8144b8, /// 0xdf4
			 0xb4b0b45b, /// 0xdf8
			 0x583f7b00, /// 0xdfc
			 0x638fe38e, /// 0xe00
			 0x5834078a, /// 0xe04
			 0xb741c681, /// 0xe08
			 0x4b47e29c, /// 0xe0c
			 0x8f4decb2, /// 0xe10
			 0x66a30a67, /// 0xe14
			 0x3dd4f977, /// 0xe18
			 0xe4a1ef58, /// 0xe1c
			 0xb3ca069a, /// 0xe20
			 0x36f47581, /// 0xe24
			 0x58ffb255, /// 0xe28
			 0xee870748, /// 0xe2c
			 0xc19f4698, /// 0xe30
			 0x8c64ff73, /// 0xe34
			 0x3e3671b3, /// 0xe38
			 0xbc265ae0, /// 0xe3c
			 0x48e5dc2f, /// 0xe40
			 0x508c6458, /// 0xe44
			 0x1574d963, /// 0xe48
			 0x9fb1b995, /// 0xe4c
			 0x4c117773, /// 0xe50
			 0x6fb7d022, /// 0xe54
			 0xa2281fc8, /// 0xe58
			 0x266c3a1c, /// 0xe5c
			 0x15dafbea, /// 0xe60
			 0x3c5b9951, /// 0xe64
			 0xf11ef0ab, /// 0xe68
			 0x83c5a605, /// 0xe6c
			 0xe4080595, /// 0xe70
			 0x97072130, /// 0xe74
			 0x3a423797, /// 0xe78
			 0x6510a65e, /// 0xe7c
			 0xc17b8302, /// 0xe80
			 0x2557f676, /// 0xe84
			 0x77ae2d22, /// 0xe88
			 0x8e7c4da7, /// 0xe8c
			 0xdfbbf92f, /// 0xe90
			 0xbff8fa65, /// 0xe94
			 0x08223471, /// 0xe98
			 0xfafff8d3, /// 0xe9c
			 0xf724f2ed, /// 0xea0
			 0xb1305dc1, /// 0xea4
			 0xbd3a4649, /// 0xea8
			 0xfdacdbcd, /// 0xeac
			 0x3af47c80, /// 0xeb0
			 0xc7a2f857, /// 0xeb4
			 0xee1e5a6c, /// 0xeb8
			 0xdceff677, /// 0xebc
			 0xc65084ae, /// 0xec0
			 0xf48a0274, /// 0xec4
			 0x45a9df1a, /// 0xec8
			 0x11c0c357, /// 0xecc
			 0xadba6bef, /// 0xed0
			 0x994ea3dc, /// 0xed4
			 0x26f4560a, /// 0xed8
			 0xaf8e3c57, /// 0xedc
			 0xaab574ae, /// 0xee0
			 0x774f798c, /// 0xee4
			 0xfa53c8b5, /// 0xee8
			 0xfb407406, /// 0xeec
			 0x0b1b831a, /// 0xef0
			 0x9abe67d4, /// 0xef4
			 0xda99162d, /// 0xef8
			 0xd2205b94, /// 0xefc
			 0x49d95f64, /// 0xf00
			 0xc65b15e6, /// 0xf04
			 0x173392d8, /// 0xf08
			 0x9bf76313, /// 0xf0c
			 0xfacb3435, /// 0xf10
			 0x66634824, /// 0xf14
			 0xfb2df503, /// 0xf18
			 0x5c534742, /// 0xf1c
			 0x42388526, /// 0xf20
			 0xf5fac840, /// 0xf24
			 0xdf093bdf, /// 0xf28
			 0xaeee2483, /// 0xf2c
			 0xf8c4c835, /// 0xf30
			 0xa3d50fc5, /// 0xf34
			 0xc464a8f5, /// 0xf38
			 0x4a4ffe41, /// 0xf3c
			 0x742796dd, /// 0xf40
			 0xde01ccf4, /// 0xf44
			 0x3a310ec9, /// 0xf48
			 0x1f707bd4, /// 0xf4c
			 0x591f09bd, /// 0xf50
			 0x5a286744, /// 0xf54
			 0xf01ff8e6, /// 0xf58
			 0x5e8a88ec, /// 0xf5c
			 0x05d01561, /// 0xf60
			 0x4792a366, /// 0xf64
			 0x890db28b, /// 0xf68
			 0xef83e9bb, /// 0xf6c
			 0x235b798c, /// 0xf70
			 0xe784fb3f, /// 0xf74
			 0x5196767c, /// 0xf78
			 0xc34a9296, /// 0xf7c
			 0x858f5ac8, /// 0xf80
			 0x5a1bf938, /// 0xf84
			 0x0acd12c1, /// 0xf88
			 0x5e011bdf, /// 0xf8c
			 0x3dc88986, /// 0xf90
			 0xf09783f5, /// 0xf94
			 0x20521b3a, /// 0xf98
			 0xaf13381c, /// 0xf9c
			 0x6de1967d, /// 0xfa0
			 0x73e96747, /// 0xfa4
			 0x5c5bac7c, /// 0xfa8
			 0xfaddbae8, /// 0xfac
			 0x80ea449f, /// 0xfb0
			 0x5e54e180, /// 0xfb4
			 0x1e4bb034, /// 0xfb8
			 0xfb8fdad3, /// 0xfbc
			 0x1c99dde1, /// 0xfc0
			 0x6cae4464, /// 0xfc4
			 0x8bb39390, /// 0xfc8
			 0x05beadf1, /// 0xfcc
			 0x151303a7, /// 0xfd0
			 0xe6ef29f5, /// 0xfd4
			 0x0b807e13, /// 0xfd8
			 0xf235e36d, /// 0xfdc
			 0xf8392ad6, /// 0xfe0
			 0xe76851a9, /// 0xfe4
			 0x5c57e628, /// 0xfe8
			 0x01c2aeaa, /// 0xfec
			 0x100bd255, /// 0xff0
			 0x5d0c7835, /// 0xff4
			 0x05da5cb9, /// 0xff8
			 0x6878892b /// last
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
			 0x000000a0,
			 0x0000052c,
			 0x000006c4,
			 0x000006d4,
			 0x000007a8,
			 0x00000458,
			 0x0000030c,
			 0x000001d8,

			 /// vpu register f2
			 0x42000000,
			 0x41e00000,
			 0x41880000,
			 0x41c80000,
			 0x41f80000,
			 0x41c80000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f3
			 0x41c80000,
			 0x41980000,
			 0x41200000,
			 0x41200000,
			 0x41800000,
			 0x40e00000,
			 0x41800000,
			 0x41200000,

			 /// vpu register f4
			 0x41800000,
			 0x41c80000,
			 0x41b00000,
			 0x40a00000,
			 0x41880000,
			 0x40400000,
			 0x41b80000,
			 0x41e80000,

			 /// vpu register f5
			 0x41100000,
			 0x40a00000,
			 0x41b80000,
			 0x41500000,
			 0x40400000,
			 0x41d00000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f6
			 0x41c00000,
			 0x41a80000,
			 0x41700000,
			 0x41600000,
			 0x41c80000,
			 0x41800000,
			 0x41600000,
			 0x41f80000,

			 /// vpu register f7
			 0x41d80000,
			 0x41800000,
			 0x41c80000,
			 0x41000000,
			 0x41800000,
			 0x3f800000,
			 0x41f80000,
			 0x41a00000,

			 /// vpu register f8
			 0x41e00000,
			 0x3f800000,
			 0x41700000,
			 0x41300000,
			 0x41c00000,
			 0x41900000,
			 0x40a00000,
			 0x41700000,

			 /// vpu register f9
			 0x41c00000,
			 0x41a00000,
			 0x41600000,
			 0x41a00000,
			 0x41800000,
			 0x42000000,
			 0x41400000,
			 0x40c00000,

			 /// vpu register f10
			 0x41880000,
			 0x41980000,
			 0x41880000,
			 0x41000000,
			 0x40000000,
			 0x40000000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f11
			 0x41000000,
			 0x41100000,
			 0x41a80000,
			 0x3f800000,
			 0x41100000,
			 0x40400000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f12
			 0x41e00000,
			 0x41a00000,
			 0x40c00000,
			 0x41e00000,
			 0x41e80000,
			 0x41a80000,
			 0x41800000,
			 0x41700000,

			 /// vpu register f13
			 0x41900000,
			 0x41f00000,
			 0x41000000,
			 0x41e80000,
			 0x41900000,
			 0x41800000,
			 0x41d80000,
			 0x40400000,

			 /// vpu register f14
			 0x41f00000,
			 0x40c00000,
			 0x41600000,
			 0x41f00000,
			 0x41900000,
			 0x40400000,
			 0x40800000,
			 0x41400000,

			 /// vpu register f15
			 0x41200000,
			 0x40800000,
			 0x41f80000,
			 0x41200000,
			 0x41b80000,
			 0x41500000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f16
			 0x40800000,
			 0x41300000,
			 0x41980000,
			 0x41d00000,
			 0x41d00000,
			 0x41a80000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f17
			 0x40800000,
			 0x41e80000,
			 0x41700000,
			 0x42000000,
			 0x41b00000,
			 0x41e80000,
			 0x41e80000,
			 0x41c80000,

			 /// vpu register f18
			 0x41200000,
			 0x41400000,
			 0x41300000,
			 0x41600000,
			 0x41700000,
			 0x40e00000,
			 0x41c00000,
			 0x41500000,

			 /// vpu register f19
			 0x41300000,
			 0x41980000,
			 0x41000000,
			 0x41c80000,
			 0x41c00000,
			 0x41f00000,
			 0x41700000,
			 0x41b80000,

			 /// vpu register f20
			 0x41900000,
			 0x41d80000,
			 0x40000000,
			 0x41300000,
			 0x42000000,
			 0x40000000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f21
			 0x41200000,
			 0x40e00000,
			 0x41000000,
			 0x41d80000,
			 0x41f80000,
			 0x41500000,
			 0x41d00000,
			 0x40a00000,

			 /// vpu register f22
			 0x3f800000,
			 0x41d80000,
			 0x40400000,
			 0x41f80000,
			 0x41400000,
			 0x41c80000,
			 0x41a00000,
			 0x40000000,

			 /// vpu register f23
			 0x41600000,
			 0x41200000,
			 0x40c00000,
			 0x41e80000,
			 0x41300000,
			 0x41c00000,
			 0x3f800000,
			 0x41b80000,

			 /// vpu register f24
			 0x41f00000,
			 0x41400000,
			 0x41300000,
			 0x41400000,
			 0x41f80000,
			 0x41c80000,
			 0x41e00000,
			 0x41b80000,

			 /// vpu register f25
			 0x41d00000,
			 0x41f80000,
			 0x41300000,
			 0x41c80000,
			 0x41000000,
			 0x41700000,
			 0x41880000,
			 0x41880000,

			 /// vpu register f26
			 0x41900000,
			 0x3f800000,
			 0x41980000,
			 0x41b00000,
			 0x41300000,
			 0x41980000,
			 0x41a80000,
			 0x41880000,

			 /// vpu register f27
			 0x3f800000,
			 0x41800000,
			 0x41200000,
			 0x40400000,
			 0x41f00000,
			 0x41a00000,
			 0x41100000,
			 0x41d00000,

			 /// vpu register f28
			 0x3f800000,
			 0x40400000,
			 0x41800000,
			 0x41700000,
			 0x41100000,
			 0x41500000,
			 0x41880000,
			 0x41500000,

			 /// vpu register f29
			 0x41f00000,
			 0x41d00000,
			 0x41c00000,
			 0x40e00000,
			 0x41200000,
			 0x41900000,
			 0x41980000,
			 0x41a80000,

			 /// vpu register f30
			 0x41800000,
			 0x41b00000,
			 0x40a00000,
			 0x41400000,
			 0x40400000,
			 0x41100000,
			 0x41d00000,
			 0x41b80000,

			 /// vpu register f31
			 0x41d00000,
			 0x41d00000,
			 0x41900000,
			 0x41f80000,
			 0x41c80000,
			 0x41c80000,
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
		"fsat8.pi f28, f14\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f15, f22\n"                                 ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f21, f7\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f20, f20\n"                                 ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f8, f19\n"                                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f21, f10\n"                                 ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f30, f21\n"                                 ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f18, f7\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f2, f25\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f11, f2\n"                                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f26, f24\n"                                 ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f18, f16\n"                                 ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f14, f9\n"                                  ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f13, f7\n"                                  ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f28, f6\n"                                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f30, f4\n"                                  ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f23, f16\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f16, f19\n"                                 ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f19, f7\n"                                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f18, f3\n"                                  ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f23, f27\n"                                 ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f23, f14\n"                                 ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f21, f0\n"                                  ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f18, f29\n"                                 ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f5, f23\n"                                  ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f25, f19\n"                                 ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f14, f28\n"                                 ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f8, f0\n"                                   ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f24, f30\n"                                 ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f10, f4\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f12, f29\n"                                 ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f9, f8\n"                                   ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f9, f6\n"                                   ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f9, f19\n"                                  ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f28, f5\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f29, f11\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f19, f3\n"                                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f24, f20\n"                                 ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f27, f25\n"                                 ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f13, f17\n"                                 ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f25, f0\n"                                  ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f26, f20\n"                                 ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f2, f16\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f27, f15\n"                                 ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f9, f1\n"                                   ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f20, f27\n"                                 ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f26, f1\n"                                  ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f11, f17\n"                                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f6, f11\n"                                  ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f25, f19\n"                                 ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f23, f8\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f11, f11\n"                                 ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f6, f20\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f4, f22\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f17, f25\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f13, f27\n"                                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f27, f6\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f16, f29\n"                                 ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f4, f15\n"                                  ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f23, f14\n"                                 ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f25, f25\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f2, f25\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f3, f4\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f18, f16\n"                                 ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f22, f29\n"                                 ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f9, f27\n"                                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f17, f23\n"                                 ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f15, f15\n"                                 ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f11, f27\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f14, f16\n"                                 ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f18, f29\n"                                 ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f26, f22\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f26, f28\n"                                 ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f17, f28\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f16, f4\n"                                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f14, f16\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f4, f18\n"                                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f8, f26\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f29, f10\n"                                 ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f22, f9\n"                                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f7, f16\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f10, f14\n"                                 ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f30, f30\n"                                 ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f7, f20\n"                                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f14, f14\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f12, f3\n"                                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f27, f15\n"                                 ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f30, f0\n"                                  ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f17, f11\n"                                 ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f11, f17\n"                                 ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f28, f17\n"                                 ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f17, f29\n"                                 ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f16, f30\n"                                 ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f21, f24\n"                                 ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f12, f12\n"                                 ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f7, f17\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f24, f1\n"                                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f6, f26\n"                                  ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsat8.pi f10, f1\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
