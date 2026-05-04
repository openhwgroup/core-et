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
			 0x0c400000,                                                  // Leading 1s:                                 /// 00000
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00004
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00008
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0000c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00010
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00014
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00018
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0001c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0xffc00001,                                                  // -signaling NaN                              /// 00028
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0002c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7f800000,                                                  // inf                                         /// 00034
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00038
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0003c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00040
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00044
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0004c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00050
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00054
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00058
			 0x7f800000,                                                  // inf                                         /// 0005c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00060
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00064
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xffc00001,                                                  // -signaling NaN                              /// 0006c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00070
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00074
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00084
			 0x0e000007,                                                  // Trailing 1s:                                /// 00088
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0008c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00090
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00094
			 0xffc00001,                                                  // -signaling NaN                              /// 00098
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000dc
			 0xbf028f5c,                                                  // -0.51                                       /// 000e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f0
			 0x7f800000,                                                  // inf                                         /// 000f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00104
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00108
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0010c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00110
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00118
			 0x0c600000,                                                  // Leading 1s:                                 /// 0011c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x33333333,                                                  // 4 random values                             /// 00124
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0012c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x7f800000,                                                  // inf                                         /// 00138
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00140
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00148
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0014c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00154
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00158
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00160
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00168
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0016c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00170
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00174
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0017c
			 0xffc00001,                                                  // -signaling NaN                              /// 00180
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00184
			 0x4b000000,                                                  // 8388608.0                                   /// 00188
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0018c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00190
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00194
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00198
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0019c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00200
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00204
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00208
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x3f028f5c,                                                  // 0.51                                        /// 00214
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00218
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00220
			 0x3f028f5c,                                                  // 0.51                                        /// 00224
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00228
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0022c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00234
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00238
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00244
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00248
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00258
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00260
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00264
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00268
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00270
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00274
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00278
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00280
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00284
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00288
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0028c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00294
			 0x80000000,                                                  // -zero                                       /// 00298
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00300
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x3f028f5c,                                                  // 0.51                                        /// 00314
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00318
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0031c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00320
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00324
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00328
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00334
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00340
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00344
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00348
			 0xcb000000,                                                  // -8388608.0                                  /// 0034c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00350
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00354
			 0x7fc00001,                                                  // signaling NaN                               /// 00358
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0035c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00364
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e000007,                                                  // Trailing 1s:                                /// 0036c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00370
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00384
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x80011111,                                                  // -9.7958E-41                                 /// 0039c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x55555555,                                                  // 4 random values                             /// 003d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x4b000000,                                                  // 8388608.0                                   /// 003f0
			 0x33333333,                                                  // 4 random values                             /// 003f4
			 0x55555555,                                                  // 4 random values                             /// 003f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0040c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00410
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00414
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00418
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0041c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00424
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0042c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00430
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00434
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00444
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00450
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00458
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0045c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00468
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00470
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00474
			 0x7f800000,                                                  // inf                                         /// 00478
			 0xff800000,                                                  // -inf                                        /// 0047c
			 0xbf028f5c,                                                  // -0.51                                       /// 00480
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00484
			 0x00000000,                                                  // zero                                        /// 00488
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00490
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00498
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0049c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a0
			 0x7fc00001,                                                  // signaling NaN                               /// 004a4
			 0xbf028f5c,                                                  // -0.51                                       /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004b0
			 0x7f800000,                                                  // inf                                         /// 004b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00500
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00508
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0050c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00510
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00514
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00518
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00524
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00528
			 0x0e000007,                                                  // Trailing 1s:                                /// 0052c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00538
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00540
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00550
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00558
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0055c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00560
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00564
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00570
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00574
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00578
			 0xcb000000,                                                  // -8388608.0                                  /// 0057c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00580
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00584
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00590
			 0x0e000007,                                                  // Trailing 1s:                                /// 00594
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00598
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ec
			 0xff800000,                                                  // -inf                                        /// 005f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00608
			 0x00000000,                                                  // zero                                        /// 0060c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00610
			 0x0c700000,                                                  // Leading 1s:                                 /// 00614
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0061c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00620
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00628
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00634
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00638
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x80000000,                                                  // -zero                                       /// 00644
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00648
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0064c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00650
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00654
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00658
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00660
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00668
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00674
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0067c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00680
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00684
			 0x80000000,                                                  // -zero                                       /// 00688
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00698
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006dc
			 0xcb000000,                                                  // -8388608.0                                  /// 006e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00700
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00708
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0070c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00710
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00718
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00728
			 0x80000000,                                                  // -zero                                       /// 0072c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00738
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00740
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00748
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0074c
			 0xff800000,                                                  // -inf                                        /// 00750
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00754
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00758
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00760
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00768
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0076c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00770
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00774
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00778
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0077c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00784
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00788
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0078c
			 0x7f800000,                                                  // inf                                         /// 00790
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00794
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00798
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0079c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 007f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00808
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0080c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00810
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00814
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00818
			 0xcb000000,                                                  // -8388608.0                                  /// 0081c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00820
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00824
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00828
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0082c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00830
			 0x80000000,                                                  // -zero                                       /// 00834
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00838
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00840
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00848
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00864
			 0x3f028f5c,                                                  // 0.51                                        /// 00868
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00880
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00884
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00888
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0088c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00894
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00898
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008b4
			 0x00000000,                                                  // zero                                        /// 008b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0xcb000000,                                                  // -8388608.0                                  /// 008dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00900
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00908
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0090c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00011111,                                                  // 9.7958E-41                                  /// 00914
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00918
			 0xffc00001,                                                  // -signaling NaN                              /// 0091c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00928
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00930
			 0x80011111,                                                  // -9.7958E-41                                 /// 00934
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00938
			 0x00000000,                                                  // zero                                        /// 0093c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00940
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0094c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00950
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00954
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00958
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0095c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00960
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x4b000000,                                                  // 8388608.0                                   /// 00968
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0096c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00974
			 0x80000000,                                                  // -zero                                       /// 00978
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0097c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00980
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00988
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00998
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e8
			 0x4b000000,                                                  // 8388608.0                                   /// 009ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a70
			 0xbf028f5c,                                                  // -0.51                                       /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aa0
			 0x4b000000,                                                  // 8388608.0                                   /// 00aa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00aa8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab8
			 0x7f800000,                                                  // inf                                         /// 00abc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x55555555,                                                  // 4 random values                             /// 00ac8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00acc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ae0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x55555555,                                                  // 4 random values                             /// 00af8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00afc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x4b000000,                                                  // 8388608.0                                   /// 00b28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0xbf028f5c,                                                  // -0.51                                       /// 00b38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xbf028f5c,                                                  // -0.51                                       /// 00b88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ba0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bb8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bbc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00000000,                                                  // zero                                        /// 00bcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00be0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00be8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x55555555,                                                  // 4 random values                             /// 00c50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x4b000000,                                                  // 8388608.0                                   /// 00c68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ca0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cb0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ccc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cf4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xcb000000,                                                  // -8388608.0                                  /// 00d24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d64
			 0x4b000000,                                                  // 8388608.0                                   /// 00d68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0xbf028f5c,                                                  // -0.51                                       /// 00da4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ddc
			 0x7f800000,                                                  // inf                                         /// 00de0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00de4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00df4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e78
			 0xffc00001,                                                  // -signaling NaN                              /// 00e7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ea0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0xffc00001,                                                  // -signaling NaN                              /// 00eac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eb0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00eb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ebc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec0
			 0x00000000,                                                  // zero                                        /// 00ec4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ecc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ed0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ed4
			 0x80000000,                                                  // -zero                                       /// 00ed8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00efc
			 0xbf028f5c,                                                  // -0.51                                       /// 00f00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f98
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xff800000,                                                  // -inf                                        /// 00fa4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fbc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fcc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fe8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7c0000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0c400000,                                                  // Leading 1s:                                 /// 00004
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00014
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00020
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00024
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0002c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00034
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00038
			 0x0c600000,                                                  // Leading 1s:                                 /// 0003c
			 0x00000000,                                                  // zero                                        /// 00040
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00044
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00048
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0004c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00050
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00054
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00060
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00064
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00068
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00070
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00084
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00088
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00094
			 0x00000000,                                                  // zero                                        /// 00098
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x55555555,                                                  // 4 random values                             /// 000a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x7f800000,                                                  // inf                                         /// 000bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000e8
			 0x4b000000,                                                  // 8388608.0                                   /// 000ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x55555555,                                                  // 4 random values                             /// 00108
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0010c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00114
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00124
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0012c
			 0xcb000000,                                                  // -8388608.0                                  /// 00130
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00134
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00140
			 0x0c600000,                                                  // Leading 1s:                                 /// 00144
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00150
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00154
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00160
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00164
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00168
			 0x80000000,                                                  // -zero                                       /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0x55555555,                                                  // 4 random values                             /// 00174
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00178
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00184
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xcb000000,                                                  // -8388608.0                                  /// 0018c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00194
			 0x80011111,                                                  // -9.7958E-41                                 /// 00198
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e0
			 0x3f028f5c,                                                  // 0.51                                        /// 001e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00200
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00204
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00208
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00214
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00218
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0021c
			 0x33333333,                                                  // 4 random values                             /// 00220
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00228
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0022c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00230
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00234
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00240
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00250
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00258
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0025c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00260
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00264
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00268
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0026c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00274
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0027c
			 0x7f800000,                                                  // inf                                         /// 00280
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00284
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0029c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x4b000000,                                                  // 8388608.0                                   /// 002a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 002b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c0
			 0xffc00001,                                                  // -signaling NaN                              /// 002c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002cc
			 0x55555555,                                                  // 4 random values                             /// 002d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e8
			 0x4b000000,                                                  // 8388608.0                                   /// 002ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0xff800000,                                                  // -inf                                        /// 00300
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00304
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00308
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0030c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00310
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00318
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0031c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00320
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00328
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00330
			 0xffc00001,                                                  // -signaling NaN                              /// 00334
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00338
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00340
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00354
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00358
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00370
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00378
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0037c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00380
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00384
			 0x7f800000,                                                  // inf                                         /// 00388
			 0x80011111,                                                  // -9.7958E-41                                 /// 0038c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00390
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00398
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0039c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xffc00001,                                                  // -signaling NaN                              /// 003d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003e8
			 0xcb000000,                                                  // -8388608.0                                  /// 003ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00400
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00408
			 0x33333333,                                                  // 4 random values                             /// 0040c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00410
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00428
			 0x00011111,                                                  // 9.7958E-41                                  /// 0042c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00444
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00448
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0044c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00450
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00454
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0xcb000000,                                                  // -8388608.0                                  /// 00460
			 0x0e000007,                                                  // Trailing 1s:                                /// 00464
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00468
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00470
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00474
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0047c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00484
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00488
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0048c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00494
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0049c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00500
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00504
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xbf028f5c,                                                  // -0.51                                       /// 0050c
			 0x80000000,                                                  // -zero                                       /// 00510
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00514
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00524
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00528
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0052c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00530
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00534
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00538
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0053c
			 0x7f800000,                                                  // inf                                         /// 00540
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00544
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00548
			 0x80000000,                                                  // -zero                                       /// 0054c
			 0x7fc00001,                                                  // signaling NaN                               /// 00550
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00554
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0055c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00568
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00574
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00578
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0057c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00580
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00584
			 0x0e000007,                                                  // Trailing 1s:                                /// 00588
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0058c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00590
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00598
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0x80011111,                                                  // -9.7958E-41                                 /// 005a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005cc
			 0x3f028f5c,                                                  // 0.51                                        /// 005d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d8
			 0xffc00001,                                                  // -signaling NaN                              /// 005dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e4
			 0x4b000000,                                                  // 8388608.0                                   /// 005e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00600
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00604
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00610
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00614
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00618
			 0x55555555,                                                  // 4 random values                             /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00634
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0063c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00648
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0064c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00658
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0065c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00660
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00664
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0066c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00678
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0067c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00680
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00684
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00694
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00698
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0069c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00700
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00704
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00710
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00714
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00718
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00724
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0072c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00734
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0xffc00001,                                                  // -signaling NaN                              /// 0073c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00740
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00744
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00748
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0074c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00750
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00754
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00760
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00768
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0076c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00778
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0077c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0078c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xcb000000,                                                  // -8388608.0                                  /// 0079c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x7fc00001,                                                  // signaling NaN                               /// 007d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00804
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00808
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00814
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00818
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0081c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00820
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00830
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00838
			 0x00011111,                                                  // 9.7958E-41                                  /// 0083c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00840
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00844
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0084c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00850
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00854
			 0xcb000000,                                                  // -8388608.0                                  /// 00858
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0086c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00874
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00878
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0088c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00890
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00894
			 0x55555555,                                                  // 4 random values                             /// 00898
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x55555555,                                                  // 4 random values                             /// 008f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00900
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00904
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0090c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0091c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00920
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00928
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0092c
			 0xbf028f5c,                                                  // -0.51                                       /// 00930
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00940
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00944
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00948
			 0x7f800000,                                                  // inf                                         /// 0094c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00958
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0095c
			 0xffc00001,                                                  // -signaling NaN                              /// 00960
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00964
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00968
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0096c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00970
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00974
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0097c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00980
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00988
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0098c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00990
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00998
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x7fc00001,                                                  // signaling NaN                               /// 009a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a04
			 0x00000000,                                                  // zero                                        /// 00a08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a18
			 0x33333333,                                                  // 4 random values                             /// 00a1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x55555555,                                                  // 4 random values                             /// 00a24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a58
			 0x4b000000,                                                  // 8388608.0                                   /// 00a5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a9c
			 0x55555555,                                                  // 4 random values                             /// 00aa0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab8
			 0x55555555,                                                  // 4 random values                             /// 00abc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ac0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ac4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00acc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ae4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b44
			 0x33333333,                                                  // 4 random values                             /// 00b48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xff800000,                                                  // -inf                                        /// 00b50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b5c
			 0x55555555,                                                  // 4 random values                             /// 00b60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b74
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x80000000,                                                  // -zero                                       /// 00b7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bbc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bdc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00be8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bf8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c34
			 0xbf028f5c,                                                  // -0.51                                       /// 00c38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x4b000000,                                                  // 8388608.0                                   /// 00c48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c78
			 0xcb000000,                                                  // -8388608.0                                  /// 00c7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0xff800000,                                                  // -inf                                        /// 00ca0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cec
			 0x7fc00001,                                                  // signaling NaN                               /// 00cf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d00
			 0x4b000000,                                                  // 8388608.0                                   /// 00d04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db0
			 0x3f028f5c,                                                  // 0.51                                        /// 00db4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd0
			 0x33333333,                                                  // 4 random values                             /// 00dd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ddc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00df4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e34
			 0xcb000000,                                                  // -8388608.0                                  /// 00e38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e58
			 0xff800000,                                                  // -inf                                        /// 00e5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e80
			 0x80000000,                                                  // -zero                                       /// 00e84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e9c
			 0x55555555,                                                  // 4 random values                             /// 00ea0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ea8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ebc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee8
			 0xff800000,                                                  // -inf                                        /// 00eec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00efc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f14
			 0xcb000000,                                                  // -8388608.0                                  /// 00f18
			 0x80000000,                                                  // -zero                                       /// 00f1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f28
			 0x55555555,                                                  // 4 random values                             /// 00f2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f30
			 0x55555555,                                                  // 4 random values                             /// 00f34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f94
			 0xcb000000,                                                  // -8388608.0                                  /// 00f98
			 0x55555555,                                                  // 4 random values                             /// 00f9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc8
			 0x55555555,                                                  // 4 random values                             /// 00fcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fdc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0xff800000                                                  // -inf                                        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00004
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0000c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00010
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00014
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00018
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00020
			 0x0c600000,                                                  // Leading 1s:                                 /// 00024
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00028
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0002c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00030
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00034
			 0xffc00001,                                                  // -signaling NaN                              /// 00038
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00040
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00044
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00048
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00050
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00054
			 0xff800000,                                                  // -inf                                        /// 00058
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00060
			 0x0c400000,                                                  // Leading 1s:                                 /// 00064
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00068
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00070
			 0x80000000,                                                  // -zero                                       /// 00074
			 0x80011111,                                                  // -9.7958E-41                                 /// 00078
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0007c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0008c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00090
			 0x80011111,                                                  // -9.7958E-41                                 /// 00094
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00098
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00104
			 0x80011111,                                                  // -9.7958E-41                                 /// 00108
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0010c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xcb000000,                                                  // -8388608.0                                  /// 00118
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0011c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00120
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00128
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0012c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00130
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0014c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xbf028f5c,                                                  // -0.51                                       /// 00158
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0015c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x3f028f5c,                                                  // 0.51                                        /// 00164
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0016c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00170
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00174
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x7fc00001,                                                  // signaling NaN                               /// 0017c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00180
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00184
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0018c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00194
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0xffc00001,                                                  // -signaling NaN                              /// 001a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00200
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00204
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00208
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00210
			 0x0c600000,                                                  // Leading 1s:                                 /// 00214
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00218
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00220
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00228
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0022c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00230
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00238
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0023c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00248
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00250
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00258
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0025c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00264
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00270
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00274
			 0x00000000,                                                  // zero                                        /// 00278
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00280
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00284
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0028c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00294
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00298
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 002b0
			 0x7fc00001,                                                  // signaling NaN                               /// 002b4
			 0x55555555,                                                  // 4 random values                             /// 002b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002cc
			 0x00000000,                                                  // zero                                        /// 002d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 002e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00300
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00304
			 0xffc00001,                                                  // -signaling NaN                              /// 00308
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00324
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00328
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0032c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00330
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0033c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00340
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00350
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00354
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00358
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00368
			 0x7fc00001,                                                  // signaling NaN                               /// 0036c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0xbf028f5c,                                                  // -0.51                                       /// 00374
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00384
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0038c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00390
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00400
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00408
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0040c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00410
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00414
			 0x0e000001,                                                  // Trailing 1s:                                /// 00418
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00424
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00428
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0042c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00434
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00438
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0043c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00440
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00444
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0044c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00450
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00458
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0045c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00460
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00468
			 0x80000000,                                                  // -zero                                       /// 0046c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00478
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0047c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00480
			 0x4b000000,                                                  // 8388608.0                                   /// 00484
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00488
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00498
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0049c
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00000000,                                                  // zero                                        /// 004b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f8
			 0x55555555,                                                  // 4 random values                             /// 004fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00500
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00504
			 0x3f028f5c,                                                  // 0.51                                        /// 00508
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0050c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00510
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00514
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00518
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0051c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00524
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00528
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00530
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00534
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00538
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0053c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00544
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00548
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00550
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00554
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00558
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0055c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00564
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00568
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00574
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00578
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00580
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00584
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00588
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0058c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00590
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0059c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005b4
			 0xffc00001,                                                  // -signaling NaN                              /// 005b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xcb000000,                                                  // -8388608.0                                  /// 005fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e000007,                                                  // Trailing 1s:                                /// 00608
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00610
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00614
			 0x0e000001,                                                  // Trailing 1s:                                /// 00618
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00624
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00630
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00638
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00640
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0064c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00650
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00658
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00668
			 0x7fc00001,                                                  // signaling NaN                               /// 0066c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00670
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00674
			 0x0e000007,                                                  // Trailing 1s:                                /// 00678
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0067c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0068c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00690
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00694
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00698
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0069c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 006b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f4
			 0x80000000,                                                  // -zero                                       /// 006f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00700
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00704
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0070c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xcb000000,                                                  // -8388608.0                                  /// 00718
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00720
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00724
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0072c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00730
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00738
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0073c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x00000000,                                                  // zero                                        /// 00748
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0074c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00750
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00758
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00764
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00778
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0078c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00790
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00794
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00798
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0079c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007c0
			 0x00000000,                                                  // zero                                        /// 007c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00800
			 0xbf028f5c,                                                  // -0.51                                       /// 00804
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00808
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0080c
			 0xffc00001,                                                  // -signaling NaN                              /// 00810
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e000001,                                                  // Trailing 1s:                                /// 00818
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00820
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0082c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00838
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00840
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00844
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00848
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0084c
			 0xff800000,                                                  // -inf                                        /// 00850
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00854
			 0x0c600000,                                                  // Leading 1s:                                 /// 00858
			 0xff800000,                                                  // -inf                                        /// 0085c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00860
			 0x0e000003,                                                  // Trailing 1s:                                /// 00864
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00868
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00870
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00874
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00878
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0087c
			 0x55555555,                                                  // 4 random values                             /// 00880
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00884
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00890
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00898
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0089c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008ac
			 0xcb000000,                                                  // -8388608.0                                  /// 008b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00900
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00908
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0090c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00910
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00914
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0091c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00924
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0092c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00930
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00934
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00938
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0093c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00011111,                                                  // 9.7958E-41                                  /// 00944
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00950
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00954
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00958
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0095c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c400000,                                                  // Leading 1s:                                 /// 00964
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00968
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00974
			 0x0e000001,                                                  // Trailing 1s:                                /// 00978
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0097c
			 0x7f800000,                                                  // inf                                         /// 00980
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00984
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0098c
			 0x7fc00001,                                                  // signaling NaN                               /// 00990
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00994
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00998
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a8
			 0xff800000,                                                  // -inf                                        /// 009ac
			 0xff800000,                                                  // -inf                                        /// 009b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b8
			 0x4b000000,                                                  // 8388608.0                                   /// 009bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009f4
			 0x33333333,                                                  // 4 random values                             /// 009f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a08
			 0x3f028f5c,                                                  // 0.51                                        /// 00a0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x00000000,                                                  // zero                                        /// 00a60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x00000000,                                                  // zero                                        /// 00a70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0xff800000,                                                  // -inf                                        /// 00a80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ab8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00abc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ac0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00acc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad4
			 0x7f800000,                                                  // inf                                         /// 00ad8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ae0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b48
			 0x80000000,                                                  // -zero                                       /// 00b4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b78
			 0xff800000,                                                  // -inf                                        /// 00b7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ba4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bcc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bd0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bdc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x00000000,                                                  // zero                                        /// 00bf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf4
			 0x00000000,                                                  // zero                                        /// 00bf8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80000000,                                                  // -zero                                       /// 00c18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c50
			 0x4b000000,                                                  // 8388608.0                                   /// 00c54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c94
			 0xcb000000,                                                  // -8388608.0                                  /// 00c98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ca0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cbc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cdc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ce8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x55555555,                                                  // 4 random values                             /// 00cf8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d10
			 0x00000000,                                                  // zero                                        /// 00d14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d24
			 0xbf028f5c,                                                  // -0.51                                       /// 00d28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d58
			 0x3f028f5c,                                                  // 0.51                                        /// 00d5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00da0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd8
			 0x7f800000,                                                  // inf                                         /// 00ddc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e24
			 0x33333333,                                                  // 4 random values                             /// 00e28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xff800000,                                                  // -inf                                        /// 00e34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e40
			 0x00000000,                                                  // zero                                        /// 00e44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e48
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e88
			 0x4b000000,                                                  // 8388608.0                                   /// 00e8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ecc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00edc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x7f800000,                                                  // inf                                         /// 00ef0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f0c
			 0x80000000,                                                  // -zero                                       /// 00f10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x4b000000,                                                  // 8388608.0                                   /// 00f38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f3c
			 0x55555555,                                                  // 4 random values                             /// 00f40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x7f800000,                                                  // inf                                         /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fb4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fe0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fe4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fe8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff8
			 0x80010000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x05e5c98f, /// 0x0
			 0x8a449e04, /// 0x4
			 0xc06c6ad2, /// 0x8
			 0x01743cd3, /// 0xc
			 0x1c4a5fb5, /// 0x10
			 0xf55bd2c8, /// 0x14
			 0xb5c9f9e5, /// 0x18
			 0xe60229c0, /// 0x1c
			 0x1c6e9bb3, /// 0x20
			 0xcbd93616, /// 0x24
			 0x8d5f3208, /// 0x28
			 0x5f3559c3, /// 0x2c
			 0xb713eb35, /// 0x30
			 0x9ce6e13b, /// 0x34
			 0x59b6b2e4, /// 0x38
			 0x5818c1a1, /// 0x3c
			 0xb2bbad17, /// 0x40
			 0x47d2e240, /// 0x44
			 0xed9dad5e, /// 0x48
			 0x66def94b, /// 0x4c
			 0x5043da22, /// 0x50
			 0x4be5fe5a, /// 0x54
			 0xba828d7e, /// 0x58
			 0xa201f6a8, /// 0x5c
			 0x014a3451, /// 0x60
			 0xeef42dc7, /// 0x64
			 0x63fa0415, /// 0x68
			 0x580ba3e2, /// 0x6c
			 0xe31fc07f, /// 0x70
			 0xa660414a, /// 0x74
			 0x9ddc6776, /// 0x78
			 0xe337eebb, /// 0x7c
			 0x17ca182c, /// 0x80
			 0x39b9fc4d, /// 0x84
			 0x501e68b8, /// 0x88
			 0x13437a76, /// 0x8c
			 0x69d8143d, /// 0x90
			 0x52f5fcd1, /// 0x94
			 0x80cc77a9, /// 0x98
			 0xde8b8294, /// 0x9c
			 0x69b9b364, /// 0xa0
			 0x50c8316e, /// 0xa4
			 0x81f18ff4, /// 0xa8
			 0x49ba69cd, /// 0xac
			 0x50219520, /// 0xb0
			 0xf4040aa5, /// 0xb4
			 0xb1ac9e00, /// 0xb8
			 0xedc51fd8, /// 0xbc
			 0xd7f5a8d2, /// 0xc0
			 0x260adca8, /// 0xc4
			 0xb16e0eb7, /// 0xc8
			 0x6b5f83f3, /// 0xcc
			 0x28c50974, /// 0xd0
			 0x23f73a03, /// 0xd4
			 0x28cfe32d, /// 0xd8
			 0x287fe6f3, /// 0xdc
			 0xd494f9ee, /// 0xe0
			 0xf49ce043, /// 0xe4
			 0x6e7fe595, /// 0xe8
			 0x520a1313, /// 0xec
			 0xe845645d, /// 0xf0
			 0x1747ebeb, /// 0xf4
			 0x90d16606, /// 0xf8
			 0x149fa44b, /// 0xfc
			 0x097f8865, /// 0x100
			 0x5b410b35, /// 0x104
			 0xae68e27a, /// 0x108
			 0x61ce4c34, /// 0x10c
			 0xc7318052, /// 0x110
			 0x1a532963, /// 0x114
			 0x49bfbf56, /// 0x118
			 0xa0cf60dd, /// 0x11c
			 0xfc05beb8, /// 0x120
			 0x443a491b, /// 0x124
			 0x76517739, /// 0x128
			 0x922f2893, /// 0x12c
			 0x62c02ae7, /// 0x130
			 0xcaf48def, /// 0x134
			 0xdf1424bf, /// 0x138
			 0xe7ec3fdc, /// 0x13c
			 0x97a9a124, /// 0x140
			 0x4d6690b1, /// 0x144
			 0xb589173c, /// 0x148
			 0xb1cbb117, /// 0x14c
			 0x190fd4d6, /// 0x150
			 0x67d9d588, /// 0x154
			 0x292432c5, /// 0x158
			 0x3845d1b1, /// 0x15c
			 0xc8221e43, /// 0x160
			 0x59a39fb8, /// 0x164
			 0xe8346c02, /// 0x168
			 0x034772cd, /// 0x16c
			 0x512d48dd, /// 0x170
			 0x71e059e0, /// 0x174
			 0xfc2d66ee, /// 0x178
			 0xe42c13b4, /// 0x17c
			 0xd3f7de88, /// 0x180
			 0xfb52da96, /// 0x184
			 0xdc74a24e, /// 0x188
			 0x77117846, /// 0x18c
			 0xe4262cd1, /// 0x190
			 0x61aa3bc6, /// 0x194
			 0x0176a0f6, /// 0x198
			 0x7c851b41, /// 0x19c
			 0x5b2a9099, /// 0x1a0
			 0x46702607, /// 0x1a4
			 0x3321e1bc, /// 0x1a8
			 0x7713514f, /// 0x1ac
			 0x7b294e54, /// 0x1b0
			 0xc6426cef, /// 0x1b4
			 0x3ec4e716, /// 0x1b8
			 0x766ac1a4, /// 0x1bc
			 0x1bfd75a5, /// 0x1c0
			 0x0f00dbc8, /// 0x1c4
			 0xd8f55240, /// 0x1c8
			 0x9c604c8d, /// 0x1cc
			 0x32e5fcae, /// 0x1d0
			 0x246f694c, /// 0x1d4
			 0x933d8a81, /// 0x1d8
			 0xa0c5838d, /// 0x1dc
			 0x1ca90d8c, /// 0x1e0
			 0x592460d6, /// 0x1e4
			 0x41c86039, /// 0x1e8
			 0x3fb03c52, /// 0x1ec
			 0x966c8e46, /// 0x1f0
			 0x9b2d9376, /// 0x1f4
			 0x85948d00, /// 0x1f8
			 0x2289183a, /// 0x1fc
			 0x15cdfc6a, /// 0x200
			 0x54c1ba09, /// 0x204
			 0x146c1148, /// 0x208
			 0xce3198d2, /// 0x20c
			 0x3f8d42a5, /// 0x210
			 0xb4f50f42, /// 0x214
			 0x78fd789f, /// 0x218
			 0xd8302270, /// 0x21c
			 0x1401697b, /// 0x220
			 0xce11eb6d, /// 0x224
			 0x466c5164, /// 0x228
			 0x8718e2a3, /// 0x22c
			 0xb6e0ec24, /// 0x230
			 0x1e24f20d, /// 0x234
			 0x75d2c6c6, /// 0x238
			 0xa9758634, /// 0x23c
			 0x46f7a7e0, /// 0x240
			 0x6872956e, /// 0x244
			 0x96ced14e, /// 0x248
			 0x33ed04c1, /// 0x24c
			 0x72f1fd41, /// 0x250
			 0x7b9cfb3e, /// 0x254
			 0xedcf0ad3, /// 0x258
			 0x449b6285, /// 0x25c
			 0x3632ca95, /// 0x260
			 0xfb427cd2, /// 0x264
			 0x26cf1480, /// 0x268
			 0xbf277bbc, /// 0x26c
			 0xb82700fb, /// 0x270
			 0x3cc7627c, /// 0x274
			 0xacda4f87, /// 0x278
			 0xa3d86276, /// 0x27c
			 0xcbcdfec2, /// 0x280
			 0x5cf5d49c, /// 0x284
			 0x001b4471, /// 0x288
			 0x966993f0, /// 0x28c
			 0x8bf622db, /// 0x290
			 0xd831d90f, /// 0x294
			 0xe577b9d4, /// 0x298
			 0xed7b3a32, /// 0x29c
			 0x48d93648, /// 0x2a0
			 0x88d7f84d, /// 0x2a4
			 0x9cc7420f, /// 0x2a8
			 0x06311488, /// 0x2ac
			 0xc3143fda, /// 0x2b0
			 0xc4163939, /// 0x2b4
			 0x639c6039, /// 0x2b8
			 0x7b0f6d18, /// 0x2bc
			 0xc50651d4, /// 0x2c0
			 0x826fff1b, /// 0x2c4
			 0xf24e9b14, /// 0x2c8
			 0xda360735, /// 0x2cc
			 0x24eaf566, /// 0x2d0
			 0x039b2ad7, /// 0x2d4
			 0xe27ffa30, /// 0x2d8
			 0x147c6bbd, /// 0x2dc
			 0x53726ce9, /// 0x2e0
			 0x331893e0, /// 0x2e4
			 0xaa54c1f7, /// 0x2e8
			 0x2bd237d0, /// 0x2ec
			 0x522713ae, /// 0x2f0
			 0x99bfe707, /// 0x2f4
			 0xd5c6afe3, /// 0x2f8
			 0xfb1adda8, /// 0x2fc
			 0x919d0ff1, /// 0x300
			 0x94dd7cec, /// 0x304
			 0x5e512685, /// 0x308
			 0x13a29bb3, /// 0x30c
			 0xa96fa659, /// 0x310
			 0x1d70d9b5, /// 0x314
			 0xf3f1c406, /// 0x318
			 0x736dd978, /// 0x31c
			 0x923cdb16, /// 0x320
			 0x3a01f014, /// 0x324
			 0xf74740e3, /// 0x328
			 0x621c098f, /// 0x32c
			 0x209dbd35, /// 0x330
			 0x83d09fc0, /// 0x334
			 0x859b8d05, /// 0x338
			 0x4b22b2d3, /// 0x33c
			 0xb9d5f81e, /// 0x340
			 0x8d0751de, /// 0x344
			 0x358317d5, /// 0x348
			 0xb5b00306, /// 0x34c
			 0xfe629609, /// 0x350
			 0xb1b64a27, /// 0x354
			 0x5ea5b7dc, /// 0x358
			 0x7611956d, /// 0x35c
			 0xadbe5b0e, /// 0x360
			 0x41244c5e, /// 0x364
			 0x28b58223, /// 0x368
			 0x047b772c, /// 0x36c
			 0x62ed630e, /// 0x370
			 0xfdbb24f8, /// 0x374
			 0x2e2cfaab, /// 0x378
			 0x93ca287b, /// 0x37c
			 0x2c808cbc, /// 0x380
			 0xdf070b34, /// 0x384
			 0x2851041d, /// 0x388
			 0x07a150c4, /// 0x38c
			 0x44ddb997, /// 0x390
			 0x21b8df09, /// 0x394
			 0xdc02111c, /// 0x398
			 0xfbbe0da5, /// 0x39c
			 0x70c1fea6, /// 0x3a0
			 0xe12b2811, /// 0x3a4
			 0x10ae3fbc, /// 0x3a8
			 0x4837149a, /// 0x3ac
			 0xd1cdfefb, /// 0x3b0
			 0xab1f8dc6, /// 0x3b4
			 0x6283c3af, /// 0x3b8
			 0x86be1eab, /// 0x3bc
			 0xd68e8de1, /// 0x3c0
			 0x691fe1e3, /// 0x3c4
			 0xf6315c5e, /// 0x3c8
			 0x20b34183, /// 0x3cc
			 0xe1ea6f89, /// 0x3d0
			 0x191873b9, /// 0x3d4
			 0x11b6a2c7, /// 0x3d8
			 0xb33b18c2, /// 0x3dc
			 0x0caeb3d9, /// 0x3e0
			 0x399b0fa9, /// 0x3e4
			 0xc08884e7, /// 0x3e8
			 0x98abf187, /// 0x3ec
			 0x116e3f4c, /// 0x3f0
			 0x458a945b, /// 0x3f4
			 0xb18ef03d, /// 0x3f8
			 0x931d6251, /// 0x3fc
			 0xf74c13a6, /// 0x400
			 0xa5c86d4c, /// 0x404
			 0xc65bc6bb, /// 0x408
			 0xa7b891a1, /// 0x40c
			 0xe6f94a81, /// 0x410
			 0x13348b36, /// 0x414
			 0x14289404, /// 0x418
			 0x08974058, /// 0x41c
			 0x7109c066, /// 0x420
			 0x48a1b5ac, /// 0x424
			 0xafa868df, /// 0x428
			 0x23253217, /// 0x42c
			 0x4e1e883b, /// 0x430
			 0x88bc472a, /// 0x434
			 0x2402ac26, /// 0x438
			 0x58ce0b6a, /// 0x43c
			 0xce680d53, /// 0x440
			 0x217f1d8b, /// 0x444
			 0xc0e518d5, /// 0x448
			 0xa128b830, /// 0x44c
			 0xadc6c493, /// 0x450
			 0x23ad2de0, /// 0x454
			 0xf84f69d0, /// 0x458
			 0xc0977ebf, /// 0x45c
			 0x24dfb672, /// 0x460
			 0x0a8a23fb, /// 0x464
			 0xfdb96b22, /// 0x468
			 0x89356a78, /// 0x46c
			 0x11dcf618, /// 0x470
			 0xbc6345b6, /// 0x474
			 0x8ecc40e7, /// 0x478
			 0x5816fc81, /// 0x47c
			 0x7d0515ea, /// 0x480
			 0x056200ab, /// 0x484
			 0x4c6a6318, /// 0x488
			 0xdcb73abe, /// 0x48c
			 0x2cea509a, /// 0x490
			 0x0a2a63c8, /// 0x494
			 0x56283a59, /// 0x498
			 0x4ea90597, /// 0x49c
			 0x07420e82, /// 0x4a0
			 0xfe853988, /// 0x4a4
			 0x9e431dbf, /// 0x4a8
			 0x68ce2ad2, /// 0x4ac
			 0xa017f33f, /// 0x4b0
			 0x13e4827b, /// 0x4b4
			 0x5fc3b2d0, /// 0x4b8
			 0x11598458, /// 0x4bc
			 0xf734d1a8, /// 0x4c0
			 0x92fc4e3b, /// 0x4c4
			 0x5af9ed01, /// 0x4c8
			 0x7f42b9dd, /// 0x4cc
			 0x22dd08d6, /// 0x4d0
			 0xb10f552f, /// 0x4d4
			 0x5735827d, /// 0x4d8
			 0xe132535c, /// 0x4dc
			 0x6d5e8c28, /// 0x4e0
			 0xb010fce9, /// 0x4e4
			 0xcda717f4, /// 0x4e8
			 0x2e69ad50, /// 0x4ec
			 0x9eabcd9f, /// 0x4f0
			 0x45f4f5ea, /// 0x4f4
			 0x283356c3, /// 0x4f8
			 0x186a4d7a, /// 0x4fc
			 0x90e5d4bf, /// 0x500
			 0x7a12fdd6, /// 0x504
			 0x1e33114a, /// 0x508
			 0x7cb07450, /// 0x50c
			 0x0e9a9004, /// 0x510
			 0x379ff366, /// 0x514
			 0x05aac520, /// 0x518
			 0x887d3a0e, /// 0x51c
			 0x0ae47d89, /// 0x520
			 0x10bc255d, /// 0x524
			 0x568f9e62, /// 0x528
			 0xbb357236, /// 0x52c
			 0x255ba157, /// 0x530
			 0xc22db506, /// 0x534
			 0x4dde5f71, /// 0x538
			 0xc5b9921a, /// 0x53c
			 0x8673b101, /// 0x540
			 0xdcc61f32, /// 0x544
			 0xfd125d5d, /// 0x548
			 0x9fb7a102, /// 0x54c
			 0xb9bc072b, /// 0x550
			 0x8d639639, /// 0x554
			 0x8c516db5, /// 0x558
			 0xe27fdfa9, /// 0x55c
			 0x77802bf2, /// 0x560
			 0xe6d7aec9, /// 0x564
			 0xc019e847, /// 0x568
			 0xb430d5f4, /// 0x56c
			 0xa96f67aa, /// 0x570
			 0x6ce6d384, /// 0x574
			 0x9ab1d314, /// 0x578
			 0xb0ae37a2, /// 0x57c
			 0xf8141c72, /// 0x580
			 0xc3c745e6, /// 0x584
			 0x5e8e6aa4, /// 0x588
			 0x761db160, /// 0x58c
			 0x383f1335, /// 0x590
			 0x6f67c19d, /// 0x594
			 0xdcd05b95, /// 0x598
			 0xe259be16, /// 0x59c
			 0xf95c1028, /// 0x5a0
			 0x30df36eb, /// 0x5a4
			 0x323aa7e6, /// 0x5a8
			 0x9f529574, /// 0x5ac
			 0xb49dd553, /// 0x5b0
			 0x37c22017, /// 0x5b4
			 0x763067af, /// 0x5b8
			 0x0eefb657, /// 0x5bc
			 0x068bb748, /// 0x5c0
			 0xebabe0b4, /// 0x5c4
			 0xf81e1efc, /// 0x5c8
			 0x6eb59707, /// 0x5cc
			 0x181d94b6, /// 0x5d0
			 0x37665128, /// 0x5d4
			 0xad245102, /// 0x5d8
			 0x7051378d, /// 0x5dc
			 0x37efd384, /// 0x5e0
			 0x73d53eb8, /// 0x5e4
			 0x50bf64f7, /// 0x5e8
			 0xc234a78b, /// 0x5ec
			 0xb64784ae, /// 0x5f0
			 0x6fffe655, /// 0x5f4
			 0xc955ed32, /// 0x5f8
			 0x5b546ae3, /// 0x5fc
			 0x4842815c, /// 0x600
			 0xb9c36dbc, /// 0x604
			 0x6ed687f8, /// 0x608
			 0xc4928133, /// 0x60c
			 0xe1a2933d, /// 0x610
			 0xcbd5599a, /// 0x614
			 0xc838fe54, /// 0x618
			 0x811c6708, /// 0x61c
			 0x5d9688e9, /// 0x620
			 0xdadc88b5, /// 0x624
			 0x56569e30, /// 0x628
			 0x3ecd7e1f, /// 0x62c
			 0x57aedc9c, /// 0x630
			 0xf1925cbe, /// 0x634
			 0xc0d23f29, /// 0x638
			 0x8d61023d, /// 0x63c
			 0x403364c9, /// 0x640
			 0x001535fc, /// 0x644
			 0x4a9b253d, /// 0x648
			 0xe32f6ee9, /// 0x64c
			 0xb302e186, /// 0x650
			 0xb231a43c, /// 0x654
			 0x94b95498, /// 0x658
			 0x51134dfd, /// 0x65c
			 0xbe8bbb9e, /// 0x660
			 0x27fc449e, /// 0x664
			 0x81f4ea86, /// 0x668
			 0xba90fa66, /// 0x66c
			 0xef1ebaf9, /// 0x670
			 0xbef8805c, /// 0x674
			 0xbfa49cc5, /// 0x678
			 0x43c9cc49, /// 0x67c
			 0x44062a75, /// 0x680
			 0xe2668c57, /// 0x684
			 0x18d9bcdb, /// 0x688
			 0x15d0f1c6, /// 0x68c
			 0xdf1c51b3, /// 0x690
			 0xd1a24e70, /// 0x694
			 0x8a2cf79b, /// 0x698
			 0x9071803f, /// 0x69c
			 0xbea5985d, /// 0x6a0
			 0x82879468, /// 0x6a4
			 0x29eb627f, /// 0x6a8
			 0xb350026f, /// 0x6ac
			 0xae91d989, /// 0x6b0
			 0xdeb012c3, /// 0x6b4
			 0x542fcdd7, /// 0x6b8
			 0x2760cc7b, /// 0x6bc
			 0xe42532af, /// 0x6c0
			 0xa542a398, /// 0x6c4
			 0x28e77f22, /// 0x6c8
			 0x43d2e706, /// 0x6cc
			 0x5881213b, /// 0x6d0
			 0x588c2466, /// 0x6d4
			 0xcd5beec9, /// 0x6d8
			 0xc9e1d73f, /// 0x6dc
			 0x52f29334, /// 0x6e0
			 0xb646d68a, /// 0x6e4
			 0x17f4a274, /// 0x6e8
			 0xad387cfb, /// 0x6ec
			 0x3de6431d, /// 0x6f0
			 0x308b837a, /// 0x6f4
			 0x3c905d05, /// 0x6f8
			 0x6f2ea731, /// 0x6fc
			 0x30d285ac, /// 0x700
			 0xf54e9b8d, /// 0x704
			 0x0ee7e4c6, /// 0x708
			 0xfbd32e5b, /// 0x70c
			 0x8d6ec7b3, /// 0x710
			 0x38a234b0, /// 0x714
			 0x7d25a6bb, /// 0x718
			 0x35eb9015, /// 0x71c
			 0xf83ba320, /// 0x720
			 0x16a29c1f, /// 0x724
			 0xa393a610, /// 0x728
			 0x94e187ea, /// 0x72c
			 0x260bff13, /// 0x730
			 0x7e3090cc, /// 0x734
			 0x8a370158, /// 0x738
			 0x02e20a87, /// 0x73c
			 0x57c9af91, /// 0x740
			 0xe75998a8, /// 0x744
			 0xa3ca18d0, /// 0x748
			 0x62ca338d, /// 0x74c
			 0x3118b4ae, /// 0x750
			 0xcf702ea1, /// 0x754
			 0x81f7bf05, /// 0x758
			 0x02667f70, /// 0x75c
			 0x40511da3, /// 0x760
			 0xbe2fb645, /// 0x764
			 0xd2cc2f53, /// 0x768
			 0x2d683fcc, /// 0x76c
			 0x66e69afc, /// 0x770
			 0x3ef00694, /// 0x774
			 0x005d9871, /// 0x778
			 0xe7264a70, /// 0x77c
			 0x91a64c4a, /// 0x780
			 0x4fd16095, /// 0x784
			 0x3e7bf486, /// 0x788
			 0xb8f3d56e, /// 0x78c
			 0x8389b2c9, /// 0x790
			 0x7dd96892, /// 0x794
			 0xb7852e75, /// 0x798
			 0x0283ea26, /// 0x79c
			 0x2ad9995a, /// 0x7a0
			 0x5f6b5bf5, /// 0x7a4
			 0x8b13d450, /// 0x7a8
			 0x06f9aa30, /// 0x7ac
			 0xa0d26842, /// 0x7b0
			 0xd5d99113, /// 0x7b4
			 0x85aa5832, /// 0x7b8
			 0x76a7cf39, /// 0x7bc
			 0x32372a20, /// 0x7c0
			 0x3f2ed193, /// 0x7c4
			 0x2b5d0ed1, /// 0x7c8
			 0x768fd74b, /// 0x7cc
			 0xbcb92b71, /// 0x7d0
			 0x7fcc38a3, /// 0x7d4
			 0xe47e67b3, /// 0x7d8
			 0x6aa2dbc5, /// 0x7dc
			 0x261ea7e5, /// 0x7e0
			 0x1a3bf6ce, /// 0x7e4
			 0x0d46727a, /// 0x7e8
			 0x63dc7282, /// 0x7ec
			 0xa074d438, /// 0x7f0
			 0x87c37f04, /// 0x7f4
			 0x3d19c471, /// 0x7f8
			 0xb12e1baa, /// 0x7fc
			 0xdec14b1d, /// 0x800
			 0xdca9e187, /// 0x804
			 0xf996bcb1, /// 0x808
			 0xe0a182fb, /// 0x80c
			 0xf8cb4071, /// 0x810
			 0xf9ac8712, /// 0x814
			 0x410896fb, /// 0x818
			 0xd14f8c70, /// 0x81c
			 0x614c9f9a, /// 0x820
			 0x50fbbce7, /// 0x824
			 0xd0102676, /// 0x828
			 0xf241c36c, /// 0x82c
			 0xfbb6b979, /// 0x830
			 0x9d4cfd7d, /// 0x834
			 0x8bdb9735, /// 0x838
			 0x48298f43, /// 0x83c
			 0xac967056, /// 0x840
			 0xa06c2504, /// 0x844
			 0x5a231254, /// 0x848
			 0x43a32ea9, /// 0x84c
			 0x0d7713b2, /// 0x850
			 0x3c58aacd, /// 0x854
			 0x1e335bd1, /// 0x858
			 0x45718341, /// 0x85c
			 0xc7ecc7d8, /// 0x860
			 0x150eb95c, /// 0x864
			 0x6f8862a6, /// 0x868
			 0x3a9c8c66, /// 0x86c
			 0xb5a255d9, /// 0x870
			 0x10104ab3, /// 0x874
			 0x8e451dc5, /// 0x878
			 0x38e3cd03, /// 0x87c
			 0x5783d687, /// 0x880
			 0xbc15bf3c, /// 0x884
			 0xd40aeaf0, /// 0x888
			 0xc8159b3d, /// 0x88c
			 0x3abdb8c2, /// 0x890
			 0x3a61b6f8, /// 0x894
			 0xd8f14ed3, /// 0x898
			 0x054fd798, /// 0x89c
			 0xa04e4b4a, /// 0x8a0
			 0x855ef5f2, /// 0x8a4
			 0xb30f2594, /// 0x8a8
			 0xd2d47eda, /// 0x8ac
			 0x84336521, /// 0x8b0
			 0x2ec1b8cc, /// 0x8b4
			 0x70493830, /// 0x8b8
			 0x078665cb, /// 0x8bc
			 0x16c84f5b, /// 0x8c0
			 0x03773adb, /// 0x8c4
			 0x207307c4, /// 0x8c8
			 0x1637d543, /// 0x8cc
			 0xe9b6ba3e, /// 0x8d0
			 0xea78305b, /// 0x8d4
			 0xe587c2ec, /// 0x8d8
			 0x6b84beab, /// 0x8dc
			 0x59a94f56, /// 0x8e0
			 0x2b822892, /// 0x8e4
			 0x02bddb00, /// 0x8e8
			 0x306107d3, /// 0x8ec
			 0xd3c02daf, /// 0x8f0
			 0x3fbe5fa1, /// 0x8f4
			 0xf57548ea, /// 0x8f8
			 0xfedc7dfd, /// 0x8fc
			 0x6b520943, /// 0x900
			 0xaa5adafe, /// 0x904
			 0x0afbbf91, /// 0x908
			 0xd6ed0711, /// 0x90c
			 0x42c28155, /// 0x910
			 0xf1fa8d3f, /// 0x914
			 0xff6bbce7, /// 0x918
			 0xb65ac7ac, /// 0x91c
			 0x81affbb6, /// 0x920
			 0xbfc164ae, /// 0x924
			 0x1806f9d8, /// 0x928
			 0xbc6ab8a2, /// 0x92c
			 0x67d81f62, /// 0x930
			 0x2bc68e80, /// 0x934
			 0x9141ecb4, /// 0x938
			 0xeddb7223, /// 0x93c
			 0xfdeada35, /// 0x940
			 0x187ee2db, /// 0x944
			 0x019c7495, /// 0x948
			 0x8ab63636, /// 0x94c
			 0xc3fc4fbc, /// 0x950
			 0x08951686, /// 0x954
			 0x5243a4ed, /// 0x958
			 0x872b456a, /// 0x95c
			 0x3573411d, /// 0x960
			 0xa60abf0d, /// 0x964
			 0x6776e0a1, /// 0x968
			 0x96caf803, /// 0x96c
			 0xb19fe027, /// 0x970
			 0x5349ff8e, /// 0x974
			 0x49315d9e, /// 0x978
			 0x3bb5e3fe, /// 0x97c
			 0xb5d70c1b, /// 0x980
			 0x1accd812, /// 0x984
			 0xe83b8a28, /// 0x988
			 0x800c463d, /// 0x98c
			 0x34bfce6f, /// 0x990
			 0xcf93c38e, /// 0x994
			 0x49654c00, /// 0x998
			 0x8542bb69, /// 0x99c
			 0x760a687a, /// 0x9a0
			 0x30ace138, /// 0x9a4
			 0x7194f5b1, /// 0x9a8
			 0x18080b5e, /// 0x9ac
			 0x99e524f2, /// 0x9b0
			 0x6a993ccd, /// 0x9b4
			 0x680e3cda, /// 0x9b8
			 0x68a0a9cd, /// 0x9bc
			 0x1086d417, /// 0x9c0
			 0x44d5f90b, /// 0x9c4
			 0x9fd98dd6, /// 0x9c8
			 0x7bb0e0ba, /// 0x9cc
			 0x4b45cfd6, /// 0x9d0
			 0x556dd64f, /// 0x9d4
			 0x10093dab, /// 0x9d8
			 0x0ae1bb90, /// 0x9dc
			 0xb0542319, /// 0x9e0
			 0x24d9b538, /// 0x9e4
			 0x2a6cedd2, /// 0x9e8
			 0xae0343fc, /// 0x9ec
			 0xec0addec, /// 0x9f0
			 0xd01758e8, /// 0x9f4
			 0x3be513cd, /// 0x9f8
			 0x54ae3fef, /// 0x9fc
			 0xe96ffc62, /// 0xa00
			 0xfd557391, /// 0xa04
			 0x1932cdb4, /// 0xa08
			 0x1f013244, /// 0xa0c
			 0xec9b81f7, /// 0xa10
			 0x8621dd96, /// 0xa14
			 0xeb74a2f3, /// 0xa18
			 0x1d62afe1, /// 0xa1c
			 0x29954fd6, /// 0xa20
			 0x365a91be, /// 0xa24
			 0x0bceaa38, /// 0xa28
			 0x615d2789, /// 0xa2c
			 0xe5491d94, /// 0xa30
			 0xa921e04c, /// 0xa34
			 0x14f25b77, /// 0xa38
			 0xdbfab266, /// 0xa3c
			 0xb838b61a, /// 0xa40
			 0xe0d85645, /// 0xa44
			 0x00433333, /// 0xa48
			 0xd53a6a94, /// 0xa4c
			 0x06f3c7dc, /// 0xa50
			 0x14ce3b53, /// 0xa54
			 0xf6452b62, /// 0xa58
			 0x4d2c38fe, /// 0xa5c
			 0xf28002c9, /// 0xa60
			 0x06c56b76, /// 0xa64
			 0x5d0fe139, /// 0xa68
			 0xdecdc6fb, /// 0xa6c
			 0x8a7bcd7c, /// 0xa70
			 0xb3344eb5, /// 0xa74
			 0xe829a5df, /// 0xa78
			 0xa6749d58, /// 0xa7c
			 0x79f23e62, /// 0xa80
			 0x01711449, /// 0xa84
			 0x99f72d2e, /// 0xa88
			 0xce0dd31e, /// 0xa8c
			 0xa406b37b, /// 0xa90
			 0xee3261f9, /// 0xa94
			 0x38aebbc5, /// 0xa98
			 0xc1b5a7e4, /// 0xa9c
			 0x634a27a2, /// 0xaa0
			 0xc5bf4795, /// 0xaa4
			 0x65423b5f, /// 0xaa8
			 0xc623b993, /// 0xaac
			 0x9f6b62e8, /// 0xab0
			 0xc0f1a40e, /// 0xab4
			 0xbabbfa2c, /// 0xab8
			 0x48169b2a, /// 0xabc
			 0x33cff934, /// 0xac0
			 0xfbde0af0, /// 0xac4
			 0x45e72c7d, /// 0xac8
			 0xc6f4e5a0, /// 0xacc
			 0xf7af5a11, /// 0xad0
			 0x1c8fc89a, /// 0xad4
			 0x3dd07f7a, /// 0xad8
			 0xe0236f2d, /// 0xadc
			 0x0be7ae91, /// 0xae0
			 0x4b3fb9f1, /// 0xae4
			 0xefc746da, /// 0xae8
			 0x345b35e6, /// 0xaec
			 0x53eca7b3, /// 0xaf0
			 0x95087bfb, /// 0xaf4
			 0x2e948b11, /// 0xaf8
			 0xd955b9e2, /// 0xafc
			 0xd7199f48, /// 0xb00
			 0xca69cd14, /// 0xb04
			 0xf5a96b41, /// 0xb08
			 0xef4206d0, /// 0xb0c
			 0x580c7dcc, /// 0xb10
			 0xf53a7a7c, /// 0xb14
			 0xb0542c7c, /// 0xb18
			 0x8513f9f0, /// 0xb1c
			 0xc6b80b82, /// 0xb20
			 0xe69c6df3, /// 0xb24
			 0x845773fc, /// 0xb28
			 0x73e2f3cd, /// 0xb2c
			 0xe59d3f9e, /// 0xb30
			 0x19744576, /// 0xb34
			 0xc167171a, /// 0xb38
			 0x0e237ca4, /// 0xb3c
			 0x78492865, /// 0xb40
			 0xfb81344d, /// 0xb44
			 0x07f21ca9, /// 0xb48
			 0xa3f82613, /// 0xb4c
			 0xbbd527d3, /// 0xb50
			 0x42949b1c, /// 0xb54
			 0xd90b12c1, /// 0xb58
			 0x2f30a60e, /// 0xb5c
			 0x58a09647, /// 0xb60
			 0xd8948a40, /// 0xb64
			 0x54d9f0b2, /// 0xb68
			 0x8ce72353, /// 0xb6c
			 0x5c5c7f4a, /// 0xb70
			 0x9bbccf93, /// 0xb74
			 0x0de4bfff, /// 0xb78
			 0xf23e8053, /// 0xb7c
			 0xfe5c12f2, /// 0xb80
			 0x6cba5aee, /// 0xb84
			 0x3947a3ec, /// 0xb88
			 0xb5964c5f, /// 0xb8c
			 0xc227e72e, /// 0xb90
			 0x8c10b692, /// 0xb94
			 0xcb0da317, /// 0xb98
			 0xec7ffccf, /// 0xb9c
			 0x76eab4bc, /// 0xba0
			 0xce888db6, /// 0xba4
			 0x1b277237, /// 0xba8
			 0x6fd3512b, /// 0xbac
			 0x9f3d6b6f, /// 0xbb0
			 0x80d73732, /// 0xbb4
			 0xcecfaab5, /// 0xbb8
			 0x3ca281dc, /// 0xbbc
			 0x5a9d290a, /// 0xbc0
			 0x469a3775, /// 0xbc4
			 0x0e469c87, /// 0xbc8
			 0xc19da515, /// 0xbcc
			 0x15e97a0a, /// 0xbd0
			 0x790f3149, /// 0xbd4
			 0xcc509374, /// 0xbd8
			 0x6c36fd80, /// 0xbdc
			 0xedd97f93, /// 0xbe0
			 0x4e6e1734, /// 0xbe4
			 0x387e5b12, /// 0xbe8
			 0x68fa4ce4, /// 0xbec
			 0x77deb3f1, /// 0xbf0
			 0x946d79fe, /// 0xbf4
			 0x14a23e10, /// 0xbf8
			 0x12c2827f, /// 0xbfc
			 0x359f4950, /// 0xc00
			 0x0fc453e7, /// 0xc04
			 0x3b5b5ea4, /// 0xc08
			 0x0393e17d, /// 0xc0c
			 0xc2a5acef, /// 0xc10
			 0x7cb0f668, /// 0xc14
			 0x22481b92, /// 0xc18
			 0xf4b14ccf, /// 0xc1c
			 0x973ff301, /// 0xc20
			 0x4c649ade, /// 0xc24
			 0xf31120cd, /// 0xc28
			 0x8511b54b, /// 0xc2c
			 0xb3c97a10, /// 0xc30
			 0x4c01d3c8, /// 0xc34
			 0x0fb4c386, /// 0xc38
			 0x1a7b82b9, /// 0xc3c
			 0x11539fb5, /// 0xc40
			 0xb25129b0, /// 0xc44
			 0xf4944df0, /// 0xc48
			 0x88e0a547, /// 0xc4c
			 0xd5e454c0, /// 0xc50
			 0x5d6fb832, /// 0xc54
			 0xc9ece842, /// 0xc58
			 0x9cf30a3c, /// 0xc5c
			 0xac0e2dce, /// 0xc60
			 0xd540fbd3, /// 0xc64
			 0xc9114144, /// 0xc68
			 0xb64c22a3, /// 0xc6c
			 0x1b2dbdf9, /// 0xc70
			 0x680fa2bc, /// 0xc74
			 0x732cc051, /// 0xc78
			 0x0c805e08, /// 0xc7c
			 0x86c373e7, /// 0xc80
			 0xf60443b9, /// 0xc84
			 0x1bf51b0e, /// 0xc88
			 0x5cc825ab, /// 0xc8c
			 0x51aa5c3e, /// 0xc90
			 0x0193bcd8, /// 0xc94
			 0xea013bed, /// 0xc98
			 0xb89947ea, /// 0xc9c
			 0xf1242de3, /// 0xca0
			 0x0dde0e92, /// 0xca4
			 0x16b2bb3d, /// 0xca8
			 0x7f5c8496, /// 0xcac
			 0x7da341d3, /// 0xcb0
			 0x6eb5b70a, /// 0xcb4
			 0xe5c77952, /// 0xcb8
			 0xf3e6b1e2, /// 0xcbc
			 0x720e7c56, /// 0xcc0
			 0x7d9617ae, /// 0xcc4
			 0x908a745d, /// 0xcc8
			 0x61729b0f, /// 0xccc
			 0x774eb799, /// 0xcd0
			 0xa21fb867, /// 0xcd4
			 0x1a765731, /// 0xcd8
			 0x0b445078, /// 0xcdc
			 0x958960d1, /// 0xce0
			 0x887a1923, /// 0xce4
			 0x1931ff13, /// 0xce8
			 0xa4b8acdd, /// 0xcec
			 0x3ba18e33, /// 0xcf0
			 0xe7fd9ff0, /// 0xcf4
			 0x564fcf08, /// 0xcf8
			 0xfdc1ac1b, /// 0xcfc
			 0xa0774e4e, /// 0xd00
			 0x48b3ddbf, /// 0xd04
			 0x72832bd0, /// 0xd08
			 0x5627f7fb, /// 0xd0c
			 0x3291a77a, /// 0xd10
			 0x3cf67635, /// 0xd14
			 0x1a3aad77, /// 0xd18
			 0x05556b9f, /// 0xd1c
			 0xbeb9342f, /// 0xd20
			 0xabd78ec0, /// 0xd24
			 0xcaeac2fd, /// 0xd28
			 0x837dd710, /// 0xd2c
			 0x02258a20, /// 0xd30
			 0x907fd5ee, /// 0xd34
			 0x18fe35d1, /// 0xd38
			 0x4b8aecee, /// 0xd3c
			 0xb577e71c, /// 0xd40
			 0x68d4d0e5, /// 0xd44
			 0x71030e53, /// 0xd48
			 0x220ed2ad, /// 0xd4c
			 0x440d0ff5, /// 0xd50
			 0x350ff6de, /// 0xd54
			 0xcc8b9193, /// 0xd58
			 0xc5cc89c8, /// 0xd5c
			 0x876c9463, /// 0xd60
			 0xf1fb3735, /// 0xd64
			 0x1aa54d9f, /// 0xd68
			 0xcb428326, /// 0xd6c
			 0xa3e75719, /// 0xd70
			 0xc6eb5690, /// 0xd74
			 0x5e5120d8, /// 0xd78
			 0x3c81550b, /// 0xd7c
			 0x8e453e60, /// 0xd80
			 0x74867136, /// 0xd84
			 0x76bd5738, /// 0xd88
			 0x7ca6860a, /// 0xd8c
			 0xb79831c3, /// 0xd90
			 0x19c0d417, /// 0xd94
			 0x7f2eb273, /// 0xd98
			 0x200d05dc, /// 0xd9c
			 0xd8d6c97d, /// 0xda0
			 0x8c84a635, /// 0xda4
			 0x0cde496d, /// 0xda8
			 0x5b94b02f, /// 0xdac
			 0x253a65f2, /// 0xdb0
			 0xa27db12f, /// 0xdb4
			 0x74a0074a, /// 0xdb8
			 0xf976966a, /// 0xdbc
			 0x6aa55102, /// 0xdc0
			 0x5f5bd841, /// 0xdc4
			 0x02d09eb5, /// 0xdc8
			 0xaf89feea, /// 0xdcc
			 0x08299575, /// 0xdd0
			 0x320b69aa, /// 0xdd4
			 0xce73388b, /// 0xdd8
			 0x948c1739, /// 0xddc
			 0x0e1705cd, /// 0xde0
			 0x338aa998, /// 0xde4
			 0x4e6920c4, /// 0xde8
			 0x0d5aad66, /// 0xdec
			 0x28782a43, /// 0xdf0
			 0xf0ec53a0, /// 0xdf4
			 0x3bcd8ff2, /// 0xdf8
			 0xcf73083e, /// 0xdfc
			 0x202a15c3, /// 0xe00
			 0xe623fd75, /// 0xe04
			 0xec467558, /// 0xe08
			 0xbbc6a19b, /// 0xe0c
			 0x2f709101, /// 0xe10
			 0x0e1cefe7, /// 0xe14
			 0xe98bf37b, /// 0xe18
			 0xb551cd94, /// 0xe1c
			 0x565135ba, /// 0xe20
			 0xe24f8b5f, /// 0xe24
			 0x4176ac0a, /// 0xe28
			 0x2ce630a6, /// 0xe2c
			 0xc0f0c8a0, /// 0xe30
			 0x0d946e72, /// 0xe34
			 0xfae60d59, /// 0xe38
			 0x481c5fe4, /// 0xe3c
			 0x6e32985a, /// 0xe40
			 0x0580f9bb, /// 0xe44
			 0x9f9cc7d4, /// 0xe48
			 0xd7eda59d, /// 0xe4c
			 0xcce291f2, /// 0xe50
			 0xa0e0c461, /// 0xe54
			 0x968de473, /// 0xe58
			 0x1266a055, /// 0xe5c
			 0xf0f0e5d6, /// 0xe60
			 0x54eb6ac6, /// 0xe64
			 0xf38bf00d, /// 0xe68
			 0xabd132cf, /// 0xe6c
			 0x639a6329, /// 0xe70
			 0x3a30af81, /// 0xe74
			 0x45e10a5c, /// 0xe78
			 0x87abe276, /// 0xe7c
			 0xa8fd5eba, /// 0xe80
			 0x7aeee853, /// 0xe84
			 0x96e5e33c, /// 0xe88
			 0x710e16cd, /// 0xe8c
			 0xd34045a2, /// 0xe90
			 0x6d984e0d, /// 0xe94
			 0xc6b2cc6a, /// 0xe98
			 0x27a42ea4, /// 0xe9c
			 0x07733632, /// 0xea0
			 0xe7b400c0, /// 0xea4
			 0x761da082, /// 0xea8
			 0xc5c6b129, /// 0xeac
			 0x5d80db96, /// 0xeb0
			 0x1188f515, /// 0xeb4
			 0x13cbf4d2, /// 0xeb8
			 0xde9684c6, /// 0xebc
			 0x4b490d81, /// 0xec0
			 0xf6cf7d7e, /// 0xec4
			 0xb35f48a9, /// 0xec8
			 0x0521ee0c, /// 0xecc
			 0xef1a81cd, /// 0xed0
			 0x597029ea, /// 0xed4
			 0xcfec487a, /// 0xed8
			 0x49c432d3, /// 0xedc
			 0x122642d0, /// 0xee0
			 0x20b71e0a, /// 0xee4
			 0x42d91cb0, /// 0xee8
			 0x786ef2ca, /// 0xeec
			 0x19a26c5b, /// 0xef0
			 0x9e7b687b, /// 0xef4
			 0xf13fa246, /// 0xef8
			 0xe3a5bdff, /// 0xefc
			 0x2b530fa8, /// 0xf00
			 0xa302c9aa, /// 0xf04
			 0x024ff1e3, /// 0xf08
			 0x172d623f, /// 0xf0c
			 0x5617f3e9, /// 0xf10
			 0xf46a7b71, /// 0xf14
			 0x61998df0, /// 0xf18
			 0x5619c1d5, /// 0xf1c
			 0x3fd573c0, /// 0xf20
			 0x321c6c96, /// 0xf24
			 0x45c286e2, /// 0xf28
			 0xc5410a3d, /// 0xf2c
			 0x614c0989, /// 0xf30
			 0x2f278338, /// 0xf34
			 0xeeb13374, /// 0xf38
			 0xf9f1bcd8, /// 0xf3c
			 0xa29d27ab, /// 0xf40
			 0x7fe87709, /// 0xf44
			 0x117b2bf6, /// 0xf48
			 0x241ad724, /// 0xf4c
			 0x6595d232, /// 0xf50
			 0x123a46f9, /// 0xf54
			 0xdc8d0486, /// 0xf58
			 0x3d0b57f7, /// 0xf5c
			 0x10c53d49, /// 0xf60
			 0xf5551326, /// 0xf64
			 0x402584f6, /// 0xf68
			 0xbc5e4b8d, /// 0xf6c
			 0xc79a9e6c, /// 0xf70
			 0x8f294c75, /// 0xf74
			 0xca666da9, /// 0xf78
			 0xc950a091, /// 0xf7c
			 0xcea15d55, /// 0xf80
			 0x35fccdd9, /// 0xf84
			 0x4d99b32b, /// 0xf88
			 0xd5bf5c98, /// 0xf8c
			 0x8b83ade2, /// 0xf90
			 0x36d4669e, /// 0xf94
			 0x582250e4, /// 0xf98
			 0x347a23bc, /// 0xf9c
			 0x2fd84cac, /// 0xfa0
			 0x2d891554, /// 0xfa4
			 0xe05ac5cf, /// 0xfa8
			 0x9775ff50, /// 0xfac
			 0xbcc1f657, /// 0xfb0
			 0x53bcb37d, /// 0xfb4
			 0x5f2c7ee4, /// 0xfb8
			 0x652488b7, /// 0xfbc
			 0xcfc011a8, /// 0xfc0
			 0x90a75d41, /// 0xfc4
			 0x39478040, /// 0xfc8
			 0x6e536e61, /// 0xfcc
			 0xa4929367, /// 0xfd0
			 0xba1bf8ed, /// 0xfd4
			 0xf89ac3da, /// 0xfd8
			 0x5c342d6b, /// 0xfdc
			 0x93c9ca8a, /// 0xfe0
			 0xf35c6420, /// 0xfe4
			 0xf76fdf56, /// 0xfe8
			 0x4032bbe4, /// 0xfec
			 0x8ea87aa7, /// 0xff0
			 0x57dffc97, /// 0xff4
			 0x79118a00, /// 0xff8
			 0xc0bb9975 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x3f78e8e4, /// 0x0
			 0x11f14fa1, /// 0x4
			 0x2ef09f77, /// 0x8
			 0x328ca6b9, /// 0xc
			 0xa2279375, /// 0x10
			 0x63635565, /// 0x14
			 0xc55df6c0, /// 0x18
			 0x71ea993f, /// 0x1c
			 0xca1b5d8f, /// 0x20
			 0xe07e16a7, /// 0x24
			 0x4c9c5d60, /// 0x28
			 0xc81e948b, /// 0x2c
			 0x95546f9d, /// 0x30
			 0x4ce2799d, /// 0x34
			 0x1d67707e, /// 0x38
			 0x09f1dd66, /// 0x3c
			 0xe29dd43c, /// 0x40
			 0xf7182b69, /// 0x44
			 0xb946c335, /// 0x48
			 0xdaccc9b2, /// 0x4c
			 0xc49dfc45, /// 0x50
			 0x7c7e94c6, /// 0x54
			 0x6bdaedd4, /// 0x58
			 0xd3be80ed, /// 0x5c
			 0xc12d7096, /// 0x60
			 0x9c945855, /// 0x64
			 0x678f4b88, /// 0x68
			 0x252393ce, /// 0x6c
			 0x2871dd8d, /// 0x70
			 0x3dcb0c0a, /// 0x74
			 0xb5754f12, /// 0x78
			 0x40dcece4, /// 0x7c
			 0x42ca51ad, /// 0x80
			 0x17b35126, /// 0x84
			 0x3284e1fb, /// 0x88
			 0xccb9fd45, /// 0x8c
			 0x4cde486d, /// 0x90
			 0x1caa7ddb, /// 0x94
			 0x66289cb6, /// 0x98
			 0xa07ae655, /// 0x9c
			 0xd18cdcd1, /// 0xa0
			 0xc75154c3, /// 0xa4
			 0xe63a2bad, /// 0xa8
			 0xe5e141bf, /// 0xac
			 0xa65d069c, /// 0xb0
			 0x6eddeaa4, /// 0xb4
			 0xce21f6e7, /// 0xb8
			 0xc85b8246, /// 0xbc
			 0xabd48c35, /// 0xc0
			 0xa8cf69e1, /// 0xc4
			 0x24bd9239, /// 0xc8
			 0x47e4399a, /// 0xcc
			 0xe3ab4960, /// 0xd0
			 0x9ac25dfd, /// 0xd4
			 0x34e2cb44, /// 0xd8
			 0xfdabae72, /// 0xdc
			 0xa2efa9a5, /// 0xe0
			 0x506bea11, /// 0xe4
			 0xd03f571a, /// 0xe8
			 0x61b4c8ba, /// 0xec
			 0x103f3501, /// 0xf0
			 0xb00d6d14, /// 0xf4
			 0xb51849e2, /// 0xf8
			 0x8d3eca9b, /// 0xfc
			 0x9f18da19, /// 0x100
			 0x6a705c73, /// 0x104
			 0x50ceb5da, /// 0x108
			 0x25d1e127, /// 0x10c
			 0xc4ac6f53, /// 0x110
			 0x2bfce919, /// 0x114
			 0x364a1418, /// 0x118
			 0x722269dc, /// 0x11c
			 0xf3473eae, /// 0x120
			 0xfee30375, /// 0x124
			 0x40af46c6, /// 0x128
			 0x9a94268b, /// 0x12c
			 0xdf7570ac, /// 0x130
			 0x4b30a31c, /// 0x134
			 0x2b2ce06c, /// 0x138
			 0x47aa9b63, /// 0x13c
			 0xaed475e6, /// 0x140
			 0x18f66dda, /// 0x144
			 0x982034f7, /// 0x148
			 0x2c138070, /// 0x14c
			 0x7e241de2, /// 0x150
			 0xa840ae60, /// 0x154
			 0xeb21b9ae, /// 0x158
			 0xbfe877fe, /// 0x15c
			 0x4e94a2e3, /// 0x160
			 0x251f94c3, /// 0x164
			 0x8fe17f29, /// 0x168
			 0xa7a3acb3, /// 0x16c
			 0xcc82393e, /// 0x170
			 0xa7e94e1c, /// 0x174
			 0x6930af27, /// 0x178
			 0x8353bd50, /// 0x17c
			 0x06bcdf26, /// 0x180
			 0xb27ed8d6, /// 0x184
			 0xc4c022f0, /// 0x188
			 0xf2e3eb96, /// 0x18c
			 0xc7b777a2, /// 0x190
			 0xa85d0e52, /// 0x194
			 0x51ba6eda, /// 0x198
			 0x468ab63f, /// 0x19c
			 0x3b526191, /// 0x1a0
			 0x5ce219f1, /// 0x1a4
			 0xd4870a43, /// 0x1a8
			 0x45666ef7, /// 0x1ac
			 0x760249b5, /// 0x1b0
			 0xc975b42a, /// 0x1b4
			 0x37c38c9a, /// 0x1b8
			 0x579300fc, /// 0x1bc
			 0x3eb8f163, /// 0x1c0
			 0x7912edc0, /// 0x1c4
			 0xe659c407, /// 0x1c8
			 0x52bf284e, /// 0x1cc
			 0xc71f337d, /// 0x1d0
			 0xf833f002, /// 0x1d4
			 0x3fc55a79, /// 0x1d8
			 0xe92ed787, /// 0x1dc
			 0xf340719c, /// 0x1e0
			 0xd0ea7cd8, /// 0x1e4
			 0x5ab94f85, /// 0x1e8
			 0xf5a7c446, /// 0x1ec
			 0xa0144407, /// 0x1f0
			 0xc18ce1e8, /// 0x1f4
			 0x80a83c98, /// 0x1f8
			 0xae2d3cbf, /// 0x1fc
			 0xc7334c7c, /// 0x200
			 0x56f3a06e, /// 0x204
			 0x65d79c44, /// 0x208
			 0x0252cbf0, /// 0x20c
			 0x05d2938c, /// 0x210
			 0x2bf6bb44, /// 0x214
			 0xbd562a35, /// 0x218
			 0x142ffc82, /// 0x21c
			 0x74e9c2eb, /// 0x220
			 0xdc21131a, /// 0x224
			 0x8eb1d5a4, /// 0x228
			 0x064af0d6, /// 0x22c
			 0xd67492d0, /// 0x230
			 0xb455e1cf, /// 0x234
			 0x439d6f84, /// 0x238
			 0x875058ea, /// 0x23c
			 0x517ee157, /// 0x240
			 0xf972aff7, /// 0x244
			 0xde49ea8c, /// 0x248
			 0x6dcabe1f, /// 0x24c
			 0x98c64504, /// 0x250
			 0xd434887a, /// 0x254
			 0x53d69b9b, /// 0x258
			 0x170783fa, /// 0x25c
			 0x55d51b40, /// 0x260
			 0x9cfcac23, /// 0x264
			 0x4ad8e520, /// 0x268
			 0x0043b180, /// 0x26c
			 0x7113198f, /// 0x270
			 0xb70fd7a2, /// 0x274
			 0x0d58bf26, /// 0x278
			 0xd3f6d6d4, /// 0x27c
			 0x5c045095, /// 0x280
			 0x72da77cf, /// 0x284
			 0x4008e338, /// 0x288
			 0x7590c903, /// 0x28c
			 0xfecc4b8c, /// 0x290
			 0x061db6e8, /// 0x294
			 0xe96e27b5, /// 0x298
			 0x425bf1d9, /// 0x29c
			 0xc4180eda, /// 0x2a0
			 0xec5e5198, /// 0x2a4
			 0xc330d87b, /// 0x2a8
			 0x6da34167, /// 0x2ac
			 0x3f8c3927, /// 0x2b0
			 0x4a929152, /// 0x2b4
			 0xf6d2365f, /// 0x2b8
			 0x346c25e8, /// 0x2bc
			 0x15a9cd02, /// 0x2c0
			 0x3683472f, /// 0x2c4
			 0xb863818b, /// 0x2c8
			 0x1d5d4f84, /// 0x2cc
			 0x32f19f81, /// 0x2d0
			 0xe03331f6, /// 0x2d4
			 0xc2ef44ae, /// 0x2d8
			 0x089d37ea, /// 0x2dc
			 0x50ae9fbe, /// 0x2e0
			 0x036b6991, /// 0x2e4
			 0x41282097, /// 0x2e8
			 0xf7a66bff, /// 0x2ec
			 0x69e479fb, /// 0x2f0
			 0x26ebb3d3, /// 0x2f4
			 0xe08c44fd, /// 0x2f8
			 0xd2939d33, /// 0x2fc
			 0x1aecd815, /// 0x300
			 0x935d4dc8, /// 0x304
			 0x2e8d9657, /// 0x308
			 0x06815977, /// 0x30c
			 0xae0c5433, /// 0x310
			 0xcf646455, /// 0x314
			 0xaa0c56ce, /// 0x318
			 0x7358668c, /// 0x31c
			 0xc1cf322b, /// 0x320
			 0x764228c6, /// 0x324
			 0x88d3d6a3, /// 0x328
			 0x7a5cf57b, /// 0x32c
			 0x59c60ab6, /// 0x330
			 0xa8bc3698, /// 0x334
			 0x6e573962, /// 0x338
			 0x4e45154b, /// 0x33c
			 0x66d5b382, /// 0x340
			 0xae6a6fbb, /// 0x344
			 0x612f5b86, /// 0x348
			 0x48be1f86, /// 0x34c
			 0xc6ffe4cf, /// 0x350
			 0x8be9d13c, /// 0x354
			 0xd0a63202, /// 0x358
			 0x3cab7c33, /// 0x35c
			 0xac85feaf, /// 0x360
			 0xaf3ba345, /// 0x364
			 0x0a0a0ac5, /// 0x368
			 0x37da7b6d, /// 0x36c
			 0x7b3ec5a9, /// 0x370
			 0x905ff176, /// 0x374
			 0xa0041a1a, /// 0x378
			 0x9c93a552, /// 0x37c
			 0x8ead0872, /// 0x380
			 0xf338d852, /// 0x384
			 0x3d63720f, /// 0x388
			 0xb818fbe1, /// 0x38c
			 0x8c7f74a9, /// 0x390
			 0xf300f293, /// 0x394
			 0x3de9564d, /// 0x398
			 0x09bf3403, /// 0x39c
			 0x37915279, /// 0x3a0
			 0x827b42be, /// 0x3a4
			 0xbb5cb4db, /// 0x3a8
			 0xe9686eb8, /// 0x3ac
			 0x6fdb68f2, /// 0x3b0
			 0xca1493e7, /// 0x3b4
			 0xe20af8da, /// 0x3b8
			 0x25fa6798, /// 0x3bc
			 0xd3767337, /// 0x3c0
			 0xfe41fdf4, /// 0x3c4
			 0x5ee4198f, /// 0x3c8
			 0xf5a492fb, /// 0x3cc
			 0x253d114e, /// 0x3d0
			 0xe7632fa5, /// 0x3d4
			 0x940438f3, /// 0x3d8
			 0xff65235c, /// 0x3dc
			 0x29177347, /// 0x3e0
			 0x2c59ac93, /// 0x3e4
			 0x1ea154c0, /// 0x3e8
			 0xd5ba5b12, /// 0x3ec
			 0xbd758892, /// 0x3f0
			 0x1b75d841, /// 0x3f4
			 0xcb3491e2, /// 0x3f8
			 0x5269340d, /// 0x3fc
			 0xc9e22570, /// 0x400
			 0xfe6ffb5a, /// 0x404
			 0x3dd94dd1, /// 0x408
			 0x841b784c, /// 0x40c
			 0x9ef62a35, /// 0x410
			 0x6244bf52, /// 0x414
			 0x6ab7fe75, /// 0x418
			 0x2ae16a57, /// 0x41c
			 0xcf854cd7, /// 0x420
			 0xa054915c, /// 0x424
			 0x1e2ea73a, /// 0x428
			 0x97fe197f, /// 0x42c
			 0xab5f190c, /// 0x430
			 0x1c66eac2, /// 0x434
			 0xf781b200, /// 0x438
			 0xcc84ee9b, /// 0x43c
			 0x050bf2c6, /// 0x440
			 0xfb0de237, /// 0x444
			 0xa64c38ab, /// 0x448
			 0x8062ddce, /// 0x44c
			 0x0f7bdcf8, /// 0x450
			 0x40bde7d4, /// 0x454
			 0x2dd98d09, /// 0x458
			 0x563556df, /// 0x45c
			 0xe91180d0, /// 0x460
			 0x4b9c98c6, /// 0x464
			 0x55006028, /// 0x468
			 0xdcfdad77, /// 0x46c
			 0x9cf7bf9f, /// 0x470
			 0x7d872118, /// 0x474
			 0x5f96bbcf, /// 0x478
			 0x37cf1223, /// 0x47c
			 0xdf4303b1, /// 0x480
			 0x3dfea27c, /// 0x484
			 0x25c8d29b, /// 0x488
			 0xb9514413, /// 0x48c
			 0xf833d6d6, /// 0x490
			 0xc834f18e, /// 0x494
			 0x6d9d2053, /// 0x498
			 0x31029566, /// 0x49c
			 0xea7e389b, /// 0x4a0
			 0x32b6ae8c, /// 0x4a4
			 0x2dc9c33b, /// 0x4a8
			 0x5749f7a0, /// 0x4ac
			 0xbae7835b, /// 0x4b0
			 0xa2d150fc, /// 0x4b4
			 0x09c7fd64, /// 0x4b8
			 0xf8c80358, /// 0x4bc
			 0x8d578aac, /// 0x4c0
			 0x35ea52b4, /// 0x4c4
			 0xc1066975, /// 0x4c8
			 0x239c095a, /// 0x4cc
			 0xd756affb, /// 0x4d0
			 0x4bf8eaee, /// 0x4d4
			 0x447e27d2, /// 0x4d8
			 0xa63d4172, /// 0x4dc
			 0xb8f9ef5a, /// 0x4e0
			 0x0ba39edb, /// 0x4e4
			 0x18c417f9, /// 0x4e8
			 0xe3d170ad, /// 0x4ec
			 0x52856c10, /// 0x4f0
			 0xe2214c2f, /// 0x4f4
			 0x7bbc33a9, /// 0x4f8
			 0xce16ec7e, /// 0x4fc
			 0xb734788c, /// 0x500
			 0x9286d840, /// 0x504
			 0xee71d673, /// 0x508
			 0xfde5585c, /// 0x50c
			 0x5f6ec856, /// 0x510
			 0x1994f89c, /// 0x514
			 0x2a9dfc26, /// 0x518
			 0xc686b9af, /// 0x51c
			 0x83226df3, /// 0x520
			 0xd3e112fb, /// 0x524
			 0x3a447b19, /// 0x528
			 0xd345450b, /// 0x52c
			 0xf6543468, /// 0x530
			 0x782fbfe9, /// 0x534
			 0x6d09c04b, /// 0x538
			 0xb45857c9, /// 0x53c
			 0x2168dcd5, /// 0x540
			 0xffdab872, /// 0x544
			 0x59f674ab, /// 0x548
			 0xf1cb3a0d, /// 0x54c
			 0x36a8504f, /// 0x550
			 0x8cf638a9, /// 0x554
			 0x06ea0141, /// 0x558
			 0x3cec9343, /// 0x55c
			 0x1de89f41, /// 0x560
			 0x920ffd93, /// 0x564
			 0xe4195378, /// 0x568
			 0x7e952bf6, /// 0x56c
			 0xf6a14341, /// 0x570
			 0x0facc51d, /// 0x574
			 0x4b2e3d78, /// 0x578
			 0x0f7aa628, /// 0x57c
			 0x4a8451e5, /// 0x580
			 0x764f305b, /// 0x584
			 0x9358c669, /// 0x588
			 0x61844cae, /// 0x58c
			 0xef845f6d, /// 0x590
			 0xf11f82b5, /// 0x594
			 0xc3634c8a, /// 0x598
			 0xd28b66a3, /// 0x59c
			 0xa26577f3, /// 0x5a0
			 0xdc8355dd, /// 0x5a4
			 0x74c2ab3b, /// 0x5a8
			 0x1371bb47, /// 0x5ac
			 0x60a5362f, /// 0x5b0
			 0x1015444b, /// 0x5b4
			 0x7b41cdcb, /// 0x5b8
			 0xacb00d2d, /// 0x5bc
			 0x726d66c9, /// 0x5c0
			 0xa8827f91, /// 0x5c4
			 0xdf7b7a4a, /// 0x5c8
			 0x3c237026, /// 0x5cc
			 0x2ac196b5, /// 0x5d0
			 0x13b01eac, /// 0x5d4
			 0xe853cae5, /// 0x5d8
			 0x4cafc481, /// 0x5dc
			 0x38c6e62b, /// 0x5e0
			 0xde7754b0, /// 0x5e4
			 0x88acbda5, /// 0x5e8
			 0xab01738f, /// 0x5ec
			 0x587d7184, /// 0x5f0
			 0x27fc4f11, /// 0x5f4
			 0x596fe3a7, /// 0x5f8
			 0xeb4794cb, /// 0x5fc
			 0x5a7dad0a, /// 0x600
			 0x5eaa8363, /// 0x604
			 0xf3dec550, /// 0x608
			 0x6f434c00, /// 0x60c
			 0xe0d42287, /// 0x610
			 0xe84bd3d6, /// 0x614
			 0x07017eee, /// 0x618
			 0x20005544, /// 0x61c
			 0xbd5d44a5, /// 0x620
			 0x0d95732d, /// 0x624
			 0x2d6c1e7f, /// 0x628
			 0x0315b20f, /// 0x62c
			 0x54aa9dca, /// 0x630
			 0x8a286220, /// 0x634
			 0x390b3ea8, /// 0x638
			 0x67e51975, /// 0x63c
			 0x862b9428, /// 0x640
			 0xcb55ac82, /// 0x644
			 0xd1d3622b, /// 0x648
			 0x4cebc9eb, /// 0x64c
			 0xaae545cc, /// 0x650
			 0x08f2b9b2, /// 0x654
			 0x4fc1ff27, /// 0x658
			 0x500ef97f, /// 0x65c
			 0x2796dc15, /// 0x660
			 0xe4d39733, /// 0x664
			 0xac2e5556, /// 0x668
			 0x994f1719, /// 0x66c
			 0x4b861edb, /// 0x670
			 0x7f1ebee7, /// 0x674
			 0xd54af4c8, /// 0x678
			 0xf3325685, /// 0x67c
			 0xe787f625, /// 0x680
			 0x78364665, /// 0x684
			 0x6c7dfe17, /// 0x688
			 0xc10fb61e, /// 0x68c
			 0xd3c11616, /// 0x690
			 0xce065a26, /// 0x694
			 0x30205fca, /// 0x698
			 0x023b853b, /// 0x69c
			 0xb596a9f3, /// 0x6a0
			 0x7443d9d9, /// 0x6a4
			 0xc86daef2, /// 0x6a8
			 0xa9e49f23, /// 0x6ac
			 0xb03a16f6, /// 0x6b0
			 0x73c37ad6, /// 0x6b4
			 0xc2eac135, /// 0x6b8
			 0x68fdd774, /// 0x6bc
			 0xc0598d72, /// 0x6c0
			 0x1771c41c, /// 0x6c4
			 0xebd2d0fe, /// 0x6c8
			 0x1369ea48, /// 0x6cc
			 0xb5678312, /// 0x6d0
			 0xeb51ea66, /// 0x6d4
			 0xc4546ca1, /// 0x6d8
			 0x5e116b3b, /// 0x6dc
			 0x803d63ba, /// 0x6e0
			 0x0813d7d1, /// 0x6e4
			 0xf658969e, /// 0x6e8
			 0xac339f03, /// 0x6ec
			 0xa2b5f93f, /// 0x6f0
			 0x879b00f4, /// 0x6f4
			 0x5e071a4a, /// 0x6f8
			 0x93f4cbfa, /// 0x6fc
			 0xd1127e94, /// 0x700
			 0x92070f53, /// 0x704
			 0xee5cf938, /// 0x708
			 0xfbcc59ea, /// 0x70c
			 0x247bbc0e, /// 0x710
			 0xadd136e7, /// 0x714
			 0xe63e2773, /// 0x718
			 0x3ce2039d, /// 0x71c
			 0x5fae553d, /// 0x720
			 0xce0bc619, /// 0x724
			 0xce37a7a7, /// 0x728
			 0x204a9c35, /// 0x72c
			 0x806d3f7c, /// 0x730
			 0x38a0ba0b, /// 0x734
			 0xa3709432, /// 0x738
			 0x727eb9eb, /// 0x73c
			 0xcf18f23c, /// 0x740
			 0x3ac641f9, /// 0x744
			 0xa4369b9f, /// 0x748
			 0x761b2d9e, /// 0x74c
			 0x69f44194, /// 0x750
			 0xfb6c06a2, /// 0x754
			 0x4f615627, /// 0x758
			 0x2a37822d, /// 0x75c
			 0x709b99f1, /// 0x760
			 0xa4df0f5a, /// 0x764
			 0x67751731, /// 0x768
			 0xc240a5f4, /// 0x76c
			 0x63a4ae1b, /// 0x770
			 0x466a26fd, /// 0x774
			 0xa4f34db4, /// 0x778
			 0x220e78e5, /// 0x77c
			 0xaf4b1380, /// 0x780
			 0xd34cab4d, /// 0x784
			 0x324a88ce, /// 0x788
			 0xcd5e0bea, /// 0x78c
			 0xac008acf, /// 0x790
			 0xf9689857, /// 0x794
			 0x175f7160, /// 0x798
			 0x5b55401c, /// 0x79c
			 0x7ef50297, /// 0x7a0
			 0x8798c481, /// 0x7a4
			 0x869e48ee, /// 0x7a8
			 0x61f92d4e, /// 0x7ac
			 0xf18e8669, /// 0x7b0
			 0x6ff9cc08, /// 0x7b4
			 0xb2da2022, /// 0x7b8
			 0x784dbcc4, /// 0x7bc
			 0x16674302, /// 0x7c0
			 0x60eb68fa, /// 0x7c4
			 0x4e93ba08, /// 0x7c8
			 0xd55c1eb0, /// 0x7cc
			 0x1b06de41, /// 0x7d0
			 0x9adb7698, /// 0x7d4
			 0x857e0a50, /// 0x7d8
			 0x5e9fd1d1, /// 0x7dc
			 0xb324a1af, /// 0x7e0
			 0x781c33ff, /// 0x7e4
			 0x54c7ba87, /// 0x7e8
			 0x04821890, /// 0x7ec
			 0x4ca427a3, /// 0x7f0
			 0x3cbf0624, /// 0x7f4
			 0x123f7090, /// 0x7f8
			 0x8b90ed07, /// 0x7fc
			 0x52a2617d, /// 0x800
			 0xe81e440e, /// 0x804
			 0xe1a1a387, /// 0x808
			 0x0374d827, /// 0x80c
			 0x958cf7f7, /// 0x810
			 0x5cbc452a, /// 0x814
			 0x3214d097, /// 0x818
			 0x406870d1, /// 0x81c
			 0xa959ed04, /// 0x820
			 0xf6a7b88f, /// 0x824
			 0x57230111, /// 0x828
			 0x45cec888, /// 0x82c
			 0x2129502b, /// 0x830
			 0xb1b59503, /// 0x834
			 0xfa7ae917, /// 0x838
			 0xb061ffb3, /// 0x83c
			 0xf6d3a1b3, /// 0x840
			 0xc1011965, /// 0x844
			 0x5f79adcc, /// 0x848
			 0x0fa69faa, /// 0x84c
			 0x1fa1b4da, /// 0x850
			 0x9a9f300e, /// 0x854
			 0x07cb0d1c, /// 0x858
			 0x1885348e, /// 0x85c
			 0x25940518, /// 0x860
			 0x43d1270c, /// 0x864
			 0xbff99c2c, /// 0x868
			 0xd30914d5, /// 0x86c
			 0x0cce5de2, /// 0x870
			 0x4d68b6fa, /// 0x874
			 0x8aa29ff7, /// 0x878
			 0x346b7168, /// 0x87c
			 0x45924fe3, /// 0x880
			 0xb941e28d, /// 0x884
			 0x651901d2, /// 0x888
			 0xab77ce9e, /// 0x88c
			 0xc90483b7, /// 0x890
			 0xe6e27be2, /// 0x894
			 0x543c610a, /// 0x898
			 0x8bbea3b6, /// 0x89c
			 0x3645ea5f, /// 0x8a0
			 0x8756c723, /// 0x8a4
			 0xc4f73d13, /// 0x8a8
			 0x6aafe772, /// 0x8ac
			 0xc938593f, /// 0x8b0
			 0xca88db12, /// 0x8b4
			 0xc0095891, /// 0x8b8
			 0xaaa5e422, /// 0x8bc
			 0x478f3664, /// 0x8c0
			 0x3cc9089c, /// 0x8c4
			 0xf88c2df6, /// 0x8c8
			 0xe2a552b0, /// 0x8cc
			 0x0f94bb29, /// 0x8d0
			 0xc53f8dfa, /// 0x8d4
			 0x65876577, /// 0x8d8
			 0x3c30b988, /// 0x8dc
			 0x0465fbc1, /// 0x8e0
			 0xaba22f26, /// 0x8e4
			 0x3afca56c, /// 0x8e8
			 0xe01f27df, /// 0x8ec
			 0xe1fa2fcf, /// 0x8f0
			 0x4384784d, /// 0x8f4
			 0x27f0931d, /// 0x8f8
			 0x2f0b7e70, /// 0x8fc
			 0x8c638c9d, /// 0x900
			 0x8fc11ad9, /// 0x904
			 0x8174464c, /// 0x908
			 0xe07cc9ce, /// 0x90c
			 0x7b841735, /// 0x910
			 0x4477927f, /// 0x914
			 0x21a4526a, /// 0x918
			 0x053250ea, /// 0x91c
			 0x6bdab0b8, /// 0x920
			 0x0510f965, /// 0x924
			 0xe392e732, /// 0x928
			 0xdf63b743, /// 0x92c
			 0xcba5ebe7, /// 0x930
			 0xc1a4d922, /// 0x934
			 0xf1a1798b, /// 0x938
			 0x14414547, /// 0x93c
			 0xfaa9de19, /// 0x940
			 0x73bdf282, /// 0x944
			 0x25ea2f27, /// 0x948
			 0xd4149c80, /// 0x94c
			 0x7feb312f, /// 0x950
			 0x52bfba0b, /// 0x954
			 0xe1183fc8, /// 0x958
			 0xf6dbef78, /// 0x95c
			 0x114621f9, /// 0x960
			 0x855a7d9e, /// 0x964
			 0x15a871e6, /// 0x968
			 0x81518216, /// 0x96c
			 0x6b9e169c, /// 0x970
			 0xf48aa38c, /// 0x974
			 0xddac8352, /// 0x978
			 0x8cd67ee9, /// 0x97c
			 0x9a9983c1, /// 0x980
			 0xa67006cc, /// 0x984
			 0x47a2d5ad, /// 0x988
			 0xad0dd33c, /// 0x98c
			 0xf43eeb5e, /// 0x990
			 0xb24ede5f, /// 0x994
			 0x5bfebc7f, /// 0x998
			 0x78945c77, /// 0x99c
			 0xe708782b, /// 0x9a0
			 0xb1399559, /// 0x9a4
			 0xe8d8d32a, /// 0x9a8
			 0xb84da353, /// 0x9ac
			 0x2757d674, /// 0x9b0
			 0x07a092d7, /// 0x9b4
			 0xa2a68494, /// 0x9b8
			 0x3b001bf3, /// 0x9bc
			 0x76163095, /// 0x9c0
			 0xe960d011, /// 0x9c4
			 0x70a3edf1, /// 0x9c8
			 0xa21befa5, /// 0x9cc
			 0x9da74547, /// 0x9d0
			 0x3fc65673, /// 0x9d4
			 0xed384c2a, /// 0x9d8
			 0x8b36917c, /// 0x9dc
			 0xc6309ceb, /// 0x9e0
			 0xb132714d, /// 0x9e4
			 0xf5e6b755, /// 0x9e8
			 0x717add0f, /// 0x9ec
			 0x4e1cce7e, /// 0x9f0
			 0xc4c2eb3e, /// 0x9f4
			 0xa6266a20, /// 0x9f8
			 0x91c3ac69, /// 0x9fc
			 0xd77b1739, /// 0xa00
			 0x8e06b90b, /// 0xa04
			 0xac4a79f8, /// 0xa08
			 0x2ba1dc84, /// 0xa0c
			 0x55e3d67d, /// 0xa10
			 0xb1e40924, /// 0xa14
			 0x6e892d41, /// 0xa18
			 0x604f9d47, /// 0xa1c
			 0xcecca5b8, /// 0xa20
			 0x2ac2571b, /// 0xa24
			 0xf60571a8, /// 0xa28
			 0xf96ff434, /// 0xa2c
			 0x09ac85b4, /// 0xa30
			 0xf90f9b25, /// 0xa34
			 0x0f838e94, /// 0xa38
			 0x3c98fe5c, /// 0xa3c
			 0x87b21266, /// 0xa40
			 0xf54dbfb6, /// 0xa44
			 0x071d856e, /// 0xa48
			 0xddcff5f0, /// 0xa4c
			 0x31afb4b2, /// 0xa50
			 0x202eb0c4, /// 0xa54
			 0x624bcb19, /// 0xa58
			 0xa55e4e60, /// 0xa5c
			 0x11a332b5, /// 0xa60
			 0xfd959557, /// 0xa64
			 0xfc5c7908, /// 0xa68
			 0xeaba50b7, /// 0xa6c
			 0xd4a95e91, /// 0xa70
			 0x73405239, /// 0xa74
			 0x209dac16, /// 0xa78
			 0x93bce27d, /// 0xa7c
			 0x7d641b12, /// 0xa80
			 0x6d49663e, /// 0xa84
			 0x6f0459c7, /// 0xa88
			 0x2d559c1b, /// 0xa8c
			 0x44316295, /// 0xa90
			 0x551eab26, /// 0xa94
			 0xb1718b4f, /// 0xa98
			 0xa5d7f0e3, /// 0xa9c
			 0xcb0d815c, /// 0xaa0
			 0xa3a722d2, /// 0xaa4
			 0xb5941510, /// 0xaa8
			 0x90424385, /// 0xaac
			 0x298116aa, /// 0xab0
			 0x3a5a54c2, /// 0xab4
			 0x1746b39e, /// 0xab8
			 0xffd03bf6, /// 0xabc
			 0xdd246c08, /// 0xac0
			 0x810ce666, /// 0xac4
			 0x00640c0c, /// 0xac8
			 0x9ac34339, /// 0xacc
			 0x24e94fe1, /// 0xad0
			 0x397864db, /// 0xad4
			 0xdc90c13d, /// 0xad8
			 0x8d5a14c2, /// 0xadc
			 0x05429185, /// 0xae0
			 0x4c2a9566, /// 0xae4
			 0x5e319a47, /// 0xae8
			 0xbe39b999, /// 0xaec
			 0x2195126e, /// 0xaf0
			 0xcb62ee81, /// 0xaf4
			 0x30577e57, /// 0xaf8
			 0x336575d5, /// 0xafc
			 0x896aac49, /// 0xb00
			 0xc7321e98, /// 0xb04
			 0xcc5850a8, /// 0xb08
			 0x0245f4ff, /// 0xb0c
			 0x8bedae04, /// 0xb10
			 0xd2d58cc1, /// 0xb14
			 0x5b6f8784, /// 0xb18
			 0x6408ec93, /// 0xb1c
			 0x7ffe45e9, /// 0xb20
			 0x96dc97d5, /// 0xb24
			 0x309a2bdb, /// 0xb28
			 0xa0142924, /// 0xb2c
			 0xf1a14e2a, /// 0xb30
			 0x52baeb40, /// 0xb34
			 0x97ff06d9, /// 0xb38
			 0xee629a18, /// 0xb3c
			 0xe5790033, /// 0xb40
			 0x4f06d09a, /// 0xb44
			 0x5252bef0, /// 0xb48
			 0x61d67dbd, /// 0xb4c
			 0x0f9d19e1, /// 0xb50
			 0xe4f77991, /// 0xb54
			 0x8f98be30, /// 0xb58
			 0x90de65b4, /// 0xb5c
			 0x82f69e48, /// 0xb60
			 0x075ed579, /// 0xb64
			 0x67a6f6b1, /// 0xb68
			 0x2f4004df, /// 0xb6c
			 0x40b9798e, /// 0xb70
			 0x81d6820b, /// 0xb74
			 0x38b3cb27, /// 0xb78
			 0x1b910ce2, /// 0xb7c
			 0x42ddd4d9, /// 0xb80
			 0x5d673ae8, /// 0xb84
			 0x7a37c00b, /// 0xb88
			 0xe3e80520, /// 0xb8c
			 0x957aef64, /// 0xb90
			 0x9933935d, /// 0xb94
			 0x963288e3, /// 0xb98
			 0x3dd19021, /// 0xb9c
			 0xa5c68731, /// 0xba0
			 0xb0331bbe, /// 0xba4
			 0x70d49a0b, /// 0xba8
			 0xe9facfc7, /// 0xbac
			 0x13f8ad58, /// 0xbb0
			 0x070f0e91, /// 0xbb4
			 0xaa9bfa53, /// 0xbb8
			 0x4c233097, /// 0xbbc
			 0xfee640b6, /// 0xbc0
			 0xb085e993, /// 0xbc4
			 0x48462fa9, /// 0xbc8
			 0x57147769, /// 0xbcc
			 0x62e4a6bf, /// 0xbd0
			 0xac67ada0, /// 0xbd4
			 0x9e25b03d, /// 0xbd8
			 0x02631869, /// 0xbdc
			 0x6abac9ee, /// 0xbe0
			 0x4dd8fe2a, /// 0xbe4
			 0xb7d70de8, /// 0xbe8
			 0xdb411909, /// 0xbec
			 0xdc5b515e, /// 0xbf0
			 0x639992ed, /// 0xbf4
			 0xdc21a110, /// 0xbf8
			 0xbc910216, /// 0xbfc
			 0x34e6a196, /// 0xc00
			 0xf358e287, /// 0xc04
			 0x5d30ea17, /// 0xc08
			 0xcac5cb10, /// 0xc0c
			 0xcf7ff4bc, /// 0xc10
			 0x03045169, /// 0xc14
			 0x9f7fd9aa, /// 0xc18
			 0x158e2f83, /// 0xc1c
			 0x61d9e95d, /// 0xc20
			 0x86eb6932, /// 0xc24
			 0xc71bd9cd, /// 0xc28
			 0x16566dad, /// 0xc2c
			 0xcd888373, /// 0xc30
			 0x453f40aa, /// 0xc34
			 0xdd4e3ed9, /// 0xc38
			 0x8d296280, /// 0xc3c
			 0x6a49b548, /// 0xc40
			 0xc4db6799, /// 0xc44
			 0x72f47cd8, /// 0xc48
			 0x3e882a4b, /// 0xc4c
			 0x2d7a4ab8, /// 0xc50
			 0x05592b4a, /// 0xc54
			 0x7a6909d0, /// 0xc58
			 0x1266292a, /// 0xc5c
			 0x2b673f81, /// 0xc60
			 0x0c55f121, /// 0xc64
			 0x0e6fbcde, /// 0xc68
			 0x89175d4f, /// 0xc6c
			 0x3dddeff9, /// 0xc70
			 0x60453ad4, /// 0xc74
			 0x0c232cd7, /// 0xc78
			 0x0e38694a, /// 0xc7c
			 0xb37bc602, /// 0xc80
			 0xf6d6a5ad, /// 0xc84
			 0x6ccdcf8a, /// 0xc88
			 0x556ba9f3, /// 0xc8c
			 0xbf377e35, /// 0xc90
			 0x5a13de10, /// 0xc94
			 0x3495b64f, /// 0xc98
			 0xf83e0b97, /// 0xc9c
			 0x238ae70e, /// 0xca0
			 0xe88a0f04, /// 0xca4
			 0x3e651b72, /// 0xca8
			 0x0cdeea52, /// 0xcac
			 0x3da512d2, /// 0xcb0
			 0x056a4324, /// 0xcb4
			 0xb5820dfc, /// 0xcb8
			 0x3acd5f66, /// 0xcbc
			 0x2b8c2769, /// 0xcc0
			 0xc069389b, /// 0xcc4
			 0x1201b4de, /// 0xcc8
			 0x9e14af37, /// 0xccc
			 0xbd58f805, /// 0xcd0
			 0xcfd405d6, /// 0xcd4
			 0xd13adfbb, /// 0xcd8
			 0x2f54cff8, /// 0xcdc
			 0xfb338a30, /// 0xce0
			 0xa014c5ee, /// 0xce4
			 0xaf646d16, /// 0xce8
			 0x489e3a45, /// 0xcec
			 0x62408212, /// 0xcf0
			 0x94464c05, /// 0xcf4
			 0x99cced73, /// 0xcf8
			 0xcf3d0f14, /// 0xcfc
			 0xe2df2d1e, /// 0xd00
			 0x11fffef6, /// 0xd04
			 0xf2e7e3cf, /// 0xd08
			 0x4f4ad6ab, /// 0xd0c
			 0x07e9436f, /// 0xd10
			 0x48834468, /// 0xd14
			 0x687f3111, /// 0xd18
			 0x78249536, /// 0xd1c
			 0x9eff8149, /// 0xd20
			 0x4209c6d2, /// 0xd24
			 0xbe0f530d, /// 0xd28
			 0xcf048584, /// 0xd2c
			 0x0c456f73, /// 0xd30
			 0x569f14a0, /// 0xd34
			 0x6405b29d, /// 0xd38
			 0x4587a0c9, /// 0xd3c
			 0xca041594, /// 0xd40
			 0xa5a87c2c, /// 0xd44
			 0x3727d0b1, /// 0xd48
			 0x14037f2c, /// 0xd4c
			 0xe32440f7, /// 0xd50
			 0xe2f9f4ed, /// 0xd54
			 0xe752d6b7, /// 0xd58
			 0xb2ec7918, /// 0xd5c
			 0x69b78028, /// 0xd60
			 0x5195af78, /// 0xd64
			 0x80a8ae1a, /// 0xd68
			 0x05882e6c, /// 0xd6c
			 0xe9d78cf5, /// 0xd70
			 0x7d56c5d7, /// 0xd74
			 0x505ad45c, /// 0xd78
			 0x26b359c4, /// 0xd7c
			 0x6eeb923f, /// 0xd80
			 0x7aec4b19, /// 0xd84
			 0x7bb7adec, /// 0xd88
			 0x5dbc5291, /// 0xd8c
			 0xa1573290, /// 0xd90
			 0xf02043c5, /// 0xd94
			 0x45c9e0aa, /// 0xd98
			 0x4d9aef72, /// 0xd9c
			 0x6f26e413, /// 0xda0
			 0xf9fd99f4, /// 0xda4
			 0xbd2737a4, /// 0xda8
			 0x9d7c73a6, /// 0xdac
			 0xb2a5bfc7, /// 0xdb0
			 0x407874f4, /// 0xdb4
			 0x64763c46, /// 0xdb8
			 0x40bea16d, /// 0xdbc
			 0x6f169f91, /// 0xdc0
			 0x4952342b, /// 0xdc4
			 0xa7b78a03, /// 0xdc8
			 0x6260a1a3, /// 0xdcc
			 0x5e274fe5, /// 0xdd0
			 0x2d747d3b, /// 0xdd4
			 0xc76fca83, /// 0xdd8
			 0x47aab90b, /// 0xddc
			 0xe2d027a4, /// 0xde0
			 0x583c292f, /// 0xde4
			 0x565f9c1a, /// 0xde8
			 0x273e2e54, /// 0xdec
			 0xb0a3c3ea, /// 0xdf0
			 0x4e000e3c, /// 0xdf4
			 0x84fa4d7f, /// 0xdf8
			 0x7da4caae, /// 0xdfc
			 0x6a332fa4, /// 0xe00
			 0x93fd8071, /// 0xe04
			 0x216a0ae9, /// 0xe08
			 0x6fdbba25, /// 0xe0c
			 0x139b6c71, /// 0xe10
			 0x63cd3dac, /// 0xe14
			 0xe73eb54c, /// 0xe18
			 0x135fabee, /// 0xe1c
			 0x7b28eb31, /// 0xe20
			 0x063be6bc, /// 0xe24
			 0xf25b7c70, /// 0xe28
			 0x7587daec, /// 0xe2c
			 0x44258c4c, /// 0xe30
			 0x8639e863, /// 0xe34
			 0x0f5386e5, /// 0xe38
			 0x77ed1d2f, /// 0xe3c
			 0x1d74b02a, /// 0xe40
			 0x22387d97, /// 0xe44
			 0x5930463b, /// 0xe48
			 0xb906bb6f, /// 0xe4c
			 0x64fdd0b8, /// 0xe50
			 0x8e0f99d5, /// 0xe54
			 0x9b84ac07, /// 0xe58
			 0xabe01166, /// 0xe5c
			 0x42064dd9, /// 0xe60
			 0x5c22e8c1, /// 0xe64
			 0x87e35591, /// 0xe68
			 0xfc04f0b7, /// 0xe6c
			 0x5d14d267, /// 0xe70
			 0x63b92546, /// 0xe74
			 0x08cd8985, /// 0xe78
			 0xe2768a4a, /// 0xe7c
			 0xca877d29, /// 0xe80
			 0x082c0570, /// 0xe84
			 0xd2d573dc, /// 0xe88
			 0x6551e466, /// 0xe8c
			 0xcba5b6ef, /// 0xe90
			 0xf4398d2d, /// 0xe94
			 0x8f368133, /// 0xe98
			 0x034999e7, /// 0xe9c
			 0x6c101f13, /// 0xea0
			 0x314ece65, /// 0xea4
			 0x1813c5d7, /// 0xea8
			 0x0dbb417f, /// 0xeac
			 0xd3891cfd, /// 0xeb0
			 0x6e558da2, /// 0xeb4
			 0xdd7ff492, /// 0xeb8
			 0xbd1f383c, /// 0xebc
			 0x7ce78c5d, /// 0xec0
			 0xc46a083c, /// 0xec4
			 0x2678d92e, /// 0xec8
			 0xe726afcf, /// 0xecc
			 0xc6453d57, /// 0xed0
			 0xf309fb63, /// 0xed4
			 0x651b72af, /// 0xed8
			 0x121f0f6f, /// 0xedc
			 0xd05b64d5, /// 0xee0
			 0x6f4ad24c, /// 0xee4
			 0x17aedddf, /// 0xee8
			 0xe20b61f9, /// 0xeec
			 0x28f2f3f2, /// 0xef0
			 0x54805791, /// 0xef4
			 0xaff9efaa, /// 0xef8
			 0xb04bfe72, /// 0xefc
			 0xce12ae6a, /// 0xf00
			 0xc3b72f27, /// 0xf04
			 0x4ba441fe, /// 0xf08
			 0x0c31a9ce, /// 0xf0c
			 0xff003f44, /// 0xf10
			 0x8fbf6c33, /// 0xf14
			 0x799a20fd, /// 0xf18
			 0xde0697f2, /// 0xf1c
			 0x8c42de76, /// 0xf20
			 0x9d710820, /// 0xf24
			 0xdd80d0fc, /// 0xf28
			 0x0a8b0f5a, /// 0xf2c
			 0x9b988e94, /// 0xf30
			 0x4dd33bbf, /// 0xf34
			 0x0d6b24d3, /// 0xf38
			 0xea0151fb, /// 0xf3c
			 0x76c9f231, /// 0xf40
			 0xe9b45b45, /// 0xf44
			 0xe6e53545, /// 0xf48
			 0x816dde4a, /// 0xf4c
			 0xb83857d3, /// 0xf50
			 0xf73169eb, /// 0xf54
			 0xc0df089b, /// 0xf58
			 0xc4d69727, /// 0xf5c
			 0xb04b43f6, /// 0xf60
			 0x6d636ba8, /// 0xf64
			 0xbed9393a, /// 0xf68
			 0xf3ba8458, /// 0xf6c
			 0xa65682a6, /// 0xf70
			 0x36965079, /// 0xf74
			 0xd4ea8fa6, /// 0xf78
			 0x168dc52f, /// 0xf7c
			 0x7a349806, /// 0xf80
			 0xbdc19e33, /// 0xf84
			 0x6b7ae081, /// 0xf88
			 0x9a046fdc, /// 0xf8c
			 0xc0d2e6af, /// 0xf90
			 0x8e624bef, /// 0xf94
			 0x92073d6c, /// 0xf98
			 0xc7df59a3, /// 0xf9c
			 0xcf9cc503, /// 0xfa0
			 0xa125a3e8, /// 0xfa4
			 0x929ed90e, /// 0xfa8
			 0x1bde471c, /// 0xfac
			 0x85881a29, /// 0xfb0
			 0xef2c8e95, /// 0xfb4
			 0xa3706f62, /// 0xfb8
			 0x855ed152, /// 0xfbc
			 0xbd253efa, /// 0xfc0
			 0xb4f31493, /// 0xfc4
			 0x1dcfa8ec, /// 0xfc8
			 0x1830567a, /// 0xfcc
			 0x7582f01c, /// 0xfd0
			 0x677c6f4e, /// 0xfd4
			 0x1b7bf7ff, /// 0xfd8
			 0x33d69326, /// 0xfdc
			 0x851b6a73, /// 0xfe0
			 0xf03d84f3, /// 0xfe4
			 0x8c8cead5, /// 0xfe8
			 0xf61c5e11, /// 0xfec
			 0xfa49e478, /// 0xff0
			 0xf0ebd2e2, /// 0xff4
			 0x764ec1af, /// 0xff8
			 0x4eb5ef67 /// last
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
			 0x00000504,
			 0x00000700,
			 0x000005f0,
			 0x0000050c,
			 0x00000738,
			 0x00000384,
			 0x00000370,
			 0x00000660,

			 /// vpu register f2
			 0x41d00000,
			 0x41e00000,
			 0x41a00000,
			 0x41b80000,
			 0x41e80000,
			 0x41700000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f3
			 0x42000000,
			 0x41300000,
			 0x41c00000,
			 0x41d80000,
			 0x41e00000,
			 0x41900000,
			 0x41f80000,
			 0x41200000,

			 /// vpu register f4
			 0x40000000,
			 0x40a00000,
			 0x40400000,
			 0x41c80000,
			 0x41100000,
			 0x40000000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f5
			 0x41f80000,
			 0x3f800000,
			 0x41500000,
			 0x41c80000,
			 0x41300000,
			 0x41400000,
			 0x40c00000,
			 0x41100000,

			 /// vpu register f6
			 0x41900000,
			 0x41b80000,
			 0x41700000,
			 0x41700000,
			 0x40000000,
			 0x40400000,
			 0x41f00000,
			 0x41900000,

			 /// vpu register f7
			 0x41500000,
			 0x41d80000,
			 0x41900000,
			 0x41a00000,
			 0x41b80000,
			 0x41f80000,
			 0x41b00000,
			 0x41100000,

			 /// vpu register f8
			 0x40400000,
			 0x41b00000,
			 0x41d80000,
			 0x41000000,
			 0x41500000,
			 0x41f80000,
			 0x41900000,
			 0x41980000,

			 /// vpu register f9
			 0x41d80000,
			 0x40c00000,
			 0x41b00000,
			 0x41500000,
			 0x41600000,
			 0x41e00000,
			 0x41700000,
			 0x41c00000,

			 /// vpu register f10
			 0x40c00000,
			 0x42000000,
			 0x41c00000,
			 0x41900000,
			 0x41e80000,
			 0x42000000,
			 0x41800000,
			 0x41400000,

			 /// vpu register f11
			 0x41f80000,
			 0x41700000,
			 0x41e00000,
			 0x40a00000,
			 0x41b00000,
			 0x41700000,
			 0x41880000,
			 0x40400000,

			 /// vpu register f12
			 0x41980000,
			 0x41800000,
			 0x41980000,
			 0x40a00000,
			 0x41a00000,
			 0x40e00000,
			 0x41000000,
			 0x41a00000,

			 /// vpu register f13
			 0x3f800000,
			 0x41900000,
			 0x41b00000,
			 0x41200000,
			 0x41100000,
			 0x3f800000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f14
			 0x41e80000,
			 0x41d00000,
			 0x41e80000,
			 0x41700000,
			 0x41500000,
			 0x40c00000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f15
			 0x41100000,
			 0x41b80000,
			 0x41e80000,
			 0x41b00000,
			 0x41100000,
			 0x41d80000,
			 0x41600000,
			 0x41900000,

			 /// vpu register f16
			 0x41b80000,
			 0x41e00000,
			 0x42000000,
			 0x40800000,
			 0x41880000,
			 0x41000000,
			 0x41d00000,
			 0x41c00000,

			 /// vpu register f17
			 0x41d00000,
			 0x41000000,
			 0x41b80000,
			 0x41800000,
			 0x40000000,
			 0x41e80000,
			 0x41f00000,
			 0x41d80000,

			 /// vpu register f18
			 0x41a00000,
			 0x41a00000,
			 0x41a00000,
			 0x41600000,
			 0x41500000,
			 0x41400000,
			 0x41a80000,
			 0x41e00000,

			 /// vpu register f19
			 0x41300000,
			 0x41b00000,
			 0x40400000,
			 0x40800000,
			 0x41f00000,
			 0x41900000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f20
			 0x41600000,
			 0x41c00000,
			 0x40e00000,
			 0x41f00000,
			 0x40000000,
			 0x41c00000,
			 0x41f80000,
			 0x40e00000,

			 /// vpu register f21
			 0x41d00000,
			 0x41980000,
			 0x41a80000,
			 0x41300000,
			 0x40400000,
			 0x41900000,
			 0x41c80000,
			 0x41f00000,

			 /// vpu register f22
			 0x41d80000,
			 0x41980000,
			 0x41700000,
			 0x41c80000,
			 0x41500000,
			 0x41800000,
			 0x3f800000,
			 0x41700000,

			 /// vpu register f23
			 0x41f80000,
			 0x41600000,
			 0x41f80000,
			 0x42000000,
			 0x40a00000,
			 0x41e00000,
			 0x41600000,
			 0x40800000,

			 /// vpu register f24
			 0x41300000,
			 0x41d80000,
			 0x41900000,
			 0x40800000,
			 0x41300000,
			 0x41200000,
			 0x40c00000,
			 0x40e00000,

			 /// vpu register f25
			 0x41000000,
			 0x41f80000,
			 0x41d00000,
			 0x41200000,
			 0x40000000,
			 0x41300000,
			 0x41f00000,
			 0x41d80000,

			 /// vpu register f26
			 0x41500000,
			 0x41a80000,
			 0x41e80000,
			 0x40400000,
			 0x41100000,
			 0x41a00000,
			 0x41c80000,
			 0x40800000,

			 /// vpu register f27
			 0x41e00000,
			 0x41000000,
			 0x41900000,
			 0x41600000,
			 0x41900000,
			 0x40c00000,
			 0x41b00000,
			 0x40400000,

			 /// vpu register f28
			 0x41880000,
			 0x40800000,
			 0x41a00000,
			 0x40400000,
			 0x40a00000,
			 0x41b00000,
			 0x40a00000,
			 0x41300000,

			 /// vpu register f29
			 0x41600000,
			 0x40000000,
			 0x40c00000,
			 0x41b00000,
			 0x40c00000,
			 0x41200000,
			 0x41200000,
			 0x41e00000,

			 /// vpu register f30
			 0x41600000,
			 0x41700000,
			 0x40e00000,
			 0x41600000,
			 0x40e00000,
			 0x41700000,
			 0x41200000,
			 0x41600000,

			 /// vpu register f31
			 0x40e00000,
			 0x42000000,
			 0x41000000,
			 0x41d80000,
			 0x41a00000,
			 0x41600000,
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
		"fcvt.ps.pwu f2, f10, rne\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f30, f28, rmm\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f7, f7, rdn\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f10, f25, rtz\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f12, f28, dyn\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f17, f4, rtz\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f9, f6, rup\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f27, f25, dyn\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f26, f6, rmm\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f22, f28, dyn\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f15, f29, rtz\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f18, f30, rdn\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f13, f29, rtz\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f22, f15, rne\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f7, f8, rmm\n"                           ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f21, f25, rtz\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f28, f7, rmm\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f20, f12, dyn\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f13, f16, rmm\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f3, f3, rmm\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f16, f29, dyn\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f9, f1, rtz\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f12, f3, rne\n"                          ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f17, f21, rup\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f6, f21, rtz\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f14, f18, rmm\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f18, f9, rne\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f18, f17, dyn\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f27, f3, rtz\n"                          ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f13, f0, dyn\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f24, f23, rup\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f8, f21, rne\n"                          ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f26, f13, rup\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f15, f26, dyn\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f9, f7, rup\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f3, f19, rmm\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f17, f21, rmm\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f21, f24, rdn\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f22, f24, rtz\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f29, f15, dyn\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f14, f28, dyn\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f26, f21, rtz\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f22, f16, rmm\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f30, f19, rmm\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f5, f19, dyn\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f19, f9, rmm\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f18, f25, rup\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f11, f21, rtz\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f27, f17, dyn\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f19, f20, rtz\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f8, f30, rne\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f19, f18, rdn\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f26, f13, rne\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f24, f7, rdn\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f18, f5, rne\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f27, f27, rdn\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f29, f15, rne\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f27, f24, rmm\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f4, f26, rdn\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f29, f0, rmm\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f30, f2, rup\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f3, f13, rdn\n"                          ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f28, f18, rdn\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f10, f10, dyn\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f13, f3, rdn\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f28, f4, rne\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f21, f10, rtz\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f6, f17, rtz\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f7, f26, rup\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f21, f19, rne\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f13, f20, rmm\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f18, f6, rup\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f27, f29, rne\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f15, f28, rne\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f7, f29, rmm\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f24, f8, dyn\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f28, f30, rtz\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f14, f13, rtz\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f7, f19, rtz\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f25, f7, dyn\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f21, f14, dyn\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f14, f23, rmm\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f7, f4, rtz\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f21, f30, rne\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f22, f10, rdn\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f24, f11, rdn\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f9, f7, rdn\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f5, f16, dyn\n"                          ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f15, f16, rtz\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f29, f20, rdn\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f28, f15, rdn\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f17, f5, rne\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f5, f20, rdn\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f30, f27, rne\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f2, f28, rdn\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f5, f23, rne\n"                          ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f24, f27, rup\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f6, f0, dyn\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.pwu f19, f8, rdn\n"                          ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
