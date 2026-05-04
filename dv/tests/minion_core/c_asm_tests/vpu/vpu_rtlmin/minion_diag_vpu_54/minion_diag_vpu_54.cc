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
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00004
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00008
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0000c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00014
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00018
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0001c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00020
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00030
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00038
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00044
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00048
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00050
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00054
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00058
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0005c
			 0xbf028f5c,                                                  // -0.51                                       /// 00060
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00064
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00074
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00078
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0007c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00084
			 0x0c700000,                                                  // Leading 1s:                                 /// 00088
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0008c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00090
			 0x00000000,                                                  // zero                                        /// 00094
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00098
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0009c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b0
			 0x00000000,                                                  // zero                                        /// 000b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c0
			 0x80000000,                                                  // -zero                                       /// 000c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00100
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00104
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00108
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0010c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00110
			 0x0e000001,                                                  // Trailing 1s:                                /// 00114
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00118
			 0x55555555,                                                  // 4 random values                             /// 0011c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00120
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x33333333,                                                  // 4 random values                             /// 00130
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00134
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0013c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00140
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x3f028f5c,                                                  // 0.51                                        /// 0014c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00150
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00154
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0015c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00160
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00168
			 0x0c600000,                                                  // Leading 1s:                                 /// 0016c
			 0x55555555,                                                  // 4 random values                             /// 00170
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00174
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00180
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00184
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00188
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00194
			 0x3f028f5c,                                                  // 0.51                                        /// 00198
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80000000,                                                  // -zero                                       /// 00204
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00214
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00220
			 0x33333333,                                                  // 4 random values                             /// 00224
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0022c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00234
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00238
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00240
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00248
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00250
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0025c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00260
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00268
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0026c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00270
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00278
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0027c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00280
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00284
			 0x0c600000,                                                  // Leading 1s:                                 /// 00288
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00290
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0029c
			 0x55555555,                                                  // 4 random values                             /// 002a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00308
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0030c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x00000000,                                                  // zero                                        /// 00318
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00320
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00328
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0032c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00330
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00334
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00338
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00344
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00348
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80011111,                                                  // -9.7958E-41                                 /// 00354
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00358
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0035c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00360
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00364
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00368
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00370
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00378
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00384
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00388
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00390
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00398
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xcb000000,                                                  // -8388608.0                                  /// 003d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00404
			 0xcb000000,                                                  // -8388608.0                                  /// 00408
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00410
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0041c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00420
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00428
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0042c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00434
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00438
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0043c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00450
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00454
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00460
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0046c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00470
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00478
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00480
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00484
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00488
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0048c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00494
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00498
			 0x00000000,                                                  // zero                                        /// 0049c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004a4
			 0x4b000000,                                                  // 8388608.0                                   /// 004a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e0
			 0xbf028f5c,                                                  // -0.51                                       /// 004e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e8
			 0x80000000,                                                  // -zero                                       /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f8
			 0x80000000,                                                  // -zero                                       /// 004fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00500
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00514
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00518
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x33333333,                                                  // 4 random values                             /// 00524
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00528
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00530
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00534
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00540
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00548
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0054c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00550
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x0c600000,                                                  // Leading 1s:                                 /// 00558
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0055c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00560
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00564
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00568
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0056c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00570
			 0x00011111,                                                  // 9.7958E-41                                  /// 00574
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00578
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00584
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00588
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00590
			 0xbf028f5c,                                                  // -0.51                                       /// 00594
			 0x0c780000,                                                  // Leading 1s:                                 /// 00598
			 0xffc00001,                                                  // -signaling NaN                              /// 0059c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00604
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0060c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00610
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00614
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00618
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0061c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00620
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00624
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0xcb000000,                                                  // -8388608.0                                  /// 0062c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00630
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00638
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00640
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00648
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0064c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00650
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00654
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0065c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00660
			 0x0c600000,                                                  // Leading 1s:                                 /// 00664
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00668
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00674
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0067c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00684
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0068c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00690
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00700
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00708
			 0x7fc00001,                                                  // signaling NaN                               /// 0070c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00714
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0071c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0072c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00730
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00738
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0073c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00740
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x0c700000,                                                  // Leading 1s:                                 /// 00754
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00758
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00770
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0077c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00780
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e000007,                                                  // Trailing 1s:                                /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00790
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00794
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x7fc00001,                                                  // signaling NaN                               /// 007b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007fc
			 0x33333333,                                                  // 4 random values                             /// 00800
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00804
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00808
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0080c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00810
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00814
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0081c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00824
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00828
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00834
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00838
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00840
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00844
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0084c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00850
			 0x80000000,                                                  // -zero                                       /// 00854
			 0x7f800000,                                                  // inf                                         /// 00858
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0085c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00864
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0086c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00870
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00878
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0087c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00890
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00894
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0089c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 008b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00900
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xff800000,                                                  // -inf                                        /// 00908
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00914
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00920
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00928
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00930
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00938
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0093c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00940
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00944
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00948
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0094c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00954
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00958
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00964
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00968
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00974
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00978
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0097c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00984
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00990
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00994
			 0x7fc00001,                                                  // signaling NaN                               /// 00998
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0099c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0xffc00001,                                                  // -signaling NaN                              /// 009c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a04
			 0xffc00001,                                                  // -signaling NaN                              /// 00a08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x55555555,                                                  // 4 random values                             /// 00a40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a48
			 0xffc00001,                                                  // -signaling NaN                              /// 00a4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a54
			 0x3f028f5c,                                                  // 0.51                                        /// 00a58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7fc00001,                                                  // signaling NaN                               /// 00a8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00aa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aa8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ab0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ab4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ab8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00abc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00acc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ad0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ad4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ad8
			 0x33333333,                                                  // 4 random values                             /// 00adc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00af4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00af8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b04
			 0x7fc00001,                                                  // signaling NaN                               /// 00b08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b84
			 0x55555555,                                                  // 4 random values                             /// 00b88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ba8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bd8
			 0x55555555,                                                  // 4 random values                             /// 00bdc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bf4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c54
			 0x80000000,                                                  // -zero                                       /// 00c58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ca0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ca4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ca8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ce0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00da0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00db4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ddc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00df8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x4b000000,                                                  // 8388608.0                                   /// 00e1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e44
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xff800000,                                                  // -inf                                        /// 00e50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00000000,                                                  // zero                                        /// 00edc
			 0x80000000,                                                  // -zero                                       /// 00ee0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x7f800000,                                                  // inf                                         /// 00ef4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00efc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000000,                                                  // -zero                                       /// 00f0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f40
			 0x4b000000,                                                  // 8388608.0                                   /// 00f44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80000000,                                                  // -zero                                       /// 00f9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fa8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fb0
			 0x33333333,                                                  // 4 random values                             /// 00fb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fe8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ff0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0xffbfffff // SNaN                                          // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00000
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00004
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00010
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00014
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0001c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00020
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0xffc00001,                                                  // -signaling NaN                              /// 00030
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00038
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00040
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00048
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00058
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0005c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00060
			 0x7f800000,                                                  // inf                                         /// 00064
			 0x0c600000,                                                  // Leading 1s:                                 /// 00068
			 0x0c400000,                                                  // Leading 1s:                                 /// 0006c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00070
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00074
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00078
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0007c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00080
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00084
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00088
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00098
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0009c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000e4
			 0x33333333,                                                  // 4 random values                             /// 000e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00100
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00104
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0010c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00110
			 0x0e000007,                                                  // Trailing 1s:                                /// 00114
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00118
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0011c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00120
			 0xffc00001,                                                  // -signaling NaN                              /// 00124
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0012c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00130
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00134
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00138
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0013c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00140
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0014c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00150
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00158
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0015c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00160
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00164
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00168
			 0x0e000001,                                                  // Trailing 1s:                                /// 0016c
			 0xff800000,                                                  // -inf                                        /// 00170
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00178
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0017c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00180
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00184
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00194
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0c700000,                                                  // Leading 1s:                                 /// 0019c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001b8
			 0xff800000,                                                  // -inf                                        /// 001bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00208
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00214
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00218
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0021c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00220
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0022c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00238
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0023c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c700000,                                                  // Leading 1s:                                 /// 00248
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00254
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00258
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0025c
			 0x33333333,                                                  // 4 random values                             /// 00260
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00264
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00268
			 0x0c600000,                                                  // Leading 1s:                                 /// 0026c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00274
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00278
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00280
			 0x4b000000,                                                  // 8388608.0                                   /// 00284
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00288
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00290
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80000000,                                                  // -zero                                       /// 0029c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00300
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00304
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0031c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00320
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00328
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00334
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00340
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x7fc00001,                                                  // signaling NaN                               /// 00348
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00000000,                                                  // zero                                        /// 00350
			 0x4b000000,                                                  // 8388608.0                                   /// 00354
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0035c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00360
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00364
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0036c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00378
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00380
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00384
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0038c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00394
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0039c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00400
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00404
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00408
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0040c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00410
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00414
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00418
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0041c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00011111,                                                  // 9.7958E-41                                  /// 00424
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00428
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00430
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00438
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0043c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00440
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00444
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00448
			 0xbf028f5c,                                                  // -0.51                                       /// 0044c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00450
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00454
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00458
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0x0e000003,                                                  // Trailing 1s:                                /// 00464
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0046c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00474
			 0xffc00001,                                                  // -signaling NaN                              /// 00478
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00480
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00488
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0048c
			 0xcb000000,                                                  // -8388608.0                                  /// 00490
			 0x0c780000,                                                  // Leading 1s:                                 /// 00494
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0049c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00504
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0050c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00518
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0051c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00524
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00528
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0052c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00534
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00538
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0053c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00540
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00544
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0054c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00550
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00554
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00560
			 0x4b000000,                                                  // 8388608.0                                   /// 00564
			 0x80000000,                                                  // -zero                                       /// 00568
			 0x33333333,                                                  // 4 random values                             /// 0056c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00574
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00578
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00580
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x55555555,                                                  // 4 random values                             /// 00588
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0058c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00594
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00600
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00604
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0060c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0061c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00624
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00628
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00634
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00638
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0063c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00644
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0064c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e000007,                                                  // Trailing 1s:                                /// 00664
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00670
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00674
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00678
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00680
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00688
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0068c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00690
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00694
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00698
			 0x0e000003,                                                  // Trailing 1s:                                /// 0069c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000000,                                                  // zero                                        /// 006f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x80000000,                                                  // -zero                                       /// 006f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c700000,                                                  // Leading 1s:                                 /// 00704
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0070c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00718
			 0xbf028f5c,                                                  // -0.51                                       /// 0071c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00724
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00728
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0072c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00734
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00738
			 0x00011111,                                                  // 9.7958E-41                                  /// 0073c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0074c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00750
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0075c
			 0x7f800000,                                                  // inf                                         /// 00760
			 0x0c400000,                                                  // Leading 1s:                                 /// 00764
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00770
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0077c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00784
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x0c780000,                                                  // Leading 1s:                                 /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d4
			 0x7f800000,                                                  // inf                                         /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00810
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00814
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00000000,                                                  // zero                                        /// 00820
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00824
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00828
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00834
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00838
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00840
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00844
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00848
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0084c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00850
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0085c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00864
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00868
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00870
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00874
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00878
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00880
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00884
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0088c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00898
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x00011111,                                                  // 9.7958E-41                                  /// 008a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a8
			 0xff800000,                                                  // -inf                                        /// 008ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b4
			 0x3f028f5c,                                                  // 0.51                                        /// 008b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00910
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00914
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00918
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00920
			 0x0c700000,                                                  // Leading 1s:                                 /// 00924
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00928
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0092c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00930
			 0x7fc00001,                                                  // signaling NaN                               /// 00934
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00940
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00944
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00948
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0094c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x4b000000,                                                  // 8388608.0                                   /// 00954
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00958
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0095c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00964
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00968
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0096c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00974
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x3f028f5c,                                                  // 0.51                                        /// 00980
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x7fc00001,                                                  // signaling NaN                               /// 0098c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00990
			 0x00011111,                                                  // 9.7958E-41                                  /// 00994
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00998
			 0x7f800000,                                                  // inf                                         /// 0099c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xff800000,                                                  // -inf                                        /// 009bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a40
			 0x80000000,                                                  // -zero                                       /// 00a44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a60
			 0xffc00001,                                                  // -signaling NaN                              /// 00a64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a70
			 0x80000000,                                                  // -zero                                       /// 00a74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a80
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a90
			 0xbf028f5c,                                                  // -0.51                                       /// 00a94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00acc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00000000,                                                  // zero                                        /// 00ad8
			 0x7fc00001,                                                  // signaling NaN                               /// 00adc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00afc
			 0x7fc00001,                                                  // signaling NaN                               /// 00b00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b68
			 0x80000000,                                                  // -zero                                       /// 00b6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b74
			 0x7f800000,                                                  // inf                                         /// 00b78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ba4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bcc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bd0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x80000000,                                                  // -zero                                       /// 00be0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00be4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00be8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ca0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ca8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce8
			 0x80000000,                                                  // -zero                                       /// 00cec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cf4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d0c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d68
			 0xffc00001,                                                  // -signaling NaN                              /// 00d6c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x3f028f5c,                                                  // 0.51                                        /// 00d9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00da4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00db4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x55555555,                                                  // 4 random values                             /// 00dc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dcc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00df0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00df4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ea4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00eb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ebc
			 0x00000000,                                                  // zero                                        /// 00ec0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ecc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed4
			 0x80000000,                                                  // -zero                                       /// 00ed8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00edc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ee0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ee4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00efc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f08
			 0xff800000,                                                  // -inf                                        /// 00f0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x55555555,                                                  // 4 random values                             /// 00f70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f88
			 0x7fc00001,                                                  // signaling NaN                               /// 00f8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fa4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x8f7ffffc                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x09f27c63, /// 0x0
			 0x10431947, /// 0x4
			 0x7a21ee69, /// 0x8
			 0x16c101f4, /// 0xc
			 0xb34ce039, /// 0x10
			 0x259034c0, /// 0x14
			 0x3a6f0f2c, /// 0x18
			 0x5bec3866, /// 0x1c
			 0x02cb12ff, /// 0x20
			 0x38db24c7, /// 0x24
			 0x5cc9aa19, /// 0x28
			 0xd7ca344a, /// 0x2c
			 0xac82cf92, /// 0x30
			 0x5faec204, /// 0x34
			 0xbdb85dbe, /// 0x38
			 0x44111b4e, /// 0x3c
			 0x6d7a358c, /// 0x40
			 0xc5493736, /// 0x44
			 0xc1ff505b, /// 0x48
			 0x7d6cbd29, /// 0x4c
			 0xe66a3568, /// 0x50
			 0x14c6593e, /// 0x54
			 0xfff6c339, /// 0x58
			 0xd96aaf49, /// 0x5c
			 0x6e142cbd, /// 0x60
			 0x7be03eef, /// 0x64
			 0x74c59495, /// 0x68
			 0xf3355990, /// 0x6c
			 0x32596df2, /// 0x70
			 0x1379ebc5, /// 0x74
			 0x2752cff6, /// 0x78
			 0x20310f1e, /// 0x7c
			 0xe87ecd5f, /// 0x80
			 0xfe2312d7, /// 0x84
			 0xe938fda5, /// 0x88
			 0xdfbee5a8, /// 0x8c
			 0xb5417977, /// 0x90
			 0xc37b5b18, /// 0x94
			 0x000d0152, /// 0x98
			 0x21be90c8, /// 0x9c
			 0x9af984a6, /// 0xa0
			 0x78eb6832, /// 0xa4
			 0x02f20b0c, /// 0xa8
			 0x705d714a, /// 0xac
			 0xa401727b, /// 0xb0
			 0x59849e3a, /// 0xb4
			 0x57c1f4b1, /// 0xb8
			 0xf87e7dce, /// 0xbc
			 0xa034581c, /// 0xc0
			 0x8ebb5b9b, /// 0xc4
			 0xa521ad86, /// 0xc8
			 0x15a092eb, /// 0xcc
			 0x7bbba397, /// 0xd0
			 0x8572dd86, /// 0xd4
			 0x593a23e2, /// 0xd8
			 0xe37257ad, /// 0xdc
			 0x0c3cbc00, /// 0xe0
			 0x9fd2fccf, /// 0xe4
			 0xe3db860a, /// 0xe8
			 0xdf53e181, /// 0xec
			 0xf85c4383, /// 0xf0
			 0xd9cf6611, /// 0xf4
			 0x82a25581, /// 0xf8
			 0xde6e618c, /// 0xfc
			 0x161585e5, /// 0x100
			 0x09340acd, /// 0x104
			 0x8b412da1, /// 0x108
			 0xedd301e1, /// 0x10c
			 0x1440a6d4, /// 0x110
			 0xe0b6bc06, /// 0x114
			 0x2f51c3a8, /// 0x118
			 0x6c9c5b07, /// 0x11c
			 0x61a07f5c, /// 0x120
			 0x02e51a31, /// 0x124
			 0xcd25fadb, /// 0x128
			 0x8f0ae93c, /// 0x12c
			 0xf31d45c8, /// 0x130
			 0xa4c52c36, /// 0x134
			 0x379e2d06, /// 0x138
			 0x1ee84830, /// 0x13c
			 0xc99a2a41, /// 0x140
			 0xc8fc7d96, /// 0x144
			 0x8419b6f5, /// 0x148
			 0xa54d44db, /// 0x14c
			 0xe74a1c35, /// 0x150
			 0x98ee2757, /// 0x154
			 0xacf74a4e, /// 0x158
			 0x8072d591, /// 0x15c
			 0x0f9afb26, /// 0x160
			 0xfbe86a76, /// 0x164
			 0x098c8c80, /// 0x168
			 0x96c2123b, /// 0x16c
			 0x219c4927, /// 0x170
			 0x65333106, /// 0x174
			 0x22bf8d96, /// 0x178
			 0xdb139e3f, /// 0x17c
			 0x66eae99d, /// 0x180
			 0xa57a7542, /// 0x184
			 0xff29922b, /// 0x188
			 0xf2dcef5a, /// 0x18c
			 0xf49ce8fd, /// 0x190
			 0xac90eda9, /// 0x194
			 0x1af474d9, /// 0x198
			 0x6351fdd2, /// 0x19c
			 0xecc7739a, /// 0x1a0
			 0x2e03df29, /// 0x1a4
			 0xa78c0b28, /// 0x1a8
			 0x1c344164, /// 0x1ac
			 0xa31c390c, /// 0x1b0
			 0x00e74795, /// 0x1b4
			 0xb237eaad, /// 0x1b8
			 0x26db00fd, /// 0x1bc
			 0x4174ad01, /// 0x1c0
			 0x0e880c60, /// 0x1c4
			 0xe29a4c4a, /// 0x1c8
			 0x6540d1e9, /// 0x1cc
			 0x070aeca9, /// 0x1d0
			 0xf38168f4, /// 0x1d4
			 0x0539affb, /// 0x1d8
			 0x21cc1275, /// 0x1dc
			 0xaca66b70, /// 0x1e0
			 0xb13076b7, /// 0x1e4
			 0xf3290d9a, /// 0x1e8
			 0x8b9d8def, /// 0x1ec
			 0xfcb2160e, /// 0x1f0
			 0xa7e1a75b, /// 0x1f4
			 0x9163974b, /// 0x1f8
			 0x9a48722a, /// 0x1fc
			 0x079ec39b, /// 0x200
			 0xbeeb9d9c, /// 0x204
			 0x4d45387b, /// 0x208
			 0x74032ebc, /// 0x20c
			 0xed05246d, /// 0x210
			 0x8c1b15c9, /// 0x214
			 0x71ae8f0c, /// 0x218
			 0x7bfcf3bb, /// 0x21c
			 0x82e3c9d8, /// 0x220
			 0x064de18b, /// 0x224
			 0xd01ad8c0, /// 0x228
			 0x763161c4, /// 0x22c
			 0xd16fe2b2, /// 0x230
			 0x2b58c040, /// 0x234
			 0x50b6add5, /// 0x238
			 0xe3103073, /// 0x23c
			 0x8666aa7c, /// 0x240
			 0xd4bdbd71, /// 0x244
			 0x8c4c9b14, /// 0x248
			 0x8418a4bc, /// 0x24c
			 0x0dc538a7, /// 0x250
			 0xe2842273, /// 0x254
			 0x2cfa54ab, /// 0x258
			 0x064a610f, /// 0x25c
			 0x0e278837, /// 0x260
			 0x35c2dd4e, /// 0x264
			 0xb17dfffb, /// 0x268
			 0x1f3e57a7, /// 0x26c
			 0x751291b5, /// 0x270
			 0xe87c1356, /// 0x274
			 0xa6e869f1, /// 0x278
			 0xfc380a46, /// 0x27c
			 0x0f0e4b8c, /// 0x280
			 0x0c327973, /// 0x284
			 0x25271fa6, /// 0x288
			 0x51db0d62, /// 0x28c
			 0xd3f9bff2, /// 0x290
			 0x0f04b118, /// 0x294
			 0x89fb67db, /// 0x298
			 0x6d107ab4, /// 0x29c
			 0x52a00175, /// 0x2a0
			 0x9cfc09b8, /// 0x2a4
			 0x6b8aecaa, /// 0x2a8
			 0xa507ddd9, /// 0x2ac
			 0x92547c5d, /// 0x2b0
			 0x5b5a250d, /// 0x2b4
			 0x52e2ebb1, /// 0x2b8
			 0xbec77918, /// 0x2bc
			 0x82d58d7d, /// 0x2c0
			 0x4a6d2e36, /// 0x2c4
			 0x6f85290b, /// 0x2c8
			 0x5c3413fe, /// 0x2cc
			 0x0c969439, /// 0x2d0
			 0xf38631bb, /// 0x2d4
			 0x4bdedca4, /// 0x2d8
			 0xd95c3bc4, /// 0x2dc
			 0xfd5f6071, /// 0x2e0
			 0x690ab72c, /// 0x2e4
			 0xdceda04c, /// 0x2e8
			 0xdfc17825, /// 0x2ec
			 0x92e8ec24, /// 0x2f0
			 0x623493ab, /// 0x2f4
			 0x56a7c89e, /// 0x2f8
			 0x610f91e0, /// 0x2fc
			 0x2e8991bf, /// 0x300
			 0xe87717fa, /// 0x304
			 0xbebaebe1, /// 0x308
			 0x9e09b9bb, /// 0x30c
			 0xcca8fdd9, /// 0x310
			 0xb07c3896, /// 0x314
			 0xa523de31, /// 0x318
			 0x6f8df3f0, /// 0x31c
			 0xb0bb986d, /// 0x320
			 0x40cd4e56, /// 0x324
			 0x49b0883b, /// 0x328
			 0x26ccc796, /// 0x32c
			 0x1ff0f581, /// 0x330
			 0x96cfce43, /// 0x334
			 0xefd287fa, /// 0x338
			 0x5dc2d71c, /// 0x33c
			 0xed5d8920, /// 0x340
			 0x5fd11961, /// 0x344
			 0x119008c5, /// 0x348
			 0x04757d30, /// 0x34c
			 0x569b81ae, /// 0x350
			 0xa9a57fb3, /// 0x354
			 0x63472012, /// 0x358
			 0x2954f7c5, /// 0x35c
			 0x35914708, /// 0x360
			 0xf733ebc8, /// 0x364
			 0x0cae0fa6, /// 0x368
			 0xd6e8a366, /// 0x36c
			 0xc078f60c, /// 0x370
			 0x01b717a7, /// 0x374
			 0xaae63b71, /// 0x378
			 0x3871aa22, /// 0x37c
			 0x77bb8b29, /// 0x380
			 0xf941fb26, /// 0x384
			 0x2a5f6ab4, /// 0x388
			 0xd0549682, /// 0x38c
			 0x3ec758ad, /// 0x390
			 0x802f6749, /// 0x394
			 0xc8ff29e2, /// 0x398
			 0xcac52173, /// 0x39c
			 0xd4162527, /// 0x3a0
			 0xbf08e6bc, /// 0x3a4
			 0x96bf8498, /// 0x3a8
			 0xb89e87bf, /// 0x3ac
			 0x704568fc, /// 0x3b0
			 0x15cfb93d, /// 0x3b4
			 0x0587303e, /// 0x3b8
			 0x439cff24, /// 0x3bc
			 0x28e197ed, /// 0x3c0
			 0x8f7886bc, /// 0x3c4
			 0x6672e92e, /// 0x3c8
			 0x5c82c682, /// 0x3cc
			 0x21f2ef18, /// 0x3d0
			 0xe3c4a944, /// 0x3d4
			 0xd6fbcfdb, /// 0x3d8
			 0x7cea509a, /// 0x3dc
			 0x37179429, /// 0x3e0
			 0x3403389f, /// 0x3e4
			 0x250a5499, /// 0x3e8
			 0xc6ebb810, /// 0x3ec
			 0x39cd8a5e, /// 0x3f0
			 0x46e64476, /// 0x3f4
			 0x05e860cd, /// 0x3f8
			 0x3f18db39, /// 0x3fc
			 0xf4aaa452, /// 0x400
			 0x086b167f, /// 0x404
			 0xcef81a2c, /// 0x408
			 0x41c1d6ad, /// 0x40c
			 0xca747883, /// 0x410
			 0x78edd283, /// 0x414
			 0xbdbfc4d8, /// 0x418
			 0x42b022d0, /// 0x41c
			 0x3897ad78, /// 0x420
			 0x2382beb2, /// 0x424
			 0x23ea64cb, /// 0x428
			 0xb44bff85, /// 0x42c
			 0xd8980a2b, /// 0x430
			 0xbf5c9a86, /// 0x434
			 0x018e378e, /// 0x438
			 0x506d5a12, /// 0x43c
			 0x2baa2113, /// 0x440
			 0xaf5d711f, /// 0x444
			 0x6e5ccab4, /// 0x448
			 0x68a82c92, /// 0x44c
			 0xdfb85929, /// 0x450
			 0x7defa9ce, /// 0x454
			 0x43adad8f, /// 0x458
			 0x5b44f872, /// 0x45c
			 0xb72fe570, /// 0x460
			 0x2b420bb8, /// 0x464
			 0x1a5e0f89, /// 0x468
			 0xe0eabeb4, /// 0x46c
			 0x1c472c70, /// 0x470
			 0xf0edfec6, /// 0x474
			 0x0f7dfa3e, /// 0x478
			 0x2b2d27d7, /// 0x47c
			 0x725d9a1d, /// 0x480
			 0xbc962feb, /// 0x484
			 0x2698f7d4, /// 0x488
			 0x12b94bba, /// 0x48c
			 0x844dc2e9, /// 0x490
			 0xeffde24a, /// 0x494
			 0x3ba4f22a, /// 0x498
			 0x90b1f913, /// 0x49c
			 0xdeb9cbb6, /// 0x4a0
			 0x166f9175, /// 0x4a4
			 0x9b5044cd, /// 0x4a8
			 0x73ca8fcb, /// 0x4ac
			 0x1beef334, /// 0x4b0
			 0x71f9401c, /// 0x4b4
			 0xa9906599, /// 0x4b8
			 0x43c4627a, /// 0x4bc
			 0xec622233, /// 0x4c0
			 0xddf26bd0, /// 0x4c4
			 0x491a8c5b, /// 0x4c8
			 0x7dbc3577, /// 0x4cc
			 0x30b87bd9, /// 0x4d0
			 0x5dd56aee, /// 0x4d4
			 0x5d900461, /// 0x4d8
			 0x49a3803d, /// 0x4dc
			 0xf5bcbb87, /// 0x4e0
			 0x04d79407, /// 0x4e4
			 0x52c31582, /// 0x4e8
			 0x92e2bc81, /// 0x4ec
			 0x1b97b080, /// 0x4f0
			 0xa08882f8, /// 0x4f4
			 0x15242a9d, /// 0x4f8
			 0x3efbc215, /// 0x4fc
			 0xc14cfd60, /// 0x500
			 0xf7a8d344, /// 0x504
			 0x7bd5d0a0, /// 0x508
			 0x0f5b75ca, /// 0x50c
			 0xd1e10f24, /// 0x510
			 0x3aa5e0b8, /// 0x514
			 0x9bbd8bf7, /// 0x518
			 0xe38dea76, /// 0x51c
			 0xd8dfc315, /// 0x520
			 0xb8f54da3, /// 0x524
			 0xfc048ab2, /// 0x528
			 0xee76d40f, /// 0x52c
			 0xa1472ce9, /// 0x530
			 0xdf4fbfa7, /// 0x534
			 0x73981afc, /// 0x538
			 0x32384710, /// 0x53c
			 0x504c155e, /// 0x540
			 0xcf8ce8b7, /// 0x544
			 0xb0bd6927, /// 0x548
			 0xf2c835da, /// 0x54c
			 0x44727fe7, /// 0x550
			 0xcc72927d, /// 0x554
			 0x332d090b, /// 0x558
			 0x1fca4d70, /// 0x55c
			 0xcada4b6b, /// 0x560
			 0x25f7dfdb, /// 0x564
			 0xbf9323ae, /// 0x568
			 0xa1dece6a, /// 0x56c
			 0x4350bd54, /// 0x570
			 0xafe68827, /// 0x574
			 0xe5a87fe5, /// 0x578
			 0x0d05784e, /// 0x57c
			 0x3e69e817, /// 0x580
			 0x3b158b09, /// 0x584
			 0x5a112c0a, /// 0x588
			 0x6a087242, /// 0x58c
			 0x9ca87b56, /// 0x590
			 0x29dd1829, /// 0x594
			 0x01b0eef0, /// 0x598
			 0xae6147f9, /// 0x59c
			 0x75458965, /// 0x5a0
			 0xb1e2e908, /// 0x5a4
			 0x8e271a59, /// 0x5a8
			 0x5bce6058, /// 0x5ac
			 0x68743b90, /// 0x5b0
			 0x03edf9c8, /// 0x5b4
			 0x7f6222c7, /// 0x5b8
			 0x6a7497b3, /// 0x5bc
			 0xb80db390, /// 0x5c0
			 0x1e9dfb2b, /// 0x5c4
			 0x6b7e45d5, /// 0x5c8
			 0x13c97d54, /// 0x5cc
			 0x6ccbc2b9, /// 0x5d0
			 0x45f30741, /// 0x5d4
			 0x0749b1ec, /// 0x5d8
			 0x4ca1a492, /// 0x5dc
			 0x18231f71, /// 0x5e0
			 0x465f0b14, /// 0x5e4
			 0xe4b24152, /// 0x5e8
			 0x65a2ff35, /// 0x5ec
			 0xd5668dd2, /// 0x5f0
			 0x3a7e661f, /// 0x5f4
			 0xebcc858b, /// 0x5f8
			 0x3a06310c, /// 0x5fc
			 0x1d22455f, /// 0x600
			 0xf03d23d2, /// 0x604
			 0x6f675296, /// 0x608
			 0xed50a356, /// 0x60c
			 0x03996f14, /// 0x610
			 0xf3249edd, /// 0x614
			 0x15829a78, /// 0x618
			 0x3b7f724f, /// 0x61c
			 0xa106af3f, /// 0x620
			 0x55b23b8d, /// 0x624
			 0x97aad8c8, /// 0x628
			 0x99f06f77, /// 0x62c
			 0x69c471ae, /// 0x630
			 0x09b97545, /// 0x634
			 0x57d00da5, /// 0x638
			 0x3c70b6c4, /// 0x63c
			 0x461a19b5, /// 0x640
			 0xdbce03d5, /// 0x644
			 0x0621d3f8, /// 0x648
			 0xd2046c99, /// 0x64c
			 0xa7b04f9c, /// 0x650
			 0xfb2a596b, /// 0x654
			 0xc5757da4, /// 0x658
			 0x99098b83, /// 0x65c
			 0xc474b76b, /// 0x660
			 0x286ad6e8, /// 0x664
			 0xc03e94f1, /// 0x668
			 0x3cad9173, /// 0x66c
			 0x5e9fe4df, /// 0x670
			 0xa14fdcb9, /// 0x674
			 0xe629d5af, /// 0x678
			 0x608272e4, /// 0x67c
			 0x1cf395e1, /// 0x680
			 0x40aab463, /// 0x684
			 0xce354735, /// 0x688
			 0x802492ab, /// 0x68c
			 0xe235027d, /// 0x690
			 0xc4701242, /// 0x694
			 0xdb8ae4ab, /// 0x698
			 0x65e3aa84, /// 0x69c
			 0xd7c9cadf, /// 0x6a0
			 0xd6e8d783, /// 0x6a4
			 0x98a8d380, /// 0x6a8
			 0xf0e2ba2b, /// 0x6ac
			 0x53cc2523, /// 0x6b0
			 0xd062b82e, /// 0x6b4
			 0x05b2680d, /// 0x6b8
			 0x622a4394, /// 0x6bc
			 0x8b507f0b, /// 0x6c0
			 0x24da25d2, /// 0x6c4
			 0x329ba02c, /// 0x6c8
			 0xabc87201, /// 0x6cc
			 0xb0a7203a, /// 0x6d0
			 0xa56ddc43, /// 0x6d4
			 0xfb9ec4b4, /// 0x6d8
			 0x8de3c2ae, /// 0x6dc
			 0x373f34ec, /// 0x6e0
			 0x1783d659, /// 0x6e4
			 0xe578bec1, /// 0x6e8
			 0x45e44ed9, /// 0x6ec
			 0x0b09383c, /// 0x6f0
			 0xd3d31c56, /// 0x6f4
			 0xc0b2edbb, /// 0x6f8
			 0xc0d18296, /// 0x6fc
			 0xb6ca2755, /// 0x700
			 0xaa52d894, /// 0x704
			 0x7f574f19, /// 0x708
			 0x13d058b2, /// 0x70c
			 0x6412208e, /// 0x710
			 0x66a37fe8, /// 0x714
			 0x3ce40f1b, /// 0x718
			 0x36c6e340, /// 0x71c
			 0x5b9f24f8, /// 0x720
			 0xa00aaa77, /// 0x724
			 0xe1263f63, /// 0x728
			 0xf801bdab, /// 0x72c
			 0x9cc58d94, /// 0x730
			 0x64235ff3, /// 0x734
			 0x385b139a, /// 0x738
			 0xa60a482a, /// 0x73c
			 0x429571ea, /// 0x740
			 0x91e7dc27, /// 0x744
			 0x1d1f956b, /// 0x748
			 0x399ccc41, /// 0x74c
			 0x350861e5, /// 0x750
			 0x09dd2230, /// 0x754
			 0xf8fd0032, /// 0x758
			 0x94c8759f, /// 0x75c
			 0x87b408bd, /// 0x760
			 0x9237d879, /// 0x764
			 0xcad785ca, /// 0x768
			 0xb7803a66, /// 0x76c
			 0xc5ba5c5e, /// 0x770
			 0xce1acd31, /// 0x774
			 0x8970dc3e, /// 0x778
			 0x3fa01074, /// 0x77c
			 0x40eca517, /// 0x780
			 0x2b2d5801, /// 0x784
			 0x201b4043, /// 0x788
			 0x6b815b23, /// 0x78c
			 0xaec0b78e, /// 0x790
			 0x7f835f07, /// 0x794
			 0x5696ad3d, /// 0x798
			 0x13a68b75, /// 0x79c
			 0xedf871d4, /// 0x7a0
			 0xd6acd9c4, /// 0x7a4
			 0x21478210, /// 0x7a8
			 0xd2f0e07f, /// 0x7ac
			 0x07194329, /// 0x7b0
			 0x2d6b4dc3, /// 0x7b4
			 0x2f3e154e, /// 0x7b8
			 0x5f385c48, /// 0x7bc
			 0xda21ea27, /// 0x7c0
			 0xd2d908aa, /// 0x7c4
			 0xafdaba1a, /// 0x7c8
			 0x7606b42a, /// 0x7cc
			 0x883e009c, /// 0x7d0
			 0x3aa1cbe0, /// 0x7d4
			 0x5baec50b, /// 0x7d8
			 0xf756f319, /// 0x7dc
			 0xad0caf5a, /// 0x7e0
			 0xe49f9566, /// 0x7e4
			 0x07308ce8, /// 0x7e8
			 0x06b8f6ec, /// 0x7ec
			 0x9d39c4bf, /// 0x7f0
			 0x3dd4cf5f, /// 0x7f4
			 0xceddba73, /// 0x7f8
			 0xad39d5ba, /// 0x7fc
			 0xb375d400, /// 0x800
			 0xfd7985eb, /// 0x804
			 0x63009898, /// 0x808
			 0x9e0a837b, /// 0x80c
			 0xc557690d, /// 0x810
			 0x97b3b5e9, /// 0x814
			 0x910be6ab, /// 0x818
			 0xa12901c9, /// 0x81c
			 0x8f7bfe39, /// 0x820
			 0x5727c6ae, /// 0x824
			 0x983a3e03, /// 0x828
			 0x522b9604, /// 0x82c
			 0x5ae24f17, /// 0x830
			 0x8620107a, /// 0x834
			 0x9867cee7, /// 0x838
			 0xb8e19759, /// 0x83c
			 0x9348fbfe, /// 0x840
			 0x093ed587, /// 0x844
			 0x32bea97a, /// 0x848
			 0xd915d2f9, /// 0x84c
			 0xbd46a733, /// 0x850
			 0x2499cf38, /// 0x854
			 0xa2ec4194, /// 0x858
			 0x8d2c9a3b, /// 0x85c
			 0x603b316f, /// 0x860
			 0x0bf4c1be, /// 0x864
			 0x6dc6061d, /// 0x868
			 0x2012dbf5, /// 0x86c
			 0xfb002167, /// 0x870
			 0x93b8b0f8, /// 0x874
			 0x26abb45f, /// 0x878
			 0xa389d434, /// 0x87c
			 0x286b2b32, /// 0x880
			 0x1f3962d5, /// 0x884
			 0xf73f1f54, /// 0x888
			 0x3d565ede, /// 0x88c
			 0x600910a6, /// 0x890
			 0x8b737426, /// 0x894
			 0x321765e7, /// 0x898
			 0x8bf79a60, /// 0x89c
			 0x2b64dfec, /// 0x8a0
			 0xcb4f13b3, /// 0x8a4
			 0x8a58b2ae, /// 0x8a8
			 0xcb210001, /// 0x8ac
			 0x34ff98d1, /// 0x8b0
			 0xcb94fa43, /// 0x8b4
			 0x89060a2b, /// 0x8b8
			 0xc38b2b42, /// 0x8bc
			 0x558f1e40, /// 0x8c0
			 0x72ea5d86, /// 0x8c4
			 0xda93f30c, /// 0x8c8
			 0x8b0f293b, /// 0x8cc
			 0xd4e90608, /// 0x8d0
			 0xf733f0a2, /// 0x8d4
			 0x23f0a06a, /// 0x8d8
			 0x37542017, /// 0x8dc
			 0x41e56c36, /// 0x8e0
			 0x91557a62, /// 0x8e4
			 0x6fb61a0f, /// 0x8e8
			 0xe64debfa, /// 0x8ec
			 0xbed53474, /// 0x8f0
			 0x0f5d6c90, /// 0x8f4
			 0xab9dd8c6, /// 0x8f8
			 0x604e14b1, /// 0x8fc
			 0x6d623e03, /// 0x900
			 0x870db6e1, /// 0x904
			 0x0ae417b8, /// 0x908
			 0x6d462c95, /// 0x90c
			 0x4e72db53, /// 0x910
			 0x389ac1c2, /// 0x914
			 0xd31368be, /// 0x918
			 0x6dc9ac19, /// 0x91c
			 0xa167d9ed, /// 0x920
			 0x1f1ed75b, /// 0x924
			 0x15b0e14f, /// 0x928
			 0x077b9ea8, /// 0x92c
			 0xc6a5cfd4, /// 0x930
			 0x58609f6d, /// 0x934
			 0x528db1d5, /// 0x938
			 0xf5c3b24b, /// 0x93c
			 0x0bb50cef, /// 0x940
			 0x20f32c41, /// 0x944
			 0x45228d54, /// 0x948
			 0xbd68ac1b, /// 0x94c
			 0x8930802b, /// 0x950
			 0x6a123db9, /// 0x954
			 0xc219596c, /// 0x958
			 0x7e4d7495, /// 0x95c
			 0x9479d439, /// 0x960
			 0x1046e161, /// 0x964
			 0x1f2b4bdd, /// 0x968
			 0x3ea62288, /// 0x96c
			 0x5a07b1e8, /// 0x970
			 0x1067d81b, /// 0x974
			 0x6074304e, /// 0x978
			 0x5cc97182, /// 0x97c
			 0xaa4f54df, /// 0x980
			 0xd1cd5eb5, /// 0x984
			 0xb80d3c23, /// 0x988
			 0x34a5f583, /// 0x98c
			 0x4087db82, /// 0x990
			 0x2669c4b0, /// 0x994
			 0x0aa2f3d3, /// 0x998
			 0xe59fa5bd, /// 0x99c
			 0xe0b1feec, /// 0x9a0
			 0x30e7c673, /// 0x9a4
			 0xe39cb582, /// 0x9a8
			 0x616da694, /// 0x9ac
			 0xe8a51dc7, /// 0x9b0
			 0x825fce2a, /// 0x9b4
			 0x73ebf3bf, /// 0x9b8
			 0x2f4c2d48, /// 0x9bc
			 0xd2f44e0b, /// 0x9c0
			 0xcd3ce2d9, /// 0x9c4
			 0xdfaba37c, /// 0x9c8
			 0x0be00805, /// 0x9cc
			 0x68dfcf9c, /// 0x9d0
			 0xa02315de, /// 0x9d4
			 0x173a1955, /// 0x9d8
			 0x2cb6acda, /// 0x9dc
			 0x24402a42, /// 0x9e0
			 0x7a78e6bb, /// 0x9e4
			 0x115da47d, /// 0x9e8
			 0xbed7fbc4, /// 0x9ec
			 0xc2048275, /// 0x9f0
			 0x111f7978, /// 0x9f4
			 0xc07c5925, /// 0x9f8
			 0x749e8cb3, /// 0x9fc
			 0x812c59f3, /// 0xa00
			 0x070f2ac0, /// 0xa04
			 0x6656f4bb, /// 0xa08
			 0x9a2c6155, /// 0xa0c
			 0xfebf2e2f, /// 0xa10
			 0x4f46b536, /// 0xa14
			 0x8ca61f97, /// 0xa18
			 0xb310dd33, /// 0xa1c
			 0x80797709, /// 0xa20
			 0xb9b46de0, /// 0xa24
			 0x3fa292b2, /// 0xa28
			 0xeb5b16ea, /// 0xa2c
			 0x1b44f118, /// 0xa30
			 0x7e933e47, /// 0xa34
			 0x9b7845d6, /// 0xa38
			 0x4bc95452, /// 0xa3c
			 0xdc4278e1, /// 0xa40
			 0xf83ff4cf, /// 0xa44
			 0x24d52065, /// 0xa48
			 0x19ea1679, /// 0xa4c
			 0x0d60e2da, /// 0xa50
			 0x11006321, /// 0xa54
			 0xf62a828e, /// 0xa58
			 0x4f64ceba, /// 0xa5c
			 0xed2b29b5, /// 0xa60
			 0x90098572, /// 0xa64
			 0x91249a40, /// 0xa68
			 0xdca01572, /// 0xa6c
			 0x72596efc, /// 0xa70
			 0x461c1fcc, /// 0xa74
			 0x8c0f77c8, /// 0xa78
			 0xe87986ce, /// 0xa7c
			 0x1c9e77c7, /// 0xa80
			 0xb959ea12, /// 0xa84
			 0x596bb279, /// 0xa88
			 0x703b201f, /// 0xa8c
			 0x773443cb, /// 0xa90
			 0x4010a9bd, /// 0xa94
			 0x6f907372, /// 0xa98
			 0x97157aaf, /// 0xa9c
			 0xb4540b17, /// 0xaa0
			 0x26dcb586, /// 0xaa4
			 0x0e4461db, /// 0xaa8
			 0x967c28c3, /// 0xaac
			 0x97dcb610, /// 0xab0
			 0x4e76f870, /// 0xab4
			 0x994db488, /// 0xab8
			 0x2895d977, /// 0xabc
			 0xe2285565, /// 0xac0
			 0x19d70b2f, /// 0xac4
			 0x11ecdc79, /// 0xac8
			 0xe1ac146e, /// 0xacc
			 0x5de496bb, /// 0xad0
			 0x55006fa4, /// 0xad4
			 0xd3e577fe, /// 0xad8
			 0x6e532b3c, /// 0xadc
			 0x946e7147, /// 0xae0
			 0x9ce34c44, /// 0xae4
			 0xe05e3f52, /// 0xae8
			 0xa37de76a, /// 0xaec
			 0x1c4d2a3e, /// 0xaf0
			 0x67d9ee85, /// 0xaf4
			 0x7fce000f, /// 0xaf8
			 0x6bd7e975, /// 0xafc
			 0x072f6662, /// 0xb00
			 0x6e73c805, /// 0xb04
			 0x27490378, /// 0xb08
			 0x8f70ee35, /// 0xb0c
			 0xab2d935a, /// 0xb10
			 0x522e9048, /// 0xb14
			 0xefbe7f62, /// 0xb18
			 0xc274e613, /// 0xb1c
			 0x31d40524, /// 0xb20
			 0x7a905ff7, /// 0xb24
			 0x2fe28926, /// 0xb28
			 0xc99d8edd, /// 0xb2c
			 0x9270b114, /// 0xb30
			 0x2dae6d34, /// 0xb34
			 0xad0722a0, /// 0xb38
			 0xb130b1e3, /// 0xb3c
			 0xa4b22a43, /// 0xb40
			 0x574569d2, /// 0xb44
			 0xd7501bc7, /// 0xb48
			 0x8023c8c7, /// 0xb4c
			 0x6cafbae4, /// 0xb50
			 0x589b88f9, /// 0xb54
			 0x94368bb1, /// 0xb58
			 0x0fa8a4d1, /// 0xb5c
			 0xb7dc4a86, /// 0xb60
			 0x27f9f364, /// 0xb64
			 0x8fb62ebf, /// 0xb68
			 0x8f1d778c, /// 0xb6c
			 0xd1588bf0, /// 0xb70
			 0xb58f5c08, /// 0xb74
			 0xe2587af1, /// 0xb78
			 0x7918a6c3, /// 0xb7c
			 0x02a8be9a, /// 0xb80
			 0xb0a38ea3, /// 0xb84
			 0xbfe3bb91, /// 0xb88
			 0xcd3c34f2, /// 0xb8c
			 0x3aeab6af, /// 0xb90
			 0xbd2425ac, /// 0xb94
			 0x39a7d667, /// 0xb98
			 0x279e6274, /// 0xb9c
			 0xb7efc7d7, /// 0xba0
			 0x7b1f49a0, /// 0xba4
			 0x1d26a2ae, /// 0xba8
			 0x548e3763, /// 0xbac
			 0xa4140843, /// 0xbb0
			 0x67f8b9fb, /// 0xbb4
			 0x60906361, /// 0xbb8
			 0x922747c4, /// 0xbbc
			 0x2361f262, /// 0xbc0
			 0xf278af9d, /// 0xbc4
			 0x8c0dccb5, /// 0xbc8
			 0xa3588bf9, /// 0xbcc
			 0x209b2e57, /// 0xbd0
			 0xf7775590, /// 0xbd4
			 0x10369873, /// 0xbd8
			 0x5d734e19, /// 0xbdc
			 0xe95f31eb, /// 0xbe0
			 0x12b81ff2, /// 0xbe4
			 0x9a1e3f34, /// 0xbe8
			 0xd28a6f2c, /// 0xbec
			 0xf9cf2803, /// 0xbf0
			 0x1e074d84, /// 0xbf4
			 0x7b2c3408, /// 0xbf8
			 0xee4b86a8, /// 0xbfc
			 0x55ad6651, /// 0xc00
			 0xb9968455, /// 0xc04
			 0xe61e9fef, /// 0xc08
			 0x6e9b1f7d, /// 0xc0c
			 0x4db7643e, /// 0xc10
			 0xfc0e799b, /// 0xc14
			 0x319d70fa, /// 0xc18
			 0x037cd170, /// 0xc1c
			 0x63294ff4, /// 0xc20
			 0x2d631417, /// 0xc24
			 0xe91eb6d8, /// 0xc28
			 0xc0a06a9c, /// 0xc2c
			 0x20d717ba, /// 0xc30
			 0x991c7780, /// 0xc34
			 0x55318242, /// 0xc38
			 0xe168786e, /// 0xc3c
			 0xbfff2f42, /// 0xc40
			 0x59213f18, /// 0xc44
			 0x0d079347, /// 0xc48
			 0x252fa0f4, /// 0xc4c
			 0x2b70e4f7, /// 0xc50
			 0x5ab8c887, /// 0xc54
			 0xe6a0392e, /// 0xc58
			 0x3edecf22, /// 0xc5c
			 0x56a8cbbc, /// 0xc60
			 0x73906519, /// 0xc64
			 0x8b9ec16e, /// 0xc68
			 0xac9d2113, /// 0xc6c
			 0x5eb1c84d, /// 0xc70
			 0xc78615b8, /// 0xc74
			 0xee6c99de, /// 0xc78
			 0x35c92e0b, /// 0xc7c
			 0x83c4b801, /// 0xc80
			 0xacc898b6, /// 0xc84
			 0x0f4df081, /// 0xc88
			 0x16674453, /// 0xc8c
			 0x4364ec33, /// 0xc90
			 0x9d0195b9, /// 0xc94
			 0xff6ae2c0, /// 0xc98
			 0x0d41a322, /// 0xc9c
			 0x17384183, /// 0xca0
			 0x48a3cc26, /// 0xca4
			 0x05529fd4, /// 0xca8
			 0xe60495ea, /// 0xcac
			 0xc6184224, /// 0xcb0
			 0x4a4a0ca1, /// 0xcb4
			 0xbdd58c18, /// 0xcb8
			 0xf0e7548f, /// 0xcbc
			 0xd83ec699, /// 0xcc0
			 0x8947c70c, /// 0xcc4
			 0x9afe89e0, /// 0xcc8
			 0x8983aa2a, /// 0xccc
			 0x18f5f6ec, /// 0xcd0
			 0x0a2478b3, /// 0xcd4
			 0x61dd17c4, /// 0xcd8
			 0x6c6c64f9, /// 0xcdc
			 0x02554978, /// 0xce0
			 0x699ad60a, /// 0xce4
			 0x84fe18cb, /// 0xce8
			 0xfb108a3f, /// 0xcec
			 0x549700e7, /// 0xcf0
			 0xceec59d5, /// 0xcf4
			 0x6afffcce, /// 0xcf8
			 0x5cc6a238, /// 0xcfc
			 0xa63613de, /// 0xd00
			 0x538f65e6, /// 0xd04
			 0x804965c2, /// 0xd08
			 0x6005f4b9, /// 0xd0c
			 0xe369ea50, /// 0xd10
			 0xdb4b8472, /// 0xd14
			 0xd559d020, /// 0xd18
			 0x8780dadd, /// 0xd1c
			 0x35f30351, /// 0xd20
			 0x4bc8b226, /// 0xd24
			 0xe2b35305, /// 0xd28
			 0x02cea4d6, /// 0xd2c
			 0xb71f4466, /// 0xd30
			 0xbf4b1c13, /// 0xd34
			 0x40b01efe, /// 0xd38
			 0xb6f4896e, /// 0xd3c
			 0x54b238eb, /// 0xd40
			 0x98f8adeb, /// 0xd44
			 0xba8eb12c, /// 0xd48
			 0x72875378, /// 0xd4c
			 0x5407b30b, /// 0xd50
			 0xbbcda681, /// 0xd54
			 0x685c22a2, /// 0xd58
			 0x755839c3, /// 0xd5c
			 0xed6f9ea1, /// 0xd60
			 0x75c85e95, /// 0xd64
			 0xae6950a5, /// 0xd68
			 0xdae07491, /// 0xd6c
			 0x7d33f099, /// 0xd70
			 0x61ecee1c, /// 0xd74
			 0xc1160364, /// 0xd78
			 0xa60ac012, /// 0xd7c
			 0x45ddc11d, /// 0xd80
			 0xbc18ad15, /// 0xd84
			 0xb9dfcf81, /// 0xd88
			 0x5ceb3521, /// 0xd8c
			 0xac34c866, /// 0xd90
			 0x2196afeb, /// 0xd94
			 0xd2d0521c, /// 0xd98
			 0xf537b518, /// 0xd9c
			 0x435c7f3f, /// 0xda0
			 0xc75ba92a, /// 0xda4
			 0x5d8cea24, /// 0xda8
			 0x25783f29, /// 0xdac
			 0x9b6ba426, /// 0xdb0
			 0x1bdda910, /// 0xdb4
			 0x9fb49d5e, /// 0xdb8
			 0xebaa72b4, /// 0xdbc
			 0x788438e7, /// 0xdc0
			 0x39e00eb1, /// 0xdc4
			 0xd66f3bb5, /// 0xdc8
			 0x6518e61b, /// 0xdcc
			 0x0f105039, /// 0xdd0
			 0x0845e57c, /// 0xdd4
			 0xd5484540, /// 0xdd8
			 0x7b3906dd, /// 0xddc
			 0xf07eec5b, /// 0xde0
			 0xdbaa3da3, /// 0xde4
			 0x9797c5f3, /// 0xde8
			 0xf38c3745, /// 0xdec
			 0x8ee3afd4, /// 0xdf0
			 0xd92ab724, /// 0xdf4
			 0xb9521dab, /// 0xdf8
			 0x595a1ac7, /// 0xdfc
			 0x98164420, /// 0xe00
			 0x68d89b52, /// 0xe04
			 0xa0a378f3, /// 0xe08
			 0xc16a138c, /// 0xe0c
			 0x84ea3e5d, /// 0xe10
			 0x4ebf33bd, /// 0xe14
			 0x10464f3c, /// 0xe18
			 0x1084eb71, /// 0xe1c
			 0x1387edb9, /// 0xe20
			 0x1221664f, /// 0xe24
			 0xb620073d, /// 0xe28
			 0x4dbef547, /// 0xe2c
			 0xcdbf9f5a, /// 0xe30
			 0xff785fb1, /// 0xe34
			 0x592c0c7a, /// 0xe38
			 0xf57536ec, /// 0xe3c
			 0xfaf24ee9, /// 0xe40
			 0x63261e64, /// 0xe44
			 0x722d29b8, /// 0xe48
			 0x1419e821, /// 0xe4c
			 0x33a526c0, /// 0xe50
			 0xca48dfdd, /// 0xe54
			 0x04b90555, /// 0xe58
			 0xfb5df4c4, /// 0xe5c
			 0x89f1e9ff, /// 0xe60
			 0x620dd8d9, /// 0xe64
			 0x4f80444f, /// 0xe68
			 0x89f2763c, /// 0xe6c
			 0xe2cbc96e, /// 0xe70
			 0x9fcce1e9, /// 0xe74
			 0xc9170d27, /// 0xe78
			 0x1eb4b3d8, /// 0xe7c
			 0x486abc06, /// 0xe80
			 0xff68df3c, /// 0xe84
			 0x40f38a0a, /// 0xe88
			 0x4b799999, /// 0xe8c
			 0x5c397612, /// 0xe90
			 0x51adae1e, /// 0xe94
			 0x05648ccf, /// 0xe98
			 0x44649b5d, /// 0xe9c
			 0x74e2255d, /// 0xea0
			 0xdc3d069f, /// 0xea4
			 0x0cf24f54, /// 0xea8
			 0x95545ef0, /// 0xeac
			 0x6f001b17, /// 0xeb0
			 0xe64b80ba, /// 0xeb4
			 0x9f9ce169, /// 0xeb8
			 0x82d9a350, /// 0xebc
			 0x080c5306, /// 0xec0
			 0x3bdda1c6, /// 0xec4
			 0xd31e8e60, /// 0xec8
			 0xd922760b, /// 0xecc
			 0xc99ecaf5, /// 0xed0
			 0x7682d1e5, /// 0xed4
			 0x09acec16, /// 0xed8
			 0x2f076c09, /// 0xedc
			 0x65db4282, /// 0xee0
			 0x39d250cf, /// 0xee4
			 0x5e0a9855, /// 0xee8
			 0x037d8ad4, /// 0xeec
			 0x69b87242, /// 0xef0
			 0x86551f56, /// 0xef4
			 0x8b643981, /// 0xef8
			 0xdb72a490, /// 0xefc
			 0xeea1ad4d, /// 0xf00
			 0x51c6d55e, /// 0xf04
			 0x0bb115a6, /// 0xf08
			 0x07d33c99, /// 0xf0c
			 0x7e3391b1, /// 0xf10
			 0x50c6f063, /// 0xf14
			 0xb130cab9, /// 0xf18
			 0x0c27f4dc, /// 0xf1c
			 0x7ae31d7a, /// 0xf20
			 0x4b3f1714, /// 0xf24
			 0x86a2da81, /// 0xf28
			 0xd559e203, /// 0xf2c
			 0xe3a39281, /// 0xf30
			 0xfa6f5322, /// 0xf34
			 0xddb00fc6, /// 0xf38
			 0x7bad73c0, /// 0xf3c
			 0xe41ddefe, /// 0xf40
			 0xbcd6543a, /// 0xf44
			 0xfd86de38, /// 0xf48
			 0x93c82900, /// 0xf4c
			 0x0d1f36b8, /// 0xf50
			 0x91935149, /// 0xf54
			 0x957f270e, /// 0xf58
			 0x5cea5ac9, /// 0xf5c
			 0x70316fde, /// 0xf60
			 0x643c568f, /// 0xf64
			 0x855fd250, /// 0xf68
			 0xf8a9374a, /// 0xf6c
			 0x631006bb, /// 0xf70
			 0xedfe74d5, /// 0xf74
			 0xdb082164, /// 0xf78
			 0xbb0be23d, /// 0xf7c
			 0xaf9bfa01, /// 0xf80
			 0xe9258596, /// 0xf84
			 0x4c3a1f82, /// 0xf88
			 0xde54ddc2, /// 0xf8c
			 0xc7b3ee56, /// 0xf90
			 0x91dee321, /// 0xf94
			 0x69b0f25e, /// 0xf98
			 0x0691d68e, /// 0xf9c
			 0x517849bc, /// 0xfa0
			 0xefc4a675, /// 0xfa4
			 0x4ada9ef5, /// 0xfa8
			 0x14a6b9f0, /// 0xfac
			 0x25f94c46, /// 0xfb0
			 0xb3a3500d, /// 0xfb4
			 0x356121cc, /// 0xfb8
			 0x49a21d95, /// 0xfbc
			 0x009b32ff, /// 0xfc0
			 0x6f0a688a, /// 0xfc4
			 0xa5249c81, /// 0xfc8
			 0x2d95f31b, /// 0xfcc
			 0x7fefaeef, /// 0xfd0
			 0x35001602, /// 0xfd4
			 0x6029630b, /// 0xfd8
			 0x0f00a435, /// 0xfdc
			 0x43c3dde2, /// 0xfe0
			 0x1e8fc2d6, /// 0xfe4
			 0x490e0216, /// 0xfe8
			 0xfe8de3cd, /// 0xfec
			 0xe2478184, /// 0xff0
			 0x51da90f6, /// 0xff4
			 0x76ce72d1, /// 0xff8
			 0xf8b164f1 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00010
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00014
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00018
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00020
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00024
			 0x00011111,                                                  // 9.7958E-41                                  /// 00028
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0002c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00030
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0xcb000000,                                                  // -8388608.0                                  /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00048
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00058
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0005c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00060
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00068
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0006c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00074
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0007c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00084
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00088
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0008c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00098
			 0x0c400000,                                                  // Leading 1s:                                 /// 0009c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x55555555,                                                  // 4 random values                             /// 000c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xffc00001,                                                  // -signaling NaN                              /// 000e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00100
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0xcb000000,                                                  // -8388608.0                                  /// 00108
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00110
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00114
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00118
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00120
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00124
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00128
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0012c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00130
			 0xcb000000,                                                  // -8388608.0                                  /// 00134
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x55555555,                                                  // 4 random values                             /// 0013c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00140
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00144
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00148
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0014c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00154
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0015c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00164
			 0xbf028f5c,                                                  // -0.51                                       /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00170
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00178
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00184
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80000000,                                                  // -zero                                       /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00194
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00198
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0019c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001c4
			 0x4b000000,                                                  // 8388608.0                                   /// 001c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001e0
			 0x4b000000,                                                  // 8388608.0                                   /// 001e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00200
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00204
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00208
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0020c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00210
			 0x33333333,                                                  // 4 random values                             /// 00214
			 0x00011111,                                                  // 9.7958E-41                                  /// 00218
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0021c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00224
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00238
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0023c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00244
			 0x00000000,                                                  // zero                                        /// 00248
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0024c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00250
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00254
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00258
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0025c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00260
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00264
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c400000,                                                  // Leading 1s:                                 /// 0026c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00270
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00278
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00280
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00284
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00290
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00294
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x33333333,                                                  // 4 random values                             /// 0029c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002c4
			 0x80000000,                                                  // -zero                                       /// 002c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0xbf028f5c,                                                  // -0.51                                       /// 002f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00000000,                                                  // zero                                        /// 002fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00310
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00314
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00318
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00324
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00328
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00338
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0033c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00340
			 0xffc00001,                                                  // -signaling NaN                              /// 00344
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00348
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0034c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00350
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00358
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0035c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00360
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00368
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0036c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00370
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00374
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00378
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00380
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00011111,                                                  // 9.7958E-41                                  /// 00388
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00390
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00394
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0e000007,                                                  // Trailing 1s:                                /// 0039c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 003b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00400
			 0x7f800000,                                                  // inf                                         /// 00404
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00408
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00410
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00418
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00420
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00424
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00428
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00430
			 0x4b000000,                                                  // 8388608.0                                   /// 00434
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0043c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00444
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00448
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0044c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00450
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00454
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00000000,                                                  // zero                                        /// 0045c
			 0x7fc00001,                                                  // signaling NaN                               /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x7f800000,                                                  // inf                                         /// 00468
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0046c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00470
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00474
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0e000001,                                                  // Trailing 1s:                                /// 00484
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00488
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00498
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004a0
			 0xffc00001,                                                  // -signaling NaN                              /// 004a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000000,                                                  // zero                                        /// 004b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004e8
			 0x7f800000,                                                  // inf                                         /// 004ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00500
			 0x7f800000,                                                  // inf                                         /// 00504
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00508
			 0x3f028f5c,                                                  // 0.51                                        /// 0050c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00510
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0051c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00520
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00538
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00540
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00548
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00554
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00558
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00560
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00564
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00568
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0056c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00574
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0057c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00580
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x7fc00001,                                                  // signaling NaN                               /// 00588
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0058c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00590
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00594
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00598
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00604
			 0x3f028f5c,                                                  // 0.51                                        /// 00608
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00610
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00614
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00618
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0061c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00624
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00628
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00634
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00638
			 0x4b000000,                                                  // 8388608.0                                   /// 0063c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00640
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00644
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00648
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00650
			 0x4b000000,                                                  // 8388608.0                                   /// 00654
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0065c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00660
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x33333333,                                                  // 4 random values                             /// 00668
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0066c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00674
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00678
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00680
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00684
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00688
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xffc00001,                                                  // -signaling NaN                              /// 00694
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0070c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0071c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00720
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00724
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00730
			 0xbf028f5c,                                                  // -0.51                                       /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0073c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00740
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80000000,                                                  // -zero                                       /// 00748
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0074c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00750
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00758
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00760
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00768
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00780
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00784
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00788
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0079c
			 0x4b000000,                                                  // 8388608.0                                   /// 007a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ec
			 0xcb000000,                                                  // -8388608.0                                  /// 007f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00800
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00804
			 0x00011111,                                                  // 9.7958E-41                                  /// 00808
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0080c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00810
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0081c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00820
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00828
			 0x7fc00001,                                                  // signaling NaN                               /// 0082c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00830
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x0e000007,                                                  // Trailing 1s:                                /// 00838
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00840
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00848
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0084c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00850
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00858
			 0x55555555,                                                  // 4 random values                             /// 0085c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00860
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x00000000,                                                  // zero                                        /// 0086c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00870
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0e000001,                                                  // Trailing 1s:                                /// 00884
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0088c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00890
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00894
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0089c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a8
			 0x33333333,                                                  // 4 random values                             /// 008ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x33333333,                                                  // 4 random values                             /// 008d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00900
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00904
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00910
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00914
			 0x80000000,                                                  // -zero                                       /// 00918
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00924
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00934
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00938
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00940
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00944
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00948
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0094c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x0c600000,                                                  // Leading 1s:                                 /// 00954
			 0x00000000,                                                  // zero                                        /// 00958
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xff800000,                                                  // -inf                                        /// 00964
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00974
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00980
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00984
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0098c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00990
			 0x0c780000,                                                  // Leading 1s:                                 /// 00994
			 0xbf028f5c,                                                  // -0.51                                       /// 00998
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009b0
			 0x33333333,                                                  // 4 random values                             /// 009b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x7f800000,                                                  // inf                                         /// 009d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x4b000000,                                                  // 8388608.0                                   /// 00a4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a84
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00aa0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab8
			 0x3f028f5c,                                                  // 0.51                                        /// 00abc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00acc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00adc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00af0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0xcb000000,                                                  // -8388608.0                                  /// 00af8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00afc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0xcb000000,                                                  // -8388608.0                                  /// 00b04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b08
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b10
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00000000,                                                  // zero                                        /// 00b2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x00000000,                                                  // zero                                        /// 00b80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ba4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bf0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bf8
			 0x7f800000,                                                  // inf                                         /// 00bfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c14
			 0x00000000,                                                  // zero                                        /// 00c18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0xffc00001,                                                  // -signaling NaN                              /// 00c38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c4c
			 0x7f800000,                                                  // inf                                         /// 00c50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x3f028f5c,                                                  // 0.51                                        /// 00c84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ca4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x7f800000,                                                  // inf                                         /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cdc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ce0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ce4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cf0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x3f028f5c,                                                  // 0.51                                        /// 00d28
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d50
			 0x55555555,                                                  // 4 random values                             /// 00d54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x33333333,                                                  // 4 random values                             /// 00d64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00da8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dbc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ddc
			 0xff800000,                                                  // -inf                                        /// 00de0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00de4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00de8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00df8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dfc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e38
			 0xcb000000,                                                  // -8388608.0                                  /// 00e3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e58
			 0xcb000000,                                                  // -8388608.0                                  /// 00e5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000000,                                                  // -zero                                       /// 00e74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e94
			 0xffc00001,                                                  // -signaling NaN                              /// 00e98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ec0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ed0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed8
			 0x33333333,                                                  // 4 random values                             /// 00edc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ee8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ef4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f00
			 0xcb000000,                                                  // -8388608.0                                  /// 00f04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f18
			 0xcb000000,                                                  // -8388608.0                                  /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f24
			 0x55555555,                                                  // 4 random values                             /// 00f28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x7fc00001,                                                  // signaling NaN                               /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fc4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80800000 // min norm                                      // subnormals -ve                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x63d7b253, /// 0x0
			 0xe7f309cb, /// 0x4
			 0x97a2f9c6, /// 0x8
			 0x0501861b, /// 0xc
			 0x9261f522, /// 0x10
			 0x83113666, /// 0x14
			 0x01421e14, /// 0x18
			 0xbaa2dc44, /// 0x1c
			 0x4a414486, /// 0x20
			 0x00c21f7b, /// 0x24
			 0x3d79490d, /// 0x28
			 0x8431c5ec, /// 0x2c
			 0xa5feff21, /// 0x30
			 0xa0733e57, /// 0x34
			 0x34275ea3, /// 0x38
			 0x644f540e, /// 0x3c
			 0x33cfd954, /// 0x40
			 0xd6de41bd, /// 0x44
			 0x014dfc37, /// 0x48
			 0x4e223c2e, /// 0x4c
			 0x5523e30f, /// 0x50
			 0xeaf74717, /// 0x54
			 0x01314d18, /// 0x58
			 0x0e36d1c7, /// 0x5c
			 0xd2bd8cf6, /// 0x60
			 0xcbfd8e5e, /// 0x64
			 0x296b25c5, /// 0x68
			 0x138a87d8, /// 0x6c
			 0x3df572a3, /// 0x70
			 0x9a3f77b7, /// 0x74
			 0xe9ddab14, /// 0x78
			 0xba9e9ba2, /// 0x7c
			 0xa26de15a, /// 0x80
			 0x4de85e27, /// 0x84
			 0x952b64bc, /// 0x88
			 0x2c5dc15f, /// 0x8c
			 0xd147a512, /// 0x90
			 0xfa5d42c2, /// 0x94
			 0xb3c29acf, /// 0x98
			 0xe7f81a59, /// 0x9c
			 0x1a501a07, /// 0xa0
			 0xd642dc1e, /// 0xa4
			 0x3cfcc85c, /// 0xa8
			 0xd2d7796d, /// 0xac
			 0xe41db81d, /// 0xb0
			 0x8e41a287, /// 0xb4
			 0xa10c451d, /// 0xb8
			 0x99d32f5e, /// 0xbc
			 0x38697ca3, /// 0xc0
			 0x0aa373d0, /// 0xc4
			 0x3e368cf9, /// 0xc8
			 0x5e83bb95, /// 0xcc
			 0x2156401f, /// 0xd0
			 0x5dd47a2f, /// 0xd4
			 0x3d651835, /// 0xd8
			 0xea410ac7, /// 0xdc
			 0x8ffcd105, /// 0xe0
			 0xbe0229af, /// 0xe4
			 0x3f2c9f99, /// 0xe8
			 0xbbff471a, /// 0xec
			 0x9ab3dc0b, /// 0xf0
			 0x8361c28e, /// 0xf4
			 0x59652245, /// 0xf8
			 0x4505a857, /// 0xfc
			 0x994f29f9, /// 0x100
			 0x6f6a5376, /// 0x104
			 0x0adbee87, /// 0x108
			 0x774aaaa7, /// 0x10c
			 0xee889516, /// 0x110
			 0xcfb28b2e, /// 0x114
			 0xcac9fd33, /// 0x118
			 0x05e27b52, /// 0x11c
			 0xf29c9b70, /// 0x120
			 0xd47a4d24, /// 0x124
			 0x6696ac09, /// 0x128
			 0x8db3139c, /// 0x12c
			 0xc2faed5a, /// 0x130
			 0x72d2e9e1, /// 0x134
			 0x54dc7dd2, /// 0x138
			 0xb16dfd63, /// 0x13c
			 0xcfedc382, /// 0x140
			 0x3978275e, /// 0x144
			 0x968b376b, /// 0x148
			 0xbf1776d2, /// 0x14c
			 0xcf36c3ca, /// 0x150
			 0xb240d9e4, /// 0x154
			 0x900617ae, /// 0x158
			 0x8bc4a223, /// 0x15c
			 0x3c0ce9f0, /// 0x160
			 0x5869e87d, /// 0x164
			 0x73ca3b0e, /// 0x168
			 0xfdefb526, /// 0x16c
			 0x7fee24c2, /// 0x170
			 0x3f6d3d3f, /// 0x174
			 0x196745a9, /// 0x178
			 0x60de9331, /// 0x17c
			 0x99dfb0fe, /// 0x180
			 0x7189adad, /// 0x184
			 0x797e97bb, /// 0x188
			 0x3bf40866, /// 0x18c
			 0xdc84eb16, /// 0x190
			 0x12213ac0, /// 0x194
			 0x8abc5fc5, /// 0x198
			 0x94e3f096, /// 0x19c
			 0x59afbb09, /// 0x1a0
			 0xc63827b7, /// 0x1a4
			 0x0ee03dec, /// 0x1a8
			 0xff053856, /// 0x1ac
			 0xa882d810, /// 0x1b0
			 0x8789a240, /// 0x1b4
			 0xc46a1fc5, /// 0x1b8
			 0xbd358258, /// 0x1bc
			 0xcae86e92, /// 0x1c0
			 0x8af235f7, /// 0x1c4
			 0x165315be, /// 0x1c8
			 0xa67c3c02, /// 0x1cc
			 0x63e73184, /// 0x1d0
			 0xd7862012, /// 0x1d4
			 0x26659cb6, /// 0x1d8
			 0x79e54f3a, /// 0x1dc
			 0x2bb60d69, /// 0x1e0
			 0x069aaca8, /// 0x1e4
			 0xe6e6f89e, /// 0x1e8
			 0x6e5f412d, /// 0x1ec
			 0xa3ea04cc, /// 0x1f0
			 0xbf89b3c1, /// 0x1f4
			 0xe30f3e9b, /// 0x1f8
			 0xf9fcaf4c, /// 0x1fc
			 0xf1aab6b6, /// 0x200
			 0x033dfe6c, /// 0x204
			 0x818afe7f, /// 0x208
			 0x9d0d2b72, /// 0x20c
			 0xa313180f, /// 0x210
			 0x8eb34c76, /// 0x214
			 0x0d9aa9d4, /// 0x218
			 0xdb515a37, /// 0x21c
			 0x2904fc39, /// 0x220
			 0x2303b07d, /// 0x224
			 0xc961214c, /// 0x228
			 0x532bda38, /// 0x22c
			 0x43bb5340, /// 0x230
			 0x97803534, /// 0x234
			 0xf2b2b8c5, /// 0x238
			 0x2d496880, /// 0x23c
			 0x512b31af, /// 0x240
			 0xe1db7074, /// 0x244
			 0x907d4224, /// 0x248
			 0x47fa7270, /// 0x24c
			 0x1cfb9bc6, /// 0x250
			 0x75831f07, /// 0x254
			 0x42ba59f1, /// 0x258
			 0x6b349913, /// 0x25c
			 0xacfc02da, /// 0x260
			 0xf3cdaed4, /// 0x264
			 0x5dd8abee, /// 0x268
			 0x660849bb, /// 0x26c
			 0x9895708e, /// 0x270
			 0x03f42197, /// 0x274
			 0x924e0f9d, /// 0x278
			 0x9e0c660e, /// 0x27c
			 0x21f3fe3d, /// 0x280
			 0x9a8ec10a, /// 0x284
			 0x2c2efdf8, /// 0x288
			 0x6fc33478, /// 0x28c
			 0xc90a5db5, /// 0x290
			 0x4c799654, /// 0x294
			 0xca4b46d3, /// 0x298
			 0x61e93fe5, /// 0x29c
			 0xe7ab6e3e, /// 0x2a0
			 0xb8c400eb, /// 0x2a4
			 0xfe83d68e, /// 0x2a8
			 0xea068cfc, /// 0x2ac
			 0x9fee757d, /// 0x2b0
			 0x0cd2f6f9, /// 0x2b4
			 0x09915a16, /// 0x2b8
			 0x12103264, /// 0x2bc
			 0x14121098, /// 0x2c0
			 0x7b0d57f6, /// 0x2c4
			 0xb82394a1, /// 0x2c8
			 0x89c02a7b, /// 0x2cc
			 0xfc67a778, /// 0x2d0
			 0x64ed3c6b, /// 0x2d4
			 0xaa1fd2af, /// 0x2d8
			 0x9ae42cb6, /// 0x2dc
			 0x5df62abb, /// 0x2e0
			 0x8f9331e8, /// 0x2e4
			 0xa0dcccf5, /// 0x2e8
			 0x2b3f66c0, /// 0x2ec
			 0x2b304272, /// 0x2f0
			 0x60b2b463, /// 0x2f4
			 0x35b55d0d, /// 0x2f8
			 0x475fdb07, /// 0x2fc
			 0x92af66b0, /// 0x300
			 0x5986791b, /// 0x304
			 0xd8397115, /// 0x308
			 0x5614c200, /// 0x30c
			 0x8fcdca85, /// 0x310
			 0x3557f582, /// 0x314
			 0x279e2751, /// 0x318
			 0xeb3d9525, /// 0x31c
			 0xf35c3083, /// 0x320
			 0x573dd898, /// 0x324
			 0x2894ace0, /// 0x328
			 0xb6e96632, /// 0x32c
			 0x9e50ae52, /// 0x330
			 0xadf99f53, /// 0x334
			 0x5866a3ed, /// 0x338
			 0x9044f76c, /// 0x33c
			 0x7067e3e8, /// 0x340
			 0x02692ca9, /// 0x344
			 0x0af1e6c4, /// 0x348
			 0xdabd353e, /// 0x34c
			 0x54b4ff93, /// 0x350
			 0xb0343c3c, /// 0x354
			 0x260eb15d, /// 0x358
			 0xd247ee59, /// 0x35c
			 0xfba589e0, /// 0x360
			 0x5c955b2e, /// 0x364
			 0x1ea47adf, /// 0x368
			 0x25ed5274, /// 0x36c
			 0x9e91e4fe, /// 0x370
			 0x67a3359d, /// 0x374
			 0xc56262de, /// 0x378
			 0xa4fcecd2, /// 0x37c
			 0xe9e44137, /// 0x380
			 0x33ecefb9, /// 0x384
			 0x75a930d6, /// 0x388
			 0x31713a4b, /// 0x38c
			 0x6987650d, /// 0x390
			 0x6a28d3a2, /// 0x394
			 0x26fd489b, /// 0x398
			 0x8fe0301c, /// 0x39c
			 0xbdd34193, /// 0x3a0
			 0xd27ae677, /// 0x3a4
			 0xf57dbc5f, /// 0x3a8
			 0xe8df14b7, /// 0x3ac
			 0x7e194d67, /// 0x3b0
			 0xcb691cbe, /// 0x3b4
			 0x041fda08, /// 0x3b8
			 0x80c55031, /// 0x3bc
			 0xe1610e35, /// 0x3c0
			 0x27a3ff78, /// 0x3c4
			 0x0a4dc32b, /// 0x3c8
			 0xb1510731, /// 0x3cc
			 0x66499fc2, /// 0x3d0
			 0xd3cb495b, /// 0x3d4
			 0x1e4916c4, /// 0x3d8
			 0xc723e6c7, /// 0x3dc
			 0x5839fc58, /// 0x3e0
			 0x819bbe63, /// 0x3e4
			 0x0362cdfc, /// 0x3e8
			 0xb5903f89, /// 0x3ec
			 0xb6f059c8, /// 0x3f0
			 0x4a3cd6af, /// 0x3f4
			 0xc4c7ce0a, /// 0x3f8
			 0x107dfc52, /// 0x3fc
			 0x5acbf590, /// 0x400
			 0xc1fff79d, /// 0x404
			 0x597335ea, /// 0x408
			 0x27519e2b, /// 0x40c
			 0x4457b956, /// 0x410
			 0x040123c7, /// 0x414
			 0x43fad30c, /// 0x418
			 0xca4d7eda, /// 0x41c
			 0x9afdb18b, /// 0x420
			 0x27447f2f, /// 0x424
			 0x15ceb89c, /// 0x428
			 0x8dd1e6a4, /// 0x42c
			 0xaf277ebf, /// 0x430
			 0xfe75caf3, /// 0x434
			 0x6e4a9452, /// 0x438
			 0x00fb92e0, /// 0x43c
			 0x0f2d8441, /// 0x440
			 0x865f3715, /// 0x444
			 0x4bef3d01, /// 0x448
			 0x33e3d04f, /// 0x44c
			 0xad9b08d6, /// 0x450
			 0x355f70b3, /// 0x454
			 0xa5d2ed92, /// 0x458
			 0xe9e30acb, /// 0x45c
			 0xc1f1c5c9, /// 0x460
			 0x49a70905, /// 0x464
			 0x6761c5d5, /// 0x468
			 0x64e8985e, /// 0x46c
			 0xf38861c0, /// 0x470
			 0xf105b344, /// 0x474
			 0x1f47f287, /// 0x478
			 0xd82a82dc, /// 0x47c
			 0xbb6c883e, /// 0x480
			 0xecad7323, /// 0x484
			 0xb43383b8, /// 0x488
			 0xede98bf4, /// 0x48c
			 0xe0e9c8bd, /// 0x490
			 0xd4b65618, /// 0x494
			 0x4322accb, /// 0x498
			 0xf7290ba2, /// 0x49c
			 0xb47f93b8, /// 0x4a0
			 0xbbf1daaf, /// 0x4a4
			 0x5ddc69c9, /// 0x4a8
			 0xee81aa54, /// 0x4ac
			 0x59462418, /// 0x4b0
			 0xa8201165, /// 0x4b4
			 0x266e3889, /// 0x4b8
			 0xf2e38752, /// 0x4bc
			 0x672c148a, /// 0x4c0
			 0x3c0fb241, /// 0x4c4
			 0x90e74dcb, /// 0x4c8
			 0x5848437a, /// 0x4cc
			 0x6db0f88e, /// 0x4d0
			 0xd3092aad, /// 0x4d4
			 0xf3653f6c, /// 0x4d8
			 0x1bac386b, /// 0x4dc
			 0x073237a0, /// 0x4e0
			 0x1a60a794, /// 0x4e4
			 0xcdc7dfc6, /// 0x4e8
			 0x388300c8, /// 0x4ec
			 0x38cd615c, /// 0x4f0
			 0x9009b118, /// 0x4f4
			 0x9e4f8656, /// 0x4f8
			 0x3b5687fd, /// 0x4fc
			 0xbc714f13, /// 0x500
			 0x3cc37610, /// 0x504
			 0xb5c2e16c, /// 0x508
			 0x9d787771, /// 0x50c
			 0x09d0eb7a, /// 0x510
			 0x4e1777cf, /// 0x514
			 0xd332ba1f, /// 0x518
			 0x2aae9ca3, /// 0x51c
			 0xac193385, /// 0x520
			 0xe683b4b9, /// 0x524
			 0x0766c20e, /// 0x528
			 0xc8a78f27, /// 0x52c
			 0x32bc8452, /// 0x530
			 0xd3385bad, /// 0x534
			 0x425cd15d, /// 0x538
			 0x3cf5782d, /// 0x53c
			 0xc923e018, /// 0x540
			 0x8795fdbe, /// 0x544
			 0x585a6901, /// 0x548
			 0x918960d5, /// 0x54c
			 0x88a0132f, /// 0x550
			 0xddb3e6b4, /// 0x554
			 0xa7657feb, /// 0x558
			 0x6214a199, /// 0x55c
			 0x35d7a7a7, /// 0x560
			 0xdcdcb8fa, /// 0x564
			 0x81d64c21, /// 0x568
			 0xbe0f9ed9, /// 0x56c
			 0x744b69c6, /// 0x570
			 0xfe5aa554, /// 0x574
			 0xd1c8b9e6, /// 0x578
			 0x1b9f8d03, /// 0x57c
			 0x4decbf49, /// 0x580
			 0x00c919c7, /// 0x584
			 0xdd54319e, /// 0x588
			 0x5f6d71fc, /// 0x58c
			 0x3947dac4, /// 0x590
			 0xe1eadd83, /// 0x594
			 0x7c18b6d7, /// 0x598
			 0x5b3513c6, /// 0x59c
			 0x02f625d0, /// 0x5a0
			 0x6883247a, /// 0x5a4
			 0xfa9ddf9a, /// 0x5a8
			 0x8981f00c, /// 0x5ac
			 0xb386b64d, /// 0x5b0
			 0x77c5e4fe, /// 0x5b4
			 0xb512ddf1, /// 0x5b8
			 0x16afe7a4, /// 0x5bc
			 0xa773da11, /// 0x5c0
			 0xc3beefdd, /// 0x5c4
			 0x3394ea7a, /// 0x5c8
			 0x4cfb5f16, /// 0x5cc
			 0xdd33604c, /// 0x5d0
			 0xc925bdc2, /// 0x5d4
			 0x20909647, /// 0x5d8
			 0x62dd371a, /// 0x5dc
			 0x8f2b380c, /// 0x5e0
			 0xe252ea91, /// 0x5e4
			 0xa3c6130c, /// 0x5e8
			 0xca4abaa6, /// 0x5ec
			 0x9f8f5ad3, /// 0x5f0
			 0x58d94156, /// 0x5f4
			 0x9afe94cc, /// 0x5f8
			 0x1b1a4e69, /// 0x5fc
			 0x3b17ade2, /// 0x600
			 0xa58e8fc3, /// 0x604
			 0x3ec71de2, /// 0x608
			 0xca77d5ac, /// 0x60c
			 0x7a98fd8b, /// 0x610
			 0x041f21ab, /// 0x614
			 0x6a1af534, /// 0x618
			 0xd37313aa, /// 0x61c
			 0x59043b41, /// 0x620
			 0xc76b6eb4, /// 0x624
			 0x08e662f9, /// 0x628
			 0xa094e712, /// 0x62c
			 0xee5c74ad, /// 0x630
			 0x4bef1348, /// 0x634
			 0xf6ebe783, /// 0x638
			 0x1e0bdd1f, /// 0x63c
			 0x3f2c1402, /// 0x640
			 0xf695509b, /// 0x644
			 0x681b7360, /// 0x648
			 0xe034cbbe, /// 0x64c
			 0xd06699f8, /// 0x650
			 0x5a110def, /// 0x654
			 0x25fd5982, /// 0x658
			 0x4c66e208, /// 0x65c
			 0xbd22c525, /// 0x660
			 0x8c2a0db3, /// 0x664
			 0xdea85753, /// 0x668
			 0x69610b6b, /// 0x66c
			 0xa1a5f770, /// 0x670
			 0x674f62c5, /// 0x674
			 0x35593400, /// 0x678
			 0x80d0269c, /// 0x67c
			 0xe0077c84, /// 0x680
			 0x335102ec, /// 0x684
			 0xa9e301c4, /// 0x688
			 0xfb167706, /// 0x68c
			 0x8376ab04, /// 0x690
			 0x3a892c0d, /// 0x694
			 0xcdc0b149, /// 0x698
			 0x423e4e9f, /// 0x69c
			 0x8aee95c2, /// 0x6a0
			 0x716de695, /// 0x6a4
			 0x6863c9b0, /// 0x6a8
			 0x62f7b534, /// 0x6ac
			 0x407689ab, /// 0x6b0
			 0x01ade2c5, /// 0x6b4
			 0xbe7cc9d2, /// 0x6b8
			 0x184ce990, /// 0x6bc
			 0x2ef3fe3b, /// 0x6c0
			 0x4491e462, /// 0x6c4
			 0xe77a2135, /// 0x6c8
			 0x9dea2d5d, /// 0x6cc
			 0x2d846f2a, /// 0x6d0
			 0xda1a6db2, /// 0x6d4
			 0xbf93f3c1, /// 0x6d8
			 0x64c401bb, /// 0x6dc
			 0xa66dceba, /// 0x6e0
			 0x5360be36, /// 0x6e4
			 0xc3ed83a3, /// 0x6e8
			 0x5db928a8, /// 0x6ec
			 0x3e17bfed, /// 0x6f0
			 0x801c8c8e, /// 0x6f4
			 0x5758dad1, /// 0x6f8
			 0x51eb70ea, /// 0x6fc
			 0x27c001d9, /// 0x700
			 0x1e60c3d5, /// 0x704
			 0xafa260d2, /// 0x708
			 0xdee94853, /// 0x70c
			 0x586d75f1, /// 0x710
			 0x4fccc40f, /// 0x714
			 0x6f85e9aa, /// 0x718
			 0xd030470b, /// 0x71c
			 0xef6fe1f5, /// 0x720
			 0xce6b2797, /// 0x724
			 0x2b4967fc, /// 0x728
			 0x3fb0c69f, /// 0x72c
			 0xc8f91cb7, /// 0x730
			 0xcb7f90e4, /// 0x734
			 0x4eb8d699, /// 0x738
			 0xe1136b8f, /// 0x73c
			 0xa6eeeaed, /// 0x740
			 0xa6a77df5, /// 0x744
			 0x88630f74, /// 0x748
			 0x070ca2b9, /// 0x74c
			 0x96495f32, /// 0x750
			 0xb34be2c8, /// 0x754
			 0xe3ee7410, /// 0x758
			 0x8db7959e, /// 0x75c
			 0x440670d6, /// 0x760
			 0x4331b332, /// 0x764
			 0xf68f4996, /// 0x768
			 0xb77b0417, /// 0x76c
			 0x450e4503, /// 0x770
			 0xb48b2466, /// 0x774
			 0x8943324b, /// 0x778
			 0x068716b5, /// 0x77c
			 0x6fcf9e79, /// 0x780
			 0xe6eaa7fd, /// 0x784
			 0xe7d10f05, /// 0x788
			 0x6675c910, /// 0x78c
			 0x94f0f895, /// 0x790
			 0x1d7b2bbe, /// 0x794
			 0xca94c1e1, /// 0x798
			 0x198d8d4a, /// 0x79c
			 0x8471854c, /// 0x7a0
			 0x185c5cbd, /// 0x7a4
			 0xb5dd991d, /// 0x7a8
			 0x4e05c59a, /// 0x7ac
			 0x89eac5a6, /// 0x7b0
			 0xf5ca5a84, /// 0x7b4
			 0xf6816913, /// 0x7b8
			 0xe46fbe31, /// 0x7bc
			 0xa72c79f1, /// 0x7c0
			 0xcd30dd2f, /// 0x7c4
			 0x2bb72a08, /// 0x7c8
			 0xc56cd1aa, /// 0x7cc
			 0xf6c95e30, /// 0x7d0
			 0xcd0942b4, /// 0x7d4
			 0x8eeaaf7d, /// 0x7d8
			 0x7e7b446f, /// 0x7dc
			 0x8d5670d8, /// 0x7e0
			 0xbe1597b3, /// 0x7e4
			 0x64240c87, /// 0x7e8
			 0x53fa0ea2, /// 0x7ec
			 0x82a3a3b8, /// 0x7f0
			 0x40eb8fab, /// 0x7f4
			 0x5ab9a4a7, /// 0x7f8
			 0xaa80a0df, /// 0x7fc
			 0x2bf1e069, /// 0x800
			 0x10452fb0, /// 0x804
			 0x86b6d833, /// 0x808
			 0x9a601ec3, /// 0x80c
			 0x701add1d, /// 0x810
			 0x726da79d, /// 0x814
			 0x713df899, /// 0x818
			 0xa69c46b2, /// 0x81c
			 0xa247a011, /// 0x820
			 0xd23ae0f5, /// 0x824
			 0x27a051a3, /// 0x828
			 0xd1cb7935, /// 0x82c
			 0x38950bb1, /// 0x830
			 0x9f935839, /// 0x834
			 0x042325b7, /// 0x838
			 0x112c0cde, /// 0x83c
			 0x854f95bc, /// 0x840
			 0xe0b3f971, /// 0x844
			 0x4f2912e7, /// 0x848
			 0xf2ece861, /// 0x84c
			 0x1ccd5b59, /// 0x850
			 0xdc7b592a, /// 0x854
			 0xb75f791e, /// 0x858
			 0x5319c085, /// 0x85c
			 0xce6d2eb8, /// 0x860
			 0xf692386e, /// 0x864
			 0x427a49d7, /// 0x868
			 0xb1390481, /// 0x86c
			 0xdd1d353e, /// 0x870
			 0x9c2db85c, /// 0x874
			 0x3f272eb7, /// 0x878
			 0x5b235c8f, /// 0x87c
			 0x5cd39535, /// 0x880
			 0x9648fa1b, /// 0x884
			 0x7c846ec4, /// 0x888
			 0xdec59d51, /// 0x88c
			 0x98ade2d1, /// 0x890
			 0xf7e4797b, /// 0x894
			 0xc394cb5f, /// 0x898
			 0xd3a30387, /// 0x89c
			 0xbe08c920, /// 0x8a0
			 0x4172400a, /// 0x8a4
			 0xf19c153d, /// 0x8a8
			 0xd51a750a, /// 0x8ac
			 0xa6d5c2a0, /// 0x8b0
			 0xd8471d9e, /// 0x8b4
			 0xdc84a574, /// 0x8b8
			 0xf9030c26, /// 0x8bc
			 0x13c89ba3, /// 0x8c0
			 0xe3fd0546, /// 0x8c4
			 0x2ceb33e3, /// 0x8c8
			 0x55cc9b1c, /// 0x8cc
			 0xf6beea86, /// 0x8d0
			 0xe6b678dd, /// 0x8d4
			 0x1cb12f44, /// 0x8d8
			 0xd5b5e281, /// 0x8dc
			 0x27f1961e, /// 0x8e0
			 0x758fddc0, /// 0x8e4
			 0xa7ea720d, /// 0x8e8
			 0xf9259362, /// 0x8ec
			 0x7192dbe2, /// 0x8f0
			 0x95dcafa9, /// 0x8f4
			 0xc511d869, /// 0x8f8
			 0xaf47a245, /// 0x8fc
			 0x4d95763a, /// 0x900
			 0xa07b6716, /// 0x904
			 0xc1e1aa2c, /// 0x908
			 0xfae38e91, /// 0x90c
			 0x2846ad2d, /// 0x910
			 0x3e3c2e9f, /// 0x914
			 0x363c1215, /// 0x918
			 0x6cee8ca4, /// 0x91c
			 0xfd6775a7, /// 0x920
			 0xd422cae7, /// 0x924
			 0x2155acd2, /// 0x928
			 0x56524a27, /// 0x92c
			 0xb84f6feb, /// 0x930
			 0x1657dabc, /// 0x934
			 0x21a13a57, /// 0x938
			 0x06735238, /// 0x93c
			 0xee6f3603, /// 0x940
			 0xf3701218, /// 0x944
			 0xe88eecb9, /// 0x948
			 0xabb35130, /// 0x94c
			 0xb2087592, /// 0x950
			 0x6fec65f1, /// 0x954
			 0x09bbb1d0, /// 0x958
			 0xd66aff9f, /// 0x95c
			 0xfabb4d0a, /// 0x960
			 0x233895f4, /// 0x964
			 0x55ab4da2, /// 0x968
			 0xf490098a, /// 0x96c
			 0x81f93b70, /// 0x970
			 0x34cbe617, /// 0x974
			 0xdd05c0d4, /// 0x978
			 0x304c8cc6, /// 0x97c
			 0x79c739be, /// 0x980
			 0xce65d78d, /// 0x984
			 0xe9523054, /// 0x988
			 0x0de7bcf3, /// 0x98c
			 0x586e662a, /// 0x990
			 0xe63c9306, /// 0x994
			 0xd4fbdd83, /// 0x998
			 0xd38bf269, /// 0x99c
			 0xe38150e6, /// 0x9a0
			 0xa017dc18, /// 0x9a4
			 0x09a5c5f7, /// 0x9a8
			 0x82572ad4, /// 0x9ac
			 0xd2ab0932, /// 0x9b0
			 0xb991263c, /// 0x9b4
			 0x6e0f7fd5, /// 0x9b8
			 0x743be380, /// 0x9bc
			 0x2e3e976f, /// 0x9c0
			 0x4c5614ea, /// 0x9c4
			 0x5997ac24, /// 0x9c8
			 0xef9e3197, /// 0x9cc
			 0xf9c9e1b4, /// 0x9d0
			 0x48bc4b08, /// 0x9d4
			 0xb8df384d, /// 0x9d8
			 0x274d2447, /// 0x9dc
			 0xf668b232, /// 0x9e0
			 0xe1a944eb, /// 0x9e4
			 0xedcd1e08, /// 0x9e8
			 0x0d850449, /// 0x9ec
			 0x8dc5a7ac, /// 0x9f0
			 0xe220e2d6, /// 0x9f4
			 0x77710653, /// 0x9f8
			 0x71a1dca3, /// 0x9fc
			 0x9963d92d, /// 0xa00
			 0x59be39f9, /// 0xa04
			 0x5268bc42, /// 0xa08
			 0x39b671da, /// 0xa0c
			 0x86bb6f25, /// 0xa10
			 0x53b959a2, /// 0xa14
			 0xab266343, /// 0xa18
			 0xe293bf71, /// 0xa1c
			 0xbfbd07f6, /// 0xa20
			 0x1c5c7076, /// 0xa24
			 0xa6d247d3, /// 0xa28
			 0xb6f44a7b, /// 0xa2c
			 0xf384b7a5, /// 0xa30
			 0x983c6290, /// 0xa34
			 0x4da77f7b, /// 0xa38
			 0x235eb17d, /// 0xa3c
			 0x399a29e7, /// 0xa40
			 0xa9e2fb4a, /// 0xa44
			 0x0911adff, /// 0xa48
			 0x43c5541a, /// 0xa4c
			 0xa490a373, /// 0xa50
			 0xf15520ef, /// 0xa54
			 0x85627007, /// 0xa58
			 0x6f605113, /// 0xa5c
			 0xb8060dc9, /// 0xa60
			 0xe8794d3c, /// 0xa64
			 0x058a89e1, /// 0xa68
			 0x5f9050a4, /// 0xa6c
			 0xbd29b4d9, /// 0xa70
			 0x9c6b0f8a, /// 0xa74
			 0xfadd8bfd, /// 0xa78
			 0x3927c32a, /// 0xa7c
			 0x7c2d6940, /// 0xa80
			 0xdd180317, /// 0xa84
			 0x5b053b3a, /// 0xa88
			 0xc8d93419, /// 0xa8c
			 0xeba2f206, /// 0xa90
			 0xf573ae99, /// 0xa94
			 0x75a9a970, /// 0xa98
			 0x0dc73081, /// 0xa9c
			 0x9e5a5b01, /// 0xaa0
			 0xe6883965, /// 0xaa4
			 0x62659be3, /// 0xaa8
			 0xd99bbff3, /// 0xaac
			 0x39d0485c, /// 0xab0
			 0x012ca4aa, /// 0xab4
			 0x4f998dac, /// 0xab8
			 0x2b747d5d, /// 0xabc
			 0x33ddf34b, /// 0xac0
			 0x910c0468, /// 0xac4
			 0x569890a2, /// 0xac8
			 0x210ea4f6, /// 0xacc
			 0x873a6e9b, /// 0xad0
			 0xf140bdf7, /// 0xad4
			 0xe3935e71, /// 0xad8
			 0x33707af0, /// 0xadc
			 0xd10ebf84, /// 0xae0
			 0x4c8ed821, /// 0xae4
			 0x6c05dab2, /// 0xae8
			 0x0c0db0ff, /// 0xaec
			 0x75c9964b, /// 0xaf0
			 0x20190f25, /// 0xaf4
			 0x21e44b09, /// 0xaf8
			 0x56207975, /// 0xafc
			 0xcb120dfc, /// 0xb00
			 0x75c17d31, /// 0xb04
			 0x5af03d5f, /// 0xb08
			 0xc571e2e2, /// 0xb0c
			 0x354a4d77, /// 0xb10
			 0xa9576326, /// 0xb14
			 0x4c24116e, /// 0xb18
			 0x141985dc, /// 0xb1c
			 0xe079f2a7, /// 0xb20
			 0xe214cffc, /// 0xb24
			 0x0cf1a6fb, /// 0xb28
			 0x1f90ec0d, /// 0xb2c
			 0xee1ba003, /// 0xb30
			 0x898e8cee, /// 0xb34
			 0x97ea4064, /// 0xb38
			 0x1b4be6bd, /// 0xb3c
			 0x61f4a721, /// 0xb40
			 0x3e23a708, /// 0xb44
			 0x7653eb8c, /// 0xb48
			 0x315d6789, /// 0xb4c
			 0x25c5f0be, /// 0xb50
			 0x5a8e4f82, /// 0xb54
			 0x6ed5cf10, /// 0xb58
			 0xcb88b311, /// 0xb5c
			 0x00c97c01, /// 0xb60
			 0x5e1c1a40, /// 0xb64
			 0x1d1dfa64, /// 0xb68
			 0x29f61394, /// 0xb6c
			 0x32913ecb, /// 0xb70
			 0x2e1eb18a, /// 0xb74
			 0x93fa1de1, /// 0xb78
			 0xc40056fd, /// 0xb7c
			 0x6ea13bde, /// 0xb80
			 0x47bbe394, /// 0xb84
			 0x409c0b16, /// 0xb88
			 0x594f7e1b, /// 0xb8c
			 0x0c96b73f, /// 0xb90
			 0x2112c78e, /// 0xb94
			 0x7c45ca14, /// 0xb98
			 0x6b0b1d08, /// 0xb9c
			 0xe5bba506, /// 0xba0
			 0x42b1306c, /// 0xba4
			 0xf7c281cd, /// 0xba8
			 0xdad784af, /// 0xbac
			 0x1bbd377e, /// 0xbb0
			 0x207355dd, /// 0xbb4
			 0xe06cd03d, /// 0xbb8
			 0x15ff0585, /// 0xbbc
			 0xbd3e13e1, /// 0xbc0
			 0xc0fa1bd5, /// 0xbc4
			 0x7ca45a17, /// 0xbc8
			 0x1cb2900c, /// 0xbcc
			 0x61026f97, /// 0xbd0
			 0x15f11795, /// 0xbd4
			 0xb0cba1ba, /// 0xbd8
			 0x07858f2e, /// 0xbdc
			 0x6992d032, /// 0xbe0
			 0xf43aaadb, /// 0xbe4
			 0xcd86ad3d, /// 0xbe8
			 0x51b9583b, /// 0xbec
			 0xdcd7e6e4, /// 0xbf0
			 0x1b364365, /// 0xbf4
			 0xd8609d65, /// 0xbf8
			 0xcbe81e36, /// 0xbfc
			 0x495d3217, /// 0xc00
			 0xd5547203, /// 0xc04
			 0x471aa5dd, /// 0xc08
			 0xb402d972, /// 0xc0c
			 0xc0b63614, /// 0xc10
			 0x3a2b8fe9, /// 0xc14
			 0x10d2c477, /// 0xc18
			 0xbf9e864b, /// 0xc1c
			 0xb17dc75b, /// 0xc20
			 0xfd5f52f5, /// 0xc24
			 0x50487fe5, /// 0xc28
			 0x74b21f33, /// 0xc2c
			 0x31a2e665, /// 0xc30
			 0x58ff8339, /// 0xc34
			 0x4d6fbc15, /// 0xc38
			 0x1469596e, /// 0xc3c
			 0x052924c3, /// 0xc40
			 0x73179847, /// 0xc44
			 0x4301e8e2, /// 0xc48
			 0xb0b1e2dd, /// 0xc4c
			 0x8e7ece5e, /// 0xc50
			 0x830845e5, /// 0xc54
			 0x9200b8b9, /// 0xc58
			 0xe75fa3b4, /// 0xc5c
			 0x90855472, /// 0xc60
			 0x537dd823, /// 0xc64
			 0x56d8f892, /// 0xc68
			 0x7e7e1320, /// 0xc6c
			 0x5101a5c6, /// 0xc70
			 0x9af24496, /// 0xc74
			 0x8e579aa2, /// 0xc78
			 0xa0ba3492, /// 0xc7c
			 0xee621aed, /// 0xc80
			 0x0af31467, /// 0xc84
			 0x1f3aa263, /// 0xc88
			 0xc7658aa7, /// 0xc8c
			 0x026c6e95, /// 0xc90
			 0x09f0b317, /// 0xc94
			 0x3cb82fec, /// 0xc98
			 0x37cb2f1f, /// 0xc9c
			 0xc446dca2, /// 0xca0
			 0xdb8dea18, /// 0xca4
			 0x51282540, /// 0xca8
			 0x882a4d60, /// 0xcac
			 0x1d1701ea, /// 0xcb0
			 0x76602212, /// 0xcb4
			 0x0b616d9f, /// 0xcb8
			 0x3e121c7c, /// 0xcbc
			 0xba8bce81, /// 0xcc0
			 0xdda57209, /// 0xcc4
			 0x07933d8a, /// 0xcc8
			 0x62588483, /// 0xccc
			 0x34721400, /// 0xcd0
			 0x0494d468, /// 0xcd4
			 0x50b4ae59, /// 0xcd8
			 0x0a4565e1, /// 0xcdc
			 0x124bed43, /// 0xce0
			 0xb03f3260, /// 0xce4
			 0x7b02d208, /// 0xce8
			 0x5b058f62, /// 0xcec
			 0xd24fe345, /// 0xcf0
			 0xb04a9bb1, /// 0xcf4
			 0x8fa70e9c, /// 0xcf8
			 0x1e4f4c87, /// 0xcfc
			 0x9c17cf83, /// 0xd00
			 0x4be3bb6f, /// 0xd04
			 0xe2d1e941, /// 0xd08
			 0xc12f61f6, /// 0xd0c
			 0x2b6fd15b, /// 0xd10
			 0x60bb872b, /// 0xd14
			 0x1507419f, /// 0xd18
			 0x6e2be11c, /// 0xd1c
			 0x8e50cd40, /// 0xd20
			 0x650141f8, /// 0xd24
			 0x6bcf59a6, /// 0xd28
			 0x32e9d7a8, /// 0xd2c
			 0xddb6e5ca, /// 0xd30
			 0x59ab4cd9, /// 0xd34
			 0x6ca97fcf, /// 0xd38
			 0xb474220b, /// 0xd3c
			 0xfd422438, /// 0xd40
			 0xff02b4a1, /// 0xd44
			 0xd83996e2, /// 0xd48
			 0x33b544cc, /// 0xd4c
			 0x916aacd1, /// 0xd50
			 0x0ec7cb20, /// 0xd54
			 0x0a0869af, /// 0xd58
			 0xd5835e16, /// 0xd5c
			 0x55f40626, /// 0xd60
			 0xbc428b93, /// 0xd64
			 0xf3a97ad6, /// 0xd68
			 0x792b12c7, /// 0xd6c
			 0x40bb3bc9, /// 0xd70
			 0xa383a662, /// 0xd74
			 0xcf0a2736, /// 0xd78
			 0xe4819b41, /// 0xd7c
			 0xa8e0d5ae, /// 0xd80
			 0xf6b2be96, /// 0xd84
			 0xdd90946b, /// 0xd88
			 0xbe1da788, /// 0xd8c
			 0xdf1fb060, /// 0xd90
			 0x85747beb, /// 0xd94
			 0xfc5600af, /// 0xd98
			 0x9c8afc75, /// 0xd9c
			 0x62c80628, /// 0xda0
			 0x9fd34982, /// 0xda4
			 0x0c83fd27, /// 0xda8
			 0xcd2a8ad2, /// 0xdac
			 0x8890b682, /// 0xdb0
			 0xe7124ce9, /// 0xdb4
			 0x4e88cfd6, /// 0xdb8
			 0x98f165be, /// 0xdbc
			 0xe96709f8, /// 0xdc0
			 0x0fa6010a, /// 0xdc4
			 0xc7f68214, /// 0xdc8
			 0xe4ed6691, /// 0xdcc
			 0xb435d8f4, /// 0xdd0
			 0x88d44202, /// 0xdd4
			 0x0a7e6f27, /// 0xdd8
			 0x40928a92, /// 0xddc
			 0xa0f6dad0, /// 0xde0
			 0xf25c2607, /// 0xde4
			 0xc87001be, /// 0xde8
			 0xfd596fa5, /// 0xdec
			 0x57a2d261, /// 0xdf0
			 0x50122c3d, /// 0xdf4
			 0xa07d592d, /// 0xdf8
			 0x1dc42ce7, /// 0xdfc
			 0x1621a955, /// 0xe00
			 0x4213ebe4, /// 0xe04
			 0x92280b00, /// 0xe08
			 0xbcf1235b, /// 0xe0c
			 0x4dfbbb07, /// 0xe10
			 0x4ffc0df8, /// 0xe14
			 0xc97e16ed, /// 0xe18
			 0x6d961ecc, /// 0xe1c
			 0xb7262dd6, /// 0xe20
			 0x27d755ef, /// 0xe24
			 0x36649e16, /// 0xe28
			 0xf1f9d378, /// 0xe2c
			 0xdcd11ea5, /// 0xe30
			 0xae73501d, /// 0xe34
			 0x5c7b1769, /// 0xe38
			 0x262e0149, /// 0xe3c
			 0xc046819a, /// 0xe40
			 0x63099039, /// 0xe44
			 0xdf05038d, /// 0xe48
			 0x9f982972, /// 0xe4c
			 0x124a9a35, /// 0xe50
			 0xb92c1ea3, /// 0xe54
			 0x19ea3bb2, /// 0xe58
			 0xd574ebbd, /// 0xe5c
			 0x81e2af79, /// 0xe60
			 0x93d79f5c, /// 0xe64
			 0x4b452dfc, /// 0xe68
			 0x819edcb5, /// 0xe6c
			 0x7c906f4e, /// 0xe70
			 0xff5b5aa8, /// 0xe74
			 0x020e3e4f, /// 0xe78
			 0xbc722c39, /// 0xe7c
			 0xc10a94b2, /// 0xe80
			 0x67119b2c, /// 0xe84
			 0x2993dae4, /// 0xe88
			 0xa385ded0, /// 0xe8c
			 0x52e17fe7, /// 0xe90
			 0xa25b80d2, /// 0xe94
			 0x173d68dc, /// 0xe98
			 0x304b8393, /// 0xe9c
			 0x76942455, /// 0xea0
			 0xf3314ef6, /// 0xea4
			 0x2da62f96, /// 0xea8
			 0x735f8d89, /// 0xeac
			 0xa852a108, /// 0xeb0
			 0xc9a26659, /// 0xeb4
			 0x0624c0f5, /// 0xeb8
			 0xf5729c94, /// 0xebc
			 0x98f1f35c, /// 0xec0
			 0x3ad2aef2, /// 0xec4
			 0x43e9568e, /// 0xec8
			 0xf1e0d77b, /// 0xecc
			 0x37170dec, /// 0xed0
			 0xeb83db86, /// 0xed4
			 0x955da33d, /// 0xed8
			 0xa90f8f56, /// 0xedc
			 0x9446adb3, /// 0xee0
			 0x6a3350d0, /// 0xee4
			 0xf61794cf, /// 0xee8
			 0xdcea8c0f, /// 0xeec
			 0x940bb608, /// 0xef0
			 0xbe2e264c, /// 0xef4
			 0xedad0527, /// 0xef8
			 0x8ebbffb9, /// 0xefc
			 0xc7221f76, /// 0xf00
			 0xd9cea9cc, /// 0xf04
			 0x299e6cd0, /// 0xf08
			 0x151b7552, /// 0xf0c
			 0xf881c76d, /// 0xf10
			 0xa52bf351, /// 0xf14
			 0xacdf7c1b, /// 0xf18
			 0x222508ab, /// 0xf1c
			 0x7b5c3144, /// 0xf20
			 0x89ead268, /// 0xf24
			 0xfb946507, /// 0xf28
			 0x4a090b4e, /// 0xf2c
			 0x1506fc78, /// 0xf30
			 0x1feb92d5, /// 0xf34
			 0x17808c44, /// 0xf38
			 0x24f95bab, /// 0xf3c
			 0x7882ecc4, /// 0xf40
			 0x684916f4, /// 0xf44
			 0xe84cdc6a, /// 0xf48
			 0x3d9d5fb6, /// 0xf4c
			 0x6d169fa9, /// 0xf50
			 0x1dfc7a13, /// 0xf54
			 0xedbf73fd, /// 0xf58
			 0x7f0f5e99, /// 0xf5c
			 0xe44f9068, /// 0xf60
			 0x02e0fbd6, /// 0xf64
			 0xbdf39e55, /// 0xf68
			 0xf099c40c, /// 0xf6c
			 0x4444b365, /// 0xf70
			 0x1f011c25, /// 0xf74
			 0xa87b11b9, /// 0xf78
			 0x0da3cf92, /// 0xf7c
			 0x4223e826, /// 0xf80
			 0x42cad01c, /// 0xf84
			 0x6d3831a9, /// 0xf88
			 0x2509be1a, /// 0xf8c
			 0xc1f92f9d, /// 0xf90
			 0xe66a2e11, /// 0xf94
			 0x5e6841a6, /// 0xf98
			 0xc17d37ef, /// 0xf9c
			 0x2eb38b39, /// 0xfa0
			 0x33f10e27, /// 0xfa4
			 0xdd689c60, /// 0xfa8
			 0x3910f265, /// 0xfac
			 0xc72a7e7c, /// 0xfb0
			 0x03978e05, /// 0xfb4
			 0x9a0f2e05, /// 0xfb8
			 0xbfde10d4, /// 0xfbc
			 0x89cbdf0b, /// 0xfc0
			 0x3ad32b2d, /// 0xfc4
			 0x040c6b6a, /// 0xfc8
			 0xe953c560, /// 0xfcc
			 0x000882f3, /// 0xfd0
			 0x00d4ddf8, /// 0xfd4
			 0x4bfe3e82, /// 0xfd8
			 0x8954a208, /// 0xfdc
			 0x2233a3ca, /// 0xfe0
			 0xa7adc44d, /// 0xfe4
			 0x23c835f8, /// 0xfe8
			 0xf201d347, /// 0xfec
			 0x3af8c58f, /// 0xff0
			 0xe91c9ed4, /// 0xff4
			 0x3ea275d9, /// 0xff8
			 0x6c34e795 /// last
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
			 0x00000688,
			 0x00000628,
			 0x00000378,
			 0x0000030c,
			 0x00000394,
			 0x000005e4,
			 0x00000540,
			 0x00000280,

			 /// vpu register f2
			 0x41c00000,
			 0x41400000,
			 0x40000000,
			 0x41b00000,
			 0x41f00000,
			 0x41700000,
			 0x40e00000,
			 0x41b00000,

			 /// vpu register f3
			 0x41400000,
			 0x41d80000,
			 0x41d00000,
			 0x40a00000,
			 0x41b80000,
			 0x41980000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f4
			 0x41800000,
			 0x41f00000,
			 0x41e80000,
			 0x41c00000,
			 0x41e00000,
			 0x41c80000,
			 0x41800000,
			 0x40c00000,

			 /// vpu register f5
			 0x41e80000,
			 0x41d80000,
			 0x41800000,
			 0x41600000,
			 0x41600000,
			 0x41b80000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f6
			 0x40c00000,
			 0x41e80000,
			 0x41c00000,
			 0x40c00000,
			 0x41e80000,
			 0x41e80000,
			 0x40c00000,
			 0x40c00000,

			 /// vpu register f7
			 0x41e00000,
			 0x40800000,
			 0x41e80000,
			 0x41300000,
			 0x41a00000,
			 0x41c80000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f8
			 0x41e80000,
			 0x41f80000,
			 0x41100000,
			 0x41b00000,
			 0x41400000,
			 0x41200000,
			 0x42000000,
			 0x3f800000,

			 /// vpu register f9
			 0x40800000,
			 0x40000000,
			 0x40400000,
			 0x41400000,
			 0x41200000,
			 0x41d80000,
			 0x41b80000,
			 0x40a00000,

			 /// vpu register f10
			 0x41f00000,
			 0x41600000,
			 0x41700000,
			 0x40a00000,
			 0x41d80000,
			 0x41700000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f11
			 0x41800000,
			 0x40800000,
			 0x41e80000,
			 0x41500000,
			 0x41b00000,
			 0x41e00000,
			 0x40800000,
			 0x41b80000,

			 /// vpu register f12
			 0x41a80000,
			 0x41000000,
			 0x41f80000,
			 0x41200000,
			 0x41d00000,
			 0x41900000,
			 0x41500000,
			 0x41900000,

			 /// vpu register f13
			 0x41d00000,
			 0x40e00000,
			 0x41000000,
			 0x41800000,
			 0x41980000,
			 0x41e00000,
			 0x41800000,
			 0x41300000,

			 /// vpu register f14
			 0x41f80000,
			 0x41500000,
			 0x41500000,
			 0x40a00000,
			 0x40400000,
			 0x41300000,
			 0x42000000,
			 0x41400000,

			 /// vpu register f15
			 0x40a00000,
			 0x3f800000,
			 0x40000000,
			 0x41f00000,
			 0x40000000,
			 0x3f800000,
			 0x41980000,
			 0x41b00000,

			 /// vpu register f16
			 0x40a00000,
			 0x41500000,
			 0x41800000,
			 0x40000000,
			 0x41300000,
			 0x41980000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f17
			 0x41f80000,
			 0x41a00000,
			 0x41d80000,
			 0x40e00000,
			 0x41000000,
			 0x41c00000,
			 0x41b80000,
			 0x41900000,

			 /// vpu register f18
			 0x3f800000,
			 0x41400000,
			 0x41f00000,
			 0x40e00000,
			 0x41400000,
			 0x41200000,
			 0x41880000,
			 0x41800000,

			 /// vpu register f19
			 0x41c00000,
			 0x42000000,
			 0x41700000,
			 0x40c00000,
			 0x41a00000,
			 0x41300000,
			 0x40e00000,
			 0x41c80000,

			 /// vpu register f20
			 0x41a80000,
			 0x41b80000,
			 0x41000000,
			 0x41c80000,
			 0x41f80000,
			 0x41200000,
			 0x41d00000,
			 0x40000000,

			 /// vpu register f21
			 0x41000000,
			 0x41a00000,
			 0x41900000,
			 0x41e00000,
			 0x41c00000,
			 0x41f00000,
			 0x41300000,
			 0x40c00000,

			 /// vpu register f22
			 0x41200000,
			 0x41000000,
			 0x41400000,
			 0x41b80000,
			 0x41600000,
			 0x41a80000,
			 0x41c80000,
			 0x41600000,

			 /// vpu register f23
			 0x41a00000,
			 0x41e80000,
			 0x41400000,
			 0x41200000,
			 0x42000000,
			 0x41f80000,
			 0x40000000,
			 0x41700000,

			 /// vpu register f24
			 0x41f00000,
			 0x41800000,
			 0x41300000,
			 0x41e00000,
			 0x41c80000,
			 0x40400000,
			 0x41400000,
			 0x41e80000,

			 /// vpu register f25
			 0x40800000,
			 0x41500000,
			 0x41a00000,
			 0x41880000,
			 0x41c00000,
			 0x40a00000,
			 0x40c00000,
			 0x40a00000,

			 /// vpu register f26
			 0x41880000,
			 0x40a00000,
			 0x41c00000,
			 0x41200000,
			 0x41a80000,
			 0x41f00000,
			 0x40000000,
			 0x3f800000,

			 /// vpu register f27
			 0x40000000,
			 0x41600000,
			 0x40a00000,
			 0x41200000,
			 0x41980000,
			 0x40800000,
			 0x41000000,
			 0x41a00000,

			 /// vpu register f28
			 0x41500000,
			 0x40a00000,
			 0x41000000,
			 0x41a00000,
			 0x41000000,
			 0x41a00000,
			 0x41100000,
			 0x40e00000,

			 /// vpu register f29
			 0x41200000,
			 0x41b80000,
			 0x41900000,
			 0x41800000,
			 0x40000000,
			 0x41100000,
			 0x40a00000,
			 0x41b80000,

			 /// vpu register f30
			 0x41700000,
			 0x41200000,
			 0x41f00000,
			 0x41c00000,
			 0x41300000,
			 0x41f80000,
			 0x41f80000,
			 0x41d80000,

			 /// vpu register f31
			 0x41f00000,
			 0x41880000,
			 0x40c00000,
			 0x41600000,
			 0x41d80000,
			 0x40400000,
			 0x41e00000,
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
		"fcmov.ps f16, f18, f11, f8\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f8, f5, f24, f0\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f17, f27, f15, f3\n"                        ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f2, f27, f22, f16\n"                        ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f23, f2, f24, f4\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f12, f24, f4, f7\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f4, f11, f13, f22\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f27, f25, f30, f25\n"                       ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f3, f24, f23, f5\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f11, f4, f5, f1\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f28, f11, f0, f18\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f14, f0, f20, f18\n"                        ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f30, f16, f12, f3\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f2, f25, f5, f18\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f26, f24, f26, f29\n"                       ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f11, f26, f15, f6\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f17, f10, f22, f7\n"                        ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f19, f0, f27, f28\n"                        ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f27, f22, f26, f10\n"                       ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f2, f2, f7, f0\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f29, f0, f29, f29\n"                        ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f10, f7, f26, f29\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f8, f9, f0, f8\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f3, f25, f5, f7\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f16, f16, f3, f0\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f18, f16, f24, f7\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f30, f18, f1, f18\n"                        ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f30, f15, f11, f22\n"                       ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f20, f27, f19, f25\n"                       ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f3, f22, f15, f14\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f29, f21, f30, f29\n"                       ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f26, f8, f1, f27\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f14, f4, f20, f6\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f12, f7, f2, f17\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f22, f29, f2, f6\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f22, f16, f12, f12\n"                       ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f5, f20, f5, f15\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f12, f25, f22, f4\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f18, f4, f30, f0\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f20, f2, f28, f10\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f10, f25, f17, f18\n"                       ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f27, f8, f7, f3\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f17, f24, f8, f3\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f10, f15, f14, f10\n"                       ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f5, f16, f15, f6\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f21, f6, f26, f22\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f23, f1, f7, f23\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f16, f11, f2, f7\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f9, f26, f6, f24\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f22, f7, f10, f23\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f3, f22, f23, f9\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f11, f30, f8, f1\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f28, f25, f8, f26\n"                        ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f17, f20, f9, f17\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f15, f21, f20, f27\n"                       ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f4, f1, f9, f8\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f22, f1, f24, f10\n"                        ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f15, f16, f10, f9\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f26, f11, f16, f22\n"                       ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f13, f26, f3, f2\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f4, f13, f23, f8\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f9, f12, f17, f28\n"                        ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f20, f27, f26, f19\n"                       ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f12, f10, f6, f30\n"                        ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f4, f9, f20, f1\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f30, f13, f3, f9\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f29, f23, f29, f7\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f5, f23, f11, f1\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f21, f10, f18, f0\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f29, f13, f9, f25\n"                        ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f12, f24, f5, f22\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f4, f0, f17, f13\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f13, f5, f14, f10\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f9, f15, f6, f28\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f5, f12, f30, f7\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f6, f10, f15, f26\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f6, f6, f30, f25\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f29, f10, f3, f19\n"                        ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f16, f19, f26, f6\n"                        ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f30, f17, f25, f18\n"                       ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f13, f27, f9, f24\n"                        ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f14, f26, f12, f24\n"                       ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f18, f18, f30, f18\n"                       ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f3, f18, f15, f27\n"                        ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f9, f29, f29, f10\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f17, f6, f23, f11\n"                        ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f24, f24, f12, f2\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f20, f9, f22, f5\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f28, f14, f12, f21\n"                       ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f5, f2, f25, f24\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f14, f24, f0, f7\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f2, f4, f20, f22\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f9, f5, f0, f6\n"                           ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f4, f9, f5, f10\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f25, f13, f1, f23\n"                        ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f15, f28, f13, f13\n"                       ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f3, f14, f21, f11\n"                        ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f9, f8, f6, f21\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmov.ps f22, f2, f22, f15\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
