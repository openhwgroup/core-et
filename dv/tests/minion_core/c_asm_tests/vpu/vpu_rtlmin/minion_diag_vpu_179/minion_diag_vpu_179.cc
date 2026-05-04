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
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00000
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00004
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00010
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00014
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00018
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00020
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0c400000,                                                  // Leading 1s:                                 /// 00028
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00030
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00038
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00040
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00044
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00048
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0004c
			 0x3f028f5c,                                                  // 0.51                                        /// 00050
			 0x0e000001,                                                  // Trailing 1s:                                /// 00054
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0005c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00070
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00074
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00078
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00080
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00084
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00088
			 0x33333333,                                                  // 4 random values                             /// 0008c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0009c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000b8
			 0x55555555,                                                  // 4 random values                             /// 000bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00104
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00108
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0010c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00110
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00118
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0011c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00120
			 0x00000000,                                                  // zero                                        /// 00124
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00128
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0012c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00130
			 0x0e000007,                                                  // Trailing 1s:                                /// 00134
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00138
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0013c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00154
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00158
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00160
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00164
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0016c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00170
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00174
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00178
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00180
			 0x7f800000,                                                  // inf                                         /// 00184
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00188
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00190
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00194
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00198
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0019c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f4
			 0x7f800000,                                                  // inf                                         /// 001f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00200
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00204
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00208
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00214
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0021c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00224
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00228
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0022c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00234
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00238
			 0x00000000,                                                  // zero                                        /// 0023c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00240
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00248
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0024c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00250
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00254
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00260
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00264
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00268
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0027c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00284
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00294
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00298
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0029c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x33333333,                                                  // 4 random values                             /// 002cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x3f028f5c,                                                  // 0.51                                        /// 002f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00304
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00318
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0031c
			 0x33333333,                                                  // 4 random values                             /// 00320
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00324
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0032c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00334
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00338
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00350
			 0x0c700000,                                                  // Leading 1s:                                 /// 00354
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00358
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00360
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00364
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00368
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00374
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0037c
			 0x4b000000,                                                  // 8388608.0                                   /// 00380
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00384
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00388
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00000000,                                                  // zero                                        /// 00394
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00398
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0039c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00404
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0040c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00410
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00414
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00418
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0041c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00420
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00428
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0042c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00430
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00444
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0044c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00450
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x4b000000,                                                  // 8388608.0                                   /// 0045c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0046c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00474
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00478
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00480
			 0x00011111,                                                  // 9.7958E-41                                  /// 00484
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00488
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00498
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0049c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x33333333,                                                  // 4 random values                             /// 004b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004bc
			 0x33333333,                                                  // 4 random values                             /// 004c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004dc
			 0x33333333,                                                  // 4 random values                             /// 004e0
			 0x00000000,                                                  // zero                                        /// 004e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0e000001,                                                  // Trailing 1s:                                /// 00504
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00508
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0050c
			 0xff800000,                                                  // -inf                                        /// 00510
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00518
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0051c
			 0x4b000000,                                                  // 8388608.0                                   /// 00520
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00524
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00528
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0052c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00530
			 0xffc00001,                                                  // -signaling NaN                              /// 00534
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x33333333,                                                  // 4 random values                             /// 00540
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00548
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0054c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x7fc00001,                                                  // signaling NaN                               /// 00554
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00558
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00568
			 0x55555555,                                                  // 4 random values                             /// 0056c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00570
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x4b000000,                                                  // 8388608.0                                   /// 00584
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00588
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0059c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c0
			 0x00000000,                                                  // zero                                        /// 005c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00608
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00610
			 0x0e000003,                                                  // Trailing 1s:                                /// 00614
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00620
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00624
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0062c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00630
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0063c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00644
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00648
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0064c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00650
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00658
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0065c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00660
			 0x0e000007,                                                  // Trailing 1s:                                /// 00664
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00011111,                                                  // 9.7958E-41                                  /// 0066c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00674
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00678
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0067c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00684
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00698
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0069c
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006a8
			 0x7fc00001,                                                  // signaling NaN                               /// 006ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xff800000,                                                  // -inf                                        /// 006b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0xcb000000,                                                  // -8388608.0                                  /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f0
			 0xffc00001,                                                  // -signaling NaN                              /// 006f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00708
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0070c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00710
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00718
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0071c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00720
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0072c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00740
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00744
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00750
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00758
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0075c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00760
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00768
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0076c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00770
			 0x33333333,                                                  // 4 random values                             /// 00774
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00778
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00784
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00788
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0078c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00790
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007b8
			 0x4b000000,                                                  // 8388608.0                                   /// 007bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e8
			 0xffc00001,                                                  // -signaling NaN                              /// 007ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00800
			 0x0c780000,                                                  // Leading 1s:                                 /// 00804
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00814
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00820
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00838
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0083c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00844
			 0xbf028f5c,                                                  // -0.51                                       /// 00848
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0084c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00850
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00860
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00864
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00868
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0086c
			 0x7f800000,                                                  // inf                                         /// 00870
			 0x0c600000,                                                  // Leading 1s:                                 /// 00874
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0087c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00880
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e000007,                                                  // Trailing 1s:                                /// 0088c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00890
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00898
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0089c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00000000,                                                  // zero                                        /// 008fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00900
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00904
			 0x3f028f5c,                                                  // 0.51                                        /// 00908
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00918
			 0x0c700000,                                                  // Leading 1s:                                 /// 0091c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0092c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00940
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c600000,                                                  // Leading 1s:                                 /// 0094c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00950
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0095c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00960
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00964
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00968
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00978
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00980
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00988
			 0xcb000000,                                                  // -8388608.0                                  /// 0098c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00990
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00994
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00998
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0099c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xffc00001,                                                  // -signaling NaN                              /// 009bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x7f800000,                                                  // inf                                         /// 00a8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aa8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x55555555,                                                  // 4 random values                             /// 00ab8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00abc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ac4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0xff800000,                                                  // -inf                                        /// 00acc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00adc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00af8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00afc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b00
			 0xcb000000,                                                  // -8388608.0                                  /// 00b04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b14
			 0xbf028f5c,                                                  // -0.51                                       /// 00b18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b4c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b88
			 0xff800000,                                                  // -inf                                        /// 00b8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x33333333,                                                  // 4 random values                             /// 00bf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c40
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ca8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cb4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cc0
			 0xff800000,                                                  // -inf                                        /// 00cc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ccc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf4
			 0x55555555,                                                  // 4 random values                             /// 00cf8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d60
			 0x00000000,                                                  // zero                                        /// 00d64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00db0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x55555555,                                                  // 4 random values                             /// 00e50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e80
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x80000000,                                                  // -zero                                       /// 00e88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ebc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ec0
			 0x80000000,                                                  // -zero                                       /// 00ec4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ecc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed8
			 0x55555555,                                                  // 4 random values                             /// 00edc
			 0x7f800000,                                                  // inf                                         /// 00ee0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ef0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f20
			 0x80000000,                                                  // -zero                                       /// 00f24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xcb000000,                                                  // -8388608.0                                  /// 00f9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fa0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fb8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff8
			 0x7f000000 // norm with max exp min mant                   // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x4053e879, /// 0x0
			 0x79b5e1a0, /// 0x4
			 0x1e5375bf, /// 0x8
			 0xd370973d, /// 0xc
			 0x5e0432a1, /// 0x10
			 0xedc5689e, /// 0x14
			 0x5597cf09, /// 0x18
			 0x544a4544, /// 0x1c
			 0xff0a8fa8, /// 0x20
			 0x1149771b, /// 0x24
			 0x4c9f3b97, /// 0x28
			 0x31cc419a, /// 0x2c
			 0x7ace811e, /// 0x30
			 0x1288567f, /// 0x34
			 0x05afa7cc, /// 0x38
			 0x1a79c192, /// 0x3c
			 0x17acd4e6, /// 0x40
			 0xa32de6a5, /// 0x44
			 0xedf99dfc, /// 0x48
			 0xc6b3c93a, /// 0x4c
			 0x202f6122, /// 0x50
			 0xe344ce3c, /// 0x54
			 0x125f9e04, /// 0x58
			 0xeb0d4da2, /// 0x5c
			 0x836fd577, /// 0x60
			 0x5c38176c, /// 0x64
			 0x05cd5a2b, /// 0x68
			 0x6f35c20f, /// 0x6c
			 0x27009667, /// 0x70
			 0xc4fe7b8f, /// 0x74
			 0x123c2aff, /// 0x78
			 0x4d2f647f, /// 0x7c
			 0x4b0d88e8, /// 0x80
			 0x92cde747, /// 0x84
			 0x40db289f, /// 0x88
			 0xae019280, /// 0x8c
			 0xb3b7ad0f, /// 0x90
			 0x8f34e4c1, /// 0x94
			 0x16617cbd, /// 0x98
			 0x55fd5f21, /// 0x9c
			 0x0385560e, /// 0xa0
			 0xffa3f92a, /// 0xa4
			 0x797fc4cd, /// 0xa8
			 0xb21c3874, /// 0xac
			 0x7267e981, /// 0xb0
			 0x91db8ed1, /// 0xb4
			 0xbcf1dc5d, /// 0xb8
			 0xdeb1aae3, /// 0xbc
			 0x5048157a, /// 0xc0
			 0x0d10d7b8, /// 0xc4
			 0x8b1a4286, /// 0xc8
			 0x0fc09af0, /// 0xcc
			 0x97607e7e, /// 0xd0
			 0x98417b60, /// 0xd4
			 0x80f7732c, /// 0xd8
			 0x8f7ba8ec, /// 0xdc
			 0x487fbc39, /// 0xe0
			 0x6a78c2a7, /// 0xe4
			 0xe1344df9, /// 0xe8
			 0xefbb177d, /// 0xec
			 0xeb5578d6, /// 0xf0
			 0xbe3e66a4, /// 0xf4
			 0x1c993855, /// 0xf8
			 0x9df1dd98, /// 0xfc
			 0x239036ba, /// 0x100
			 0x910febb9, /// 0x104
			 0xff7068f5, /// 0x108
			 0x896e1883, /// 0x10c
			 0xab99653c, /// 0x110
			 0x1c27c719, /// 0x114
			 0x2d4601a8, /// 0x118
			 0xd65d39b3, /// 0x11c
			 0x80c9768e, /// 0x120
			 0x7a16e328, /// 0x124
			 0x3df5b223, /// 0x128
			 0x16c75e0a, /// 0x12c
			 0x8483d634, /// 0x130
			 0x51922d1c, /// 0x134
			 0x1bcdbc16, /// 0x138
			 0xcd400590, /// 0x13c
			 0x64a15236, /// 0x140
			 0x427e4910, /// 0x144
			 0xb1b760f5, /// 0x148
			 0xafbf4dd1, /// 0x14c
			 0x0bef2f29, /// 0x150
			 0x83c908b4, /// 0x154
			 0x62c54f22, /// 0x158
			 0x033317ed, /// 0x15c
			 0x509d4208, /// 0x160
			 0x31910e8c, /// 0x164
			 0xd3dfb2c1, /// 0x168
			 0xee5ec16f, /// 0x16c
			 0x0befdc0f, /// 0x170
			 0xcbe79337, /// 0x174
			 0x4957b1fe, /// 0x178
			 0x26c64c3a, /// 0x17c
			 0xe7e32613, /// 0x180
			 0x4e86a9d0, /// 0x184
			 0xf1abab14, /// 0x188
			 0xf1ad19a6, /// 0x18c
			 0x114ef26a, /// 0x190
			 0x5d59a57a, /// 0x194
			 0x66ef00d6, /// 0x198
			 0x4cad3197, /// 0x19c
			 0xc37e9f13, /// 0x1a0
			 0x832df953, /// 0x1a4
			 0x7a64ea02, /// 0x1a8
			 0xd78c2c39, /// 0x1ac
			 0xf59fcade, /// 0x1b0
			 0x8445c005, /// 0x1b4
			 0xc48bf6af, /// 0x1b8
			 0xb5abbf73, /// 0x1bc
			 0x2057bc62, /// 0x1c0
			 0x2bc3bbe0, /// 0x1c4
			 0x1663e2f7, /// 0x1c8
			 0x407b53f1, /// 0x1cc
			 0x64002cee, /// 0x1d0
			 0x7200d759, /// 0x1d4
			 0x8d5f43d0, /// 0x1d8
			 0x3bb0e491, /// 0x1dc
			 0x98b16e31, /// 0x1e0
			 0xeb523418, /// 0x1e4
			 0x8f341485, /// 0x1e8
			 0xab6d1833, /// 0x1ec
			 0xa91703ed, /// 0x1f0
			 0x4c55973c, /// 0x1f4
			 0xdccc663e, /// 0x1f8
			 0x3dd425c4, /// 0x1fc
			 0x1a600c4f, /// 0x200
			 0x1914b9f1, /// 0x204
			 0xd958a0aa, /// 0x208
			 0x819b14c5, /// 0x20c
			 0x6716f7f7, /// 0x210
			 0x18f4b07f, /// 0x214
			 0x338c57ef, /// 0x218
			 0x7d4011e5, /// 0x21c
			 0xd40ed5ad, /// 0x220
			 0x29593c4d, /// 0x224
			 0xe706f702, /// 0x228
			 0xc6c3be9a, /// 0x22c
			 0x5fd12de6, /// 0x230
			 0xfe7526cf, /// 0x234
			 0x83be1779, /// 0x238
			 0x3727f6ff, /// 0x23c
			 0x0daf70be, /// 0x240
			 0xe43ea17a, /// 0x244
			 0x6149ae49, /// 0x248
			 0x8cb42d2a, /// 0x24c
			 0xf5c35f29, /// 0x250
			 0xeaea8497, /// 0x254
			 0xc491bd7e, /// 0x258
			 0x976ab596, /// 0x25c
			 0x4c910f26, /// 0x260
			 0x8b2eeacf, /// 0x264
			 0x30bb4a88, /// 0x268
			 0x4bf4e5de, /// 0x26c
			 0xbef4be18, /// 0x270
			 0x1b2a7ea6, /// 0x274
			 0x1af63c11, /// 0x278
			 0x2bdcb4d2, /// 0x27c
			 0x2779cdeb, /// 0x280
			 0x853fa75f, /// 0x284
			 0x82b53650, /// 0x288
			 0x490ab34f, /// 0x28c
			 0x3dc87418, /// 0x290
			 0x85b79e34, /// 0x294
			 0x47934c6d, /// 0x298
			 0x08ed550c, /// 0x29c
			 0x84988b9d, /// 0x2a0
			 0x01968840, /// 0x2a4
			 0x6d018443, /// 0x2a8
			 0x22eebed7, /// 0x2ac
			 0x4015194b, /// 0x2b0
			 0xd744be8d, /// 0x2b4
			 0xf857683b, /// 0x2b8
			 0xed35147f, /// 0x2bc
			 0x03cc4260, /// 0x2c0
			 0xb829b4ee, /// 0x2c4
			 0x71d1e049, /// 0x2c8
			 0x403b7497, /// 0x2cc
			 0x56a13a16, /// 0x2d0
			 0x79066917, /// 0x2d4
			 0xe56dc63c, /// 0x2d8
			 0x5ac7dc28, /// 0x2dc
			 0x645e412b, /// 0x2e0
			 0x0025ab61, /// 0x2e4
			 0x86357831, /// 0x2e8
			 0x8ba539f0, /// 0x2ec
			 0x82cd5b84, /// 0x2f0
			 0xb1974499, /// 0x2f4
			 0xa555bb71, /// 0x2f8
			 0xdd3742b1, /// 0x2fc
			 0x40679104, /// 0x300
			 0x012414f7, /// 0x304
			 0xdf0f349e, /// 0x308
			 0x884aa5aa, /// 0x30c
			 0xdc9ef4a1, /// 0x310
			 0xf31b1760, /// 0x314
			 0x9296731f, /// 0x318
			 0x5bddf409, /// 0x31c
			 0xbae9c426, /// 0x320
			 0xe7624a26, /// 0x324
			 0x02c93ec6, /// 0x328
			 0x43b1b92f, /// 0x32c
			 0x6e6f226a, /// 0x330
			 0x6ff5a7ee, /// 0x334
			 0x41db102c, /// 0x338
			 0x1361c342, /// 0x33c
			 0x2eda477d, /// 0x340
			 0x8c276ff0, /// 0x344
			 0x7555d19e, /// 0x348
			 0xe6c0921a, /// 0x34c
			 0x77b39142, /// 0x350
			 0x402bc530, /// 0x354
			 0xf1fb8f41, /// 0x358
			 0xdc6e1c46, /// 0x35c
			 0xefd36c9f, /// 0x360
			 0xbd65e94b, /// 0x364
			 0x87096b49, /// 0x368
			 0xef96a279, /// 0x36c
			 0x6ba41872, /// 0x370
			 0x46e4ca2d, /// 0x374
			 0x8ed410ab, /// 0x378
			 0x02504e1c, /// 0x37c
			 0x2cff5df5, /// 0x380
			 0xe006105e, /// 0x384
			 0xa8be8894, /// 0x388
			 0x3a2f16c6, /// 0x38c
			 0xcf0848e2, /// 0x390
			 0x99971671, /// 0x394
			 0x471d6a09, /// 0x398
			 0x41dee486, /// 0x39c
			 0xf64e01f0, /// 0x3a0
			 0xcc9c2594, /// 0x3a4
			 0xbe9df9fe, /// 0x3a8
			 0x01aa0335, /// 0x3ac
			 0x26419fb8, /// 0x3b0
			 0x600a2f30, /// 0x3b4
			 0xab2c06a2, /// 0x3b8
			 0x48c0a44e, /// 0x3bc
			 0x9773cc5d, /// 0x3c0
			 0x3a54cc81, /// 0x3c4
			 0xea38565d, /// 0x3c8
			 0xc7ec85d9, /// 0x3cc
			 0x646e23a7, /// 0x3d0
			 0x9e2ad783, /// 0x3d4
			 0xa85c5c70, /// 0x3d8
			 0x7006c757, /// 0x3dc
			 0xf3c1b3d9, /// 0x3e0
			 0xa4413c75, /// 0x3e4
			 0x5fe0b0e5, /// 0x3e8
			 0x1ed5c1b7, /// 0x3ec
			 0xe45671be, /// 0x3f0
			 0x0864fe07, /// 0x3f4
			 0x307cc013, /// 0x3f8
			 0xe2c21bc2, /// 0x3fc
			 0x5de73cd6, /// 0x400
			 0x00abbece, /// 0x404
			 0x31f1c0bb, /// 0x408
			 0x9c075d54, /// 0x40c
			 0xdfff8aa8, /// 0x410
			 0xfe988384, /// 0x414
			 0x641ee02f, /// 0x418
			 0x5a54584b, /// 0x41c
			 0x358ed920, /// 0x420
			 0x775093c7, /// 0x424
			 0xcfd26eb6, /// 0x428
			 0x90c3d9a9, /// 0x42c
			 0xa11d2ac2, /// 0x430
			 0xaa9f3112, /// 0x434
			 0x8399aba2, /// 0x438
			 0x5068331d, /// 0x43c
			 0x620daac5, /// 0x440
			 0x16f5ddaa, /// 0x444
			 0xfb1322fa, /// 0x448
			 0xc49bd187, /// 0x44c
			 0x61847fa3, /// 0x450
			 0xcd8f642d, /// 0x454
			 0xaa554b2b, /// 0x458
			 0x30c628a3, /// 0x45c
			 0xefa6db9d, /// 0x460
			 0x3bfea700, /// 0x464
			 0x2f0f2a1b, /// 0x468
			 0x7918106b, /// 0x46c
			 0x8fc26ce4, /// 0x470
			 0x7e7873a5, /// 0x474
			 0xff7191c3, /// 0x478
			 0x8c913337, /// 0x47c
			 0xcba48ba3, /// 0x480
			 0x5b88b042, /// 0x484
			 0x3c9fda41, /// 0x488
			 0x9181e350, /// 0x48c
			 0xc8fffd0a, /// 0x490
			 0x49b458e1, /// 0x494
			 0x0d0b5e0c, /// 0x498
			 0x8ba94416, /// 0x49c
			 0x0f1f7a7c, /// 0x4a0
			 0xcc02d514, /// 0x4a4
			 0x47fa31ba, /// 0x4a8
			 0xd67e8f89, /// 0x4ac
			 0x8d76b830, /// 0x4b0
			 0x206cbc2d, /// 0x4b4
			 0xc0a4c5dd, /// 0x4b8
			 0xd44e0328, /// 0x4bc
			 0xfc113476, /// 0x4c0
			 0xac8047a3, /// 0x4c4
			 0xfd8bda8a, /// 0x4c8
			 0x06e217cb, /// 0x4cc
			 0x4b62c8cc, /// 0x4d0
			 0xaa397b12, /// 0x4d4
			 0x77588750, /// 0x4d8
			 0x860fa8db, /// 0x4dc
			 0x349cbe18, /// 0x4e0
			 0x13e54c0f, /// 0x4e4
			 0x0ba1036e, /// 0x4e8
			 0x17b3c1df, /// 0x4ec
			 0x108acfb9, /// 0x4f0
			 0xd454ce90, /// 0x4f4
			 0x672ca204, /// 0x4f8
			 0xea24b50b, /// 0x4fc
			 0xe8f09c15, /// 0x500
			 0xd8898921, /// 0x504
			 0xe1fbdefe, /// 0x508
			 0xb711cdfb, /// 0x50c
			 0x03e4cb3e, /// 0x510
			 0x680a8d2c, /// 0x514
			 0x9ad19705, /// 0x518
			 0xb9d851e5, /// 0x51c
			 0xe3e1b37b, /// 0x520
			 0xb9cbb064, /// 0x524
			 0x7963cc53, /// 0x528
			 0xfbe55d94, /// 0x52c
			 0x6b7d8e4f, /// 0x530
			 0x8a533630, /// 0x534
			 0x4502c3ef, /// 0x538
			 0xced87af8, /// 0x53c
			 0x9ddd6346, /// 0x540
			 0x0f416429, /// 0x544
			 0x5ee56c7b, /// 0x548
			 0x80fd2462, /// 0x54c
			 0x2bfe3673, /// 0x550
			 0x4c110e76, /// 0x554
			 0xe36fa58f, /// 0x558
			 0x02f49c2b, /// 0x55c
			 0xd22787d3, /// 0x560
			 0x2877ad7f, /// 0x564
			 0x83bedffe, /// 0x568
			 0x7e75ba3f, /// 0x56c
			 0x3d2c701c, /// 0x570
			 0x32e98470, /// 0x574
			 0x450c675a, /// 0x578
			 0x04c42958, /// 0x57c
			 0x63ab22a7, /// 0x580
			 0x88ba1c59, /// 0x584
			 0x58b919e0, /// 0x588
			 0xa5318b4e, /// 0x58c
			 0x9cdee954, /// 0x590
			 0xbd744732, /// 0x594
			 0x00387245, /// 0x598
			 0x37aadce1, /// 0x59c
			 0xde85779f, /// 0x5a0
			 0x1ff27ea4, /// 0x5a4
			 0x2838e69c, /// 0x5a8
			 0xb406a4ea, /// 0x5ac
			 0xe532b779, /// 0x5b0
			 0xdd6ec915, /// 0x5b4
			 0x16a0b7a3, /// 0x5b8
			 0x239f10c7, /// 0x5bc
			 0xd439698e, /// 0x5c0
			 0x96483340, /// 0x5c4
			 0x1e166035, /// 0x5c8
			 0x956b0fae, /// 0x5cc
			 0x17dab9ce, /// 0x5d0
			 0xe583dbea, /// 0x5d4
			 0xc8036899, /// 0x5d8
			 0xe56632cc, /// 0x5dc
			 0xd8d43218, /// 0x5e0
			 0xbfda923a, /// 0x5e4
			 0x46c52102, /// 0x5e8
			 0x93a7135e, /// 0x5ec
			 0x640e4ac4, /// 0x5f0
			 0x4575cafc, /// 0x5f4
			 0x763bfbff, /// 0x5f8
			 0x88be4fc2, /// 0x5fc
			 0xd4aa12be, /// 0x600
			 0x2e01516f, /// 0x604
			 0x0f04bea5, /// 0x608
			 0x98dccb48, /// 0x60c
			 0xe715c039, /// 0x610
			 0x380c56e8, /// 0x614
			 0x20dc3ec6, /// 0x618
			 0x0c5bef76, /// 0x61c
			 0xcaf1eba0, /// 0x620
			 0xd9639d7b, /// 0x624
			 0xf9a542d6, /// 0x628
			 0x73adb6b8, /// 0x62c
			 0x2f887378, /// 0x630
			 0xf863c304, /// 0x634
			 0xfc733e5b, /// 0x638
			 0x7fa9c0ad, /// 0x63c
			 0x099b8532, /// 0x640
			 0x61a904bd, /// 0x644
			 0x6f41c870, /// 0x648
			 0xcf0f6d9c, /// 0x64c
			 0xd437c79a, /// 0x650
			 0xcc505afb, /// 0x654
			 0x20091f68, /// 0x658
			 0xefd900cd, /// 0x65c
			 0x62038a32, /// 0x660
			 0x225cfec9, /// 0x664
			 0xdacdb946, /// 0x668
			 0x938920c2, /// 0x66c
			 0x5c786462, /// 0x670
			 0x5dc70ff4, /// 0x674
			 0xe783a25d, /// 0x678
			 0x2f95b90f, /// 0x67c
			 0x140f396a, /// 0x680
			 0xc55ad620, /// 0x684
			 0x40ceadef, /// 0x688
			 0x1d2ee05f, /// 0x68c
			 0x073eb621, /// 0x690
			 0x064a5c14, /// 0x694
			 0x7f1650b7, /// 0x698
			 0x3a7ca1e7, /// 0x69c
			 0xd9e2e36b, /// 0x6a0
			 0xa5d65a02, /// 0x6a4
			 0xf4834b1b, /// 0x6a8
			 0x3c97280a, /// 0x6ac
			 0x9195d5f4, /// 0x6b0
			 0x24691276, /// 0x6b4
			 0xe20fa894, /// 0x6b8
			 0xdac21b3e, /// 0x6bc
			 0xa395bfb2, /// 0x6c0
			 0x64b89252, /// 0x6c4
			 0xa6a07569, /// 0x6c8
			 0xc6a976cf, /// 0x6cc
			 0x9dcc0d25, /// 0x6d0
			 0x1a3fd831, /// 0x6d4
			 0xa309672e, /// 0x6d8
			 0xed4e902a, /// 0x6dc
			 0x506c8d50, /// 0x6e0
			 0x04df8256, /// 0x6e4
			 0xf8b22520, /// 0x6e8
			 0x4cc6c018, /// 0x6ec
			 0xf4f11d96, /// 0x6f0
			 0x954337a5, /// 0x6f4
			 0xc40a9220, /// 0x6f8
			 0xb9c4967b, /// 0x6fc
			 0xed921922, /// 0x700
			 0x85258755, /// 0x704
			 0x97500815, /// 0x708
			 0x9b03a4ee, /// 0x70c
			 0xf7488c8a, /// 0x710
			 0x3fa261b2, /// 0x714
			 0x7b9f469b, /// 0x718
			 0x672dad52, /// 0x71c
			 0x7a7c6889, /// 0x720
			 0x1a59f390, /// 0x724
			 0x135ac659, /// 0x728
			 0x337e9325, /// 0x72c
			 0x796ebe4a, /// 0x730
			 0x46c430aa, /// 0x734
			 0x4364f38d, /// 0x738
			 0xfd8543fc, /// 0x73c
			 0x7394817c, /// 0x740
			 0x234a7fb0, /// 0x744
			 0x1fe6036f, /// 0x748
			 0x4d46a891, /// 0x74c
			 0xa59c4116, /// 0x750
			 0x08afd1c1, /// 0x754
			 0xfe47b7f5, /// 0x758
			 0xf39994b0, /// 0x75c
			 0x48cf40b5, /// 0x760
			 0xf4392b29, /// 0x764
			 0xa7dff116, /// 0x768
			 0xcc05941a, /// 0x76c
			 0xde41a4f9, /// 0x770
			 0xe972cf37, /// 0x774
			 0x476d3595, /// 0x778
			 0x8ba7dd11, /// 0x77c
			 0x3a1f960e, /// 0x780
			 0xb03e48a3, /// 0x784
			 0xe891ba42, /// 0x788
			 0x362073d1, /// 0x78c
			 0xd27403ca, /// 0x790
			 0xf73bfe4d, /// 0x794
			 0xb3af6cab, /// 0x798
			 0x2ae7efaf, /// 0x79c
			 0x9b2bcbbc, /// 0x7a0
			 0xf2a6450a, /// 0x7a4
			 0xb8dd4a82, /// 0x7a8
			 0xe246c71c, /// 0x7ac
			 0x726f9209, /// 0x7b0
			 0xd37bb4a8, /// 0x7b4
			 0x9f77d512, /// 0x7b8
			 0xdb5d35e4, /// 0x7bc
			 0x98b23d82, /// 0x7c0
			 0x79278a6a, /// 0x7c4
			 0xbce8f02e, /// 0x7c8
			 0x985bc7f9, /// 0x7cc
			 0x191fe414, /// 0x7d0
			 0x1e59a791, /// 0x7d4
			 0x03805de3, /// 0x7d8
			 0x2c8665ba, /// 0x7dc
			 0xccca7e92, /// 0x7e0
			 0x4a7f6fee, /// 0x7e4
			 0x26fa7e1c, /// 0x7e8
			 0x1eb82610, /// 0x7ec
			 0x7a9989fc, /// 0x7f0
			 0x16d8f410, /// 0x7f4
			 0x8b157da1, /// 0x7f8
			 0xc435f4b1, /// 0x7fc
			 0x034eb02e, /// 0x800
			 0xe0a208f4, /// 0x804
			 0x31347510, /// 0x808
			 0x4a95dc65, /// 0x80c
			 0x66724388, /// 0x810
			 0x698af992, /// 0x814
			 0xa05aeba1, /// 0x818
			 0xbe343f5b, /// 0x81c
			 0x36541874, /// 0x820
			 0x4c60cc32, /// 0x824
			 0x39317fec, /// 0x828
			 0x1ea83d8b, /// 0x82c
			 0xdca7ded9, /// 0x830
			 0x9a14cae0, /// 0x834
			 0x49ec9492, /// 0x838
			 0x39302b4e, /// 0x83c
			 0x10bcfc9f, /// 0x840
			 0x0d449d2b, /// 0x844
			 0x024d45b6, /// 0x848
			 0x55b9dea7, /// 0x84c
			 0xb6d2d0ed, /// 0x850
			 0x47126ac3, /// 0x854
			 0x4935f2f1, /// 0x858
			 0xb23bd395, /// 0x85c
			 0xbe625fbe, /// 0x860
			 0x4bbbf3f4, /// 0x864
			 0x22a5cd88, /// 0x868
			 0x856d913c, /// 0x86c
			 0x8759c9eb, /// 0x870
			 0x98a0dca8, /// 0x874
			 0xc575188a, /// 0x878
			 0xf5449558, /// 0x87c
			 0x89b2d11a, /// 0x880
			 0x0e43569f, /// 0x884
			 0x330d47c4, /// 0x888
			 0x84d682bb, /// 0x88c
			 0xab482912, /// 0x890
			 0xd4d361af, /// 0x894
			 0x59d88681, /// 0x898
			 0xfa94e3fa, /// 0x89c
			 0xba7ef7dd, /// 0x8a0
			 0xee30934f, /// 0x8a4
			 0x34499290, /// 0x8a8
			 0xf19c52b9, /// 0x8ac
			 0x1d650804, /// 0x8b0
			 0x1d831ebe, /// 0x8b4
			 0x5bff433a, /// 0x8b8
			 0x5ef6cefe, /// 0x8bc
			 0x62100b0f, /// 0x8c0
			 0xef7ad7d7, /// 0x8c4
			 0x4cb4addf, /// 0x8c8
			 0xfa1bc090, /// 0x8cc
			 0x9fef1ad7, /// 0x8d0
			 0x749e68ff, /// 0x8d4
			 0xc76d5592, /// 0x8d8
			 0x5c47ace2, /// 0x8dc
			 0xae2445fa, /// 0x8e0
			 0x0280034c, /// 0x8e4
			 0x818e20b0, /// 0x8e8
			 0xa01ea7c0, /// 0x8ec
			 0x0e2cdc97, /// 0x8f0
			 0x8fe9a91c, /// 0x8f4
			 0xc9d8ce7a, /// 0x8f8
			 0x8cfda155, /// 0x8fc
			 0xf666342e, /// 0x900
			 0x7476f41d, /// 0x904
			 0x7031cb2d, /// 0x908
			 0x1eb52c9b, /// 0x90c
			 0x20f2f7bf, /// 0x910
			 0x7fb977f7, /// 0x914
			 0xbe490a2e, /// 0x918
			 0xda79c65c, /// 0x91c
			 0x8df5fd5b, /// 0x920
			 0xa2f2287f, /// 0x924
			 0x5f37a3ac, /// 0x928
			 0x044532c8, /// 0x92c
			 0x79dc8c49, /// 0x930
			 0xfbaf132c, /// 0x934
			 0x5fdd8a4a, /// 0x938
			 0xa1d07cef, /// 0x93c
			 0x21a3152e, /// 0x940
			 0x701772da, /// 0x944
			 0x08079f99, /// 0x948
			 0x74de5c36, /// 0x94c
			 0xcfd6bdea, /// 0x950
			 0x32baf74d, /// 0x954
			 0x3774c1ca, /// 0x958
			 0xb4efcdb5, /// 0x95c
			 0x02c3598f, /// 0x960
			 0xeab46f3a, /// 0x964
			 0x9bf3fdb8, /// 0x968
			 0xc9ede1f5, /// 0x96c
			 0x040a17da, /// 0x970
			 0xb3c45210, /// 0x974
			 0xd9aae6f0, /// 0x978
			 0x7480e7b6, /// 0x97c
			 0x4ecbdcb4, /// 0x980
			 0x9f2e7048, /// 0x984
			 0x472f126c, /// 0x988
			 0xb127b3e9, /// 0x98c
			 0x2586c79f, /// 0x990
			 0x11c9c1c0, /// 0x994
			 0xe991c274, /// 0x998
			 0x27999055, /// 0x99c
			 0x94fd68c5, /// 0x9a0
			 0x0fd01b59, /// 0x9a4
			 0x4ab910ab, /// 0x9a8
			 0x1ebd036c, /// 0x9ac
			 0x20f5a0c1, /// 0x9b0
			 0xbd79eb6b, /// 0x9b4
			 0x3f6587be, /// 0x9b8
			 0x41f2c4ad, /// 0x9bc
			 0xf6e43e7e, /// 0x9c0
			 0xa6308150, /// 0x9c4
			 0x40ebc509, /// 0x9c8
			 0xe73f245a, /// 0x9cc
			 0x87290748, /// 0x9d0
			 0x7f33b3f9, /// 0x9d4
			 0x721515d1, /// 0x9d8
			 0x1f2e5ae1, /// 0x9dc
			 0x4090ab9e, /// 0x9e0
			 0x08a182d1, /// 0x9e4
			 0x47d2e0eb, /// 0x9e8
			 0x27fa0d2d, /// 0x9ec
			 0x255da05e, /// 0x9f0
			 0xfa508c35, /// 0x9f4
			 0xb50ddc3b, /// 0x9f8
			 0x018d96a4, /// 0x9fc
			 0x8c42baef, /// 0xa00
			 0x29dd58b3, /// 0xa04
			 0x4c3bf9fe, /// 0xa08
			 0x610b5d1e, /// 0xa0c
			 0x6d42da79, /// 0xa10
			 0x498ccca1, /// 0xa14
			 0xd9714923, /// 0xa18
			 0x2fb683dd, /// 0xa1c
			 0x6f6a3088, /// 0xa20
			 0xda260cc1, /// 0xa24
			 0x1ff21972, /// 0xa28
			 0xa46abdac, /// 0xa2c
			 0x29701f81, /// 0xa30
			 0x7966939d, /// 0xa34
			 0x4a76293c, /// 0xa38
			 0x81d2cc48, /// 0xa3c
			 0x3898e0c9, /// 0xa40
			 0xa20f0b90, /// 0xa44
			 0x1aa97c4f, /// 0xa48
			 0x8d570323, /// 0xa4c
			 0x1dba1e3d, /// 0xa50
			 0x137ef90f, /// 0xa54
			 0x889c4783, /// 0xa58
			 0xe53de397, /// 0xa5c
			 0x74084789, /// 0xa60
			 0x9127d925, /// 0xa64
			 0xa31db626, /// 0xa68
			 0x35fff963, /// 0xa6c
			 0x2e8f83aa, /// 0xa70
			 0xf61f712b, /// 0xa74
			 0xb9a2ad43, /// 0xa78
			 0x510aa909, /// 0xa7c
			 0x3f5e5f5c, /// 0xa80
			 0x8c6cdb31, /// 0xa84
			 0xbbefa142, /// 0xa88
			 0x0eecb251, /// 0xa8c
			 0xd3ed6462, /// 0xa90
			 0x108546e3, /// 0xa94
			 0x70197129, /// 0xa98
			 0x5cd9a6f6, /// 0xa9c
			 0x422c15e0, /// 0xaa0
			 0x982ce6f5, /// 0xaa4
			 0x2e748526, /// 0xaa8
			 0xaec1f39c, /// 0xaac
			 0xe51fc259, /// 0xab0
			 0x90ac580c, /// 0xab4
			 0xddbee742, /// 0xab8
			 0x0db89a5e, /// 0xabc
			 0xc274e583, /// 0xac0
			 0xb89c2d0e, /// 0xac4
			 0x3a04f2bb, /// 0xac8
			 0x49e921b5, /// 0xacc
			 0xdc7526b2, /// 0xad0
			 0x0e569c92, /// 0xad4
			 0x7652a129, /// 0xad8
			 0x9bf0103b, /// 0xadc
			 0xa54f9a1b, /// 0xae0
			 0xbea27821, /// 0xae4
			 0xfe14b63b, /// 0xae8
			 0x90375b62, /// 0xaec
			 0x7c822de2, /// 0xaf0
			 0xdeb5da52, /// 0xaf4
			 0xeac54069, /// 0xaf8
			 0x7dcba64d, /// 0xafc
			 0x51bb5ca8, /// 0xb00
			 0xd3b9bade, /// 0xb04
			 0x8c097b12, /// 0xb08
			 0x8d820b15, /// 0xb0c
			 0x6f2a442d, /// 0xb10
			 0xdd582f5f, /// 0xb14
			 0x3d9fb744, /// 0xb18
			 0x5762a20f, /// 0xb1c
			 0x5eb84f56, /// 0xb20
			 0x850f320b, /// 0xb24
			 0x200aede6, /// 0xb28
			 0x5e7827fd, /// 0xb2c
			 0x10723f68, /// 0xb30
			 0x9ad7209b, /// 0xb34
			 0xfe3c3611, /// 0xb38
			 0xd746f899, /// 0xb3c
			 0x348eda5a, /// 0xb40
			 0xf6ef61f6, /// 0xb44
			 0xa34a6bdd, /// 0xb48
			 0x4215e204, /// 0xb4c
			 0x74f574cf, /// 0xb50
			 0xb27970c5, /// 0xb54
			 0x4a723dcf, /// 0xb58
			 0x6d3b325a, /// 0xb5c
			 0x64581901, /// 0xb60
			 0x5439fd57, /// 0xb64
			 0x562718e0, /// 0xb68
			 0x8a6332f7, /// 0xb6c
			 0x005e632e, /// 0xb70
			 0x1535f50a, /// 0xb74
			 0xf0477edd, /// 0xb78
			 0xf6bc6bf1, /// 0xb7c
			 0xec476161, /// 0xb80
			 0x74e16e15, /// 0xb84
			 0xc33cf726, /// 0xb88
			 0x1cb44f80, /// 0xb8c
			 0x9d88fbf3, /// 0xb90
			 0xe5b7727c, /// 0xb94
			 0x1bf28301, /// 0xb98
			 0x8bacebd2, /// 0xb9c
			 0x0da41e78, /// 0xba0
			 0xd4871340, /// 0xba4
			 0x3ef41422, /// 0xba8
			 0xa9127c86, /// 0xbac
			 0x5d7d5e7d, /// 0xbb0
			 0xc848eece, /// 0xbb4
			 0x93f3aca2, /// 0xbb8
			 0x56e6d10b, /// 0xbbc
			 0xfdcce49c, /// 0xbc0
			 0x30536c79, /// 0xbc4
			 0xccaf800f, /// 0xbc8
			 0x2bd9a395, /// 0xbcc
			 0x87ff3351, /// 0xbd0
			 0x745f40e1, /// 0xbd4
			 0x8c17844c, /// 0xbd8
			 0x330b36d7, /// 0xbdc
			 0xa9d59ac6, /// 0xbe0
			 0x33d3f9c4, /// 0xbe4
			 0x0f71e4ed, /// 0xbe8
			 0x5f2552dc, /// 0xbec
			 0x0430bbcd, /// 0xbf0
			 0xc08a3759, /// 0xbf4
			 0x51b120d6, /// 0xbf8
			 0x205e67cb, /// 0xbfc
			 0x0dec8243, /// 0xc00
			 0x4c266602, /// 0xc04
			 0x5ef31f9c, /// 0xc08
			 0xc14c0174, /// 0xc0c
			 0x58d57ab3, /// 0xc10
			 0x70be8c85, /// 0xc14
			 0x3723d3b7, /// 0xc18
			 0xed08677f, /// 0xc1c
			 0xe0b435d7, /// 0xc20
			 0xd5a1d364, /// 0xc24
			 0xe958231d, /// 0xc28
			 0xbd08cebd, /// 0xc2c
			 0x938ae1f3, /// 0xc30
			 0x8ecafdb3, /// 0xc34
			 0x44a27052, /// 0xc38
			 0x6936af2a, /// 0xc3c
			 0xadcaf39b, /// 0xc40
			 0x081ca95c, /// 0xc44
			 0x75cc4932, /// 0xc48
			 0x76617adc, /// 0xc4c
			 0xa4d3c3c6, /// 0xc50
			 0x662e42ad, /// 0xc54
			 0x0b250824, /// 0xc58
			 0x0ea2d3e0, /// 0xc5c
			 0x1eeec007, /// 0xc60
			 0xd50d4ec4, /// 0xc64
			 0x4b0f99c1, /// 0xc68
			 0xa93b8165, /// 0xc6c
			 0x18fe485b, /// 0xc70
			 0xf0e3dd42, /// 0xc74
			 0x4c4ec979, /// 0xc78
			 0x8685ddd0, /// 0xc7c
			 0xf5778804, /// 0xc80
			 0x71049854, /// 0xc84
			 0x027f65b5, /// 0xc88
			 0x924d7fa0, /// 0xc8c
			 0xf5168a9b, /// 0xc90
			 0xece13622, /// 0xc94
			 0xa1475f42, /// 0xc98
			 0x3150f5d8, /// 0xc9c
			 0xb5b7a6ab, /// 0xca0
			 0x0236a2e4, /// 0xca4
			 0x10d0323d, /// 0xca8
			 0x5c83d330, /// 0xcac
			 0xd31c15c6, /// 0xcb0
			 0xa8850b1e, /// 0xcb4
			 0xe0e2bab4, /// 0xcb8
			 0x3022a673, /// 0xcbc
			 0xc9554f7d, /// 0xcc0
			 0x2a91a5ba, /// 0xcc4
			 0xd62ec3a7, /// 0xcc8
			 0x9a78ccd9, /// 0xccc
			 0x68b831c9, /// 0xcd0
			 0x9aafee32, /// 0xcd4
			 0x2d972b30, /// 0xcd8
			 0xcc265a14, /// 0xcdc
			 0xd6b1d20d, /// 0xce0
			 0xbd756f53, /// 0xce4
			 0xe2d3d2d5, /// 0xce8
			 0xf56b4782, /// 0xcec
			 0xfdd42801, /// 0xcf0
			 0xd707539f, /// 0xcf4
			 0x5c4fd4bb, /// 0xcf8
			 0xbccbbee6, /// 0xcfc
			 0x3cf7c083, /// 0xd00
			 0xa8d3b19f, /// 0xd04
			 0x3ed8ba07, /// 0xd08
			 0xfff86b81, /// 0xd0c
			 0xbf92418f, /// 0xd10
			 0xa8600623, /// 0xd14
			 0xad691d75, /// 0xd18
			 0x97821d3a, /// 0xd1c
			 0x47ac538f, /// 0xd20
			 0x55bd1843, /// 0xd24
			 0x4d64b5b1, /// 0xd28
			 0x5dce9c4d, /// 0xd2c
			 0x28e81e03, /// 0xd30
			 0x821f3897, /// 0xd34
			 0xd5677517, /// 0xd38
			 0xa7d2935d, /// 0xd3c
			 0x265df7cb, /// 0xd40
			 0x7b12996d, /// 0xd44
			 0x6bfb1368, /// 0xd48
			 0xd3610fbc, /// 0xd4c
			 0xa516fab8, /// 0xd50
			 0xc73f21f2, /// 0xd54
			 0x7fb1f504, /// 0xd58
			 0x679ad588, /// 0xd5c
			 0x593be5e4, /// 0xd60
			 0x373d5313, /// 0xd64
			 0x485a064a, /// 0xd68
			 0x7ef35ac9, /// 0xd6c
			 0x4b863c45, /// 0xd70
			 0xf5ba0855, /// 0xd74
			 0x2bf9cce2, /// 0xd78
			 0x955c8b70, /// 0xd7c
			 0x1350a8c3, /// 0xd80
			 0xe63e518c, /// 0xd84
			 0xa15db12b, /// 0xd88
			 0x8c60ab76, /// 0xd8c
			 0xdb370776, /// 0xd90
			 0xe2ecfe97, /// 0xd94
			 0x07bf4aa4, /// 0xd98
			 0x54d9f88f, /// 0xd9c
			 0x6b2d830c, /// 0xda0
			 0x1b68a0ad, /// 0xda4
			 0xdd70f2ec, /// 0xda8
			 0xf21bcf2b, /// 0xdac
			 0x9c4a244e, /// 0xdb0
			 0x7bebc169, /// 0xdb4
			 0x87b910f6, /// 0xdb8
			 0x4cf32a51, /// 0xdbc
			 0x4a62ebf1, /// 0xdc0
			 0x08956b1e, /// 0xdc4
			 0x5ef129c4, /// 0xdc8
			 0x31f17eed, /// 0xdcc
			 0x21820ec2, /// 0xdd0
			 0xfd1e7847, /// 0xdd4
			 0x3d495c04, /// 0xdd8
			 0x3df8a82f, /// 0xddc
			 0x3b8c6523, /// 0xde0
			 0x5e4fb9a4, /// 0xde4
			 0x8bb486e0, /// 0xde8
			 0xb269a80a, /// 0xdec
			 0x630398fd, /// 0xdf0
			 0xb1f96423, /// 0xdf4
			 0x08560957, /// 0xdf8
			 0x714fc2c8, /// 0xdfc
			 0x1871cb01, /// 0xe00
			 0x8a16d0c0, /// 0xe04
			 0xc65573ef, /// 0xe08
			 0xde593fba, /// 0xe0c
			 0x085e1501, /// 0xe10
			 0x2609be1c, /// 0xe14
			 0x177b9abf, /// 0xe18
			 0x40dc62cc, /// 0xe1c
			 0xe28f69e4, /// 0xe20
			 0x9093ec5f, /// 0xe24
			 0x9ad8047a, /// 0xe28
			 0x129ae5b3, /// 0xe2c
			 0xc07b5b59, /// 0xe30
			 0xb061c66b, /// 0xe34
			 0x17d3df3a, /// 0xe38
			 0xed756dda, /// 0xe3c
			 0x89857fa4, /// 0xe40
			 0xe2420024, /// 0xe44
			 0xb84903bf, /// 0xe48
			 0x950b79f8, /// 0xe4c
			 0x05526db5, /// 0xe50
			 0x6acadc24, /// 0xe54
			 0x33064988, /// 0xe58
			 0xd72a9fce, /// 0xe5c
			 0xfc263f87, /// 0xe60
			 0x4d2d9ced, /// 0xe64
			 0x52f92cb8, /// 0xe68
			 0x03fe1f5a, /// 0xe6c
			 0x3486bb7b, /// 0xe70
			 0x4a70b011, /// 0xe74
			 0x0ff11dbc, /// 0xe78
			 0x5693642c, /// 0xe7c
			 0xfe0eb913, /// 0xe80
			 0x61e6bae2, /// 0xe84
			 0x2e1f14e0, /// 0xe88
			 0x22c8b20f, /// 0xe8c
			 0x824b361b, /// 0xe90
			 0x7adfa81d, /// 0xe94
			 0x9988487b, /// 0xe98
			 0xc0ad0d39, /// 0xe9c
			 0xc3ec7681, /// 0xea0
			 0x6e0c4efc, /// 0xea4
			 0x71c9be1c, /// 0xea8
			 0x1bd122d7, /// 0xeac
			 0x7e438d1c, /// 0xeb0
			 0x2e508301, /// 0xeb4
			 0x998ba1ce, /// 0xeb8
			 0x068c90ad, /// 0xebc
			 0x10795dfc, /// 0xec0
			 0x462598ee, /// 0xec4
			 0xc6fda8ed, /// 0xec8
			 0x5168c5b6, /// 0xecc
			 0x903a7ec9, /// 0xed0
			 0x9392637e, /// 0xed4
			 0x5d763874, /// 0xed8
			 0x1039fcea, /// 0xedc
			 0x78238966, /// 0xee0
			 0xf311f613, /// 0xee4
			 0xada1ab19, /// 0xee8
			 0xb27482b3, /// 0xeec
			 0xc0b130ee, /// 0xef0
			 0x865c4858, /// 0xef4
			 0x78f11b40, /// 0xef8
			 0xaaccac47, /// 0xefc
			 0xfdd1fc20, /// 0xf00
			 0xb21cd714, /// 0xf04
			 0xeb06be1c, /// 0xf08
			 0x51d05ce3, /// 0xf0c
			 0xc7c6d3fb, /// 0xf10
			 0x30c180ba, /// 0xf14
			 0x54ceb4d6, /// 0xf18
			 0xff7dce6a, /// 0xf1c
			 0xe5904985, /// 0xf20
			 0x7195661d, /// 0xf24
			 0xf879becd, /// 0xf28
			 0x554b0d21, /// 0xf2c
			 0x6d866cf3, /// 0xf30
			 0xa2872bd6, /// 0xf34
			 0x5f367d16, /// 0xf38
			 0x8db060e0, /// 0xf3c
			 0xc6ae57e0, /// 0xf40
			 0xa89bae5b, /// 0xf44
			 0xc157b092, /// 0xf48
			 0x11c8f7e1, /// 0xf4c
			 0x1a6bd66b, /// 0xf50
			 0x6e916231, /// 0xf54
			 0x81b19b1f, /// 0xf58
			 0x29e1b4b1, /// 0xf5c
			 0x9c585c55, /// 0xf60
			 0x6493fd36, /// 0xf64
			 0x086e7e2c, /// 0xf68
			 0xb8328aee, /// 0xf6c
			 0x55710240, /// 0xf70
			 0x5dd2fbbd, /// 0xf74
			 0x979601a0, /// 0xf78
			 0xf9353f2c, /// 0xf7c
			 0x377f3765, /// 0xf80
			 0xea7edfe8, /// 0xf84
			 0x97412efb, /// 0xf88
			 0xc7cebe89, /// 0xf8c
			 0x418d2d78, /// 0xf90
			 0xb2641ce8, /// 0xf94
			 0x91099599, /// 0xf98
			 0xbabd45ec, /// 0xf9c
			 0x4e2c1c1a, /// 0xfa0
			 0x9ae6da45, /// 0xfa4
			 0xf7687d45, /// 0xfa8
			 0xba5c0d43, /// 0xfac
			 0x4d28896f, /// 0xfb0
			 0xcde72ee9, /// 0xfb4
			 0xf2896e7c, /// 0xfb8
			 0x70484160, /// 0xfbc
			 0xa2fa70d1, /// 0xfc0
			 0x1592e913, /// 0xfc4
			 0x5c1946ff, /// 0xfc8
			 0x4729be2e, /// 0xfcc
			 0xad234fbe, /// 0xfd0
			 0x5e1da2a0, /// 0xfd4
			 0x501005f6, /// 0xfd8
			 0xcc9d24fc, /// 0xfdc
			 0x2fcef28c, /// 0xfe0
			 0xb0e6466e, /// 0xfe4
			 0x22cca370, /// 0xfe8
			 0x5c872a18, /// 0xfec
			 0xaa0efbd4, /// 0xff0
			 0x7620ddb5, /// 0xff4
			 0x695990cd, /// 0xff8
			 0xbb3e6698 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xf594e7eb, /// 0x0
			 0x688950f7, /// 0x4
			 0x741e56b6, /// 0x8
			 0xa566c19d, /// 0xc
			 0x42761168, /// 0x10
			 0x11956163, /// 0x14
			 0xff2625fc, /// 0x18
			 0xb7db424d, /// 0x1c
			 0xf0b40263, /// 0x20
			 0x03ccdd7f, /// 0x24
			 0xf0d830dd, /// 0x28
			 0x26067995, /// 0x2c
			 0x7d209417, /// 0x30
			 0xa555a317, /// 0x34
			 0x21b2580b, /// 0x38
			 0x23575c4d, /// 0x3c
			 0x2a54fdfa, /// 0x40
			 0xe04f8673, /// 0x44
			 0xb9a8ff3b, /// 0x48
			 0x977841ad, /// 0x4c
			 0x1ea0b887, /// 0x50
			 0xaa7244e7, /// 0x54
			 0xb2893631, /// 0x58
			 0x31a6e5cd, /// 0x5c
			 0x63ce8540, /// 0x60
			 0x41fb724a, /// 0x64
			 0x74d55c01, /// 0x68
			 0x3b5bb5c4, /// 0x6c
			 0xc88b0919, /// 0x70
			 0x5eb0fb0e, /// 0x74
			 0xe60a473e, /// 0x78
			 0x5fbbef5c, /// 0x7c
			 0xa7f30740, /// 0x80
			 0xbdf62634, /// 0x84
			 0xa67ac772, /// 0x88
			 0xaf85a8fc, /// 0x8c
			 0x930b254e, /// 0x90
			 0xbeec644f, /// 0x94
			 0x643909f1, /// 0x98
			 0x0146317f, /// 0x9c
			 0xb8f608b3, /// 0xa0
			 0x2179e9bc, /// 0xa4
			 0x0522ad60, /// 0xa8
			 0x7b2a988e, /// 0xac
			 0x31c828e4, /// 0xb0
			 0x9ea100c4, /// 0xb4
			 0x5df00461, /// 0xb8
			 0x70e46bf2, /// 0xbc
			 0x55e4c46a, /// 0xc0
			 0xdbef5012, /// 0xc4
			 0xf31797a8, /// 0xc8
			 0x532b0af6, /// 0xcc
			 0x1637598f, /// 0xd0
			 0xb942383b, /// 0xd4
			 0xccd43659, /// 0xd8
			 0xb47c7723, /// 0xdc
			 0x154fbbb2, /// 0xe0
			 0xbce97b1a, /// 0xe4
			 0xcb2c6d19, /// 0xe8
			 0xe5473cbd, /// 0xec
			 0xd8c9ba3c, /// 0xf0
			 0xabba7ca2, /// 0xf4
			 0xb37cec6e, /// 0xf8
			 0x266f6aa1, /// 0xfc
			 0xf1b15db1, /// 0x100
			 0x788ad068, /// 0x104
			 0xabaff8df, /// 0x108
			 0x7b261a9c, /// 0x10c
			 0x46600b8a, /// 0x110
			 0x160b9f8c, /// 0x114
			 0x65aed77b, /// 0x118
			 0x57831c61, /// 0x11c
			 0xc49f7c7d, /// 0x120
			 0xc817cae3, /// 0x124
			 0xd6284d9d, /// 0x128
			 0x70fef4e1, /// 0x12c
			 0x15f8e812, /// 0x130
			 0xcce9fbee, /// 0x134
			 0xdec1d5e1, /// 0x138
			 0x1f1b72a9, /// 0x13c
			 0x83fa18fe, /// 0x140
			 0xcf22025a, /// 0x144
			 0x4dc8cb2b, /// 0x148
			 0xd4775114, /// 0x14c
			 0x17664669, /// 0x150
			 0x37a6a415, /// 0x154
			 0x4332518a, /// 0x158
			 0x54c53bbe, /// 0x15c
			 0x2daa3d84, /// 0x160
			 0xb6df4bed, /// 0x164
			 0x593e181d, /// 0x168
			 0xf7509254, /// 0x16c
			 0x60cecf7c, /// 0x170
			 0x61da7bab, /// 0x174
			 0x43d5e2ab, /// 0x178
			 0xd355cb7a, /// 0x17c
			 0x9f31593f, /// 0x180
			 0xd32d8301, /// 0x184
			 0xe77e67df, /// 0x188
			 0xde77e354, /// 0x18c
			 0x7e8fa6be, /// 0x190
			 0xb504cb40, /// 0x194
			 0xf182ac3d, /// 0x198
			 0x3f6a7f1b, /// 0x19c
			 0x0cdedc73, /// 0x1a0
			 0x979fee97, /// 0x1a4
			 0xa281da34, /// 0x1a8
			 0x74789e41, /// 0x1ac
			 0xd54ff26f, /// 0x1b0
			 0x70841af8, /// 0x1b4
			 0xbf0d88ab, /// 0x1b8
			 0x26cfb5cf, /// 0x1bc
			 0xdb4834b1, /// 0x1c0
			 0xde9d324f, /// 0x1c4
			 0x8bfaf52d, /// 0x1c8
			 0x39f4608a, /// 0x1cc
			 0x3a1c3345, /// 0x1d0
			 0x6240e9ea, /// 0x1d4
			 0x6f5a3d01, /// 0x1d8
			 0xd4c17dd7, /// 0x1dc
			 0x361e8e25, /// 0x1e0
			 0x596ea552, /// 0x1e4
			 0x51b8c99c, /// 0x1e8
			 0x10d97c90, /// 0x1ec
			 0x0f41d7da, /// 0x1f0
			 0x2169494c, /// 0x1f4
			 0x61d5da77, /// 0x1f8
			 0x6dd5e7b0, /// 0x1fc
			 0x6c3bed79, /// 0x200
			 0xdfa2632c, /// 0x204
			 0xbbcea791, /// 0x208
			 0x0bba1d13, /// 0x20c
			 0x6aa45120, /// 0x210
			 0xea9d7421, /// 0x214
			 0xd5930bdb, /// 0x218
			 0xa7542d0b, /// 0x21c
			 0xedfe6d52, /// 0x220
			 0x49b98fd3, /// 0x224
			 0x3f1a5cfb, /// 0x228
			 0x85a92608, /// 0x22c
			 0x8cac2840, /// 0x230
			 0x48022a8b, /// 0x234
			 0x43063e58, /// 0x238
			 0xabf5f2e0, /// 0x23c
			 0xbc6a0e4a, /// 0x240
			 0x017d1e31, /// 0x244
			 0xd3d0fa7d, /// 0x248
			 0x5f6e9a42, /// 0x24c
			 0x4bdbbf66, /// 0x250
			 0x0ccbb531, /// 0x254
			 0x00d08fce, /// 0x258
			 0x004d7462, /// 0x25c
			 0xe6e7cc10, /// 0x260
			 0x24d15de6, /// 0x264
			 0x4b2891ab, /// 0x268
			 0xe9c2030c, /// 0x26c
			 0xde85fed0, /// 0x270
			 0xc9eaa2f2, /// 0x274
			 0x51fc2ceb, /// 0x278
			 0x5be15c90, /// 0x27c
			 0x88129646, /// 0x280
			 0xff80147a, /// 0x284
			 0x313c5b72, /// 0x288
			 0x5c1c4a59, /// 0x28c
			 0xde58221b, /// 0x290
			 0x0e49423a, /// 0x294
			 0xd63706c4, /// 0x298
			 0x139b8061, /// 0x29c
			 0x11dc7240, /// 0x2a0
			 0xa8f5b927, /// 0x2a4
			 0x32f0ea56, /// 0x2a8
			 0x7ba1941f, /// 0x2ac
			 0x1e54e786, /// 0x2b0
			 0x7d3e2528, /// 0x2b4
			 0x003ab6fb, /// 0x2b8
			 0x94394f68, /// 0x2bc
			 0xb8478014, /// 0x2c0
			 0x5169d565, /// 0x2c4
			 0xaa5f65fa, /// 0x2c8
			 0x862d7475, /// 0x2cc
			 0x49cce1f1, /// 0x2d0
			 0x84e2b2a3, /// 0x2d4
			 0x27389eb1, /// 0x2d8
			 0x8eccfe37, /// 0x2dc
			 0xdd534f11, /// 0x2e0
			 0x48b08610, /// 0x2e4
			 0x687769dd, /// 0x2e8
			 0xddb4d95b, /// 0x2ec
			 0x901191c9, /// 0x2f0
			 0xc5024ba1, /// 0x2f4
			 0x34ade908, /// 0x2f8
			 0xa5273287, /// 0x2fc
			 0x93c67d45, /// 0x300
			 0xa830c60f, /// 0x304
			 0xb70ae6d6, /// 0x308
			 0x8eeddf8a, /// 0x30c
			 0xe809ee86, /// 0x310
			 0x8e98524b, /// 0x314
			 0xc1b1ff43, /// 0x318
			 0xedd7233a, /// 0x31c
			 0x2c92feb7, /// 0x320
			 0xb569f88d, /// 0x324
			 0xa0d66e2a, /// 0x328
			 0x3cf4bc4f, /// 0x32c
			 0x8088b736, /// 0x330
			 0x0ec29169, /// 0x334
			 0x8145a10e, /// 0x338
			 0x085c0ad2, /// 0x33c
			 0x52d003e6, /// 0x340
			 0x54049cde, /// 0x344
			 0x8538719a, /// 0x348
			 0x251a4b7d, /// 0x34c
			 0x21a0a538, /// 0x350
			 0x6da426b2, /// 0x354
			 0x5335fbaf, /// 0x358
			 0xdbcf0d6e, /// 0x35c
			 0xe3e948fa, /// 0x360
			 0xd232dd6e, /// 0x364
			 0x1e6432d2, /// 0x368
			 0x97e7339b, /// 0x36c
			 0x102a01fb, /// 0x370
			 0xeea6d58d, /// 0x374
			 0x8e1fad7f, /// 0x378
			 0xca01df84, /// 0x37c
			 0xc0316b19, /// 0x380
			 0xc20fbf96, /// 0x384
			 0x8af05339, /// 0x388
			 0xc9d02d22, /// 0x38c
			 0x6c522a0c, /// 0x390
			 0x9cfac668, /// 0x394
			 0x51fc3847, /// 0x398
			 0x70a22434, /// 0x39c
			 0xf7a276da, /// 0x3a0
			 0x2195736b, /// 0x3a4
			 0xc080a216, /// 0x3a8
			 0xcb95b7c7, /// 0x3ac
			 0x268b5f39, /// 0x3b0
			 0xd14f2144, /// 0x3b4
			 0xfcd2db9b, /// 0x3b8
			 0x9212ae38, /// 0x3bc
			 0x0115192f, /// 0x3c0
			 0xcffe3ef3, /// 0x3c4
			 0xd284e935, /// 0x3c8
			 0x2b224686, /// 0x3cc
			 0x51a83538, /// 0x3d0
			 0xf88dc3de, /// 0x3d4
			 0x3a1ba36d, /// 0x3d8
			 0xd0a4726c, /// 0x3dc
			 0x2d864589, /// 0x3e0
			 0xcc8ad2b2, /// 0x3e4
			 0x1f58b5cc, /// 0x3e8
			 0x20258b3a, /// 0x3ec
			 0x84a76241, /// 0x3f0
			 0x5cdb7991, /// 0x3f4
			 0x4489ae5b, /// 0x3f8
			 0x35b0a3e6, /// 0x3fc
			 0x6650196d, /// 0x400
			 0x437e8f02, /// 0x404
			 0x009222c5, /// 0x408
			 0x3f1a544c, /// 0x40c
			 0x09290c8d, /// 0x410
			 0x13935690, /// 0x414
			 0x5599d43c, /// 0x418
			 0x7a023988, /// 0x41c
			 0x5dd99a78, /// 0x420
			 0xdb6df422, /// 0x424
			 0xf2dbc84e, /// 0x428
			 0x420efa6b, /// 0x42c
			 0x32d295d7, /// 0x430
			 0xdd28eaba, /// 0x434
			 0xfd88e1fa, /// 0x438
			 0x19052a6e, /// 0x43c
			 0x08001dc8, /// 0x440
			 0x5aac3b06, /// 0x444
			 0x17e9a70b, /// 0x448
			 0xcef5d185, /// 0x44c
			 0xd371b61e, /// 0x450
			 0x2dbe6e16, /// 0x454
			 0x724ec4db, /// 0x458
			 0x45b34a9c, /// 0x45c
			 0xb91f976c, /// 0x460
			 0xb7d0f30a, /// 0x464
			 0x0dabf515, /// 0x468
			 0x6527fe08, /// 0x46c
			 0x4b614cbb, /// 0x470
			 0x8e7f19f1, /// 0x474
			 0x590d9e30, /// 0x478
			 0x6e071789, /// 0x47c
			 0x836ddbdf, /// 0x480
			 0xbe26ec34, /// 0x484
			 0x0419bca6, /// 0x488
			 0x10301b52, /// 0x48c
			 0x21aa47a1, /// 0x490
			 0x6d075477, /// 0x494
			 0x40e21b00, /// 0x498
			 0x4b3e5f9c, /// 0x49c
			 0x194f38a0, /// 0x4a0
			 0xd8a58f06, /// 0x4a4
			 0x212b20e0, /// 0x4a8
			 0x4f27a2a4, /// 0x4ac
			 0x7d9db39b, /// 0x4b0
			 0xee101599, /// 0x4b4
			 0xab16d87e, /// 0x4b8
			 0x5599c9d8, /// 0x4bc
			 0x3730d5ec, /// 0x4c0
			 0x16177c10, /// 0x4c4
			 0xe9393e69, /// 0x4c8
			 0x4230b9c8, /// 0x4cc
			 0x50f01885, /// 0x4d0
			 0x998072ce, /// 0x4d4
			 0x5d5b0bbe, /// 0x4d8
			 0xf1acadb0, /// 0x4dc
			 0xf96e3d68, /// 0x4e0
			 0xc283a679, /// 0x4e4
			 0x5c3161b5, /// 0x4e8
			 0x9b970514, /// 0x4ec
			 0x4a0ffb8d, /// 0x4f0
			 0x795ceb1e, /// 0x4f4
			 0x9363da6e, /// 0x4f8
			 0xefa86dd0, /// 0x4fc
			 0x9929a89e, /// 0x500
			 0xfe69cf16, /// 0x504
			 0xa0dfbec5, /// 0x508
			 0x6b4a2c93, /// 0x50c
			 0x359048a3, /// 0x510
			 0x39b3729c, /// 0x514
			 0x96d556ad, /// 0x518
			 0xc3801ad8, /// 0x51c
			 0x9ede632f, /// 0x520
			 0xf7d5e241, /// 0x524
			 0xc650f073, /// 0x528
			 0x646aef63, /// 0x52c
			 0xd74bf2ea, /// 0x530
			 0xcf4e5312, /// 0x534
			 0xa6f46b65, /// 0x538
			 0xe36bb5c2, /// 0x53c
			 0x21e093c1, /// 0x540
			 0x333bccbf, /// 0x544
			 0xd41dd2c3, /// 0x548
			 0x26813f2f, /// 0x54c
			 0xb35299bd, /// 0x550
			 0x2dc9ba84, /// 0x554
			 0xdc688036, /// 0x558
			 0xd3e8c310, /// 0x55c
			 0x8bce059a, /// 0x560
			 0x39068c07, /// 0x564
			 0x56a8b886, /// 0x568
			 0xfcf57ed8, /// 0x56c
			 0x261a8fc5, /// 0x570
			 0x42a3d2ee, /// 0x574
			 0xa23a96c8, /// 0x578
			 0x07bf28d1, /// 0x57c
			 0x19e8335d, /// 0x580
			 0x21c97e19, /// 0x584
			 0xe1022c84, /// 0x588
			 0x01c92e3d, /// 0x58c
			 0xd50158da, /// 0x590
			 0x5e783ed1, /// 0x594
			 0x1129569e, /// 0x598
			 0xd032cb4f, /// 0x59c
			 0xb9e5fcf4, /// 0x5a0
			 0xdd46ad8a, /// 0x5a4
			 0xf8b8b197, /// 0x5a8
			 0xb7e7ec11, /// 0x5ac
			 0x58c83707, /// 0x5b0
			 0x8df358b0, /// 0x5b4
			 0x26d8a2a1, /// 0x5b8
			 0x2b87e493, /// 0x5bc
			 0xc0d7a1be, /// 0x5c0
			 0x5f542baa, /// 0x5c4
			 0x45af8ddc, /// 0x5c8
			 0x6c6f9b56, /// 0x5cc
			 0x47d0e36b, /// 0x5d0
			 0x5949c20c, /// 0x5d4
			 0x9e20700e, /// 0x5d8
			 0x29db48de, /// 0x5dc
			 0xdfff0204, /// 0x5e0
			 0x591c5445, /// 0x5e4
			 0xaacb4903, /// 0x5e8
			 0x985eb3f4, /// 0x5ec
			 0x4787618f, /// 0x5f0
			 0x9ebd65c3, /// 0x5f4
			 0x65eb23de, /// 0x5f8
			 0x84c3c223, /// 0x5fc
			 0x18f96236, /// 0x600
			 0x17686146, /// 0x604
			 0xc8b429d1, /// 0x608
			 0x5e2db633, /// 0x60c
			 0x016123ad, /// 0x610
			 0x9376bff6, /// 0x614
			 0x7dfbba8f, /// 0x618
			 0xccd0f557, /// 0x61c
			 0x3f520884, /// 0x620
			 0x6312df4e, /// 0x624
			 0x0db4b989, /// 0x628
			 0x5c9c7d8f, /// 0x62c
			 0x8915b9ec, /// 0x630
			 0xb092e24d, /// 0x634
			 0xfe2c4d1f, /// 0x638
			 0x0659d893, /// 0x63c
			 0x871c41c7, /// 0x640
			 0xf3c60ba4, /// 0x644
			 0xc3a2c1fd, /// 0x648
			 0x7c4b97f4, /// 0x64c
			 0x4c8b9e7a, /// 0x650
			 0xb380d474, /// 0x654
			 0xbba2e937, /// 0x658
			 0x11981366, /// 0x65c
			 0xfacb960b, /// 0x660
			 0x65e146ef, /// 0x664
			 0x35efa492, /// 0x668
			 0x9957e725, /// 0x66c
			 0x08a159fe, /// 0x670
			 0x1457b370, /// 0x674
			 0x22633453, /// 0x678
			 0xd260e852, /// 0x67c
			 0x2be67d68, /// 0x680
			 0x612ea3ec, /// 0x684
			 0xa62ef5eb, /// 0x688
			 0xad4f158f, /// 0x68c
			 0x54f5b76d, /// 0x690
			 0x63aa8837, /// 0x694
			 0x4424b124, /// 0x698
			 0xcfd579e3, /// 0x69c
			 0x239ea19e, /// 0x6a0
			 0x30594d28, /// 0x6a4
			 0x27388c7b, /// 0x6a8
			 0x234b3927, /// 0x6ac
			 0x6a155da4, /// 0x6b0
			 0x022b6163, /// 0x6b4
			 0x4e9d188f, /// 0x6b8
			 0xfab11208, /// 0x6bc
			 0xf6adb6cc, /// 0x6c0
			 0xb26bf355, /// 0x6c4
			 0x5d29e0af, /// 0x6c8
			 0x6f9e5c7d, /// 0x6cc
			 0xfe99e91f, /// 0x6d0
			 0xd837c139, /// 0x6d4
			 0xfde77c4b, /// 0x6d8
			 0xa979cf61, /// 0x6dc
			 0x86b342b8, /// 0x6e0
			 0x67ead086, /// 0x6e4
			 0x97f9db87, /// 0x6e8
			 0xde77ef57, /// 0x6ec
			 0x15c52de8, /// 0x6f0
			 0x8f0522aa, /// 0x6f4
			 0xd1d279e9, /// 0x6f8
			 0x36d47b20, /// 0x6fc
			 0x3c62878b, /// 0x700
			 0xbf8da601, /// 0x704
			 0xc2ed1cba, /// 0x708
			 0x9e7c4a5d, /// 0x70c
			 0xd8295d8f, /// 0x710
			 0x4bb564a8, /// 0x714
			 0xa736dad9, /// 0x718
			 0xe1961c3e, /// 0x71c
			 0xa3e5ef63, /// 0x720
			 0x2c706734, /// 0x724
			 0xb93bb859, /// 0x728
			 0xf0aec5f4, /// 0x72c
			 0xc133fe2e, /// 0x730
			 0x04bb69dc, /// 0x734
			 0x8e16e59a, /// 0x738
			 0x9af27068, /// 0x73c
			 0x147a87b2, /// 0x740
			 0x123ee921, /// 0x744
			 0xb83b17b2, /// 0x748
			 0x11b45c6f, /// 0x74c
			 0x64668dc9, /// 0x750
			 0xd690bc69, /// 0x754
			 0x9abf25f7, /// 0x758
			 0x5110e37f, /// 0x75c
			 0x10674176, /// 0x760
			 0x4c0153ee, /// 0x764
			 0x87f6635b, /// 0x768
			 0x12b5577b, /// 0x76c
			 0x9546a1c9, /// 0x770
			 0x3094b46b, /// 0x774
			 0xea0059ed, /// 0x778
			 0xd1b0d0e7, /// 0x77c
			 0x103acfe0, /// 0x780
			 0x5b598e8d, /// 0x784
			 0x30bba3e8, /// 0x788
			 0x7b421354, /// 0x78c
			 0xd4edb6a2, /// 0x790
			 0xb3aa3fc6, /// 0x794
			 0xf145c823, /// 0x798
			 0xbb2f8f23, /// 0x79c
			 0x515f0477, /// 0x7a0
			 0x3984abe1, /// 0x7a4
			 0xa6abe932, /// 0x7a8
			 0x1b05cbd4, /// 0x7ac
			 0x3099c9da, /// 0x7b0
			 0x993b86e6, /// 0x7b4
			 0xd4146018, /// 0x7b8
			 0x49072cb3, /// 0x7bc
			 0xd1ddd025, /// 0x7c0
			 0xb932b8ba, /// 0x7c4
			 0xe2eac4fb, /// 0x7c8
			 0x49bb3b66, /// 0x7cc
			 0x652f7d4c, /// 0x7d0
			 0x954100ea, /// 0x7d4
			 0x0b8559f8, /// 0x7d8
			 0x2a6bc95d, /// 0x7dc
			 0xec01a663, /// 0x7e0
			 0x0b3a1afb, /// 0x7e4
			 0x2e86ef0f, /// 0x7e8
			 0x1e58e354, /// 0x7ec
			 0x92045c98, /// 0x7f0
			 0x139d2ee4, /// 0x7f4
			 0x4795198d, /// 0x7f8
			 0xfd6c44db, /// 0x7fc
			 0x6e7fe9bd, /// 0x800
			 0xa1dddc49, /// 0x804
			 0xe53040cb, /// 0x808
			 0x068019c5, /// 0x80c
			 0xd0683c9b, /// 0x810
			 0xa1c86363, /// 0x814
			 0xa448b038, /// 0x818
			 0xa8ea46b9, /// 0x81c
			 0x2b0f12cb, /// 0x820
			 0x4d40e54c, /// 0x824
			 0x3983816a, /// 0x828
			 0xc4c96b38, /// 0x82c
			 0xc734cb78, /// 0x830
			 0x2b671fd9, /// 0x834
			 0x589fc168, /// 0x838
			 0x2f5de31b, /// 0x83c
			 0x2315754e, /// 0x840
			 0x4dc05bb9, /// 0x844
			 0x9fc73a9d, /// 0x848
			 0xee9a5d77, /// 0x84c
			 0xcf1ec0b4, /// 0x850
			 0x215bf35e, /// 0x854
			 0xf2f9cc5f, /// 0x858
			 0x640570fe, /// 0x85c
			 0x550f9e6d, /// 0x860
			 0x8630268f, /// 0x864
			 0x6a291252, /// 0x868
			 0x2971074f, /// 0x86c
			 0x1c61f66d, /// 0x870
			 0x1160947c, /// 0x874
			 0x3b2ea835, /// 0x878
			 0x5bb157d4, /// 0x87c
			 0x286ce48f, /// 0x880
			 0xac9ac44b, /// 0x884
			 0xc1d253a2, /// 0x888
			 0x8c71d787, /// 0x88c
			 0xc1ae72d8, /// 0x890
			 0x2244009b, /// 0x894
			 0xadde5dbf, /// 0x898
			 0xd1c9ab3c, /// 0x89c
			 0xae8d177f, /// 0x8a0
			 0xce336938, /// 0x8a4
			 0xb365ddb7, /// 0x8a8
			 0x0d849648, /// 0x8ac
			 0x55714cb2, /// 0x8b0
			 0xb41dd7b6, /// 0x8b4
			 0x301cfd42, /// 0x8b8
			 0x560347e4, /// 0x8bc
			 0xa754d111, /// 0x8c0
			 0x28408665, /// 0x8c4
			 0xc6f658a6, /// 0x8c8
			 0xb054db74, /// 0x8cc
			 0x2136b6d3, /// 0x8d0
			 0xb7e377eb, /// 0x8d4
			 0x098c097e, /// 0x8d8
			 0x484cfc49, /// 0x8dc
			 0xe88f7cfb, /// 0x8e0
			 0x0741b0fd, /// 0x8e4
			 0x01aecf10, /// 0x8e8
			 0xadc5bbe5, /// 0x8ec
			 0x2e5bd84a, /// 0x8f0
			 0x4bbda1ec, /// 0x8f4
			 0x60fcdf3c, /// 0x8f8
			 0x7bcdf0b7, /// 0x8fc
			 0x9f55c4ce, /// 0x900
			 0xf15ba2f8, /// 0x904
			 0xc5c84284, /// 0x908
			 0xbf3be3ad, /// 0x90c
			 0xbd650f1a, /// 0x910
			 0x5aeebc91, /// 0x914
			 0x074a1c4a, /// 0x918
			 0x586efa29, /// 0x91c
			 0x80e39e38, /// 0x920
			 0xc5b9fb2d, /// 0x924
			 0x20abc2cf, /// 0x928
			 0x24a914b5, /// 0x92c
			 0x156534ce, /// 0x930
			 0xc756adb4, /// 0x934
			 0x2f529876, /// 0x938
			 0x91bb41ef, /// 0x93c
			 0xf5c89cff, /// 0x940
			 0x926a0b54, /// 0x944
			 0x4a275874, /// 0x948
			 0x928c44ac, /// 0x94c
			 0x821ea7d9, /// 0x950
			 0x11c078db, /// 0x954
			 0x4a8c76ba, /// 0x958
			 0x51fc616b, /// 0x95c
			 0x86dcb1b2, /// 0x960
			 0x765a6c81, /// 0x964
			 0xd40c5610, /// 0x968
			 0xd4f5585c, /// 0x96c
			 0xafbd6b15, /// 0x970
			 0xe14685f6, /// 0x974
			 0x9bdc0b42, /// 0x978
			 0x99663449, /// 0x97c
			 0x5fd90875, /// 0x980
			 0xe8ad084b, /// 0x984
			 0xce883f8d, /// 0x988
			 0x39e7336d, /// 0x98c
			 0xe42c9104, /// 0x990
			 0x243ecf70, /// 0x994
			 0x018121ec, /// 0x998
			 0x7ba5c39e, /// 0x99c
			 0x4982858e, /// 0x9a0
			 0x9357f2f7, /// 0x9a4
			 0xd0ef223a, /// 0x9a8
			 0xcdf9fcec, /// 0x9ac
			 0x67ac2dca, /// 0x9b0
			 0x60c6c389, /// 0x9b4
			 0xb89f7f1b, /// 0x9b8
			 0xb8d5601e, /// 0x9bc
			 0x53151e6a, /// 0x9c0
			 0xf8757236, /// 0x9c4
			 0xdb9804a0, /// 0x9c8
			 0x76a0326c, /// 0x9cc
			 0x9438f74b, /// 0x9d0
			 0x86234388, /// 0x9d4
			 0x270fd099, /// 0x9d8
			 0xcd9cfef5, /// 0x9dc
			 0x4aa5d2fc, /// 0x9e0
			 0x4a68f200, /// 0x9e4
			 0x14e3fd51, /// 0x9e8
			 0x77cf610e, /// 0x9ec
			 0xb2795e86, /// 0x9f0
			 0xa45017f2, /// 0x9f4
			 0x70cd4bb9, /// 0x9f8
			 0xec02d7b1, /// 0x9fc
			 0xeb4f6c9c, /// 0xa00
			 0x7e308d68, /// 0xa04
			 0x8985c27c, /// 0xa08
			 0x89233534, /// 0xa0c
			 0x9870dce2, /// 0xa10
			 0x77c4f221, /// 0xa14
			 0x201c39b8, /// 0xa18
			 0xf88f00b0, /// 0xa1c
			 0x84c798d5, /// 0xa20
			 0xb865f1c4, /// 0xa24
			 0x889fd5e9, /// 0xa28
			 0x9f08d8fe, /// 0xa2c
			 0xb097ff0c, /// 0xa30
			 0xd4f4e89e, /// 0xa34
			 0xaf83477e, /// 0xa38
			 0x577f4b03, /// 0xa3c
			 0x49223632, /// 0xa40
			 0xba4591b9, /// 0xa44
			 0x98da7dd7, /// 0xa48
			 0x6ecf606d, /// 0xa4c
			 0xa6f10e8d, /// 0xa50
			 0xe6c0e8c0, /// 0xa54
			 0x2f83d7a2, /// 0xa58
			 0xeb55f5c0, /// 0xa5c
			 0xbc7eee8c, /// 0xa60
			 0xdb669acd, /// 0xa64
			 0x91c309cc, /// 0xa68
			 0x17a14b6d, /// 0xa6c
			 0xacd6bf79, /// 0xa70
			 0xcb5f23c6, /// 0xa74
			 0x4705108e, /// 0xa78
			 0x96f2c787, /// 0xa7c
			 0x61a85588, /// 0xa80
			 0xca15b357, /// 0xa84
			 0xa1a4514b, /// 0xa88
			 0xc2e1cfd0, /// 0xa8c
			 0x14d21ad3, /// 0xa90
			 0x85b80321, /// 0xa94
			 0xab3a935b, /// 0xa98
			 0x4ca5428c, /// 0xa9c
			 0x07cab53b, /// 0xaa0
			 0x7c0f7e14, /// 0xaa4
			 0xa7327d5a, /// 0xaa8
			 0xe09c78d1, /// 0xaac
			 0xf32461ac, /// 0xab0
			 0x2ae33a44, /// 0xab4
			 0x7e343d72, /// 0xab8
			 0x9f937a6f, /// 0xabc
			 0xd447ebc4, /// 0xac0
			 0xbaed177f, /// 0xac4
			 0x5be467ca, /// 0xac8
			 0x112e77c9, /// 0xacc
			 0x34801c65, /// 0xad0
			 0xcc9f77da, /// 0xad4
			 0x8ee88ce9, /// 0xad8
			 0x3090bd4a, /// 0xadc
			 0x67212e9f, /// 0xae0
			 0xfa3298bf, /// 0xae4
			 0x207c6edb, /// 0xae8
			 0xffdbbc3f, /// 0xaec
			 0x61af778b, /// 0xaf0
			 0x67e1d726, /// 0xaf4
			 0xf78055ca, /// 0xaf8
			 0xc1d4b827, /// 0xafc
			 0xfb9d14b6, /// 0xb00
			 0x59d52b0d, /// 0xb04
			 0xde1a5d1c, /// 0xb08
			 0xc984afee, /// 0xb0c
			 0xc3cb4152, /// 0xb10
			 0xe1f50014, /// 0xb14
			 0xa637f7c9, /// 0xb18
			 0x2d684b82, /// 0xb1c
			 0x0450c7d7, /// 0xb20
			 0xe65d7727, /// 0xb24
			 0xd3b85c96, /// 0xb28
			 0x7bb42cfe, /// 0xb2c
			 0x8d4cd8f1, /// 0xb30
			 0x136a9d79, /// 0xb34
			 0x24b1fec0, /// 0xb38
			 0xf1d079a7, /// 0xb3c
			 0x3bc89857, /// 0xb40
			 0x91691459, /// 0xb44
			 0xbcd8cf95, /// 0xb48
			 0x614ee3c0, /// 0xb4c
			 0xcd68c34d, /// 0xb50
			 0xc1e5871f, /// 0xb54
			 0x145e3f34, /// 0xb58
			 0xce80443a, /// 0xb5c
			 0x093be3b3, /// 0xb60
			 0xb4c3e69c, /// 0xb64
			 0x037ea8a2, /// 0xb68
			 0x3b097faf, /// 0xb6c
			 0x1ef547f9, /// 0xb70
			 0xc1ac9b67, /// 0xb74
			 0x892b8fbb, /// 0xb78
			 0xe4b81987, /// 0xb7c
			 0x60896420, /// 0xb80
			 0x0a8e71f7, /// 0xb84
			 0xcee48267, /// 0xb88
			 0x5e6b1bfd, /// 0xb8c
			 0xe252858b, /// 0xb90
			 0x0af9a691, /// 0xb94
			 0x979769a7, /// 0xb98
			 0xbb5721eb, /// 0xb9c
			 0x1c78ee40, /// 0xba0
			 0xd54add83, /// 0xba4
			 0x5cfc80f3, /// 0xba8
			 0x24129bf5, /// 0xbac
			 0x6180aeaf, /// 0xbb0
			 0x475f39f2, /// 0xbb4
			 0xa76739ab, /// 0xbb8
			 0x7d7ff5ca, /// 0xbbc
			 0xc324159d, /// 0xbc0
			 0x0de81e25, /// 0xbc4
			 0xe1f1627c, /// 0xbc8
			 0x553a5386, /// 0xbcc
			 0xf9ef9196, /// 0xbd0
			 0xbea8a42e, /// 0xbd4
			 0x3729f38a, /// 0xbd8
			 0x4251915a, /// 0xbdc
			 0x01622bbd, /// 0xbe0
			 0x9ba1f6e7, /// 0xbe4
			 0x5a58a36c, /// 0xbe8
			 0x552c190e, /// 0xbec
			 0x41c3c5cd, /// 0xbf0
			 0x0e1a456a, /// 0xbf4
			 0xc85f1527, /// 0xbf8
			 0xf1adf748, /// 0xbfc
			 0xc9be566f, /// 0xc00
			 0x1ea7c9a4, /// 0xc04
			 0xf85cf489, /// 0xc08
			 0xa348a650, /// 0xc0c
			 0x2c80fc28, /// 0xc10
			 0xb2354bf7, /// 0xc14
			 0xd30d5baf, /// 0xc18
			 0xe2f34a93, /// 0xc1c
			 0x25b987a4, /// 0xc20
			 0xb6d6e750, /// 0xc24
			 0x01cbfe27, /// 0xc28
			 0x47b5eafa, /// 0xc2c
			 0x576c2064, /// 0xc30
			 0x87ad0cc5, /// 0xc34
			 0xe232df01, /// 0xc38
			 0x120d2eeb, /// 0xc3c
			 0x737acd75, /// 0xc40
			 0x1c191d5a, /// 0xc44
			 0x732de9d4, /// 0xc48
			 0x3cb70b41, /// 0xc4c
			 0x5c0c0af6, /// 0xc50
			 0x719a3e2c, /// 0xc54
			 0x3bf17d47, /// 0xc58
			 0x1669a878, /// 0xc5c
			 0x5efbf8b5, /// 0xc60
			 0xd2022ccc, /// 0xc64
			 0x4c550caa, /// 0xc68
			 0x5b08b490, /// 0xc6c
			 0x0b7c7b35, /// 0xc70
			 0x73d165a1, /// 0xc74
			 0xe0bfabfd, /// 0xc78
			 0xd6964ec0, /// 0xc7c
			 0x26757f5c, /// 0xc80
			 0x71e27851, /// 0xc84
			 0x11ef82ab, /// 0xc88
			 0x6a421597, /// 0xc8c
			 0x48707a6c, /// 0xc90
			 0x5e35bf4a, /// 0xc94
			 0xcbd5149f, /// 0xc98
			 0x182201dc, /// 0xc9c
			 0x0fed49f4, /// 0xca0
			 0x375f604d, /// 0xca4
			 0x25fbb5fa, /// 0xca8
			 0xc2cb95d6, /// 0xcac
			 0xa2aced4f, /// 0xcb0
			 0x4bb91463, /// 0xcb4
			 0x01cea621, /// 0xcb8
			 0xe052b210, /// 0xcbc
			 0x6f278211, /// 0xcc0
			 0x05fd4060, /// 0xcc4
			 0x0a5467fb, /// 0xcc8
			 0x8759d7ba, /// 0xccc
			 0xab7ba579, /// 0xcd0
			 0xf6a4a8ed, /// 0xcd4
			 0xb11e13fe, /// 0xcd8
			 0x68272b49, /// 0xcdc
			 0xe5c97629, /// 0xce0
			 0x9d8fe715, /// 0xce4
			 0x3cad2994, /// 0xce8
			 0x049b3043, /// 0xcec
			 0x240fbe83, /// 0xcf0
			 0x6d135855, /// 0xcf4
			 0x7a14dde0, /// 0xcf8
			 0xa6552d54, /// 0xcfc
			 0x1dd8f949, /// 0xd00
			 0x609b1d0b, /// 0xd04
			 0x01ab5629, /// 0xd08
			 0xe48f6fb1, /// 0xd0c
			 0x11f8db75, /// 0xd10
			 0x7ba8386f, /// 0xd14
			 0xa7213b0e, /// 0xd18
			 0xfdb4c732, /// 0xd1c
			 0x1f3fdee8, /// 0xd20
			 0xa3cb4633, /// 0xd24
			 0x67aa6747, /// 0xd28
			 0x6bc3902c, /// 0xd2c
			 0x37e59fe1, /// 0xd30
			 0x1ead4a22, /// 0xd34
			 0x0a564add, /// 0xd38
			 0x64590335, /// 0xd3c
			 0x768dd968, /// 0xd40
			 0xfbe85d17, /// 0xd44
			 0x09f529a4, /// 0xd48
			 0x82e8c967, /// 0xd4c
			 0x17aadb92, /// 0xd50
			 0x9db9e38b, /// 0xd54
			 0x5c82d3f4, /// 0xd58
			 0xbc020800, /// 0xd5c
			 0x167ffb2e, /// 0xd60
			 0x6a219b57, /// 0xd64
			 0x6e08f7e0, /// 0xd68
			 0x44aac357, /// 0xd6c
			 0x4a302de6, /// 0xd70
			 0xaca04104, /// 0xd74
			 0x92186392, /// 0xd78
			 0x9bb3ded4, /// 0xd7c
			 0xf1433285, /// 0xd80
			 0x61d40eda, /// 0xd84
			 0xba696466, /// 0xd88
			 0x0e74534a, /// 0xd8c
			 0xc22f8514, /// 0xd90
			 0x43363ab9, /// 0xd94
			 0x7e76d1d1, /// 0xd98
			 0x193a6bb5, /// 0xd9c
			 0x2072ab45, /// 0xda0
			 0x520c29e9, /// 0xda4
			 0x52ee88bb, /// 0xda8
			 0xc36bbc84, /// 0xdac
			 0x4cd20016, /// 0xdb0
			 0xef2a3bd2, /// 0xdb4
			 0xdec1d768, /// 0xdb8
			 0x8805232f, /// 0xdbc
			 0x098b79dd, /// 0xdc0
			 0x1a8cb0ff, /// 0xdc4
			 0x41a44ac3, /// 0xdc8
			 0x1f029c05, /// 0xdcc
			 0x6ea2c271, /// 0xdd0
			 0x7e30bdcc, /// 0xdd4
			 0x30210ce9, /// 0xdd8
			 0xd0117a73, /// 0xddc
			 0x10129d54, /// 0xde0
			 0xa719575b, /// 0xde4
			 0x4b227222, /// 0xde8
			 0x2fef81c5, /// 0xdec
			 0x0e3c4a64, /// 0xdf0
			 0x7ab93bed, /// 0xdf4
			 0xb9ef580a, /// 0xdf8
			 0xa73399f7, /// 0xdfc
			 0x528edfbe, /// 0xe00
			 0xdd5e4f66, /// 0xe04
			 0x9348e51a, /// 0xe08
			 0x3041a0b5, /// 0xe0c
			 0x90ebf9ce, /// 0xe10
			 0xf6fe5f13, /// 0xe14
			 0x9b184a9d, /// 0xe18
			 0xfd2ee573, /// 0xe1c
			 0xc22d2223, /// 0xe20
			 0x4ea70aab, /// 0xe24
			 0xb3306d7f, /// 0xe28
			 0xb7c383c2, /// 0xe2c
			 0xe75f782d, /// 0xe30
			 0x0bf0e71c, /// 0xe34
			 0x574e6def, /// 0xe38
			 0x68a00fc9, /// 0xe3c
			 0xeeebbb76, /// 0xe40
			 0x8f44263e, /// 0xe44
			 0x8c1a75c8, /// 0xe48
			 0x9482d4c0, /// 0xe4c
			 0x11b4908a, /// 0xe50
			 0xcf95ea72, /// 0xe54
			 0x2a471b0f, /// 0xe58
			 0xabe2326f, /// 0xe5c
			 0x55b9b60f, /// 0xe60
			 0xf4e702b1, /// 0xe64
			 0x9b469616, /// 0xe68
			 0xda319646, /// 0xe6c
			 0xa0078786, /// 0xe70
			 0xff3cd75f, /// 0xe74
			 0xb4bae29e, /// 0xe78
			 0x1a0584ad, /// 0xe7c
			 0x77dabfef, /// 0xe80
			 0xf824bebb, /// 0xe84
			 0x805c7414, /// 0xe88
			 0x87cc9645, /// 0xe8c
			 0x1cf29bcf, /// 0xe90
			 0x2d271aa6, /// 0xe94
			 0x703a2eb8, /// 0xe98
			 0x15ffbf6d, /// 0xe9c
			 0x2b47083e, /// 0xea0
			 0x98e41808, /// 0xea4
			 0xee5b8102, /// 0xea8
			 0x8d179868, /// 0xeac
			 0xaddd3ddf, /// 0xeb0
			 0x8f0f85f6, /// 0xeb4
			 0x40b5adca, /// 0xeb8
			 0x347ed4ca, /// 0xebc
			 0x4a516518, /// 0xec0
			 0xb3b2d83f, /// 0xec4
			 0xd6c86b24, /// 0xec8
			 0x97066137, /// 0xecc
			 0x70710d4f, /// 0xed0
			 0x89f6a7cd, /// 0xed4
			 0x89f21b54, /// 0xed8
			 0x3e676510, /// 0xedc
			 0x485df5fb, /// 0xee0
			 0xf7db1eb2, /// 0xee4
			 0xf9daed34, /// 0xee8
			 0xeb7a8aea, /// 0xeec
			 0x2c3d99f3, /// 0xef0
			 0x56bbf892, /// 0xef4
			 0x1222ce6c, /// 0xef8
			 0x784d4d4d, /// 0xefc
			 0x482f4f55, /// 0xf00
			 0x7cc4d895, /// 0xf04
			 0x85437457, /// 0xf08
			 0x8e7d0a73, /// 0xf0c
			 0x8cdf69ab, /// 0xf10
			 0x085baf79, /// 0xf14
			 0xf071e1f5, /// 0xf18
			 0xd42ed1ba, /// 0xf1c
			 0xf3080d71, /// 0xf20
			 0x4f662da6, /// 0xf24
			 0x3ce34542, /// 0xf28
			 0xe9e34f59, /// 0xf2c
			 0xbd13f99a, /// 0xf30
			 0xd94a7084, /// 0xf34
			 0xfdae2298, /// 0xf38
			 0xdf0e6a63, /// 0xf3c
			 0x1b00e527, /// 0xf40
			 0x0a2780a3, /// 0xf44
			 0x9aee8bb9, /// 0xf48
			 0xef888bb3, /// 0xf4c
			 0xee46737d, /// 0xf50
			 0xe47a8a5f, /// 0xf54
			 0xed09aadb, /// 0xf58
			 0xa94276e2, /// 0xf5c
			 0x46940890, /// 0xf60
			 0x4d8932e2, /// 0xf64
			 0xe1caa5f8, /// 0xf68
			 0xf1b2e37d, /// 0xf6c
			 0x043e3cea, /// 0xf70
			 0x4909346f, /// 0xf74
			 0x9ef465bb, /// 0xf78
			 0xa2b54661, /// 0xf7c
			 0x9273d895, /// 0xf80
			 0x6207cea8, /// 0xf84
			 0xe5bf6a84, /// 0xf88
			 0xdc1883d1, /// 0xf8c
			 0x2791cc30, /// 0xf90
			 0x8b30b861, /// 0xf94
			 0xb6279a24, /// 0xf98
			 0xb8289010, /// 0xf9c
			 0x4ed97a90, /// 0xfa0
			 0xa065207b, /// 0xfa4
			 0xefbf3977, /// 0xfa8
			 0x43ce4d1a, /// 0xfac
			 0xe834e4d2, /// 0xfb0
			 0xb7c780a5, /// 0xfb4
			 0x2fb486d0, /// 0xfb8
			 0xd3e52d04, /// 0xfbc
			 0x338717b5, /// 0xfc0
			 0x4fbf8f64, /// 0xfc4
			 0xf07761f7, /// 0xfc8
			 0x097da477, /// 0xfcc
			 0x207eb249, /// 0xfd0
			 0x991ee391, /// 0xfd4
			 0xf664ce34, /// 0xfd8
			 0xa1985cbe, /// 0xfdc
			 0x8233c9ef, /// 0xfe0
			 0x63bef33f, /// 0xfe4
			 0xb16e558e, /// 0xfe8
			 0x85b740d6, /// 0xfec
			 0x33f9a69d, /// 0xff0
			 0xcb261a6b, /// 0xff4
			 0xf792bee2, /// 0xff8
			 0xc69f838b /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00000
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00008
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00010
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00018
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00028
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0002c
			 0x00000000,                                                  // zero                                        /// 00030
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0003c
			 0xbf028f5c,                                                  // -0.51                                       /// 00040
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00044
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00050
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00054
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0005c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00060
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00064
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00068
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0006c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00070
			 0xcb000000,                                                  // -8388608.0                                  /// 00074
			 0xcb000000,                                                  // -8388608.0                                  /// 00078
			 0x7f800000,                                                  // inf                                         /// 0007c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00080
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00088
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0009c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c4
			 0x7fc00001,                                                  // signaling NaN                               /// 000c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d0
			 0x55555555,                                                  // 4 random values                             /// 000d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 000e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000f4
			 0xcb000000,                                                  // -8388608.0                                  /// 000f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000fc
			 0x7f800000,                                                  // inf                                         /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00110
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00118
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0011c
			 0xff800000,                                                  // -inf                                        /// 00120
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00124
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0012c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00130
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00134
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00138
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00144
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0014c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00150
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00154
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00158
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x4b000000,                                                  // 8388608.0                                   /// 00168
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0016c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00174
			 0xff800000,                                                  // -inf                                        /// 00178
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00180
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00188
			 0x0e000007,                                                  // Trailing 1s:                                /// 0018c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00190
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00198
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0019c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ec
			 0x55555555,                                                  // 4 random values                             /// 001f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00200
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00204
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00208
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0020c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00214
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00218
			 0xffc00001,                                                  // -signaling NaN                              /// 0021c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00228
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00230
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0023c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00240
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00244
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e000003,                                                  // Trailing 1s:                                /// 00254
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00258
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0025c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00260
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00264
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80011111,                                                  // -9.7958E-41                                 /// 0026c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00278
			 0x7fc00001,                                                  // signaling NaN                               /// 0027c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00280
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00284
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0028c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00290
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00298
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c0
			 0x7f800000,                                                  // inf                                         /// 002c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00000000,                                                  // zero                                        /// 002e0
			 0x33333333,                                                  // 4 random values                             /// 002e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002ec
			 0x00000000,                                                  // zero                                        /// 002f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00304
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x7f800000,                                                  // inf                                         /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00314
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0031c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00324
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00328
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0032c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00330
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00334
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00338
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0033c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00340
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00348
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00368
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0036c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00370
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00378
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0037c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00380
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00388
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0038c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00390
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00394
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00398
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0039c
			 0x7fc00001,                                                  // signaling NaN                               /// 003a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00408
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0040c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00410
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00414
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00418
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0041c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00420
			 0xff800000,                                                  // -inf                                        /// 00424
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00428
			 0x0c400000,                                                  // Leading 1s:                                 /// 0042c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00430
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0043c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00440
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80011111,                                                  // -9.7958E-41                                 /// 00448
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0044c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00454
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00458
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0045c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00460
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00470
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00474
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00478
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00480
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00484
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00488
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0048c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00490
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00498
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0049c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e4
			 0x7f800000,                                                  // inf                                         /// 004e8
			 0x4b000000,                                                  // 8388608.0                                   /// 004ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00500
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00504
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00508
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00510
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00518
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0051c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00520
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00524
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00528
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00534
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00538
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00540
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00544
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00548
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0054c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00550
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0055c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00560
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00568
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0056c
			 0xbf028f5c,                                                  // -0.51                                       /// 00570
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0057c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00580
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0058c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00594
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00598
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0059c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00600
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00604
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00608
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00610
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00614
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0061c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00620
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00624
			 0x0e000003,                                                  // Trailing 1s:                                /// 00628
			 0x55555555,                                                  // 4 random values                             /// 0062c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0063c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00640
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00644
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00648
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0064c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00650
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00654
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00658
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00678
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00684
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00690
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00698
			 0x00000000,                                                  // zero                                        /// 0069c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0xbf028f5c,                                                  // -0.51                                       /// 006a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0xffc00001,                                                  // -signaling NaN                              /// 006c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006e4
			 0x80000000,                                                  // -zero                                       /// 006e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00700
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0070c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00710
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00720
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00724
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00728
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0072c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00730
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xff800000,                                                  // -inf                                        /// 00744
			 0x0c400000,                                                  // Leading 1s:                                 /// 00748
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0074c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00750
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00754
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00758
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0075c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00760
			 0x0e000001,                                                  // Trailing 1s:                                /// 00764
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00768
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00770
			 0xbf028f5c,                                                  // -0.51                                       /// 00774
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0077c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00780
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00784
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x7f800000,                                                  // inf                                         /// 0078c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00794
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00798
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0079c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00800
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00804
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00808
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0080c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00810
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00814
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00818
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00820
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00828
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0082c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0083c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00840
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00844
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00848
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00850
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00854
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0086c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00874
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00878
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00880
			 0x00011111,                                                  // 9.7958E-41                                  /// 00884
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0088c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00894
			 0x55555555,                                                  // 4 random values                             /// 00898
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00900
			 0x80000000,                                                  // -zero                                       /// 00904
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00914
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00918
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0091c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00924
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00928
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0092c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00930
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00938
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0093c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00940
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00944
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00948
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0094c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00950
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x7f800000,                                                  // inf                                         /// 00958
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0095c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00964
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00968
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0096c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00970
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00980
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00988
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00990
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00994
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00998
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0099c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00000000,                                                  // zero                                        /// 009ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b8
			 0xffc00001,                                                  // -signaling NaN                              /// 009bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x33333333,                                                  // 4 random values                             /// 009cc
			 0x7f800000,                                                  // inf                                         /// 009d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009e8
			 0x7fc00001,                                                  // signaling NaN                               /// 009ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a7c
			 0x00000000,                                                  // zero                                        /// 00a80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a90
			 0x55555555,                                                  // 4 random values                             /// 00a94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ab8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00abc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ac4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00adc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ae0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00af0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00afc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b98
			 0xff800000,                                                  // -inf                                        /// 00b9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bf8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c30
			 0x55555555,                                                  // 4 random values                             /// 00c34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ccc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cdc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce8
			 0x7f800000,                                                  // inf                                         /// 00cec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cf4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cfc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d50
			 0xbf028f5c,                                                  // -0.51                                       /// 00d54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d84
			 0x33333333,                                                  // 4 random values                             /// 00d88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00da4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dcc
			 0x7f800000,                                                  // inf                                         /// 00dd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ddc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x4b000000,                                                  // 8388608.0                                   /// 00e38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e64
			 0x7fc00001,                                                  // signaling NaN                               /// 00e68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e94
			 0x3f028f5c,                                                  // 0.51                                        /// 00e98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ea4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00eb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ebc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ec8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ef0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ef8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00efc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f00
			 0xffc00001,                                                  // -signaling NaN                              /// 00f04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x7f800000,                                                  // inf                                         /// 00f8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fdc
			 0x33333333,                                                  // 4 random values                             /// 00fe0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ff0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x00080000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00000
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0000c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00010
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00014
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00018
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00020
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00024
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0002c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0003c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00040
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00044
			 0x80000000,                                                  // -zero                                       /// 00048
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0004c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0c780000,                                                  // Leading 1s:                                 /// 0005c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00060
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00064
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0006c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00070
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00074
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00078
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0007c
			 0xff800000,                                                  // -inf                                        /// 00080
			 0xcb000000,                                                  // -8388608.0                                  /// 00084
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00088
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0008c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00094
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0009c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00104
			 0x0c600000,                                                  // Leading 1s:                                 /// 00108
			 0x80000000,                                                  // -zero                                       /// 0010c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xffc00001,                                                  // -signaling NaN                              /// 00114
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0011c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00124
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0012c
			 0x7f800000,                                                  // inf                                         /// 00130
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00134
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00138
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00140
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xcb000000,                                                  // -8388608.0                                  /// 00148
			 0xcb000000,                                                  // -8388608.0                                  /// 0014c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0015c
			 0xff800000,                                                  // -inf                                        /// 00160
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00164
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00168
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00170
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0017c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00180
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00184
			 0x7fc00001,                                                  // signaling NaN                               /// 00188
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0018c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00190
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00194
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00198
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0019c
			 0x0c400000,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00204
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0xbf028f5c,                                                  // -0.51                                       /// 00210
			 0x00011111,                                                  // 9.7958E-41                                  /// 00214
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0021c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00220
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00224
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00228
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00230
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00240
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00244
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00248
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0024c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00250
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0025c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00260
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00264
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00268
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00270
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0027c
			 0x55555555,                                                  // 4 random values                             /// 00280
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00284
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00288
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00294
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00298
			 0x0e000007,                                                  // Trailing 1s:                                /// 0029c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f0
			 0xbf028f5c,                                                  // -0.51                                       /// 002f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00300
			 0x3f028f5c,                                                  // 0.51                                        /// 00304
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00308
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00310
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00318
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00328
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0032c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00334
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00344
			 0xffc00001,                                                  // -signaling NaN                              /// 00348
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x33333333,                                                  // 4 random values                             /// 00354
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00358
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00368
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0036c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00370
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00374
			 0x55555555,                                                  // 4 random values                             /// 00378
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0037c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00380
			 0x0e000003,                                                  // Trailing 1s:                                /// 00384
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00388
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00398
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0039c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003fc
			 0x55555555,                                                  // 4 random values                             /// 00400
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00408
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00410
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00418
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00420
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00424
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00428
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00430
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00434
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00440
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00460
			 0x0e000003,                                                  // Trailing 1s:                                /// 00464
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00470
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00474
			 0x0e000001,                                                  // Trailing 1s:                                /// 00478
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00480
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x0c780000,                                                  // Leading 1s:                                 /// 00488
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00490
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00494
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x4b000000,                                                  // 8388608.0                                   /// 004f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00508
			 0x0e000003,                                                  // Trailing 1s:                                /// 0050c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00514
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0051c
			 0x00000000,                                                  // zero                                        /// 00520
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00528
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0052c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00530
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00538
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00540
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00544
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00548
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0054c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00554
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x33333333,                                                  // 4 random values                             /// 0055c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00564
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00568
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00570
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00574
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00578
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0057c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00580
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00584
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00588
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0058c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00594
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x80011111,                                                  // -9.7958E-41                                 /// 005a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d8
			 0x00000000,                                                  // zero                                        /// 005dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00600
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00604
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0060c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00610
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00620
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80000000,                                                  // -zero                                       /// 0062c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00630
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00634
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00638
			 0xbf028f5c,                                                  // -0.51                                       /// 0063c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00640
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00644
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00648
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0064c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00654
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00658
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0065c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00660
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00664
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00668
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0066c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00674
			 0x00000000,                                                  // zero                                        /// 00678
			 0x80000000,                                                  // -zero                                       /// 0067c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00680
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00684
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00688
			 0x4b000000,                                                  // 8388608.0                                   /// 0068c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00690
			 0x00000000,                                                  // zero                                        /// 00694
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00698
			 0x0c780000,                                                  // Leading 1s:                                 /// 0069c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ec
			 0x7f800000,                                                  // inf                                         /// 006f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f4
			 0x7f800000,                                                  // inf                                         /// 006f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006fc
			 0x33333333,                                                  // 4 random values                             /// 00700
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00708
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0070c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00710
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00714
			 0x7fc00001,                                                  // signaling NaN                               /// 00718
			 0x0c780000,                                                  // Leading 1s:                                 /// 0071c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00728
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0072c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00734
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00738
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0073c
			 0xbf028f5c,                                                  // -0.51                                       /// 00740
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00748
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00754
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0075c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00760
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0076c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00770
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00774
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00778
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0077c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80011111,                                                  // -9.7958E-41                                 /// 00784
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00788
			 0xff800000,                                                  // -inf                                        /// 0078c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0079c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0xbf028f5c,                                                  // -0.51                                       /// 007e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00810
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00814
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00818
			 0x80011111,                                                  // -9.7958E-41                                 /// 0081c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00824
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00838
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00840
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00844
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00848
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0084c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0085c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00860
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00864
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00868
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0086c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00870
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x33333333,                                                  // 4 random values                             /// 0087c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00888
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0088c
			 0xff800000,                                                  // -inf                                        /// 00890
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00894
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00898
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0089c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x55555555,                                                  // 4 random values                             /// 0090c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00910
			 0xcb000000,                                                  // -8388608.0                                  /// 00914
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00918
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00920
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00924
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00928
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0092c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00934
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00940
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00948
			 0x7f800000,                                                  // inf                                         /// 0094c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00954
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x7fc00001,                                                  // signaling NaN                               /// 00960
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00964
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00968
			 0x0e000007,                                                  // Trailing 1s:                                /// 0096c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00974
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00980
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00988
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00990
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00998
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0099c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00a00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ab0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ab4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00adc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00afc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b00
			 0x00000000,                                                  // zero                                        /// 00b04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b18
			 0x3f028f5c,                                                  // 0.51                                        /// 00b1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b24
			 0xffc00001,                                                  // -signaling NaN                              /// 00b28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00000000,                                                  // zero                                        /// 00b90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bf8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c14
			 0x7fc00001,                                                  // signaling NaN                               /// 00c18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ca4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ccc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ce0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ce8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cf8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d44
			 0xcb000000,                                                  // -8388608.0                                  /// 00d48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d54
			 0xbf028f5c,                                                  // -0.51                                       /// 00d58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00da8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00db8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ddc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00de0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00df4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ea0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ea4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00eac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xff800000,                                                  // -inf                                        /// 00eb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ec4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed4
			 0x55555555,                                                  // 4 random values                             /// 00ed8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00edc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ee0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ef4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xcb000000,                                                  // -8388608.0                                  /// 00f00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f18
			 0x4b000000,                                                  // 8388608.0                                   /// 00f1c
			 0x7f800000,                                                  // inf                                         /// 00f20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0xffc00001,                                                  // -signaling NaN                              /// 00f48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f50
			 0xffc00001,                                                  // -signaling NaN                              /// 00f54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x3f028f5c,                                                  // 0.51                                        /// 00f6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f88
			 0x4b000000,                                                  // 8388608.0                                   /// 00f8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00000000,                                                  // zero                                        /// 00fc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff8
			 0x3f800000 // 1                                             // SP +ve numbers                              /// last
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
			 0x000000fc,
			 0x00000358,
			 0x00000628,
			 0x000005d0,
			 0x000007c0,
			 0x00000400,
			 0x0000053c,
			 0x00000468,

			 /// vpu register f2
			 0x41f00000,
			 0x40c00000,
			 0x41b00000,
			 0x41000000,
			 0x40c00000,
			 0x40a00000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f3
			 0x41e80000,
			 0x41a00000,
			 0x41d00000,
			 0x40400000,
			 0x40400000,
			 0x41b00000,
			 0x41a00000,
			 0x41d00000,

			 /// vpu register f4
			 0x41700000,
			 0x41600000,
			 0x41d00000,
			 0x41a80000,
			 0x41f80000,
			 0x41a80000,
			 0x41100000,
			 0x41500000,

			 /// vpu register f5
			 0x41f80000,
			 0x41e80000,
			 0x41600000,
			 0x41f80000,
			 0x41a00000,
			 0x41700000,
			 0x41000000,
			 0x41f00000,

			 /// vpu register f6
			 0x40c00000,
			 0x41800000,
			 0x41f80000,
			 0x40e00000,
			 0x41b80000,
			 0x41c00000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f7
			 0x41900000,
			 0x41800000,
			 0x41c80000,
			 0x41c00000,
			 0x41800000,
			 0x41500000,
			 0x42000000,
			 0x40e00000,

			 /// vpu register f8
			 0x41100000,
			 0x41d80000,
			 0x41000000,
			 0x40e00000,
			 0x41200000,
			 0x3f800000,
			 0x41f80000,
			 0x41200000,

			 /// vpu register f9
			 0x41c80000,
			 0x41d00000,
			 0x41700000,
			 0x41300000,
			 0x41c80000,
			 0x41c80000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f10
			 0x41700000,
			 0x41a00000,
			 0x40800000,
			 0x42000000,
			 0x40c00000,
			 0x41b00000,
			 0x40400000,
			 0x41d80000,

			 /// vpu register f11
			 0x41500000,
			 0x41a80000,
			 0x40c00000,
			 0x41980000,
			 0x41f00000,
			 0x41980000,
			 0x41200000,
			 0x41f80000,

			 /// vpu register f12
			 0x41b80000,
			 0x40a00000,
			 0x41f80000,
			 0x41500000,
			 0x41600000,
			 0x41300000,
			 0x41880000,
			 0x40c00000,

			 /// vpu register f13
			 0x41a80000,
			 0x41300000,
			 0x41700000,
			 0x41200000,
			 0x41c80000,
			 0x41c80000,
			 0x41c80000,
			 0x41b00000,

			 /// vpu register f14
			 0x41e00000,
			 0x41000000,
			 0x41800000,
			 0x41e80000,
			 0x41c00000,
			 0x40c00000,
			 0x41b00000,
			 0x40a00000,

			 /// vpu register f15
			 0x41e00000,
			 0x41b00000,
			 0x3f800000,
			 0x41c80000,
			 0x41e00000,
			 0x40e00000,
			 0x41100000,
			 0x41300000,

			 /// vpu register f16
			 0x41f80000,
			 0x41000000,
			 0x41a80000,
			 0x41000000,
			 0x41d80000,
			 0x41c80000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f17
			 0x41e00000,
			 0x41800000,
			 0x41f80000,
			 0x41700000,
			 0x41c80000,
			 0x41e00000,
			 0x41f00000,
			 0x42000000,

			 /// vpu register f18
			 0x41300000,
			 0x41d80000,
			 0x41f80000,
			 0x41000000,
			 0x41500000,
			 0x41e80000,
			 0x41c00000,
			 0x41f00000,

			 /// vpu register f19
			 0x40e00000,
			 0x40c00000,
			 0x41f00000,
			 0x41f00000,
			 0x41600000,
			 0x41c00000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f20
			 0x41d00000,
			 0x41f80000,
			 0x41880000,
			 0x41900000,
			 0x41800000,
			 0x41c00000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f21
			 0x41980000,
			 0x41b80000,
			 0x41400000,
			 0x41200000,
			 0x42000000,
			 0x40800000,
			 0x41100000,
			 0x40800000,

			 /// vpu register f22
			 0x41f80000,
			 0x42000000,
			 0x41b80000,
			 0x40e00000,
			 0x41a00000,
			 0x40800000,
			 0x41a80000,
			 0x41880000,

			 /// vpu register f23
			 0x41e80000,
			 0x3f800000,
			 0x41800000,
			 0x41300000,
			 0x41980000,
			 0x41200000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f24
			 0x41880000,
			 0x41d00000,
			 0x41f80000,
			 0x41e80000,
			 0x40a00000,
			 0x41000000,
			 0x41300000,
			 0x41e00000,

			 /// vpu register f25
			 0x41300000,
			 0x41100000,
			 0x41e00000,
			 0x40e00000,
			 0x41200000,
			 0x41b00000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f26
			 0x41f80000,
			 0x41300000,
			 0x40e00000,
			 0x41900000,
			 0x41f80000,
			 0x41700000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f27
			 0x41200000,
			 0x40400000,
			 0x41880000,
			 0x41300000,
			 0x40400000,
			 0x41800000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f28
			 0x41600000,
			 0x41000000,
			 0x40a00000,
			 0x41700000,
			 0x41b80000,
			 0x41880000,
			 0x41300000,
			 0x3f800000,

			 /// vpu register f29
			 0x40800000,
			 0x41a80000,
			 0x40800000,
			 0x41e80000,
			 0x40e00000,
			 0x42000000,
			 0x41300000,
			 0x41e80000,

			 /// vpu register f30
			 0x41d00000,
			 0x41300000,
			 0x40e00000,
			 0x40400000,
			 0x42000000,
			 0x40e00000,
			 0x41c80000,
			 0x40800000,

			 /// vpu register f31
			 0x41980000,
			 0x41b00000,
			 0x41700000,
			 0x42000000,
			 0x40c00000,
			 0x41800000,
			 0x40a00000,
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
		"famoming.pi f26, f1 (x14)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f24, f1 (x11)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f28, f1 (x14)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f7, f1 (x15)\n"                          ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f20, f1 (x14)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x12)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x14)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f27, f1 (x14)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x11)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f30, f1 (x12)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x15)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f30, f1 (x11)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f18, f1 (x14)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f17, f1 (x12)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f16, f1 (x11)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f13, f1 (x13)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x14)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x12)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x14)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f28, f1 (x14)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f6, f1 (x12)\n"                          ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f30, f1 (x13)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f29, f1 (x11)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f4, f1 (x12)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f25, f1 (x14)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f25, f1 (x12)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x13)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f20, f1 (x14)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f26, f1 (x13)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f5, f1 (x11)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f8, f1 (x14)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f29, f1 (x13)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f26, f1 (x15)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f16, f1 (x14)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f17, f1 (x12)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f13, f1 (x15)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f24, f1 (x14)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f15, f1 (x12)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f6, f1 (x11)\n"                          ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f12, f1 (x11)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f17, f1 (x15)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f9, f1 (x11)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f9, f1 (x14)\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f3, f1 (x11)\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f2, f1 (x11)\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f13, f1 (x12)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f22, f1 (x14)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f21, f1 (x15)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f12, f1 (x12)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f24, f1 (x14)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f18, f1 (x14)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f27, f1 (x11)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f5, f1 (x13)\n"                          ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f20, f1 (x14)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f11, f1 (x15)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x11)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f29, f1 (x14)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x15)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f12, f1 (x11)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f20, f1 (x11)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f8, f1 (x15)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x11)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f11, f1 (x13)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f22, f1 (x12)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f7, f1 (x11)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f26, f1 (x15)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f6, f1 (x11)\n"                          ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f11, f1 (x11)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f15, f1 (x13)\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f24, f1 (x15)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f7, f1 (x15)\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f17, f1 (x14)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f29, f1 (x13)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f19, f1 (x14)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f23, f1 (x15)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f7, f1 (x13)\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f11, f1 (x12)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f16, f1 (x12)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x15)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f3, f1 (x12)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f19, f1 (x14)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f17, f1 (x13)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f5, f1 (x11)\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f22, f1 (x15)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f23, f1 (x15)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f29, f1 (x15)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f5, f1 (x11)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f24, f1 (x14)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f10, f1 (x12)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f25, f1 (x11)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f19, f1 (x13)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f14, f1 (x12)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f9, f1 (x13)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f6, f1 (x13)\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f20, f1 (x14)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f28, f1 (x11)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f2, f1 (x14)\n"                          ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f26, f1 (x14)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoming.pi f21, f1 (x14)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
