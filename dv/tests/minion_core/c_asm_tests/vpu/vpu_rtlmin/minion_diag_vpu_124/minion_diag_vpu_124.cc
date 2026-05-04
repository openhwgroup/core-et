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
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00008
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00010
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00018
			 0xbf028f5c,                                                  // -0.51                                       /// 0001c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00020
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0002c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00030
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00034
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0003c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00044
			 0x33333333,                                                  // 4 random values                             /// 00048
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c600000,                                                  // Leading 1s:                                 /// 00054
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00058
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0005c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00060
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00064
			 0x3f028f5c,                                                  // 0.51                                        /// 00068
			 0xbf028f5c,                                                  // -0.51                                       /// 0006c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00074
			 0x0c600000,                                                  // Leading 1s:                                 /// 00078
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00080
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00084
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00088
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x7fc00001,                                                  // signaling NaN                               /// 00090
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00094
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00000000,                                                  // zero                                        /// 000b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000cc
			 0xcb000000,                                                  // -8388608.0                                  /// 000d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000d8
			 0x4b000000,                                                  // 8388608.0                                   /// 000dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000f0
			 0x80000000,                                                  // -zero                                       /// 000f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00108
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0010c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0c700000,                                                  // Leading 1s:                                 /// 00118
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00124
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00128
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0012c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00144
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00148
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0014c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00154
			 0x00011111,                                                  // 9.7958E-41                                  /// 00158
			 0x0c700000,                                                  // Leading 1s:                                 /// 0015c
			 0xff800000,                                                  // -inf                                        /// 00160
			 0x0c700000,                                                  // Leading 1s:                                 /// 00164
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00194
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00198
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0019c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0xff800000,                                                  // -inf                                        /// 001b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00200
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00208
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00210
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00218
			 0x3f028f5c,                                                  // 0.51                                        /// 0021c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00220
			 0x0c600000,                                                  // Leading 1s:                                 /// 00224
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0022c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00230
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00238
			 0x55555555,                                                  // 4 random values                             /// 0023c
			 0x55555555,                                                  // 4 random values                             /// 00240
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00244
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00250
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00258
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0025c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00264
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00268
			 0x0c780000,                                                  // Leading 1s:                                 /// 0026c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00274
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00278
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00280
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00284
			 0x0e000003,                                                  // Trailing 1s:                                /// 00288
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0028c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0029c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002dc
			 0xbf028f5c,                                                  // -0.51                                       /// 002e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0xffc00001,                                                  // -signaling NaN                              /// 002f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00300
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00304
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00308
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00310
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00324
			 0xff800000,                                                  // -inf                                        /// 00328
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0032c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00330
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00334
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00338
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00340
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00344
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00350
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00354
			 0x33333333,                                                  // 4 random values                             /// 00358
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0035c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00360
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00364
			 0x55555555,                                                  // 4 random values                             /// 00368
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0036c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00374
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0037c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00380
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00384
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0c400000,                                                  // Leading 1s:                                 /// 0038c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00390
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00398
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003bc
			 0x33333333,                                                  // 4 random values                             /// 003c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003d0
			 0x3f028f5c,                                                  // 0.51                                        /// 003d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00404
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00408
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0040c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00414
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00418
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00428
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0042c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00440
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00450
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00454
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00458
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0045c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00460
			 0x7f800000,                                                  // inf                                         /// 00464
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00468
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0046c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00470
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00478
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0047c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00480
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00484
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x3f028f5c,                                                  // 0.51                                        /// 00490
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00494
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0049c
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 004d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x3f028f5c,                                                  // 0.51                                        /// 004f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00500
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00508
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0050c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00510
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00518
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0051c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00520
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00524
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0052c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80011111,                                                  // -9.7958E-41                                 /// 0053c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00540
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0xffc00001,                                                  // -signaling NaN                              /// 00548
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00558
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0055c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00560
			 0x0e000003,                                                  // Trailing 1s:                                /// 00564
			 0x7f800000,                                                  // inf                                         /// 00568
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00570
			 0x0c700000,                                                  // Leading 1s:                                 /// 00574
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00578
			 0x0c400000,                                                  // Leading 1s:                                 /// 0057c
			 0x7fc00001,                                                  // signaling NaN                               /// 00580
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00584
			 0x3f028f5c,                                                  // 0.51                                        /// 00588
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0059c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f8
			 0x7f800000,                                                  // inf                                         /// 005fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00604
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00608
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0060c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00610
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00614
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00620
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00000000,                                                  // zero                                        /// 0063c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00640
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00644
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00654
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0065c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00668
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00670
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00674
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00678
			 0x7fc00001,                                                  // signaling NaN                               /// 0067c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00680
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00684
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00688
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0068c
			 0x00000000,                                                  // zero                                        /// 00690
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00694
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00698
			 0x0c600000,                                                  // Leading 1s:                                 /// 0069c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c8
			 0x3f028f5c,                                                  // 0.51                                        /// 006cc
			 0x55555555,                                                  // 4 random values                             /// 006d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e8
			 0xff800000,                                                  // -inf                                        /// 006ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00704
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00714
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00718
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0071c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00720
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0072c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00734
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00738
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00000000,                                                  // zero                                        /// 00740
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00011111,                                                  // 9.7958E-41                                  /// 0074c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00750
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00760
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00764
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00768
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00770
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00774
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00778
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0077c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00780
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00784
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00788
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00790
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00794
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0079c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x7fc00001,                                                  // signaling NaN                               /// 007e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00804
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00808
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00810
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00814
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00818
			 0x7fc00001,                                                  // signaling NaN                               /// 0081c
			 0x3f028f5c,                                                  // 0.51                                        /// 00820
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00824
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00828
			 0x00000000,                                                  // zero                                        /// 0082c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00838
			 0x0e000007,                                                  // Trailing 1s:                                /// 0083c
			 0x55555555,                                                  // 4 random values                             /// 00840
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00848
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00850
			 0x0e000007,                                                  // Trailing 1s:                                /// 00854
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0085c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00860
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00864
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00868
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0086c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00870
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00878
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00880
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00888
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0088c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0c400000,                                                  // Leading 1s:                                 /// 00898
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0089c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x7f800000,                                                  // inf                                         /// 008ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f4
			 0x7f800000,                                                  // inf                                         /// 008f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0e000001,                                                  // Trailing 1s:                                /// 00904
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0090c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00910
			 0x4b000000,                                                  // 8388608.0                                   /// 00914
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00918
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00920
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00924
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0092c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00930
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0093c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0094c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00954
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00960
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0096c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00970
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00974
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00978
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0097c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00988
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0098c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00994
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00998
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009c4
			 0x55555555,                                                  // 4 random values                             /// 009c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f4
			 0x33333333,                                                  // 4 random values                             /// 009f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a10
			 0x00000000,                                                  // zero                                        /// 00a14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xff800000,                                                  // -inf                                        /// 00a48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00000000,                                                  // zero                                        /// 00a5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aa8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00abc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ac4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ad4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00adc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ae0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ae4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ae8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00af4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b10
			 0xbf028f5c,                                                  // -0.51                                       /// 00b14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x3f028f5c,                                                  // 0.51                                        /// 00b8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ba0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ba8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00be0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be4
			 0xff800000,                                                  // -inf                                        /// 00be8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x7f800000,                                                  // inf                                         /// 00bf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x33333333,                                                  // 4 random values                             /// 00c28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c84
			 0xbf028f5c,                                                  // -0.51                                       /// 00c88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc8
			 0x80000000,                                                  // -zero                                       /// 00ccc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0x7f800000,                                                  // inf                                         /// 00cdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ce0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ce4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cf4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cf8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d30
			 0x4b000000,                                                  // 8388608.0                                   /// 00d34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d44
			 0x00000000,                                                  // zero                                        /// 00d48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x7f800000,                                                  // inf                                         /// 00d5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00de8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00df0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00df4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00df8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e04
			 0x7f800000,                                                  // inf                                         /// 00e08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e60
			 0x7fc00001,                                                  // signaling NaN                               /// 00e64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e70
			 0x55555555,                                                  // 4 random values                             /// 00e74
			 0xff800000,                                                  // -inf                                        /// 00e78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e84
			 0x3f028f5c,                                                  // 0.51                                        /// 00e88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ea8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eb4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ebc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ec4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ecc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00edc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80000000,                                                  // -zero                                       /// 00ef4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00efc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x55555555,                                                  // 4 random values                             /// 00fac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80800002                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00000
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00004
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00010
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00018
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0001c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00020
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00028
			 0x0e000003,                                                  // Trailing 1s:                                /// 0002c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00038
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00050
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00054
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0005c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00060
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00068
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0006c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x33333333,                                                  // 4 random values                             /// 00074
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00078
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0007c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00080
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00088
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00090
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00094
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0009c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x33333333,                                                  // 4 random values                             /// 00100
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x33333333,                                                  // 4 random values                             /// 00108
			 0x7fc00001,                                                  // signaling NaN                               /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00114
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00118
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00120
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0012c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00134
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00138
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00148
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00154
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00158
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0015c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00164
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00180
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00184
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0018c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00190
			 0xffc00001,                                                  // -signaling NaN                              /// 00194
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0019c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0xcb000000,                                                  // -8388608.0                                  /// 001ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b8
			 0x80000000,                                                  // -zero                                       /// 001bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 001f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00200
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00204
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0020c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00220
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0022c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00234
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00238
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0023c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0024c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00250
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00254
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00258
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0025c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00260
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00264
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00270
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00278
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0027c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00284
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00288
			 0x7f800000,                                                  // inf                                         /// 0028c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00290
			 0x3f028f5c,                                                  // 0.51                                        /// 00294
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00298
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a8
			 0xcb000000,                                                  // -8388608.0                                  /// 002ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 002b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00000000,                                                  // zero                                        /// 002d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00304
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00308
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0030c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x80000000,                                                  // -zero                                       /// 0031c
			 0x3f028f5c,                                                  // 0.51                                        /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00324
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00328
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00330
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00334
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00338
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0033c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00344
			 0x7fc00001,                                                  // signaling NaN                               /// 00348
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0034c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80011111,                                                  // -9.7958E-41                                 /// 00354
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00358
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00360
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xbf028f5c,                                                  // -0.51                                       /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00378
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0037c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00380
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00384
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00388
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0038c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00398
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0039c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b4
			 0x80000000,                                                  // -zero                                       /// 003b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x7f800000,                                                  // inf                                         /// 003d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00400
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00404
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00408
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0040c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00410
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00418
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0041c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00424
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00428
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0042c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00430
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00434
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00438
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0043c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00440
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0044c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00450
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00458
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0045c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00464
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00470
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00474
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00478
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0047c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x4b000000,                                                  // 8388608.0                                   /// 00488
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0048c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00490
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00494
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00498
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0049c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x55555555,                                                  // 4 random values                             /// 004b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x80000000,                                                  // -zero                                       /// 004e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00500
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00504
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0050c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00514
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00518
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0051c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00520
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00528
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0052c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x4b000000,                                                  // 8388608.0                                   /// 00534
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00538
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0053c
			 0xff800000,                                                  // -inf                                        /// 00540
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00548
			 0x0c700000,                                                  // Leading 1s:                                 /// 0054c
			 0x55555555,                                                  // 4 random values                             /// 00550
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00554
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00560
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00564
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00568
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0056c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00570
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00574
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00578
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00584
			 0x0e000001,                                                  // Trailing 1s:                                /// 00588
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00594
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00598
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0059c
			 0x0c780000,                                                  // Leading 1s:                                 /// 005a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005bc
			 0x80000000,                                                  // -zero                                       /// 005c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80000000,                                                  // -zero                                       /// 005d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00600
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0060c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00610
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00614
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00618
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00620
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00624
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0062c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00630
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00640
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00644
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00648
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0064c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00650
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0065c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00660
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00011111,                                                  // 9.7958E-41                                  /// 0066c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00674
			 0x0c780000,                                                  // Leading 1s:                                 /// 00678
			 0x80000000,                                                  // -zero                                       /// 0067c
			 0x7fc00001,                                                  // signaling NaN                               /// 00680
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00688
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0068c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00690
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00694
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00698
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0069c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006ac
			 0x55555555,                                                  // 4 random values                             /// 006b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00700
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00704
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00708
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0070c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00710
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00714
			 0x80011111,                                                  // -9.7958E-41                                 /// 00718
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0071c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00720
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00724
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00728
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0072c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00730
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0073c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00740
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00744
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00748
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0074c
			 0x7fc00001,                                                  // signaling NaN                               /// 00750
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00754
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00758
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0075c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00760
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00768
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00774
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00778
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00780
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0xffc00001,                                                  // -signaling NaN                              /// 00788
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0xcb000000,                                                  // -8388608.0                                  /// 00790
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00798
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a4
			 0x33333333,                                                  // 4 random values                             /// 007a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x4b000000,                                                  // 8388608.0                                   /// 007f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00800
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00808
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0080c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00810
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00818
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00820
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00824
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00828
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00830
			 0x0e000003,                                                  // Trailing 1s:                                /// 00834
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00838
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00840
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00844
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00848
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00858
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0085c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00864
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00868
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0086c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00870
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00874
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00880
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00884
			 0x7fc00001,                                                  // signaling NaN                               /// 00888
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0088c
			 0x7f800000,                                                  // inf                                         /// 00890
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00894
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00900
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00908
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0090c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00910
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00914
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0091c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00928
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0092c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00930
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00934
			 0x0e000007,                                                  // Trailing 1s:                                /// 00938
			 0x55555555,                                                  // 4 random values                             /// 0093c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00940
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00944
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00948
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0094c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00950
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00958
			 0xcb000000,                                                  // -8388608.0                                  /// 0095c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00960
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0096c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00970
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00974
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0097c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00980
			 0x4b000000,                                                  // 8388608.0                                   /// 00984
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00988
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00994
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00998
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0099c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7f800000,                                                  // inf                                         /// 009b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009bc
			 0xbf028f5c,                                                  // -0.51                                       /// 009c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c8
			 0x3f028f5c,                                                  // 0.51                                        /// 009cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x7f800000,                                                  // inf                                         /// 00a74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a8c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aa4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ab4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00abc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00acc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad0
			 0x7f800000,                                                  // inf                                         /// 00ad4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00af0
			 0x4b000000,                                                  // 8388608.0                                   /// 00af4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00afc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b00
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b40
			 0xcb000000,                                                  // -8388608.0                                  /// 00b44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ba0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bbc
			 0x7f800000,                                                  // inf                                         /// 00bc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00be4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00be8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bf0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c78
			 0xffc00001,                                                  // -signaling NaN                              /// 00c7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ca4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cac
			 0x33333333,                                                  // 4 random values                             /// 00cb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cc4
			 0x55555555,                                                  // 4 random values                             /// 00cc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cf8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cfc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00da8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00db4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dfc
			 0xff800000,                                                  // -inf                                        /// 00e00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e04
			 0xff800000,                                                  // -inf                                        /// 00e08
			 0x33333333,                                                  // 4 random values                             /// 00e0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0xff800000,                                                  // -inf                                        /// 00e14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e78
			 0xffc00001,                                                  // -signaling NaN                              /// 00e7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e84
			 0x3f028f5c,                                                  // 0.51                                        /// 00e88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ec4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ed4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80000000,                                                  // -zero                                       /// 00ee8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ef0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x3f028f5c,                                                  // 0.51                                        /// 00efc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fd8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe0
			 0x7f800000,                                                  // inf                                         /// 00fe4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xcb8c4b40                                                  // -18388608.0                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x40e9ce2c, /// 0x0
			 0x4a456d80, /// 0x4
			 0x9fb4fd47, /// 0x8
			 0x85331eac, /// 0xc
			 0x027cb8d9, /// 0x10
			 0x3f5fb71b, /// 0x14
			 0x2d1a85e2, /// 0x18
			 0x17ff1ddb, /// 0x1c
			 0x90dbffea, /// 0x20
			 0xee353174, /// 0x24
			 0xad902b0f, /// 0x28
			 0x0cf67ae7, /// 0x2c
			 0x6319da01, /// 0x30
			 0x561dc0a5, /// 0x34
			 0x14fbd514, /// 0x38
			 0x209748ca, /// 0x3c
			 0xe3a41657, /// 0x40
			 0x51413838, /// 0x44
			 0x1ee6509f, /// 0x48
			 0x63317b37, /// 0x4c
			 0x166db358, /// 0x50
			 0x7e1463eb, /// 0x54
			 0x7d00d2bd, /// 0x58
			 0x71d911aa, /// 0x5c
			 0xae009d65, /// 0x60
			 0x1bb2e700, /// 0x64
			 0xa2341468, /// 0x68
			 0x2ba8581c, /// 0x6c
			 0xcd0399c6, /// 0x70
			 0xec876d66, /// 0x74
			 0x6808709e, /// 0x78
			 0x817f8cd6, /// 0x7c
			 0x77d5007a, /// 0x80
			 0x4836ebfa, /// 0x84
			 0x070f8346, /// 0x88
			 0x22c02be0, /// 0x8c
			 0xbcee4a9a, /// 0x90
			 0xcdebb4a8, /// 0x94
			 0xd9948e41, /// 0x98
			 0x850d57c4, /// 0x9c
			 0xd00d2d8d, /// 0xa0
			 0x089b1735, /// 0xa4
			 0x65ca58f0, /// 0xa8
			 0xecd108f5, /// 0xac
			 0x791aa3bd, /// 0xb0
			 0xc85df9a1, /// 0xb4
			 0xc7028722, /// 0xb8
			 0x3d04615b, /// 0xbc
			 0x4d1c72c2, /// 0xc0
			 0x435b0921, /// 0xc4
			 0x8ec8552c, /// 0xc8
			 0x3e3f56ea, /// 0xcc
			 0xfcd52448, /// 0xd0
			 0x86301582, /// 0xd4
			 0x305f4396, /// 0xd8
			 0xa9c38d3f, /// 0xdc
			 0x626e9308, /// 0xe0
			 0xb2f8ffa7, /// 0xe4
			 0xebd80557, /// 0xe8
			 0x5b9bb6c5, /// 0xec
			 0x1ae23da6, /// 0xf0
			 0x225454da, /// 0xf4
			 0x3b78b663, /// 0xf8
			 0x5aa3c906, /// 0xfc
			 0xb45df2a8, /// 0x100
			 0xebde7ede, /// 0x104
			 0x5e09ee90, /// 0x108
			 0x5c052aec, /// 0x10c
			 0x857bd386, /// 0x110
			 0x9fa183b1, /// 0x114
			 0x87d49500, /// 0x118
			 0x08736760, /// 0x11c
			 0xa115912f, /// 0x120
			 0x218f7564, /// 0x124
			 0x815935c2, /// 0x128
			 0xac716567, /// 0x12c
			 0x2620a1d8, /// 0x130
			 0x4f3dbd34, /// 0x134
			 0x2405c7f8, /// 0x138
			 0x62af22fd, /// 0x13c
			 0x641df95e, /// 0x140
			 0x8b839ec7, /// 0x144
			 0x83660537, /// 0x148
			 0x0b20a3a2, /// 0x14c
			 0x96ee5106, /// 0x150
			 0x0ac8a2ab, /// 0x154
			 0xa457804f, /// 0x158
			 0x4606e61e, /// 0x15c
			 0x7e7aae75, /// 0x160
			 0xb42494ee, /// 0x164
			 0x4e26599f, /// 0x168
			 0xfd083865, /// 0x16c
			 0x96936af0, /// 0x170
			 0x131a2ad6, /// 0x174
			 0xfbb7370d, /// 0x178
			 0x650079cd, /// 0x17c
			 0x791ebca5, /// 0x180
			 0x409cafbc, /// 0x184
			 0x870eeba6, /// 0x188
			 0xd094126d, /// 0x18c
			 0x6cb78ef3, /// 0x190
			 0x8f178ac6, /// 0x194
			 0x70e7835a, /// 0x198
			 0xe2a210f8, /// 0x19c
			 0x82304304, /// 0x1a0
			 0x272751a3, /// 0x1a4
			 0x4b2d5b4f, /// 0x1a8
			 0xc866dc7f, /// 0x1ac
			 0xe77f76dc, /// 0x1b0
			 0x3d037625, /// 0x1b4
			 0x8223111c, /// 0x1b8
			 0x7f5cf941, /// 0x1bc
			 0x8e0187b9, /// 0x1c0
			 0xb94f6b63, /// 0x1c4
			 0xd9103476, /// 0x1c8
			 0x91b3dec2, /// 0x1cc
			 0xb7ac789d, /// 0x1d0
			 0x99d41dc3, /// 0x1d4
			 0x109fe4e6, /// 0x1d8
			 0x96fdca66, /// 0x1dc
			 0xd91866c8, /// 0x1e0
			 0x3c947d38, /// 0x1e4
			 0xdbf52e3b, /// 0x1e8
			 0x329a29c5, /// 0x1ec
			 0x913815da, /// 0x1f0
			 0xf6257eff, /// 0x1f4
			 0x28b2c505, /// 0x1f8
			 0x9405bed1, /// 0x1fc
			 0xfd468659, /// 0x200
			 0x82830e39, /// 0x204
			 0x211d658b, /// 0x208
			 0xbebfb6c7, /// 0x20c
			 0x65cda981, /// 0x210
			 0x86b34d2b, /// 0x214
			 0x383211f2, /// 0x218
			 0xd896ecf6, /// 0x21c
			 0x10e82bab, /// 0x220
			 0xe3a19e17, /// 0x224
			 0x3589e0f5, /// 0x228
			 0xc367bd88, /// 0x22c
			 0x545d0801, /// 0x230
			 0x179ca99c, /// 0x234
			 0x7598d86c, /// 0x238
			 0xaf4ea1dc, /// 0x23c
			 0xe1f1e5f6, /// 0x240
			 0xd020f411, /// 0x244
			 0x860c449d, /// 0x248
			 0xe65f921c, /// 0x24c
			 0x9c533b69, /// 0x250
			 0x4c7bb35e, /// 0x254
			 0x14a6f641, /// 0x258
			 0xcf2bcaa1, /// 0x25c
			 0x76c56717, /// 0x260
			 0x6ba3b310, /// 0x264
			 0x1bd2b852, /// 0x268
			 0xbfaf3f05, /// 0x26c
			 0xf2630d3c, /// 0x270
			 0xf4160f37, /// 0x274
			 0xf73f720d, /// 0x278
			 0x1256d66b, /// 0x27c
			 0x532d0e02, /// 0x280
			 0x2562b980, /// 0x284
			 0x904daef9, /// 0x288
			 0x1f148b69, /// 0x28c
			 0x9335ea01, /// 0x290
			 0xadd8584b, /// 0x294
			 0x736976ad, /// 0x298
			 0x71197cda, /// 0x29c
			 0x1d4adc3e, /// 0x2a0
			 0x4833d72c, /// 0x2a4
			 0xc87548f4, /// 0x2a8
			 0xfd28cfbd, /// 0x2ac
			 0x767b4e5b, /// 0x2b0
			 0x801d3a4b, /// 0x2b4
			 0x9e475dfb, /// 0x2b8
			 0x97a29f16, /// 0x2bc
			 0x54957ecb, /// 0x2c0
			 0x8eacb3bb, /// 0x2c4
			 0x59147eab, /// 0x2c8
			 0x52004747, /// 0x2cc
			 0xc55bc1c0, /// 0x2d0
			 0x47a8e40c, /// 0x2d4
			 0xea29c44e, /// 0x2d8
			 0x53d78c18, /// 0x2dc
			 0x80c5715a, /// 0x2e0
			 0xa466094c, /// 0x2e4
			 0xe43214a8, /// 0x2e8
			 0xc9bb6f03, /// 0x2ec
			 0x101c4c9f, /// 0x2f0
			 0x930bc180, /// 0x2f4
			 0xe861f859, /// 0x2f8
			 0xc5d2a722, /// 0x2fc
			 0x48b50a45, /// 0x300
			 0x47853103, /// 0x304
			 0xcd278939, /// 0x308
			 0x61dac403, /// 0x30c
			 0x3fb03e19, /// 0x310
			 0x51fafc67, /// 0x314
			 0xe49cc30d, /// 0x318
			 0x7848667e, /// 0x31c
			 0xcb2d8aac, /// 0x320
			 0xb95da547, /// 0x324
			 0xc174a929, /// 0x328
			 0x1a0dbab9, /// 0x32c
			 0x0b8f75a0, /// 0x330
			 0x6cb5e1a7, /// 0x334
			 0x058ab4c4, /// 0x338
			 0x6b798ab8, /// 0x33c
			 0xd9813b3e, /// 0x340
			 0x93e3087e, /// 0x344
			 0xa25cc318, /// 0x348
			 0x29612c2f, /// 0x34c
			 0x4269c3b2, /// 0x350
			 0x828afb9b, /// 0x354
			 0x23d7b49c, /// 0x358
			 0x10d839c0, /// 0x35c
			 0x5ab7e732, /// 0x360
			 0x6648e393, /// 0x364
			 0x25a8e78f, /// 0x368
			 0x17601c4e, /// 0x36c
			 0x5fba2915, /// 0x370
			 0x5195fcfd, /// 0x374
			 0x4340dafa, /// 0x378
			 0x2763cdfe, /// 0x37c
			 0x4cafdb50, /// 0x380
			 0x4b452a89, /// 0x384
			 0xb65e191e, /// 0x388
			 0x61ed261a, /// 0x38c
			 0xc5b42f66, /// 0x390
			 0x44769729, /// 0x394
			 0xcf40e1e4, /// 0x398
			 0x0f1d5652, /// 0x39c
			 0xe65eea02, /// 0x3a0
			 0xc439efb7, /// 0x3a4
			 0xbfb718ca, /// 0x3a8
			 0x20dc48e3, /// 0x3ac
			 0x3e222827, /// 0x3b0
			 0x86543d42, /// 0x3b4
			 0x641546f5, /// 0x3b8
			 0x39fcd698, /// 0x3bc
			 0x931603db, /// 0x3c0
			 0x9df3f256, /// 0x3c4
			 0xfd97bd56, /// 0x3c8
			 0xd6c841f7, /// 0x3cc
			 0xd3d17555, /// 0x3d0
			 0x0fa4d658, /// 0x3d4
			 0x40eda9e7, /// 0x3d8
			 0x03d7b130, /// 0x3dc
			 0xb9ac9076, /// 0x3e0
			 0xf9581377, /// 0x3e4
			 0xf1ca9905, /// 0x3e8
			 0x0b6faef7, /// 0x3ec
			 0xf796af51, /// 0x3f0
			 0x48e5e1a0, /// 0x3f4
			 0xf97e2140, /// 0x3f8
			 0xc7bd66da, /// 0x3fc
			 0x48b7f598, /// 0x400
			 0xb0797980, /// 0x404
			 0xf2c161ec, /// 0x408
			 0x47bc5a9b, /// 0x40c
			 0x96f9683d, /// 0x410
			 0x18f6962d, /// 0x414
			 0xb55b81a6, /// 0x418
			 0x34edacb4, /// 0x41c
			 0x42b2a6e1, /// 0x420
			 0xf999a0e9, /// 0x424
			 0x01cc2b59, /// 0x428
			 0x093221b7, /// 0x42c
			 0xab4ed390, /// 0x430
			 0xfcc8bd6c, /// 0x434
			 0x5ae2767e, /// 0x438
			 0x29519599, /// 0x43c
			 0xad4bc8be, /// 0x440
			 0xb611fbbf, /// 0x444
			 0xa6833fd1, /// 0x448
			 0x39c6f63c, /// 0x44c
			 0x95da6ddb, /// 0x450
			 0xf05a778d, /// 0x454
			 0x2b499412, /// 0x458
			 0xa584e582, /// 0x45c
			 0x5b2bedfc, /// 0x460
			 0x96989357, /// 0x464
			 0x66bb2fdb, /// 0x468
			 0x8086ab1e, /// 0x46c
			 0x3be0f934, /// 0x470
			 0xe96f9167, /// 0x474
			 0xdbd8fbf5, /// 0x478
			 0x6a74e6de, /// 0x47c
			 0x6ace2185, /// 0x480
			 0xf2482249, /// 0x484
			 0x35d46064, /// 0x488
			 0xd8fb3a1b, /// 0x48c
			 0xa3648f05, /// 0x490
			 0x936aa554, /// 0x494
			 0xa04713b0, /// 0x498
			 0x500b3b69, /// 0x49c
			 0xafe13e3b, /// 0x4a0
			 0xec61d622, /// 0x4a4
			 0xe8db19b1, /// 0x4a8
			 0x2294be26, /// 0x4ac
			 0xba280794, /// 0x4b0
			 0x47dd02b8, /// 0x4b4
			 0x29b61e6f, /// 0x4b8
			 0x232332cd, /// 0x4bc
			 0xa33046da, /// 0x4c0
			 0x486376d0, /// 0x4c4
			 0x522d8c2f, /// 0x4c8
			 0xd2d8399c, /// 0x4cc
			 0x6d7feb54, /// 0x4d0
			 0xb543bd74, /// 0x4d4
			 0xc719782c, /// 0x4d8
			 0x5c11cdb1, /// 0x4dc
			 0x45b8b64f, /// 0x4e0
			 0x8d43e83b, /// 0x4e4
			 0x3f93c4e6, /// 0x4e8
			 0xe8f2c620, /// 0x4ec
			 0x1dc59ab3, /// 0x4f0
			 0x665b7fce, /// 0x4f4
			 0xed83a2ff, /// 0x4f8
			 0x41c5d7e8, /// 0x4fc
			 0xc4d3ccb8, /// 0x500
			 0xc44a0203, /// 0x504
			 0x4ac32631, /// 0x508
			 0x2537918f, /// 0x50c
			 0xd867d706, /// 0x510
			 0x993f4d6c, /// 0x514
			 0x8c180f7f, /// 0x518
			 0x0bc47460, /// 0x51c
			 0x1a8bbfa2, /// 0x520
			 0x246b6819, /// 0x524
			 0x67469d93, /// 0x528
			 0x2eb13192, /// 0x52c
			 0x726fd736, /// 0x530
			 0x7bd1276f, /// 0x534
			 0x6d279d19, /// 0x538
			 0xcddd7a42, /// 0x53c
			 0xa8399db9, /// 0x540
			 0xc4f23dbc, /// 0x544
			 0x364f9860, /// 0x548
			 0x1a783449, /// 0x54c
			 0x08c4c5e1, /// 0x550
			 0x76272afe, /// 0x554
			 0xe8dca4a8, /// 0x558
			 0xd6f7e6a8, /// 0x55c
			 0x503e3b4b, /// 0x560
			 0x36f59e31, /// 0x564
			 0x95b1fe70, /// 0x568
			 0xf6ce524a, /// 0x56c
			 0xa09bed94, /// 0x570
			 0xf8bbf24f, /// 0x574
			 0xb6cfee94, /// 0x578
			 0x8b302edd, /// 0x57c
			 0x1978f5d4, /// 0x580
			 0xfc637968, /// 0x584
			 0xc3b4be17, /// 0x588
			 0xfbd21b30, /// 0x58c
			 0xa555cbc5, /// 0x590
			 0x3475b8f0, /// 0x594
			 0x5348aeed, /// 0x598
			 0x091118ad, /// 0x59c
			 0x87a89436, /// 0x5a0
			 0xbe93461a, /// 0x5a4
			 0x8a43f989, /// 0x5a8
			 0xc8611c43, /// 0x5ac
			 0x2eccfcbe, /// 0x5b0
			 0xeff254b5, /// 0x5b4
			 0xa6c6ac2d, /// 0x5b8
			 0xe1366915, /// 0x5bc
			 0xa61e0cec, /// 0x5c0
			 0xd5e72d7a, /// 0x5c4
			 0x802b8ec0, /// 0x5c8
			 0x8424ad84, /// 0x5cc
			 0x02cb0195, /// 0x5d0
			 0xd060aaa8, /// 0x5d4
			 0xeca52da8, /// 0x5d8
			 0xccdf6388, /// 0x5dc
			 0xa2d59caa, /// 0x5e0
			 0x6e19bed6, /// 0x5e4
			 0x32156452, /// 0x5e8
			 0x63ade57e, /// 0x5ec
			 0x82bc66bf, /// 0x5f0
			 0xdd3ac92f, /// 0x5f4
			 0x8f9b1594, /// 0x5f8
			 0x01aee5ba, /// 0x5fc
			 0xd1789b4d, /// 0x600
			 0x117f4072, /// 0x604
			 0x1ae250b9, /// 0x608
			 0x25346379, /// 0x60c
			 0x4405d0e7, /// 0x610
			 0x6cfca052, /// 0x614
			 0x3254ed8b, /// 0x618
			 0xae066c76, /// 0x61c
			 0x809a198c, /// 0x620
			 0x3c014325, /// 0x624
			 0xf41530b9, /// 0x628
			 0xa478b261, /// 0x62c
			 0x37866fc7, /// 0x630
			 0x9893e4a5, /// 0x634
			 0x44e1b6ad, /// 0x638
			 0x5e28edb9, /// 0x63c
			 0xf7841a26, /// 0x640
			 0x028992e8, /// 0x644
			 0x0d60fc36, /// 0x648
			 0x8867402c, /// 0x64c
			 0xa477024c, /// 0x650
			 0x32033981, /// 0x654
			 0xa51614d5, /// 0x658
			 0x5ba53764, /// 0x65c
			 0x950040b2, /// 0x660
			 0xc5bd6654, /// 0x664
			 0x907ee662, /// 0x668
			 0x3474886f, /// 0x66c
			 0xd604d4e9, /// 0x670
			 0xa182ed4c, /// 0x674
			 0xf5f0709b, /// 0x678
			 0x73558abd, /// 0x67c
			 0x3379cb43, /// 0x680
			 0xea0f203d, /// 0x684
			 0x6a926aae, /// 0x688
			 0x5d0ef6af, /// 0x68c
			 0x593bd097, /// 0x690
			 0x29e79d1b, /// 0x694
			 0x18e04eec, /// 0x698
			 0xe8ce1821, /// 0x69c
			 0xa39cc575, /// 0x6a0
			 0x0d1d14dd, /// 0x6a4
			 0xd86c93ee, /// 0x6a8
			 0x90e11bbb, /// 0x6ac
			 0x3325b87b, /// 0x6b0
			 0x1a3b1ca1, /// 0x6b4
			 0xff114da2, /// 0x6b8
			 0x7a44df2f, /// 0x6bc
			 0x0a40e160, /// 0x6c0
			 0x9b74bacd, /// 0x6c4
			 0x1d5697d0, /// 0x6c8
			 0x78121cc6, /// 0x6cc
			 0x8b841972, /// 0x6d0
			 0x061345fb, /// 0x6d4
			 0x21ba5745, /// 0x6d8
			 0x60de7b7e, /// 0x6dc
			 0x4ca189a1, /// 0x6e0
			 0x856b36dd, /// 0x6e4
			 0x340922c2, /// 0x6e8
			 0xdb7af496, /// 0x6ec
			 0x03b08dd4, /// 0x6f0
			 0x4e8ffb67, /// 0x6f4
			 0x69d52c65, /// 0x6f8
			 0x0ce32124, /// 0x6fc
			 0x5f774f95, /// 0x700
			 0xddd72b82, /// 0x704
			 0xeee382c9, /// 0x708
			 0x3b3f59c5, /// 0x70c
			 0x4601b30e, /// 0x710
			 0x782017e6, /// 0x714
			 0x92b36fff, /// 0x718
			 0xbaa5d549, /// 0x71c
			 0xf3081f66, /// 0x720
			 0x7980eec8, /// 0x724
			 0x36929b95, /// 0x728
			 0x7cdcc2f0, /// 0x72c
			 0xd8e09931, /// 0x730
			 0xa09b2597, /// 0x734
			 0x0f05f4d8, /// 0x738
			 0x93af48ef, /// 0x73c
			 0x1f47bc72, /// 0x740
			 0x9d3c6f4c, /// 0x744
			 0x3f178bac, /// 0x748
			 0x7e6ed8df, /// 0x74c
			 0xcbc0af29, /// 0x750
			 0x3b85ed2e, /// 0x754
			 0x79b7105f, /// 0x758
			 0x5908949b, /// 0x75c
			 0xe5d52eca, /// 0x760
			 0x75e9974c, /// 0x764
			 0x3d9b4062, /// 0x768
			 0x1a4ac57a, /// 0x76c
			 0x68578686, /// 0x770
			 0xc8ae7442, /// 0x774
			 0x4c3abb4b, /// 0x778
			 0xec96b2ee, /// 0x77c
			 0x7e614d9d, /// 0x780
			 0x7212de28, /// 0x784
			 0x450c1928, /// 0x788
			 0x11a8f783, /// 0x78c
			 0xeeb1fc47, /// 0x790
			 0x1255ddbe, /// 0x794
			 0x28b66184, /// 0x798
			 0xdbba94ea, /// 0x79c
			 0xcd00efe2, /// 0x7a0
			 0xd50efc66, /// 0x7a4
			 0x5f1b1312, /// 0x7a8
			 0xef971fe7, /// 0x7ac
			 0x03798c6f, /// 0x7b0
			 0x37595ada, /// 0x7b4
			 0x4ed5a06c, /// 0x7b8
			 0xcb25a7e3, /// 0x7bc
			 0x42a9fc6a, /// 0x7c0
			 0x238fe4d3, /// 0x7c4
			 0x1e1dd76e, /// 0x7c8
			 0x96322346, /// 0x7cc
			 0x065f6213, /// 0x7d0
			 0x2f556a90, /// 0x7d4
			 0x44431e83, /// 0x7d8
			 0x4bc029c1, /// 0x7dc
			 0x150e4758, /// 0x7e0
			 0xc41257a6, /// 0x7e4
			 0x8a06d5ab, /// 0x7e8
			 0x06711fde, /// 0x7ec
			 0x776476d2, /// 0x7f0
			 0xc1597057, /// 0x7f4
			 0x0f33839c, /// 0x7f8
			 0x6c6ed9cd, /// 0x7fc
			 0x47e1a984, /// 0x800
			 0xe5aa6e6e, /// 0x804
			 0xb46b94f7, /// 0x808
			 0x995329ec, /// 0x80c
			 0x9f533a61, /// 0x810
			 0xa6cd8e63, /// 0x814
			 0x44a4f69a, /// 0x818
			 0x7ff54f70, /// 0x81c
			 0xd112512e, /// 0x820
			 0x2b52167d, /// 0x824
			 0xc7f24781, /// 0x828
			 0x99d0a311, /// 0x82c
			 0xc6258fa9, /// 0x830
			 0xa12712c5, /// 0x834
			 0xed7bebe4, /// 0x838
			 0xd322e0f8, /// 0x83c
			 0x83f29fa1, /// 0x840
			 0x2cbdd219, /// 0x844
			 0x228e852d, /// 0x848
			 0x2edb4635, /// 0x84c
			 0x314799f0, /// 0x850
			 0xda480b91, /// 0x854
			 0x6ee68c29, /// 0x858
			 0xaff4be54, /// 0x85c
			 0xc7e7bbba, /// 0x860
			 0x51b8a08f, /// 0x864
			 0x96c596ef, /// 0x868
			 0xcb488450, /// 0x86c
			 0xad8c7546, /// 0x870
			 0x5615c18e, /// 0x874
			 0x9cf90190, /// 0x878
			 0x919dd210, /// 0x87c
			 0x34edd15d, /// 0x880
			 0xec8a1cd7, /// 0x884
			 0x3aa57aa8, /// 0x888
			 0x1c87153e, /// 0x88c
			 0xcde5c6e9, /// 0x890
			 0x724d3f47, /// 0x894
			 0xda2bc593, /// 0x898
			 0x88df915d, /// 0x89c
			 0x505281cc, /// 0x8a0
			 0x65fe41d8, /// 0x8a4
			 0x92138ba9, /// 0x8a8
			 0xc4862d58, /// 0x8ac
			 0x1a27c36f, /// 0x8b0
			 0x1dc82ec9, /// 0x8b4
			 0xc25ace55, /// 0x8b8
			 0x97301f11, /// 0x8bc
			 0xb0dc4a96, /// 0x8c0
			 0x26354c9e, /// 0x8c4
			 0xe0cd94eb, /// 0x8c8
			 0x44282b6f, /// 0x8cc
			 0xc0e4ef8e, /// 0x8d0
			 0xf34e9678, /// 0x8d4
			 0x521f2700, /// 0x8d8
			 0x4763d52a, /// 0x8dc
			 0x216ed48e, /// 0x8e0
			 0x1d31d065, /// 0x8e4
			 0x2a250d56, /// 0x8e8
			 0x4da1d1cc, /// 0x8ec
			 0xe63dd101, /// 0x8f0
			 0xe88808fa, /// 0x8f4
			 0xbaf1e5fc, /// 0x8f8
			 0x5d09642a, /// 0x8fc
			 0x235d5f91, /// 0x900
			 0x39b82424, /// 0x904
			 0x8965fc77, /// 0x908
			 0x2a7a9189, /// 0x90c
			 0x84a8ac2c, /// 0x910
			 0x0f86866f, /// 0x914
			 0x446eca3c, /// 0x918
			 0x0f450cdc, /// 0x91c
			 0x38ec3531, /// 0x920
			 0xd9384cc8, /// 0x924
			 0x6315ff2a, /// 0x928
			 0x199e5643, /// 0x92c
			 0x74c6a45f, /// 0x930
			 0xb5a35570, /// 0x934
			 0xcb4698f9, /// 0x938
			 0x131163cc, /// 0x93c
			 0xde7c0eb1, /// 0x940
			 0x2cf416ea, /// 0x944
			 0xc51ba4c4, /// 0x948
			 0xa829fdd4, /// 0x94c
			 0x6d807d20, /// 0x950
			 0xd76c7f7a, /// 0x954
			 0x7103f013, /// 0x958
			 0x42b56c09, /// 0x95c
			 0xd0ee7c38, /// 0x960
			 0x0f2f60df, /// 0x964
			 0x5a4ea0f4, /// 0x968
			 0x262db9cc, /// 0x96c
			 0xb9629b2d, /// 0x970
			 0xb407e1ae, /// 0x974
			 0x00c2644e, /// 0x978
			 0x3da6866b, /// 0x97c
			 0xf5dfbddc, /// 0x980
			 0x68f2dd1a, /// 0x984
			 0x904c6354, /// 0x988
			 0x7f525a77, /// 0x98c
			 0xa9a23524, /// 0x990
			 0x0997beff, /// 0x994
			 0xe522a7d9, /// 0x998
			 0x1bf54ad6, /// 0x99c
			 0x3bc4f994, /// 0x9a0
			 0x052cc6aa, /// 0x9a4
			 0x5ef73edf, /// 0x9a8
			 0x2eee6ba7, /// 0x9ac
			 0x65028463, /// 0x9b0
			 0x39a2ad03, /// 0x9b4
			 0x4b378f24, /// 0x9b8
			 0xc0515304, /// 0x9bc
			 0x9128acd1, /// 0x9c0
			 0x561a3553, /// 0x9c4
			 0x8fe8f877, /// 0x9c8
			 0xe618b328, /// 0x9cc
			 0x4e514197, /// 0x9d0
			 0x1270f667, /// 0x9d4
			 0x95e88dc6, /// 0x9d8
			 0x82bd763a, /// 0x9dc
			 0xe03e6dc6, /// 0x9e0
			 0x05c4b7df, /// 0x9e4
			 0x3708fdd0, /// 0x9e8
			 0xe3118ad7, /// 0x9ec
			 0xdb8935ee, /// 0x9f0
			 0x5815a377, /// 0x9f4
			 0x9647268c, /// 0x9f8
			 0x1b99bc40, /// 0x9fc
			 0x44f63551, /// 0xa00
			 0x89d78a4a, /// 0xa04
			 0x46770960, /// 0xa08
			 0x2af79ebb, /// 0xa0c
			 0x22e79a2d, /// 0xa10
			 0xca036516, /// 0xa14
			 0x99e35df1, /// 0xa18
			 0x78290a9d, /// 0xa1c
			 0x0e410a71, /// 0xa20
			 0xe39069ec, /// 0xa24
			 0xad3d8b2d, /// 0xa28
			 0x134079cc, /// 0xa2c
			 0x141be00c, /// 0xa30
			 0x7064ddda, /// 0xa34
			 0x1485945f, /// 0xa38
			 0x53e32588, /// 0xa3c
			 0x41c20748, /// 0xa40
			 0x21fc9f37, /// 0xa44
			 0x430465b5, /// 0xa48
			 0x4c5a65f9, /// 0xa4c
			 0x12d4192a, /// 0xa50
			 0x2c8751a7, /// 0xa54
			 0xa1470c93, /// 0xa58
			 0x98f44531, /// 0xa5c
			 0xc93a75c2, /// 0xa60
			 0xc7984e1c, /// 0xa64
			 0x91e344d7, /// 0xa68
			 0x5a97ff8c, /// 0xa6c
			 0xc9fb98d1, /// 0xa70
			 0xb6112e26, /// 0xa74
			 0x41d72614, /// 0xa78
			 0xa1b5c951, /// 0xa7c
			 0x3bff8c75, /// 0xa80
			 0x61208a5d, /// 0xa84
			 0xee439d87, /// 0xa88
			 0x34273978, /// 0xa8c
			 0x9bcdff3e, /// 0xa90
			 0x269df42b, /// 0xa94
			 0x7af40573, /// 0xa98
			 0x937fd642, /// 0xa9c
			 0xcfaad238, /// 0xaa0
			 0xd010c3a1, /// 0xaa4
			 0x7310d174, /// 0xaa8
			 0x64228d07, /// 0xaac
			 0xbba92a8b, /// 0xab0
			 0x429a61e1, /// 0xab4
			 0x95c58795, /// 0xab8
			 0x2d69d551, /// 0xabc
			 0x9b95563b, /// 0xac0
			 0x7a3635c9, /// 0xac4
			 0x37a4694e, /// 0xac8
			 0xa94c98f0, /// 0xacc
			 0x9695f3e5, /// 0xad0
			 0x2d1e9f45, /// 0xad4
			 0x0fd15919, /// 0xad8
			 0x7be58f47, /// 0xadc
			 0xa016c076, /// 0xae0
			 0x1fdd404c, /// 0xae4
			 0x9186a4e0, /// 0xae8
			 0x1f9daaf8, /// 0xaec
			 0x33f43613, /// 0xaf0
			 0xa0c6b079, /// 0xaf4
			 0xeba119d0, /// 0xaf8
			 0xe3658b5b, /// 0xafc
			 0xc15caf11, /// 0xb00
			 0x8048cd66, /// 0xb04
			 0xd83f1e61, /// 0xb08
			 0xb04e3f22, /// 0xb0c
			 0xda202bae, /// 0xb10
			 0x9d8f67bc, /// 0xb14
			 0xb41aa3e4, /// 0xb18
			 0xb113d922, /// 0xb1c
			 0xb78cf147, /// 0xb20
			 0xc617675f, /// 0xb24
			 0xd79d0cf8, /// 0xb28
			 0xc510496b, /// 0xb2c
			 0x27a9a494, /// 0xb30
			 0x464a2b0b, /// 0xb34
			 0xcba7f8dd, /// 0xb38
			 0xa722bc83, /// 0xb3c
			 0x8098af09, /// 0xb40
			 0x95ff4d3e, /// 0xb44
			 0xf7849878, /// 0xb48
			 0xede416a2, /// 0xb4c
			 0x37218059, /// 0xb50
			 0x84ea4e89, /// 0xb54
			 0xf9620362, /// 0xb58
			 0x89138afd, /// 0xb5c
			 0xfa40da9e, /// 0xb60
			 0x5ea37fc7, /// 0xb64
			 0x7cd34427, /// 0xb68
			 0xfd245e95, /// 0xb6c
			 0x15b00d7e, /// 0xb70
			 0x3e8e5440, /// 0xb74
			 0xfc333cd0, /// 0xb78
			 0xe0ea2234, /// 0xb7c
			 0xb6754840, /// 0xb80
			 0x0408f73f, /// 0xb84
			 0x2d241e9d, /// 0xb88
			 0xcf62ac65, /// 0xb8c
			 0x08e01133, /// 0xb90
			 0x3ac6d9f9, /// 0xb94
			 0xa6fa314f, /// 0xb98
			 0x88893454, /// 0xb9c
			 0xc6c85a8f, /// 0xba0
			 0xc476540a, /// 0xba4
			 0xe9ec0db8, /// 0xba8
			 0xe0650521, /// 0xbac
			 0xa87cf6f4, /// 0xbb0
			 0xe90fd47b, /// 0xbb4
			 0x3ad3337a, /// 0xbb8
			 0x6012d87b, /// 0xbbc
			 0xfec8cf8f, /// 0xbc0
			 0x04b1c2bc, /// 0xbc4
			 0xc1fb52d1, /// 0xbc8
			 0xb3709e95, /// 0xbcc
			 0xe49de5fb, /// 0xbd0
			 0x8ecbb403, /// 0xbd4
			 0xf3763f34, /// 0xbd8
			 0xdfdd50b1, /// 0xbdc
			 0x9b50c963, /// 0xbe0
			 0x38f03399, /// 0xbe4
			 0x2f413400, /// 0xbe8
			 0x695140c2, /// 0xbec
			 0xbbb9fb54, /// 0xbf0
			 0x41dada2a, /// 0xbf4
			 0x67f47c6f, /// 0xbf8
			 0x4dd0720c, /// 0xbfc
			 0x83dd29d8, /// 0xc00
			 0xa0e40a9f, /// 0xc04
			 0xe619416b, /// 0xc08
			 0xa2caf75e, /// 0xc0c
			 0x811e423d, /// 0xc10
			 0x9cde8748, /// 0xc14
			 0x4439f4b9, /// 0xc18
			 0xe5e2c0fe, /// 0xc1c
			 0xc1f46ff3, /// 0xc20
			 0x9c32abe0, /// 0xc24
			 0xfe1258f5, /// 0xc28
			 0xa20e1fec, /// 0xc2c
			 0x8aa7d9b9, /// 0xc30
			 0xd06eb830, /// 0xc34
			 0x07056a5f, /// 0xc38
			 0x80a994c8, /// 0xc3c
			 0x4e8e2fb7, /// 0xc40
			 0xc5e24459, /// 0xc44
			 0x8f15c978, /// 0xc48
			 0x94b545e5, /// 0xc4c
			 0x411b6e1a, /// 0xc50
			 0x2bf480e5, /// 0xc54
			 0x5db7d6d9, /// 0xc58
			 0xbcbf336e, /// 0xc5c
			 0x3c95d0e7, /// 0xc60
			 0x10b5c19a, /// 0xc64
			 0xdddc187c, /// 0xc68
			 0xf3f2383a, /// 0xc6c
			 0xed7d6595, /// 0xc70
			 0x370f147e, /// 0xc74
			 0xb1761753, /// 0xc78
			 0xb147c72f, /// 0xc7c
			 0xce3b99d6, /// 0xc80
			 0xc4b506d7, /// 0xc84
			 0x46411b4f, /// 0xc88
			 0xa85ad86e, /// 0xc8c
			 0x1a6c9ec5, /// 0xc90
			 0x7698ce66, /// 0xc94
			 0xb9b5f4df, /// 0xc98
			 0x9f06c612, /// 0xc9c
			 0xe7c48c96, /// 0xca0
			 0x549bae56, /// 0xca4
			 0x7d70c37a, /// 0xca8
			 0x186aa4fd, /// 0xcac
			 0xaaabc471, /// 0xcb0
			 0xfafdae99, /// 0xcb4
			 0xd7b1c73f, /// 0xcb8
			 0xaa2cebf6, /// 0xcbc
			 0x65a1aa06, /// 0xcc0
			 0x7c6010f3, /// 0xcc4
			 0xc3a2e2d8, /// 0xcc8
			 0xab605eb9, /// 0xccc
			 0xc133916c, /// 0xcd0
			 0xafd8c41a, /// 0xcd4
			 0x2bb2b2f2, /// 0xcd8
			 0x521646ae, /// 0xcdc
			 0x380b43d8, /// 0xce0
			 0x0fb12184, /// 0xce4
			 0x7f813760, /// 0xce8
			 0x9160b42d, /// 0xcec
			 0xe9f3812e, /// 0xcf0
			 0x34698a67, /// 0xcf4
			 0x18ae67c3, /// 0xcf8
			 0x1d5bdfcf, /// 0xcfc
			 0x08fa6483, /// 0xd00
			 0x757a1728, /// 0xd04
			 0x28fa70dd, /// 0xd08
			 0xfa671dd0, /// 0xd0c
			 0xae204415, /// 0xd10
			 0x23680340, /// 0xd14
			 0x2a3b5221, /// 0xd18
			 0x6ef55e13, /// 0xd1c
			 0x3c4ffca7, /// 0xd20
			 0xd3b63c44, /// 0xd24
			 0xb72a6303, /// 0xd28
			 0xff996a86, /// 0xd2c
			 0x7bd494d6, /// 0xd30
			 0x41b60ab0, /// 0xd34
			 0xe597ff63, /// 0xd38
			 0x54e7566a, /// 0xd3c
			 0x4f28a842, /// 0xd40
			 0xf89134ae, /// 0xd44
			 0x4ba8ecfb, /// 0xd48
			 0x774e0cb6, /// 0xd4c
			 0x62a00b62, /// 0xd50
			 0xa19c5ce5, /// 0xd54
			 0x723fb434, /// 0xd58
			 0x55c6e06a, /// 0xd5c
			 0x9641b7cc, /// 0xd60
			 0x94f4e417, /// 0xd64
			 0x1d3058dd, /// 0xd68
			 0x5d2703b2, /// 0xd6c
			 0xc8ba7368, /// 0xd70
			 0x97b78e9f, /// 0xd74
			 0xfead0403, /// 0xd78
			 0xca21fec0, /// 0xd7c
			 0xa4162f52, /// 0xd80
			 0x04e6ce77, /// 0xd84
			 0xd1f42cae, /// 0xd88
			 0xa5eb3fb6, /// 0xd8c
			 0x14ebc036, /// 0xd90
			 0x564aa59f, /// 0xd94
			 0xa9edb6eb, /// 0xd98
			 0xb97fffa2, /// 0xd9c
			 0xac4dc163, /// 0xda0
			 0x75b0f031, /// 0xda4
			 0x04d72342, /// 0xda8
			 0xf5c00b3f, /// 0xdac
			 0xaa7abadf, /// 0xdb0
			 0x93594022, /// 0xdb4
			 0xc3b2e721, /// 0xdb8
			 0xe9d23c3c, /// 0xdbc
			 0x6f7c35a4, /// 0xdc0
			 0xf0b1224c, /// 0xdc4
			 0xf331d997, /// 0xdc8
			 0x2c283263, /// 0xdcc
			 0xebc72746, /// 0xdd0
			 0x31353e0c, /// 0xdd4
			 0xfa63316e, /// 0xdd8
			 0xeab21d20, /// 0xddc
			 0x75807618, /// 0xde0
			 0x25e388fd, /// 0xde4
			 0x62fc5e9c, /// 0xde8
			 0x1d129b6a, /// 0xdec
			 0x5db2e57f, /// 0xdf0
			 0xaa95dedc, /// 0xdf4
			 0xe72bfd70, /// 0xdf8
			 0xc71e3698, /// 0xdfc
			 0x7f3621d0, /// 0xe00
			 0x4b8f871d, /// 0xe04
			 0x312be5c6, /// 0xe08
			 0xedde51bf, /// 0xe0c
			 0x05aab333, /// 0xe10
			 0xc2d47030, /// 0xe14
			 0xeecfa97e, /// 0xe18
			 0x56521315, /// 0xe1c
			 0x65c64406, /// 0xe20
			 0x102ef3f4, /// 0xe24
			 0x55c33c42, /// 0xe28
			 0xbe10a90b, /// 0xe2c
			 0x65bd6032, /// 0xe30
			 0x690b9616, /// 0xe34
			 0xc70b38d5, /// 0xe38
			 0x03054f2f, /// 0xe3c
			 0x7fae2c84, /// 0xe40
			 0xa7dbcb1f, /// 0xe44
			 0xd89c5618, /// 0xe48
			 0xdb91e042, /// 0xe4c
			 0x48d9aaf5, /// 0xe50
			 0x2a70a545, /// 0xe54
			 0xe3bfcc46, /// 0xe58
			 0xa0779aaf, /// 0xe5c
			 0x82346841, /// 0xe60
			 0xd49762b6, /// 0xe64
			 0x2a2086d0, /// 0xe68
			 0x55f62fc7, /// 0xe6c
			 0xf13707ec, /// 0xe70
			 0x30659211, /// 0xe74
			 0xcf7f3231, /// 0xe78
			 0x74cbb06b, /// 0xe7c
			 0xf49d6b91, /// 0xe80
			 0x60373174, /// 0xe84
			 0xb806b757, /// 0xe88
			 0x9b0125c5, /// 0xe8c
			 0x3610707d, /// 0xe90
			 0x8d9a7c80, /// 0xe94
			 0x844c6935, /// 0xe98
			 0xc78441fe, /// 0xe9c
			 0x2500d5db, /// 0xea0
			 0xf80a7e25, /// 0xea4
			 0x013da6f5, /// 0xea8
			 0xc1693197, /// 0xeac
			 0xa97a15a6, /// 0xeb0
			 0x0d234f79, /// 0xeb4
			 0xa417f24d, /// 0xeb8
			 0x817a74bc, /// 0xebc
			 0x74148231, /// 0xec0
			 0x7fa65584, /// 0xec4
			 0xb7df3ab3, /// 0xec8
			 0x22000048, /// 0xecc
			 0x34670623, /// 0xed0
			 0xfd352e20, /// 0xed4
			 0x29186f98, /// 0xed8
			 0x2717e4b8, /// 0xedc
			 0x53bedf03, /// 0xee0
			 0xbc0b16a5, /// 0xee4
			 0xf5f8f5cf, /// 0xee8
			 0xf7ce3739, /// 0xeec
			 0x63725dab, /// 0xef0
			 0xf216d5c7, /// 0xef4
			 0x0924ed36, /// 0xef8
			 0x36406a89, /// 0xefc
			 0x67e88b01, /// 0xf00
			 0x3334e951, /// 0xf04
			 0xca75792b, /// 0xf08
			 0x8ea8a151, /// 0xf0c
			 0x06eba1f3, /// 0xf10
			 0x982b6730, /// 0xf14
			 0xaeead3de, /// 0xf18
			 0xa5d38004, /// 0xf1c
			 0xfd364847, /// 0xf20
			 0x599d424b, /// 0xf24
			 0xd4ab3e61, /// 0xf28
			 0x3def5849, /// 0xf2c
			 0xb2cb7556, /// 0xf30
			 0x950e9127, /// 0xf34
			 0x07367dc2, /// 0xf38
			 0x608e12e7, /// 0xf3c
			 0xf260608e, /// 0xf40
			 0x30991862, /// 0xf44
			 0xb638593e, /// 0xf48
			 0xa8d65fec, /// 0xf4c
			 0xae00a6b8, /// 0xf50
			 0xdfbd021c, /// 0xf54
			 0x134869e3, /// 0xf58
			 0x9186d2b0, /// 0xf5c
			 0x38317916, /// 0xf60
			 0x556993b7, /// 0xf64
			 0x21ea8358, /// 0xf68
			 0x50f29a65, /// 0xf6c
			 0xa2fe2e4c, /// 0xf70
			 0xbd019b6d, /// 0xf74
			 0x94924619, /// 0xf78
			 0xfc8b2efe, /// 0xf7c
			 0x7cb12bba, /// 0xf80
			 0x9e192e5a, /// 0xf84
			 0x1767eafd, /// 0xf88
			 0xd66d235e, /// 0xf8c
			 0x0db7ac5e, /// 0xf90
			 0xb7c6ee4f, /// 0xf94
			 0x470673be, /// 0xf98
			 0xb422edae, /// 0xf9c
			 0xf72a8b0c, /// 0xfa0
			 0xbce02868, /// 0xfa4
			 0xf0d109c2, /// 0xfa8
			 0xb0b01136, /// 0xfac
			 0x31ff47d6, /// 0xfb0
			 0x444f85d4, /// 0xfb4
			 0x6d7be823, /// 0xfb8
			 0xf9a17dd6, /// 0xfbc
			 0xbf9d4f60, /// 0xfc0
			 0xf074fc65, /// 0xfc4
			 0x83e9edbc, /// 0xfc8
			 0x39352bcc, /// 0xfcc
			 0x7de2659b, /// 0xfd0
			 0xfd612c3d, /// 0xfd4
			 0xfd93c302, /// 0xfd8
			 0xd40e7bc3, /// 0xfdc
			 0x92432069, /// 0xfe0
			 0xaaf8e6af, /// 0xfe4
			 0x72e88edb, /// 0xfe8
			 0x540b5fac, /// 0xfec
			 0xa14584be, /// 0xff0
			 0xa2e5fa60, /// 0xff4
			 0xc3a1dfce, /// 0xff8
			 0x64a92fc8 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x1220e4e9, /// 0x0
			 0xeda536ea, /// 0x4
			 0xdf1cd533, /// 0x8
			 0x8b2b5c60, /// 0xc
			 0xd89f0324, /// 0x10
			 0x9477381c, /// 0x14
			 0x560b7146, /// 0x18
			 0xa8d80e4e, /// 0x1c
			 0xaf2d1804, /// 0x20
			 0xcebf3968, /// 0x24
			 0xe3cb4e6d, /// 0x28
			 0xa45ee013, /// 0x2c
			 0x4d9661f4, /// 0x30
			 0x6ea97d78, /// 0x34
			 0x538e9110, /// 0x38
			 0xa1b8b653, /// 0x3c
			 0x70dc6e77, /// 0x40
			 0x6e9e9d72, /// 0x44
			 0x291e38af, /// 0x48
			 0xf2318014, /// 0x4c
			 0x0573350a, /// 0x50
			 0x4bcb6368, /// 0x54
			 0x3097c193, /// 0x58
			 0xb7c41290, /// 0x5c
			 0x65dfd087, /// 0x60
			 0xd2a4b0a0, /// 0x64
			 0x426be703, /// 0x68
			 0xce9ee62e, /// 0x6c
			 0xd54dc6dc, /// 0x70
			 0xa0043c33, /// 0x74
			 0xe74fb376, /// 0x78
			 0x76b486d3, /// 0x7c
			 0x3269aa76, /// 0x80
			 0xfddfea7f, /// 0x84
			 0x070a7d57, /// 0x88
			 0x6e10cdc2, /// 0x8c
			 0xf128c7a0, /// 0x90
			 0x152110cd, /// 0x94
			 0xfc5ed0f0, /// 0x98
			 0x6dd7f833, /// 0x9c
			 0x77e1996b, /// 0xa0
			 0xe1a15f3c, /// 0xa4
			 0x13232a37, /// 0xa8
			 0x0e16bcaa, /// 0xac
			 0x093875e1, /// 0xb0
			 0x8a721c57, /// 0xb4
			 0x68db6628, /// 0xb8
			 0xf3588f6c, /// 0xbc
			 0x4a2f5bce, /// 0xc0
			 0x61d94176, /// 0xc4
			 0x9e9b07c0, /// 0xc8
			 0x41911d8a, /// 0xcc
			 0x84d858f0, /// 0xd0
			 0xa6567671, /// 0xd4
			 0x00f25bfc, /// 0xd8
			 0x4935840b, /// 0xdc
			 0x80a88f5e, /// 0xe0
			 0x9abb32ae, /// 0xe4
			 0x1710df36, /// 0xe8
			 0x2ec2c830, /// 0xec
			 0xf4309c9c, /// 0xf0
			 0x4994c18f, /// 0xf4
			 0xc9e0e5e1, /// 0xf8
			 0x80b8527d, /// 0xfc
			 0x408958c2, /// 0x100
			 0xd025de2d, /// 0x104
			 0xcc4c19ce, /// 0x108
			 0x91aa4f42, /// 0x10c
			 0xaeb3e276, /// 0x110
			 0x5c26f7bb, /// 0x114
			 0x52a0beee, /// 0x118
			 0x8b4e04d1, /// 0x11c
			 0x89ed1493, /// 0x120
			 0x03ded85c, /// 0x124
			 0x92f3b5f3, /// 0x128
			 0x5478ff03, /// 0x12c
			 0x8cc794bc, /// 0x130
			 0xd2bd4f7d, /// 0x134
			 0x16e88994, /// 0x138
			 0xf25d673f, /// 0x13c
			 0xfa25aee3, /// 0x140
			 0x4ebb0c80, /// 0x144
			 0x5f498607, /// 0x148
			 0xa0cca82c, /// 0x14c
			 0xa67eaf49, /// 0x150
			 0x297a993b, /// 0x154
			 0xd1be8cac, /// 0x158
			 0xc90fc509, /// 0x15c
			 0xa35829c6, /// 0x160
			 0x3712d486, /// 0x164
			 0xb34c25b2, /// 0x168
			 0xf716dbeb, /// 0x16c
			 0xf0bd7cfb, /// 0x170
			 0x5808859a, /// 0x174
			 0xd7fed507, /// 0x178
			 0xfdc59b43, /// 0x17c
			 0x11d3611d, /// 0x180
			 0xb2b6602b, /// 0x184
			 0x3d2c1793, /// 0x188
			 0x461381e4, /// 0x18c
			 0x04b5cde7, /// 0x190
			 0xa8135c82, /// 0x194
			 0x323bc51d, /// 0x198
			 0x8a34b0af, /// 0x19c
			 0x007f3baa, /// 0x1a0
			 0xa9fa7783, /// 0x1a4
			 0x1c005521, /// 0x1a8
			 0x1e63ab83, /// 0x1ac
			 0xd094af22, /// 0x1b0
			 0x5f235bc8, /// 0x1b4
			 0xcb1c707f, /// 0x1b8
			 0x3464ca39, /// 0x1bc
			 0x287665d2, /// 0x1c0
			 0xdceb24db, /// 0x1c4
			 0x8a3522d6, /// 0x1c8
			 0x4ad1c2bd, /// 0x1cc
			 0xc43855d1, /// 0x1d0
			 0x809f5136, /// 0x1d4
			 0x76ec929e, /// 0x1d8
			 0x063ab211, /// 0x1dc
			 0x9350709b, /// 0x1e0
			 0x23bc6cec, /// 0x1e4
			 0x92324535, /// 0x1e8
			 0x6749696b, /// 0x1ec
			 0x5b2e52a5, /// 0x1f0
			 0x33bfc97e, /// 0x1f4
			 0x94a603e5, /// 0x1f8
			 0x7d29e80d, /// 0x1fc
			 0x1f268c9e, /// 0x200
			 0xc7c01d60, /// 0x204
			 0x251d6339, /// 0x208
			 0x3f0c9c89, /// 0x20c
			 0xdda641dd, /// 0x210
			 0x42a4f6fc, /// 0x214
			 0x47d09c7c, /// 0x218
			 0x27a8eb20, /// 0x21c
			 0xdf6bf0f3, /// 0x220
			 0xd39667bb, /// 0x224
			 0xf408ac7e, /// 0x228
			 0x96ce005f, /// 0x22c
			 0x40a9382d, /// 0x230
			 0xf925b511, /// 0x234
			 0xa04642d4, /// 0x238
			 0xbd21f28e, /// 0x23c
			 0x25ba142e, /// 0x240
			 0x176b3034, /// 0x244
			 0x68128f6d, /// 0x248
			 0x6c4f306d, /// 0x24c
			 0xd2094481, /// 0x250
			 0xaa505732, /// 0x254
			 0xdc311ac4, /// 0x258
			 0x4f2f71cd, /// 0x25c
			 0x22817b86, /// 0x260
			 0x0a826c83, /// 0x264
			 0xd396b076, /// 0x268
			 0x9336c061, /// 0x26c
			 0xd85aaded, /// 0x270
			 0xdf3b862d, /// 0x274
			 0x8c5827f9, /// 0x278
			 0x8c2976ee, /// 0x27c
			 0x2478e4cb, /// 0x280
			 0x2e7bc476, /// 0x284
			 0x867fb204, /// 0x288
			 0x99eba305, /// 0x28c
			 0x009a299e, /// 0x290
			 0xf941c511, /// 0x294
			 0xda2d1f4e, /// 0x298
			 0x68b33939, /// 0x29c
			 0x2fe77564, /// 0x2a0
			 0xe26fb8d5, /// 0x2a4
			 0xebe1d312, /// 0x2a8
			 0x51f9c79d, /// 0x2ac
			 0x6265ce46, /// 0x2b0
			 0x3c8945d4, /// 0x2b4
			 0x46599eb5, /// 0x2b8
			 0xbf6489dc, /// 0x2bc
			 0x3250e24c, /// 0x2c0
			 0x3b17d371, /// 0x2c4
			 0x1c4d2b08, /// 0x2c8
			 0xad375244, /// 0x2cc
			 0x51337631, /// 0x2d0
			 0xc8b5fa5c, /// 0x2d4
			 0x645c5601, /// 0x2d8
			 0xd5f9e231, /// 0x2dc
			 0x592ec023, /// 0x2e0
			 0xb8f1784c, /// 0x2e4
			 0x9e4059f7, /// 0x2e8
			 0x9abf5a17, /// 0x2ec
			 0x7a2da903, /// 0x2f0
			 0x82f989e1, /// 0x2f4
			 0xf0583844, /// 0x2f8
			 0x31f3455b, /// 0x2fc
			 0xf7e60a29, /// 0x300
			 0x35118006, /// 0x304
			 0x6b3ebd8e, /// 0x308
			 0x79e28495, /// 0x30c
			 0x23e023b2, /// 0x310
			 0xbbd7336b, /// 0x314
			 0x9d2bbb6a, /// 0x318
			 0xb782da05, /// 0x31c
			 0x96c6e407, /// 0x320
			 0x39d97925, /// 0x324
			 0x5f50e139, /// 0x328
			 0x3809b856, /// 0x32c
			 0x7a8a2198, /// 0x330
			 0x71434463, /// 0x334
			 0x3058a579, /// 0x338
			 0x016666f9, /// 0x33c
			 0x686043c1, /// 0x340
			 0x06580d63, /// 0x344
			 0xdccddf52, /// 0x348
			 0xffe9e64e, /// 0x34c
			 0x8fcbc1ce, /// 0x350
			 0x6542c351, /// 0x354
			 0x1a4daa36, /// 0x358
			 0xcdfabdce, /// 0x35c
			 0xc82656c7, /// 0x360
			 0x4bd28ca2, /// 0x364
			 0x43ad9e7c, /// 0x368
			 0x0682e097, /// 0x36c
			 0x502610aa, /// 0x370
			 0x31f39b9f, /// 0x374
			 0x3dad8b18, /// 0x378
			 0x7649d6ca, /// 0x37c
			 0xe6966297, /// 0x380
			 0xcae3b8a1, /// 0x384
			 0x061ab0b0, /// 0x388
			 0x9409fe1c, /// 0x38c
			 0xd1c3b349, /// 0x390
			 0x57112404, /// 0x394
			 0x119b04b6, /// 0x398
			 0x5e76b800, /// 0x39c
			 0x8409558b, /// 0x3a0
			 0x45f749d9, /// 0x3a4
			 0x8f18a738, /// 0x3a8
			 0xadc26b4b, /// 0x3ac
			 0x85874c2b, /// 0x3b0
			 0x4fd54c5c, /// 0x3b4
			 0x35b18081, /// 0x3b8
			 0x86a3d171, /// 0x3bc
			 0x90511c4f, /// 0x3c0
			 0xfa88ba71, /// 0x3c4
			 0x467c0f8a, /// 0x3c8
			 0x0a29a612, /// 0x3cc
			 0x4b48205d, /// 0x3d0
			 0x2b3d795e, /// 0x3d4
			 0x49c47af9, /// 0x3d8
			 0x68681ed4, /// 0x3dc
			 0x635655a2, /// 0x3e0
			 0xa7ec0f12, /// 0x3e4
			 0x18450fd6, /// 0x3e8
			 0xf117b70d, /// 0x3ec
			 0xf4f95b27, /// 0x3f0
			 0x4c874189, /// 0x3f4
			 0xffafa847, /// 0x3f8
			 0x2de65411, /// 0x3fc
			 0xb5590317, /// 0x400
			 0x0c7ea5e3, /// 0x404
			 0xc48d8cc8, /// 0x408
			 0x487e09e0, /// 0x40c
			 0xf57cc72f, /// 0x410
			 0x8c047f5e, /// 0x414
			 0x8166527b, /// 0x418
			 0xdca9fafc, /// 0x41c
			 0xba05a9c4, /// 0x420
			 0x860a57de, /// 0x424
			 0x0f0afd27, /// 0x428
			 0x0652ac02, /// 0x42c
			 0xb06c8ccd, /// 0x430
			 0x7d45d77b, /// 0x434
			 0x36226caa, /// 0x438
			 0xf5ac79eb, /// 0x43c
			 0xc541b9d1, /// 0x440
			 0xa947ace9, /// 0x444
			 0x19fba78b, /// 0x448
			 0x93236114, /// 0x44c
			 0x56a8fd17, /// 0x450
			 0xe626c807, /// 0x454
			 0x6e242661, /// 0x458
			 0x484136b9, /// 0x45c
			 0x365ecdc9, /// 0x460
			 0x38e17a62, /// 0x464
			 0x59fd1430, /// 0x468
			 0xbe9c0eed, /// 0x46c
			 0xc1f75b9a, /// 0x470
			 0x80fc27bc, /// 0x474
			 0x679119d6, /// 0x478
			 0x052eb0dc, /// 0x47c
			 0xbe4f0fb4, /// 0x480
			 0x0e02e5ec, /// 0x484
			 0x5882311f, /// 0x488
			 0x4861c8f0, /// 0x48c
			 0xa3dd55fb, /// 0x490
			 0x6759f7d7, /// 0x494
			 0x01ece740, /// 0x498
			 0x76212b45, /// 0x49c
			 0xba396a67, /// 0x4a0
			 0x630e795d, /// 0x4a4
			 0xeea4c2b8, /// 0x4a8
			 0x73aa631e, /// 0x4ac
			 0x45452fb1, /// 0x4b0
			 0x7d2ab537, /// 0x4b4
			 0x1b25b3ce, /// 0x4b8
			 0x52582d99, /// 0x4bc
			 0x11cba9d8, /// 0x4c0
			 0x81b0fc82, /// 0x4c4
			 0x6109de97, /// 0x4c8
			 0x1a374b76, /// 0x4cc
			 0x149623ff, /// 0x4d0
			 0xccaceeb4, /// 0x4d4
			 0xb0266ded, /// 0x4d8
			 0x6f76ed20, /// 0x4dc
			 0x0f2e5700, /// 0x4e0
			 0x8f7c4e3e, /// 0x4e4
			 0x8db43243, /// 0x4e8
			 0xc622a420, /// 0x4ec
			 0x0217066c, /// 0x4f0
			 0xda25cbbb, /// 0x4f4
			 0xefd02bf0, /// 0x4f8
			 0x714bd2c0, /// 0x4fc
			 0xe84638a2, /// 0x500
			 0xdf294b94, /// 0x504
			 0x1ccfce3e, /// 0x508
			 0x6f4543cc, /// 0x50c
			 0x8784b954, /// 0x510
			 0x6eb8183f, /// 0x514
			 0x6ec50e50, /// 0x518
			 0xa6372cfa, /// 0x51c
			 0x0880e13f, /// 0x520
			 0x12a00b62, /// 0x524
			 0x593c44ab, /// 0x528
			 0x79c4637a, /// 0x52c
			 0xbb0b1a4e, /// 0x530
			 0xeaf54dfc, /// 0x534
			 0x332496f0, /// 0x538
			 0x64cf256d, /// 0x53c
			 0x6c0f0b85, /// 0x540
			 0xcc2b5b57, /// 0x544
			 0x3707a43a, /// 0x548
			 0xc6e17e83, /// 0x54c
			 0xcd6bfcfa, /// 0x550
			 0x15dd7382, /// 0x554
			 0x8a646ee6, /// 0x558
			 0x11d7a2f0, /// 0x55c
			 0xa13c4b8b, /// 0x560
			 0x662cc337, /// 0x564
			 0x6f122412, /// 0x568
			 0xca5a6923, /// 0x56c
			 0x39e3f3ad, /// 0x570
			 0xfb29f44a, /// 0x574
			 0x5f0d05d7, /// 0x578
			 0xc1a6b379, /// 0x57c
			 0x743093fd, /// 0x580
			 0xb2108a95, /// 0x584
			 0xaa43bdf9, /// 0x588
			 0xfb1a8845, /// 0x58c
			 0x999e2393, /// 0x590
			 0xc3d07977, /// 0x594
			 0xb20f568f, /// 0x598
			 0xf6d3fae7, /// 0x59c
			 0x3a55040a, /// 0x5a0
			 0xf976861d, /// 0x5a4
			 0x951825ed, /// 0x5a8
			 0x99b3e5f5, /// 0x5ac
			 0xa71de112, /// 0x5b0
			 0x7bfccf24, /// 0x5b4
			 0xeded6fa3, /// 0x5b8
			 0x9b700289, /// 0x5bc
			 0x797e7b28, /// 0x5c0
			 0x0f28a8fd, /// 0x5c4
			 0x7c05c4d7, /// 0x5c8
			 0xe85ac7d4, /// 0x5cc
			 0x54f5d6c7, /// 0x5d0
			 0x55ee99b8, /// 0x5d4
			 0x15d1e29e, /// 0x5d8
			 0x5ba82132, /// 0x5dc
			 0xbe694e0d, /// 0x5e0
			 0x88b1e368, /// 0x5e4
			 0x6c259895, /// 0x5e8
			 0xf9673ed7, /// 0x5ec
			 0xd9df71ac, /// 0x5f0
			 0xb0daaac6, /// 0x5f4
			 0x11cd63b5, /// 0x5f8
			 0xa4028085, /// 0x5fc
			 0x7b1a2136, /// 0x600
			 0x93c93484, /// 0x604
			 0x51a90bed, /// 0x608
			 0x62ba23e4, /// 0x60c
			 0x9d892572, /// 0x610
			 0xff2ad816, /// 0x614
			 0x16ffc050, /// 0x618
			 0xb9f08bfb, /// 0x61c
			 0x2b04e1a4, /// 0x620
			 0x3a17a757, /// 0x624
			 0xf491b836, /// 0x628
			 0x8ae6c841, /// 0x62c
			 0x550f4d48, /// 0x630
			 0xd55ebd10, /// 0x634
			 0x9e88bdcd, /// 0x638
			 0xea647087, /// 0x63c
			 0xdac76733, /// 0x640
			 0x5665e37e, /// 0x644
			 0xa1e836f5, /// 0x648
			 0xc978bcde, /// 0x64c
			 0x14b82bd7, /// 0x650
			 0xcec3d067, /// 0x654
			 0xcdd0e746, /// 0x658
			 0xf76a7389, /// 0x65c
			 0x0c51b984, /// 0x660
			 0x25ef95a9, /// 0x664
			 0x2c2d713a, /// 0x668
			 0xedde691a, /// 0x66c
			 0xa89872e8, /// 0x670
			 0x5b1a7bca, /// 0x674
			 0x29481e06, /// 0x678
			 0xfca5511e, /// 0x67c
			 0xb62d93d2, /// 0x680
			 0x98c157ef, /// 0x684
			 0x13ae6fca, /// 0x688
			 0x5da03c51, /// 0x68c
			 0x659ab7de, /// 0x690
			 0x0062608c, /// 0x694
			 0xa3dd4d06, /// 0x698
			 0xb0fad966, /// 0x69c
			 0x3b09d0ae, /// 0x6a0
			 0x9571ae58, /// 0x6a4
			 0x8e8e8cf2, /// 0x6a8
			 0x40135cdb, /// 0x6ac
			 0x570936c2, /// 0x6b0
			 0x806629fe, /// 0x6b4
			 0x5b23e52e, /// 0x6b8
			 0xaf4696ab, /// 0x6bc
			 0xfaf104fa, /// 0x6c0
			 0x0276a490, /// 0x6c4
			 0xb49224c6, /// 0x6c8
			 0x0b1c175f, /// 0x6cc
			 0x85a33325, /// 0x6d0
			 0xa1098176, /// 0x6d4
			 0xc25bf58d, /// 0x6d8
			 0xea8c467b, /// 0x6dc
			 0x7bdc3b70, /// 0x6e0
			 0x238584fd, /// 0x6e4
			 0xf5414ce2, /// 0x6e8
			 0x44e144ea, /// 0x6ec
			 0xefbf721b, /// 0x6f0
			 0xfee9550f, /// 0x6f4
			 0xccd2c70d, /// 0x6f8
			 0x4e3b6346, /// 0x6fc
			 0x0f1ae506, /// 0x700
			 0x9f36e878, /// 0x704
			 0xc0d4068e, /// 0x708
			 0x040ed67f, /// 0x70c
			 0x47088564, /// 0x710
			 0xd3de9d9c, /// 0x714
			 0xfc11e8c3, /// 0x718
			 0x5bf1e196, /// 0x71c
			 0x3b020297, /// 0x720
			 0x4499120d, /// 0x724
			 0xb0a00f84, /// 0x728
			 0x57669a8f, /// 0x72c
			 0x9ab52335, /// 0x730
			 0x18f054cf, /// 0x734
			 0x5bad9ec6, /// 0x738
			 0xb1602e90, /// 0x73c
			 0xb7d32236, /// 0x740
			 0xd2bf4a72, /// 0x744
			 0xcba33c05, /// 0x748
			 0xad55d88e, /// 0x74c
			 0xe9e72714, /// 0x750
			 0xece7f502, /// 0x754
			 0x8ed9afbe, /// 0x758
			 0xa8c5b5a2, /// 0x75c
			 0x241e61ee, /// 0x760
			 0x23d338b6, /// 0x764
			 0xdb89170d, /// 0x768
			 0xe0df0303, /// 0x76c
			 0xc8568464, /// 0x770
			 0x2877072e, /// 0x774
			 0xc930cb07, /// 0x778
			 0x03891188, /// 0x77c
			 0x49b7a5e8, /// 0x780
			 0x3377d4ad, /// 0x784
			 0x4029f1aa, /// 0x788
			 0xfaa4cd8a, /// 0x78c
			 0x053673a1, /// 0x790
			 0xf0f6699d, /// 0x794
			 0xcd131eee, /// 0x798
			 0xae0ed6a7, /// 0x79c
			 0x14d67256, /// 0x7a0
			 0x50cecdd2, /// 0x7a4
			 0xb4aadc25, /// 0x7a8
			 0x679c2256, /// 0x7ac
			 0xfe24f672, /// 0x7b0
			 0x79328124, /// 0x7b4
			 0xf30a9cca, /// 0x7b8
			 0x1dddb4a6, /// 0x7bc
			 0x1bbf8329, /// 0x7c0
			 0x4463aab7, /// 0x7c4
			 0x7759a478, /// 0x7c8
			 0x02c58877, /// 0x7cc
			 0x3b8ebb41, /// 0x7d0
			 0x6efc44a7, /// 0x7d4
			 0x3136c596, /// 0x7d8
			 0x204bfae6, /// 0x7dc
			 0xff725e36, /// 0x7e0
			 0xe9609f93, /// 0x7e4
			 0xad6ef574, /// 0x7e8
			 0x44ba859e, /// 0x7ec
			 0x9cb7f191, /// 0x7f0
			 0x4a48f536, /// 0x7f4
			 0x308a2f9d, /// 0x7f8
			 0x3c913e42, /// 0x7fc
			 0x0a61ea15, /// 0x800
			 0xeb76ad56, /// 0x804
			 0xc9ce4280, /// 0x808
			 0x6503ef29, /// 0x80c
			 0x4a2d1c1f, /// 0x810
			 0xd69128c7, /// 0x814
			 0x065dd32c, /// 0x818
			 0x25640e14, /// 0x81c
			 0xebeba3ef, /// 0x820
			 0x48075333, /// 0x824
			 0x3cabc1c6, /// 0x828
			 0x576013f7, /// 0x82c
			 0x0c1a0bc1, /// 0x830
			 0x61d2f3a4, /// 0x834
			 0xc6d287f1, /// 0x838
			 0xd3a592d1, /// 0x83c
			 0x0c545658, /// 0x840
			 0x4b9f9159, /// 0x844
			 0x92368c97, /// 0x848
			 0x17682928, /// 0x84c
			 0x522cdb98, /// 0x850
			 0x80627e3f, /// 0x854
			 0xfd4bbcbe, /// 0x858
			 0x67fa711d, /// 0x85c
			 0xc7f6622c, /// 0x860
			 0x1ea53840, /// 0x864
			 0x407f567b, /// 0x868
			 0x38ceada7, /// 0x86c
			 0x2bf7fcba, /// 0x870
			 0x6660c904, /// 0x874
			 0x50325ae9, /// 0x878
			 0x27e23093, /// 0x87c
			 0x000e2d34, /// 0x880
			 0x4249e543, /// 0x884
			 0x71b9396b, /// 0x888
			 0x1cad9a8f, /// 0x88c
			 0xe9360915, /// 0x890
			 0xbcb5a61a, /// 0x894
			 0x6b826049, /// 0x898
			 0x12f96733, /// 0x89c
			 0xa0787698, /// 0x8a0
			 0x58e199da, /// 0x8a4
			 0xcfda1af6, /// 0x8a8
			 0x66cc4c31, /// 0x8ac
			 0x21934fbf, /// 0x8b0
			 0x3b1abda9, /// 0x8b4
			 0xddacad59, /// 0x8b8
			 0xc2844df7, /// 0x8bc
			 0xc66d1cdb, /// 0x8c0
			 0x72c3480a, /// 0x8c4
			 0xa7f15300, /// 0x8c8
			 0x6ee9491d, /// 0x8cc
			 0x9601e647, /// 0x8d0
			 0x940f919d, /// 0x8d4
			 0x6348bfaa, /// 0x8d8
			 0x4b37f1c8, /// 0x8dc
			 0xeb87e4ab, /// 0x8e0
			 0x89a4bf54, /// 0x8e4
			 0xab6ffa7e, /// 0x8e8
			 0xd40a9a2c, /// 0x8ec
			 0xc704fe83, /// 0x8f0
			 0x95b3148e, /// 0x8f4
			 0xeaf4626f, /// 0x8f8
			 0x7f4c600c, /// 0x8fc
			 0x54d60053, /// 0x900
			 0xa8672ce7, /// 0x904
			 0x059ecbd6, /// 0x908
			 0xbca9889e, /// 0x90c
			 0x56b1f6d6, /// 0x910
			 0x8b64eba5, /// 0x914
			 0x36c7890e, /// 0x918
			 0xa383c27d, /// 0x91c
			 0xe90d9b10, /// 0x920
			 0x2c11e7ba, /// 0x924
			 0x385bd3df, /// 0x928
			 0x62bf7b78, /// 0x92c
			 0x601ec32d, /// 0x930
			 0xbdca1c99, /// 0x934
			 0x53a18b90, /// 0x938
			 0x157bf540, /// 0x93c
			 0x9c2c5e73, /// 0x940
			 0x26164926, /// 0x944
			 0x87de79bb, /// 0x948
			 0x771c3d5b, /// 0x94c
			 0x29974cf6, /// 0x950
			 0xd14b1dac, /// 0x954
			 0x52f6a202, /// 0x958
			 0x86d4d4a1, /// 0x95c
			 0x6d521d49, /// 0x960
			 0x6ae4ee75, /// 0x964
			 0x61cd981c, /// 0x968
			 0x2735e796, /// 0x96c
			 0x276cdc26, /// 0x970
			 0xeaf00e80, /// 0x974
			 0x7009876f, /// 0x978
			 0x42a41131, /// 0x97c
			 0x632f105c, /// 0x980
			 0x8f39fb9d, /// 0x984
			 0xff4ee577, /// 0x988
			 0xba936f17, /// 0x98c
			 0x7ba8f6c1, /// 0x990
			 0xc7d9c6bb, /// 0x994
			 0xbe4be94a, /// 0x998
			 0x284d9b57, /// 0x99c
			 0x751f2f06, /// 0x9a0
			 0xb0879a57, /// 0x9a4
			 0xb544eeda, /// 0x9a8
			 0x61ed16c9, /// 0x9ac
			 0x938eec37, /// 0x9b0
			 0xddcba146, /// 0x9b4
			 0xb15a770d, /// 0x9b8
			 0xf635f46e, /// 0x9bc
			 0xee99c56a, /// 0x9c0
			 0x1efff661, /// 0x9c4
			 0xb654bf99, /// 0x9c8
			 0x48852f32, /// 0x9cc
			 0xd98c0a91, /// 0x9d0
			 0x1738d06a, /// 0x9d4
			 0x289333da, /// 0x9d8
			 0xf6e80336, /// 0x9dc
			 0xe14f4397, /// 0x9e0
			 0x353e600f, /// 0x9e4
			 0x86b5de59, /// 0x9e8
			 0x9ee4d1ad, /// 0x9ec
			 0x48682e8b, /// 0x9f0
			 0xd967d547, /// 0x9f4
			 0xdf5d2259, /// 0x9f8
			 0x71d0d234, /// 0x9fc
			 0x0a386c73, /// 0xa00
			 0x5dcff078, /// 0xa04
			 0xbd81d35d, /// 0xa08
			 0x5e65b151, /// 0xa0c
			 0x1a071599, /// 0xa10
			 0xa7c6bf7d, /// 0xa14
			 0xa4f972ea, /// 0xa18
			 0xaab68570, /// 0xa1c
			 0x0900f399, /// 0xa20
			 0x3fc45474, /// 0xa24
			 0x6359f30c, /// 0xa28
			 0x2c1bfe79, /// 0xa2c
			 0x6bd43e3e, /// 0xa30
			 0x76471b03, /// 0xa34
			 0x5ef5aedb, /// 0xa38
			 0xb4396351, /// 0xa3c
			 0xa2cb0ffe, /// 0xa40
			 0xe378b1d4, /// 0xa44
			 0x7d97f438, /// 0xa48
			 0xb273843d, /// 0xa4c
			 0x0e461cbb, /// 0xa50
			 0xc17317fb, /// 0xa54
			 0x71bf6704, /// 0xa58
			 0x2f8f3dc2, /// 0xa5c
			 0x15d6a175, /// 0xa60
			 0xc660eeb2, /// 0xa64
			 0x84f98392, /// 0xa68
			 0x9483fefc, /// 0xa6c
			 0xbe965f6b, /// 0xa70
			 0x0a03bf5f, /// 0xa74
			 0x5ec2454d, /// 0xa78
			 0x37a768c5, /// 0xa7c
			 0x649fb251, /// 0xa80
			 0x0fc244d4, /// 0xa84
			 0x138b009e, /// 0xa88
			 0xc3f34a72, /// 0xa8c
			 0xdc4e1a73, /// 0xa90
			 0x4f29aa5b, /// 0xa94
			 0x3d4a948d, /// 0xa98
			 0x51e74621, /// 0xa9c
			 0x6bf24fa9, /// 0xaa0
			 0x88983aa1, /// 0xaa4
			 0x34e2e004, /// 0xaa8
			 0x137b004f, /// 0xaac
			 0x94cb4dcf, /// 0xab0
			 0x472b81d5, /// 0xab4
			 0x7e21a994, /// 0xab8
			 0x8cedef57, /// 0xabc
			 0xaea00ece, /// 0xac0
			 0x50ae18cf, /// 0xac4
			 0x8b03dd9c, /// 0xac8
			 0xd1de0461, /// 0xacc
			 0x3b4ee9fe, /// 0xad0
			 0x9109465a, /// 0xad4
			 0xd71a7aae, /// 0xad8
			 0x0ffdd750, /// 0xadc
			 0x07270cf2, /// 0xae0
			 0x5b868044, /// 0xae4
			 0x7fba555a, /// 0xae8
			 0xbca2bbff, /// 0xaec
			 0x4fd1a194, /// 0xaf0
			 0x3a185bfd, /// 0xaf4
			 0xe84fd14d, /// 0xaf8
			 0x830e5b9d, /// 0xafc
			 0x59d2651f, /// 0xb00
			 0x7a8a3d09, /// 0xb04
			 0x2f9e5083, /// 0xb08
			 0x8a98a5a0, /// 0xb0c
			 0x6cb65627, /// 0xb10
			 0xd02932e2, /// 0xb14
			 0x59463ca1, /// 0xb18
			 0xc85bd1f0, /// 0xb1c
			 0x5d2e80ef, /// 0xb20
			 0x503c1e37, /// 0xb24
			 0xb269dd3b, /// 0xb28
			 0x3ab928f8, /// 0xb2c
			 0x8173f67c, /// 0xb30
			 0x7e8e3213, /// 0xb34
			 0x1a506aac, /// 0xb38
			 0x343b795a, /// 0xb3c
			 0xcab3af61, /// 0xb40
			 0x2730385b, /// 0xb44
			 0xb279bb39, /// 0xb48
			 0x46a84fe1, /// 0xb4c
			 0x78fda6d0, /// 0xb50
			 0x724735bb, /// 0xb54
			 0x5a446eaa, /// 0xb58
			 0x9990bbac, /// 0xb5c
			 0x694bd248, /// 0xb60
			 0x8246c981, /// 0xb64
			 0x549cbb5c, /// 0xb68
			 0x6f0e2972, /// 0xb6c
			 0x474e25c4, /// 0xb70
			 0x89ef0631, /// 0xb74
			 0xc2d873b5, /// 0xb78
			 0xb2bbfff5, /// 0xb7c
			 0xdb4dbec2, /// 0xb80
			 0x7e4d8c2a, /// 0xb84
			 0x8cd33877, /// 0xb88
			 0xf12d56ff, /// 0xb8c
			 0x36bb77f9, /// 0xb90
			 0xf0b98888, /// 0xb94
			 0x2a55a579, /// 0xb98
			 0x0561d73c, /// 0xb9c
			 0xb7770f76, /// 0xba0
			 0xcb569284, /// 0xba4
			 0x6dfe5ecf, /// 0xba8
			 0xea5d92b0, /// 0xbac
			 0xc1c118e1, /// 0xbb0
			 0xeddf7580, /// 0xbb4
			 0x913b32d2, /// 0xbb8
			 0xc2b136e4, /// 0xbbc
			 0xb48a380b, /// 0xbc0
			 0x236f5bb5, /// 0xbc4
			 0x5f0193e5, /// 0xbc8
			 0xec5742cf, /// 0xbcc
			 0x2f00782b, /// 0xbd0
			 0x157f01ff, /// 0xbd4
			 0x1af84994, /// 0xbd8
			 0xca33fa53, /// 0xbdc
			 0x60f8517e, /// 0xbe0
			 0xb3edca53, /// 0xbe4
			 0xcd1f473e, /// 0xbe8
			 0x1037128b, /// 0xbec
			 0xee2808fd, /// 0xbf0
			 0xa4dd2479, /// 0xbf4
			 0xb43917c0, /// 0xbf8
			 0x3f4e306d, /// 0xbfc
			 0xc4041314, /// 0xc00
			 0x04b6d114, /// 0xc04
			 0xbb195959, /// 0xc08
			 0xd968c686, /// 0xc0c
			 0x73765f03, /// 0xc10
			 0xb096d0c3, /// 0xc14
			 0x22b2895f, /// 0xc18
			 0xc641c77b, /// 0xc1c
			 0x3b2755da, /// 0xc20
			 0x07af63c1, /// 0xc24
			 0xebce9570, /// 0xc28
			 0xf335b6bb, /// 0xc2c
			 0x719c8d00, /// 0xc30
			 0x4a35c056, /// 0xc34
			 0x23476eb7, /// 0xc38
			 0x252794b9, /// 0xc3c
			 0xf3d09f80, /// 0xc40
			 0xe4116ce3, /// 0xc44
			 0xe65a55e8, /// 0xc48
			 0x9d0558fe, /// 0xc4c
			 0x7deafe11, /// 0xc50
			 0x9980baf8, /// 0xc54
			 0x65ff61b4, /// 0xc58
			 0x7e64aa33, /// 0xc5c
			 0xa970747f, /// 0xc60
			 0x56b79f9e, /// 0xc64
			 0x9341b52e, /// 0xc68
			 0x105e6316, /// 0xc6c
			 0x53b3d12b, /// 0xc70
			 0xe5a91515, /// 0xc74
			 0xf881ecd7, /// 0xc78
			 0x8127c5f9, /// 0xc7c
			 0x4d1a6557, /// 0xc80
			 0xc904268c, /// 0xc84
			 0xf775be56, /// 0xc88
			 0xfe1a79f6, /// 0xc8c
			 0x200bacd7, /// 0xc90
			 0x214a9b7c, /// 0xc94
			 0x25d5dcb9, /// 0xc98
			 0xc65a4df6, /// 0xc9c
			 0x087aaa51, /// 0xca0
			 0x0090546c, /// 0xca4
			 0xd19beadf, /// 0xca8
			 0x168c8a00, /// 0xcac
			 0x3c61ad08, /// 0xcb0
			 0xd7d69c57, /// 0xcb4
			 0x6459e213, /// 0xcb8
			 0xc2ed6594, /// 0xcbc
			 0x0eba2c6a, /// 0xcc0
			 0x216cd59f, /// 0xcc4
			 0x7f945d38, /// 0xcc8
			 0x083ae79d, /// 0xccc
			 0xc29d8cea, /// 0xcd0
			 0x980f6771, /// 0xcd4
			 0xfb8140be, /// 0xcd8
			 0x285addab, /// 0xcdc
			 0xa6017f69, /// 0xce0
			 0x5e7d7004, /// 0xce4
			 0x43116df5, /// 0xce8
			 0x265b3879, /// 0xcec
			 0x6f015228, /// 0xcf0
			 0xd35f057c, /// 0xcf4
			 0xfe8b758e, /// 0xcf8
			 0xb7f20cc8, /// 0xcfc
			 0x80789290, /// 0xd00
			 0xa7021112, /// 0xd04
			 0x2c5860f4, /// 0xd08
			 0x1f0b1d79, /// 0xd0c
			 0xcf24e932, /// 0xd10
			 0x7f281dd0, /// 0xd14
			 0x2587b6a7, /// 0xd18
			 0x8840efc8, /// 0xd1c
			 0xb0b55382, /// 0xd20
			 0xb1594ed3, /// 0xd24
			 0x3cc16b69, /// 0xd28
			 0x3596e711, /// 0xd2c
			 0x8be1fa12, /// 0xd30
			 0xe6506126, /// 0xd34
			 0x1506e3e2, /// 0xd38
			 0x5f9b45b3, /// 0xd3c
			 0xff1972f2, /// 0xd40
			 0x3d47876f, /// 0xd44
			 0x7e7aa161, /// 0xd48
			 0xe8699662, /// 0xd4c
			 0x5da1f51d, /// 0xd50
			 0x6b6d3b28, /// 0xd54
			 0x2c57488f, /// 0xd58
			 0x479f794c, /// 0xd5c
			 0xc7a5ed05, /// 0xd60
			 0x5f16b92e, /// 0xd64
			 0xeb8eaec3, /// 0xd68
			 0x84c0547c, /// 0xd6c
			 0x584b20fd, /// 0xd70
			 0x163f925b, /// 0xd74
			 0x5fd73fb3, /// 0xd78
			 0x833c6c09, /// 0xd7c
			 0xd1089d4b, /// 0xd80
			 0x400eb591, /// 0xd84
			 0x0214ae47, /// 0xd88
			 0x7dc617d5, /// 0xd8c
			 0x4b4de00f, /// 0xd90
			 0x4facd2e3, /// 0xd94
			 0x605f3c5a, /// 0xd98
			 0x3f48a085, /// 0xd9c
			 0xd01a19b5, /// 0xda0
			 0xb15eb01f, /// 0xda4
			 0x3c7aa9e9, /// 0xda8
			 0x38da7261, /// 0xdac
			 0x683efd2c, /// 0xdb0
			 0xd125858a, /// 0xdb4
			 0xd1b43151, /// 0xdb8
			 0x61d56beb, /// 0xdbc
			 0x28831fbe, /// 0xdc0
			 0x664504f9, /// 0xdc4
			 0xb2daf640, /// 0xdc8
			 0x758f86f8, /// 0xdcc
			 0xe3405ae7, /// 0xdd0
			 0xc126a400, /// 0xdd4
			 0xcdda8d38, /// 0xdd8
			 0xceca3293, /// 0xddc
			 0xabf8c5b3, /// 0xde0
			 0x9bfcce11, /// 0xde4
			 0x2e0e9576, /// 0xde8
			 0xb13424d7, /// 0xdec
			 0x01db647f, /// 0xdf0
			 0xb1d6cac8, /// 0xdf4
			 0x6d631f03, /// 0xdf8
			 0x23f5a7f4, /// 0xdfc
			 0xa50f745c, /// 0xe00
			 0xac116f65, /// 0xe04
			 0xcb0979d2, /// 0xe08
			 0xb99da6ff, /// 0xe0c
			 0x39f4ed5c, /// 0xe10
			 0x91df1d0c, /// 0xe14
			 0xa8b6806b, /// 0xe18
			 0xb04377db, /// 0xe1c
			 0xdea3b0eb, /// 0xe20
			 0xbf0ae93b, /// 0xe24
			 0x6d5c3940, /// 0xe28
			 0xca9f91fa, /// 0xe2c
			 0x05013135, /// 0xe30
			 0xa1a77a88, /// 0xe34
			 0xe83611b7, /// 0xe38
			 0x3254417b, /// 0xe3c
			 0xb7d270a2, /// 0xe40
			 0x2d8e1d18, /// 0xe44
			 0x414608e0, /// 0xe48
			 0x20a10bae, /// 0xe4c
			 0xc9ab549b, /// 0xe50
			 0xca58e2e0, /// 0xe54
			 0x4b53865c, /// 0xe58
			 0x6061ec2d, /// 0xe5c
			 0x4974cf98, /// 0xe60
			 0x0877508a, /// 0xe64
			 0x31e83bf1, /// 0xe68
			 0xc7a21a95, /// 0xe6c
			 0x6769977e, /// 0xe70
			 0xe49b1927, /// 0xe74
			 0x61d245c1, /// 0xe78
			 0xb3eae8fa, /// 0xe7c
			 0x894abb40, /// 0xe80
			 0xabec87c8, /// 0xe84
			 0xa68bcbbd, /// 0xe88
			 0x3dd7dfe7, /// 0xe8c
			 0xb4fbb180, /// 0xe90
			 0x6e514797, /// 0xe94
			 0x7a553d04, /// 0xe98
			 0xeca9dddf, /// 0xe9c
			 0x54b098b8, /// 0xea0
			 0xefda0a3b, /// 0xea4
			 0x14de1e8a, /// 0xea8
			 0x87e7efe3, /// 0xeac
			 0x2f261c0e, /// 0xeb0
			 0x51b45538, /// 0xeb4
			 0xf99013bc, /// 0xeb8
			 0xe8cc080c, /// 0xebc
			 0xd5752204, /// 0xec0
			 0x6b6ad938, /// 0xec4
			 0xb8490903, /// 0xec8
			 0xef57b1ca, /// 0xecc
			 0x0e00324e, /// 0xed0
			 0x46ad940f, /// 0xed4
			 0xd84c4e9b, /// 0xed8
			 0x1bcd0c42, /// 0xedc
			 0xf65a4324, /// 0xee0
			 0xa01ba7cb, /// 0xee4
			 0xe83bdd3e, /// 0xee8
			 0xc475a05b, /// 0xeec
			 0xc5329adf, /// 0xef0
			 0xd3743fa0, /// 0xef4
			 0x2740e0a9, /// 0xef8
			 0x965bbe6b, /// 0xefc
			 0x86f3e6a1, /// 0xf00
			 0x1a77e17a, /// 0xf04
			 0x1cf0e4e1, /// 0xf08
			 0xfd3e8aee, /// 0xf0c
			 0x078b379c, /// 0xf10
			 0x980e95f0, /// 0xf14
			 0x1efcc38b, /// 0xf18
			 0x769e8554, /// 0xf1c
			 0x56ad20cd, /// 0xf20
			 0xa131e3a2, /// 0xf24
			 0xe3757fe1, /// 0xf28
			 0x26d7d010, /// 0xf2c
			 0xa6db262d, /// 0xf30
			 0xa8570b29, /// 0xf34
			 0x2634b83c, /// 0xf38
			 0xe97ffd4e, /// 0xf3c
			 0xfc64cc36, /// 0xf40
			 0xb0be84a7, /// 0xf44
			 0x73ef434f, /// 0xf48
			 0x814ef3b6, /// 0xf4c
			 0x0874becc, /// 0xf50
			 0xe72d923d, /// 0xf54
			 0x75c9dd9f, /// 0xf58
			 0xea6994e2, /// 0xf5c
			 0x7973ff57, /// 0xf60
			 0x1bb72541, /// 0xf64
			 0x0bcc771d, /// 0xf68
			 0x6f0e1eee, /// 0xf6c
			 0xa9b27b0f, /// 0xf70
			 0x3807b57b, /// 0xf74
			 0x967b2db5, /// 0xf78
			 0x17bf9ed5, /// 0xf7c
			 0x8490b179, /// 0xf80
			 0xac22d6b6, /// 0xf84
			 0xea74295b, /// 0xf88
			 0x7a6b0fd6, /// 0xf8c
			 0xe713f9f1, /// 0xf90
			 0x16c6448a, /// 0xf94
			 0x999918ef, /// 0xf98
			 0x69555f39, /// 0xf9c
			 0xf7c2aee6, /// 0xfa0
			 0x007986a1, /// 0xfa4
			 0xcbfb6296, /// 0xfa8
			 0x8ea7c6a4, /// 0xfac
			 0x37ecfe73, /// 0xfb0
			 0xc126baee, /// 0xfb4
			 0x06ea082b, /// 0xfb8
			 0x5ba1c8e1, /// 0xfbc
			 0x54293985, /// 0xfc0
			 0xaa24d068, /// 0xfc4
			 0x2a7b7ccf, /// 0xfc8
			 0xb53c4ae7, /// 0xfcc
			 0x602d1b57, /// 0xfd0
			 0xecfb820b, /// 0xfd4
			 0x0e02421a, /// 0xfd8
			 0x7e8f973c, /// 0xfdc
			 0x1379aaf9, /// 0xfe0
			 0x44cbe3c3, /// 0xfe4
			 0xca2ff571, /// 0xfe8
			 0xea46bfb5, /// 0xfec
			 0xc9e27115, /// 0xff0
			 0xfce3742b, /// 0xff4
			 0x05295570, /// 0xff8
			 0xb5de8dfe /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x07456009, /// 0x0
			 0xe2d3e0a1, /// 0x4
			 0x71c18235, /// 0x8
			 0x39c6d5bd, /// 0xc
			 0xdc033d19, /// 0x10
			 0xb1ab4fde, /// 0x14
			 0xcf9ab25e, /// 0x18
			 0xe5060076, /// 0x1c
			 0xa0c62b10, /// 0x20
			 0xc74194dc, /// 0x24
			 0x116a72eb, /// 0x28
			 0xd2b2eff6, /// 0x2c
			 0x2d712965, /// 0x30
			 0x910845c9, /// 0x34
			 0x7bd641cc, /// 0x38
			 0xf5ef6c4a, /// 0x3c
			 0xa9b8e8c4, /// 0x40
			 0x2bc6695a, /// 0x44
			 0xcd8ebb2a, /// 0x48
			 0xea57892c, /// 0x4c
			 0x7f25b5ed, /// 0x50
			 0x2f73ef3e, /// 0x54
			 0x96d7f2c9, /// 0x58
			 0xbe238805, /// 0x5c
			 0x48ed4307, /// 0x60
			 0x5cf5ae38, /// 0x64
			 0x8ee29bb7, /// 0x68
			 0xf6daa472, /// 0x6c
			 0x5ae35f57, /// 0x70
			 0xd9512ace, /// 0x74
			 0x77838c7b, /// 0x78
			 0x56c978af, /// 0x7c
			 0x0c482ea8, /// 0x80
			 0x0ad18591, /// 0x84
			 0x9204e9cf, /// 0x88
			 0x1be140ce, /// 0x8c
			 0xf4d7dffb, /// 0x90
			 0x23ec5f76, /// 0x94
			 0x775f04a5, /// 0x98
			 0xeb18bff8, /// 0x9c
			 0x0ffbe525, /// 0xa0
			 0x4207cc77, /// 0xa4
			 0x723df915, /// 0xa8
			 0x1000554f, /// 0xac
			 0x1c214984, /// 0xb0
			 0x936cd44f, /// 0xb4
			 0x51d292e4, /// 0xb8
			 0x22463681, /// 0xbc
			 0x033d6b29, /// 0xc0
			 0x6e4a91a2, /// 0xc4
			 0x8065129b, /// 0xc8
			 0x36c2cb7e, /// 0xcc
			 0xf3f315f0, /// 0xd0
			 0x1380d336, /// 0xd4
			 0xd6df95fa, /// 0xd8
			 0x2487c227, /// 0xdc
			 0x3cc2f2ae, /// 0xe0
			 0xadfc9d0b, /// 0xe4
			 0x69d5618d, /// 0xe8
			 0x79bceff3, /// 0xec
			 0xc96e289e, /// 0xf0
			 0x7a750465, /// 0xf4
			 0x07fed036, /// 0xf8
			 0x8537dbf9, /// 0xfc
			 0x0dee7f1a, /// 0x100
			 0xea215324, /// 0x104
			 0x34318bca, /// 0x108
			 0x833dfb98, /// 0x10c
			 0x9e9638c0, /// 0x110
			 0xd8d0165b, /// 0x114
			 0xf1abc12d, /// 0x118
			 0x674af9f8, /// 0x11c
			 0xf42b74a8, /// 0x120
			 0x0520e59e, /// 0x124
			 0xa3a73f54, /// 0x128
			 0x8cc8c3bf, /// 0x12c
			 0x25e1a876, /// 0x130
			 0xb8dd5b6c, /// 0x134
			 0x1386d44d, /// 0x138
			 0x5236037f, /// 0x13c
			 0x61bbd7e8, /// 0x140
			 0x658aec69, /// 0x144
			 0xf42fd88d, /// 0x148
			 0xb65cc1c6, /// 0x14c
			 0xed4cb4d6, /// 0x150
			 0x72acf874, /// 0x154
			 0x0f4f2549, /// 0x158
			 0x76163079, /// 0x15c
			 0x6c57b9fb, /// 0x160
			 0x76643384, /// 0x164
			 0xdf0cbbf1, /// 0x168
			 0x388006d9, /// 0x16c
			 0xbe9b562e, /// 0x170
			 0xeeaaf92b, /// 0x174
			 0xa82d7854, /// 0x178
			 0x3570091a, /// 0x17c
			 0x2a17aa1c, /// 0x180
			 0xf92a4a9a, /// 0x184
			 0xbff1f5fb, /// 0x188
			 0x355069c2, /// 0x18c
			 0xd9ab5490, /// 0x190
			 0xc8111a47, /// 0x194
			 0x12aa8b47, /// 0x198
			 0x62a64c51, /// 0x19c
			 0x39a64e5b, /// 0x1a0
			 0xcf35e31b, /// 0x1a4
			 0x10390589, /// 0x1a8
			 0x7e9732c7, /// 0x1ac
			 0x7744410d, /// 0x1b0
			 0x184ca801, /// 0x1b4
			 0x13438cf0, /// 0x1b8
			 0x29703b56, /// 0x1bc
			 0x836d189c, /// 0x1c0
			 0x99cb19fc, /// 0x1c4
			 0xd6b9a69f, /// 0x1c8
			 0x6fef0c74, /// 0x1cc
			 0x6ff771e3, /// 0x1d0
			 0x6cde66c3, /// 0x1d4
			 0xe0ee65ef, /// 0x1d8
			 0x1cb0092f, /// 0x1dc
			 0x36d168ea, /// 0x1e0
			 0x681f51a4, /// 0x1e4
			 0xb8954eaf, /// 0x1e8
			 0x0cebfea9, /// 0x1ec
			 0x2254646c, /// 0x1f0
			 0x7d42b7a0, /// 0x1f4
			 0x16825a05, /// 0x1f8
			 0x2bd9a972, /// 0x1fc
			 0x6df3a7ae, /// 0x200
			 0x33ffa538, /// 0x204
			 0x929a778d, /// 0x208
			 0x7be8142f, /// 0x20c
			 0x8f3c3eb5, /// 0x210
			 0x69ff6ca2, /// 0x214
			 0x7f0b2f6b, /// 0x218
			 0x6c34225f, /// 0x21c
			 0x18fd742e, /// 0x220
			 0x6588ecad, /// 0x224
			 0xea2b505f, /// 0x228
			 0xd888a055, /// 0x22c
			 0x8b9f3ca2, /// 0x230
			 0x62ef718f, /// 0x234
			 0xc96cb227, /// 0x238
			 0xc9cd00a9, /// 0x23c
			 0x2d630a73, /// 0x240
			 0xddf15c1b, /// 0x244
			 0x23f9681e, /// 0x248
			 0x573fd1cc, /// 0x24c
			 0xbdf216fb, /// 0x250
			 0x124e807c, /// 0x254
			 0xc313932f, /// 0x258
			 0x553ab53f, /// 0x25c
			 0x8cd70f68, /// 0x260
			 0xa9a5d5ac, /// 0x264
			 0xd7879327, /// 0x268
			 0xd68ef1f1, /// 0x26c
			 0x6637ae66, /// 0x270
			 0x55c4e30b, /// 0x274
			 0xba019ee0, /// 0x278
			 0xfd3ef68d, /// 0x27c
			 0x555e79e9, /// 0x280
			 0xcce5cfa6, /// 0x284
			 0xd407edd1, /// 0x288
			 0x0efc9778, /// 0x28c
			 0xd28db563, /// 0x290
			 0xc0e35a26, /// 0x294
			 0xb5b4ac60, /// 0x298
			 0x8015e053, /// 0x29c
			 0x0fa0a4f0, /// 0x2a0
			 0x365173a7, /// 0x2a4
			 0x30a6462f, /// 0x2a8
			 0xb54b99f8, /// 0x2ac
			 0x9cdce210, /// 0x2b0
			 0xf9e4a433, /// 0x2b4
			 0x17d8190d, /// 0x2b8
			 0xb0051be5, /// 0x2bc
			 0x1cdedc18, /// 0x2c0
			 0x5a32e4eb, /// 0x2c4
			 0x4cd42089, /// 0x2c8
			 0x31346b35, /// 0x2cc
			 0x2e145a22, /// 0x2d0
			 0x1715da0c, /// 0x2d4
			 0x098689cc, /// 0x2d8
			 0x669a28e8, /// 0x2dc
			 0x38ece035, /// 0x2e0
			 0xa568c76f, /// 0x2e4
			 0xf83bc325, /// 0x2e8
			 0xf9ffcb9f, /// 0x2ec
			 0xf2a304f0, /// 0x2f0
			 0xeba672ad, /// 0x2f4
			 0xf31360ed, /// 0x2f8
			 0xebe155e1, /// 0x2fc
			 0xdcc8f06a, /// 0x300
			 0x39519c03, /// 0x304
			 0x58f96155, /// 0x308
			 0x99ddc9ac, /// 0x30c
			 0xa5f7d8ef, /// 0x310
			 0xfc4313d4, /// 0x314
			 0x9a816c6f, /// 0x318
			 0xfabfe5db, /// 0x31c
			 0x1a1adf1e, /// 0x320
			 0x534ea4fd, /// 0x324
			 0x6640b522, /// 0x328
			 0xebbd1c9b, /// 0x32c
			 0xaa20e350, /// 0x330
			 0xf4d683b6, /// 0x334
			 0x0775838d, /// 0x338
			 0x87997c09, /// 0x33c
			 0x43b37ee5, /// 0x340
			 0x3c9445fe, /// 0x344
			 0xa6c1fd8c, /// 0x348
			 0x03616b29, /// 0x34c
			 0x673e4e11, /// 0x350
			 0x241bcd11, /// 0x354
			 0xa24c2376, /// 0x358
			 0xc88ae7ad, /// 0x35c
			 0xd4550028, /// 0x360
			 0xbaf1295b, /// 0x364
			 0x9bfaea93, /// 0x368
			 0xdf0ac407, /// 0x36c
			 0x10c345f1, /// 0x370
			 0xa0c6b5f5, /// 0x374
			 0x4b254e9b, /// 0x378
			 0x4d1c5ff0, /// 0x37c
			 0x6338516a, /// 0x380
			 0x13226c3e, /// 0x384
			 0xf9929030, /// 0x388
			 0xc94dd59c, /// 0x38c
			 0x34baa660, /// 0x390
			 0xc954641e, /// 0x394
			 0xd3563129, /// 0x398
			 0xf37470f1, /// 0x39c
			 0x0a97af72, /// 0x3a0
			 0x07589115, /// 0x3a4
			 0x95ae1b9b, /// 0x3a8
			 0xed68939f, /// 0x3ac
			 0xd9f5a323, /// 0x3b0
			 0xe6ba315a, /// 0x3b4
			 0x87e62791, /// 0x3b8
			 0xab1cd1a8, /// 0x3bc
			 0x0a28d90c, /// 0x3c0
			 0x4c741ee1, /// 0x3c4
			 0xfb6f856f, /// 0x3c8
			 0xe3e067c6, /// 0x3cc
			 0xbe4784f9, /// 0x3d0
			 0xaa7cc234, /// 0x3d4
			 0x96c26037, /// 0x3d8
			 0xb967eed1, /// 0x3dc
			 0xd3f9b8b9, /// 0x3e0
			 0x58633310, /// 0x3e4
			 0x4c9070cd, /// 0x3e8
			 0x9e587ffb, /// 0x3ec
			 0xf0c3cd14, /// 0x3f0
			 0x44680a81, /// 0x3f4
			 0x05f32d99, /// 0x3f8
			 0x174ea5e0, /// 0x3fc
			 0x0d11bd5e, /// 0x400
			 0x64707c3e, /// 0x404
			 0xb460b0b4, /// 0x408
			 0x07d39e63, /// 0x40c
			 0x4220483b, /// 0x410
			 0xfd567cca, /// 0x414
			 0x3ae798d1, /// 0x418
			 0xc733ef0d, /// 0x41c
			 0x3c9f1586, /// 0x420
			 0xb90316b1, /// 0x424
			 0x6f77b2ac, /// 0x428
			 0xa7ed1d11, /// 0x42c
			 0xceee286c, /// 0x430
			 0x839a1dee, /// 0x434
			 0x37014e69, /// 0x438
			 0x0e612810, /// 0x43c
			 0xc5514a4b, /// 0x440
			 0xd0033f8a, /// 0x444
			 0x87211db5, /// 0x448
			 0x9c24047a, /// 0x44c
			 0xad4cabeb, /// 0x450
			 0xc1bd4b51, /// 0x454
			 0x991debea, /// 0x458
			 0x20aa389a, /// 0x45c
			 0xb045cbb5, /// 0x460
			 0xa2615d43, /// 0x464
			 0x3c0e5318, /// 0x468
			 0xec1f475c, /// 0x46c
			 0xd15d24f6, /// 0x470
			 0x329c74cb, /// 0x474
			 0x6812574a, /// 0x478
			 0xb942b25e, /// 0x47c
			 0x458c8ed9, /// 0x480
			 0xd746857b, /// 0x484
			 0x03861856, /// 0x488
			 0xa73aca49, /// 0x48c
			 0x9c381906, /// 0x490
			 0xc1ae2a97, /// 0x494
			 0x7e2c4cfa, /// 0x498
			 0xba59ffd2, /// 0x49c
			 0x1154dfc5, /// 0x4a0
			 0x6d292176, /// 0x4a4
			 0x461097b3, /// 0x4a8
			 0x01849a53, /// 0x4ac
			 0xe91f36f6, /// 0x4b0
			 0xe757f42e, /// 0x4b4
			 0x612e66bb, /// 0x4b8
			 0x0389d934, /// 0x4bc
			 0xe155aacb, /// 0x4c0
			 0x806e3bba, /// 0x4c4
			 0x32f7c497, /// 0x4c8
			 0x6f38551a, /// 0x4cc
			 0xed99e669, /// 0x4d0
			 0x58a82d6e, /// 0x4d4
			 0x3df7043a, /// 0x4d8
			 0xc57182d4, /// 0x4dc
			 0xaa846bd9, /// 0x4e0
			 0x0fd6bce3, /// 0x4e4
			 0x2fde0cba, /// 0x4e8
			 0xe7ed58e9, /// 0x4ec
			 0x4ec0b131, /// 0x4f0
			 0x6e2b07c2, /// 0x4f4
			 0xe3b96fdd, /// 0x4f8
			 0xbb3e6481, /// 0x4fc
			 0x4a14d861, /// 0x500
			 0x7949daee, /// 0x504
			 0x2c2423f6, /// 0x508
			 0x61914969, /// 0x50c
			 0x9fb2afaf, /// 0x510
			 0xe1c54f7e, /// 0x514
			 0x8271f5ef, /// 0x518
			 0x277d4826, /// 0x51c
			 0x4c6a5f6c, /// 0x520
			 0x2a90d884, /// 0x524
			 0x418814fc, /// 0x528
			 0x8a8ed2a5, /// 0x52c
			 0xfb0cb472, /// 0x530
			 0xf13ef6ec, /// 0x534
			 0xb9b9b18d, /// 0x538
			 0xe3710b9f, /// 0x53c
			 0xf2f712a9, /// 0x540
			 0x7dc54241, /// 0x544
			 0x0297f8c0, /// 0x548
			 0xc2e29616, /// 0x54c
			 0x6b0c2e40, /// 0x550
			 0xe792b3d4, /// 0x554
			 0xbccb1ad9, /// 0x558
			 0xf738b1c9, /// 0x55c
			 0x649a67e7, /// 0x560
			 0xca917600, /// 0x564
			 0x0dc3e2ff, /// 0x568
			 0x181aed9a, /// 0x56c
			 0x080ccbd9, /// 0x570
			 0xca7aac16, /// 0x574
			 0xbb30d845, /// 0x578
			 0x283e32b8, /// 0x57c
			 0x0b43af89, /// 0x580
			 0xe0f2251f, /// 0x584
			 0x489fb79f, /// 0x588
			 0x8aa7162f, /// 0x58c
			 0x8d641691, /// 0x590
			 0x1d842acb, /// 0x594
			 0x4096f37f, /// 0x598
			 0x89ba50ee, /// 0x59c
			 0x8204f30d, /// 0x5a0
			 0xf318c180, /// 0x5a4
			 0x7f594e7f, /// 0x5a8
			 0x79121147, /// 0x5ac
			 0xef0bb335, /// 0x5b0
			 0x28fde041, /// 0x5b4
			 0x77d96895, /// 0x5b8
			 0x9945c185, /// 0x5bc
			 0xd5e3e86f, /// 0x5c0
			 0x3fb403a5, /// 0x5c4
			 0x8518555f, /// 0x5c8
			 0x3ef7c353, /// 0x5cc
			 0xa756c252, /// 0x5d0
			 0x8138936c, /// 0x5d4
			 0x3a529e1d, /// 0x5d8
			 0xb969605b, /// 0x5dc
			 0xd7d213d9, /// 0x5e0
			 0xc36bd064, /// 0x5e4
			 0x4fe05ee2, /// 0x5e8
			 0x62cd96e8, /// 0x5ec
			 0x63f3994b, /// 0x5f0
			 0x60b53144, /// 0x5f4
			 0x0b96011a, /// 0x5f8
			 0xbe038fe6, /// 0x5fc
			 0xf0a3f0c5, /// 0x600
			 0x08bd7a72, /// 0x604
			 0xcc8bd84e, /// 0x608
			 0x40ac9470, /// 0x60c
			 0x0f762fab, /// 0x610
			 0xb8b73812, /// 0x614
			 0xb189fec5, /// 0x618
			 0x56551dd4, /// 0x61c
			 0xf06aad6f, /// 0x620
			 0x4bc1defd, /// 0x624
			 0x42a51b8f, /// 0x628
			 0x17ee1d75, /// 0x62c
			 0x29ce04c8, /// 0x630
			 0x855449ba, /// 0x634
			 0x3669f383, /// 0x638
			 0x9f4145e1, /// 0x63c
			 0x867f3e87, /// 0x640
			 0x7724b96b, /// 0x644
			 0x66c7a427, /// 0x648
			 0xcc694b59, /// 0x64c
			 0x594e2e07, /// 0x650
			 0xda3d061b, /// 0x654
			 0x0679c8eb, /// 0x658
			 0x3d5d2899, /// 0x65c
			 0xea9e4253, /// 0x660
			 0xda8066e5, /// 0x664
			 0x6a1bfff7, /// 0x668
			 0x81c702a3, /// 0x66c
			 0xb25a3829, /// 0x670
			 0x73d0796b, /// 0x674
			 0x9532c440, /// 0x678
			 0x7c17d336, /// 0x67c
			 0x8a6b3f57, /// 0x680
			 0x22f4f992, /// 0x684
			 0x055a5f58, /// 0x688
			 0xb6abdf89, /// 0x68c
			 0x6ab928eb, /// 0x690
			 0xa8da1cef, /// 0x694
			 0x9542d5e1, /// 0x698
			 0x81d58545, /// 0x69c
			 0x95165c59, /// 0x6a0
			 0x690f97c1, /// 0x6a4
			 0xbbc3e79a, /// 0x6a8
			 0x1101d5b1, /// 0x6ac
			 0xc707a2ba, /// 0x6b0
			 0x02ca15d9, /// 0x6b4
			 0x7a556342, /// 0x6b8
			 0x87012090, /// 0x6bc
			 0x2d930113, /// 0x6c0
			 0x2b8402e2, /// 0x6c4
			 0x90657b54, /// 0x6c8
			 0x12dcff7c, /// 0x6cc
			 0xf261e702, /// 0x6d0
			 0x37afc611, /// 0x6d4
			 0xf8d55b89, /// 0x6d8
			 0x727c0cf1, /// 0x6dc
			 0x0bc5c9b2, /// 0x6e0
			 0x447aef57, /// 0x6e4
			 0xf7ca8ff6, /// 0x6e8
			 0x7f6ab528, /// 0x6ec
			 0xde687f2e, /// 0x6f0
			 0x0692783a, /// 0x6f4
			 0x225f1385, /// 0x6f8
			 0xcb7d8771, /// 0x6fc
			 0xc04445c9, /// 0x700
			 0x585f86be, /// 0x704
			 0x9816e482, /// 0x708
			 0xbda6038d, /// 0x70c
			 0x56c2b699, /// 0x710
			 0xb5dde45c, /// 0x714
			 0x946ff086, /// 0x718
			 0x56641b21, /// 0x71c
			 0x271384c5, /// 0x720
			 0x0bc86a73, /// 0x724
			 0xbe376c65, /// 0x728
			 0xc94059e0, /// 0x72c
			 0xee48625f, /// 0x730
			 0x81d18204, /// 0x734
			 0xcf84f1a6, /// 0x738
			 0x67454275, /// 0x73c
			 0x3ef6ab84, /// 0x740
			 0xc2a42da3, /// 0x744
			 0x9e1af586, /// 0x748
			 0x8e8b1438, /// 0x74c
			 0x610e952f, /// 0x750
			 0x7ce886aa, /// 0x754
			 0x891f1140, /// 0x758
			 0xc5749225, /// 0x75c
			 0x6c0b3ff7, /// 0x760
			 0x849ba931, /// 0x764
			 0xcf8870a0, /// 0x768
			 0xf319c319, /// 0x76c
			 0x60bc76ee, /// 0x770
			 0xc439fee9, /// 0x774
			 0x55b75cff, /// 0x778
			 0x7acbacc6, /// 0x77c
			 0x6deb946f, /// 0x780
			 0xbc17fbb4, /// 0x784
			 0x24a89fa1, /// 0x788
			 0x29c52853, /// 0x78c
			 0x35062f98, /// 0x790
			 0x278052f5, /// 0x794
			 0x0ec02a8a, /// 0x798
			 0xb25d791d, /// 0x79c
			 0xe3756a14, /// 0x7a0
			 0x2ad8c7ea, /// 0x7a4
			 0x6df92304, /// 0x7a8
			 0xa43a30b7, /// 0x7ac
			 0xed8c421a, /// 0x7b0
			 0xb00efdc9, /// 0x7b4
			 0x52d90566, /// 0x7b8
			 0x9a512798, /// 0x7bc
			 0xc5edb882, /// 0x7c0
			 0x5887fbdf, /// 0x7c4
			 0x1dbc8683, /// 0x7c8
			 0xf58d938d, /// 0x7cc
			 0xa79dbe8d, /// 0x7d0
			 0xfdcc15e1, /// 0x7d4
			 0xf00639a5, /// 0x7d8
			 0x464a3149, /// 0x7dc
			 0xfc1c30be, /// 0x7e0
			 0xe41fa240, /// 0x7e4
			 0xe781ebe9, /// 0x7e8
			 0x7023e025, /// 0x7ec
			 0x7242167f, /// 0x7f0
			 0xd6f6cc30, /// 0x7f4
			 0x068dd04f, /// 0x7f8
			 0x3e249421, /// 0x7fc
			 0xa04c5c6f, /// 0x800
			 0x0a09ecad, /// 0x804
			 0xd8f4c20e, /// 0x808
			 0x854cad17, /// 0x80c
			 0x7eb8887e, /// 0x810
			 0xab779f99, /// 0x814
			 0x28a3d999, /// 0x818
			 0xba955059, /// 0x81c
			 0xd6aea839, /// 0x820
			 0xb09f5857, /// 0x824
			 0x894f86c7, /// 0x828
			 0x5c0c10c1, /// 0x82c
			 0x12f708ed, /// 0x830
			 0xa5c0afa8, /// 0x834
			 0x625d8fc1, /// 0x838
			 0xe2d2679d, /// 0x83c
			 0x85d05689, /// 0x840
			 0x41d2359d, /// 0x844
			 0x2de8b435, /// 0x848
			 0xb19ca7ba, /// 0x84c
			 0xf4c1fd7d, /// 0x850
			 0x40cd9925, /// 0x854
			 0xd3a9587b, /// 0x858
			 0x33aae9aa, /// 0x85c
			 0xb36cbc76, /// 0x860
			 0x45e99be6, /// 0x864
			 0x577b1545, /// 0x868
			 0xdc047253, /// 0x86c
			 0x6479d9f1, /// 0x870
			 0x8d25f1a0, /// 0x874
			 0xf9b09c5c, /// 0x878
			 0x1d8bfdf4, /// 0x87c
			 0x8e3ccbe3, /// 0x880
			 0x635eb9cf, /// 0x884
			 0x92c9efa1, /// 0x888
			 0xa0ef2a1a, /// 0x88c
			 0xd999de50, /// 0x890
			 0xf6a250df, /// 0x894
			 0xa857b06d, /// 0x898
			 0x12b7d291, /// 0x89c
			 0x024c9eaa, /// 0x8a0
			 0xcef8cc39, /// 0x8a4
			 0x8b4bd9ac, /// 0x8a8
			 0x6f47d80a, /// 0x8ac
			 0xfc1810e9, /// 0x8b0
			 0x300c680b, /// 0x8b4
			 0xbee34002, /// 0x8b8
			 0x4492f95d, /// 0x8bc
			 0x28c4e8f7, /// 0x8c0
			 0xd8c6d660, /// 0x8c4
			 0xba06295a, /// 0x8c8
			 0xb4741f07, /// 0x8cc
			 0xea3a2b0a, /// 0x8d0
			 0xeca14f4a, /// 0x8d4
			 0xa5c29db2, /// 0x8d8
			 0xc102844c, /// 0x8dc
			 0x639dbea6, /// 0x8e0
			 0x33eee751, /// 0x8e4
			 0xc86b4950, /// 0x8e8
			 0x1c6895fa, /// 0x8ec
			 0x6e5186a4, /// 0x8f0
			 0x63719e3d, /// 0x8f4
			 0xc6766d03, /// 0x8f8
			 0x57d1d954, /// 0x8fc
			 0x8f34949b, /// 0x900
			 0xf0e8e501, /// 0x904
			 0xb89a7a34, /// 0x908
			 0x63e62e78, /// 0x90c
			 0xb9ea171c, /// 0x910
			 0xb9639f95, /// 0x914
			 0xe618feb6, /// 0x918
			 0xdd27886d, /// 0x91c
			 0x7e954c35, /// 0x920
			 0xf0f54ccd, /// 0x924
			 0xa95930d4, /// 0x928
			 0x4dbc3bee, /// 0x92c
			 0x2b6c028c, /// 0x930
			 0x9110b783, /// 0x934
			 0x25008504, /// 0x938
			 0xe5a7fb68, /// 0x93c
			 0x7ebe005c, /// 0x940
			 0xc358a6fe, /// 0x944
			 0xaf087d3e, /// 0x948
			 0x35ecb502, /// 0x94c
			 0xdf926caa, /// 0x950
			 0x8068697f, /// 0x954
			 0x37f21eec, /// 0x958
			 0xf0c5a3cd, /// 0x95c
			 0xbd38ca8f, /// 0x960
			 0xd729c486, /// 0x964
			 0x794fb12a, /// 0x968
			 0x43ca7e91, /// 0x96c
			 0xb2dfb36d, /// 0x970
			 0xc058c376, /// 0x974
			 0xf1640d2e, /// 0x978
			 0x3d4554b8, /// 0x97c
			 0xdbec181b, /// 0x980
			 0x7630b9f0, /// 0x984
			 0x546b66ec, /// 0x988
			 0xd1f34bce, /// 0x98c
			 0xf1ce1771, /// 0x990
			 0x87679197, /// 0x994
			 0x21e3c399, /// 0x998
			 0x406542bd, /// 0x99c
			 0x9f686bf1, /// 0x9a0
			 0xb2f125c3, /// 0x9a4
			 0x77ac8eaf, /// 0x9a8
			 0x4acadb12, /// 0x9ac
			 0xf0a292f7, /// 0x9b0
			 0x97ff03e2, /// 0x9b4
			 0xbcb98228, /// 0x9b8
			 0x18e1c60a, /// 0x9bc
			 0x538f2f91, /// 0x9c0
			 0xa1a42d98, /// 0x9c4
			 0x2bbf4546, /// 0x9c8
			 0xc8b02ca4, /// 0x9cc
			 0x96fd6fe7, /// 0x9d0
			 0xc565228a, /// 0x9d4
			 0xe9d8aa7c, /// 0x9d8
			 0x317fbd2e, /// 0x9dc
			 0x57573d7b, /// 0x9e0
			 0x6a9a06c9, /// 0x9e4
			 0x7a7e4724, /// 0x9e8
			 0x3d1ceb4b, /// 0x9ec
			 0x82fcfbfa, /// 0x9f0
			 0x2274c27e, /// 0x9f4
			 0xeb5fb93c, /// 0x9f8
			 0x8b07ceaf, /// 0x9fc
			 0x2b00adc9, /// 0xa00
			 0xabef10cb, /// 0xa04
			 0x98255ee9, /// 0xa08
			 0x934b4823, /// 0xa0c
			 0xcc96f24f, /// 0xa10
			 0x220a48bf, /// 0xa14
			 0xe649bae0, /// 0xa18
			 0xdb061753, /// 0xa1c
			 0xca440c8f, /// 0xa20
			 0x733d19dd, /// 0xa24
			 0x5929e1dd, /// 0xa28
			 0xa77267c6, /// 0xa2c
			 0xa938aff2, /// 0xa30
			 0x3ae69909, /// 0xa34
			 0x416ab1a4, /// 0xa38
			 0xafd6d634, /// 0xa3c
			 0xa81b70e5, /// 0xa40
			 0x6b946a12, /// 0xa44
			 0x9b67097f, /// 0xa48
			 0xed6b7783, /// 0xa4c
			 0xee44572b, /// 0xa50
			 0x2f8ed600, /// 0xa54
			 0xeb60c784, /// 0xa58
			 0x935e34cb, /// 0xa5c
			 0x80573992, /// 0xa60
			 0x6fee07f7, /// 0xa64
			 0x95de8b32, /// 0xa68
			 0xe511f597, /// 0xa6c
			 0xd87fdeac, /// 0xa70
			 0x88ecd0ad, /// 0xa74
			 0x572d1e27, /// 0xa78
			 0xd5f5b81f, /// 0xa7c
			 0x12df7984, /// 0xa80
			 0xf7ed5c66, /// 0xa84
			 0x1030cf5a, /// 0xa88
			 0x228e85ee, /// 0xa8c
			 0x5827e569, /// 0xa90
			 0xdda65bd2, /// 0xa94
			 0x0daff287, /// 0xa98
			 0x90c896a9, /// 0xa9c
			 0x629fc852, /// 0xaa0
			 0x8998689d, /// 0xaa4
			 0x9d2a5843, /// 0xaa8
			 0x5bb955d4, /// 0xaac
			 0x141e0db9, /// 0xab0
			 0x4722e880, /// 0xab4
			 0xf460a034, /// 0xab8
			 0xb11672ec, /// 0xabc
			 0x50374e3d, /// 0xac0
			 0xc33cd880, /// 0xac4
			 0x75b56dc8, /// 0xac8
			 0x4a69343a, /// 0xacc
			 0x34b2e56e, /// 0xad0
			 0xe594ccaa, /// 0xad4
			 0x274893a2, /// 0xad8
			 0xe45f9c3a, /// 0xadc
			 0x75f86714, /// 0xae0
			 0xcb794e5b, /// 0xae4
			 0x6209ac1a, /// 0xae8
			 0x8ac300ae, /// 0xaec
			 0xafa3dc2c, /// 0xaf0
			 0x7ad01118, /// 0xaf4
			 0x0bb0946e, /// 0xaf8
			 0x0fb73ffa, /// 0xafc
			 0x2aaf1027, /// 0xb00
			 0xb815db2a, /// 0xb04
			 0xa36a26e9, /// 0xb08
			 0xfe3eb588, /// 0xb0c
			 0x0e9bd36a, /// 0xb10
			 0xb65235ba, /// 0xb14
			 0x7793f026, /// 0xb18
			 0x48e81e4f, /// 0xb1c
			 0x148e8f1d, /// 0xb20
			 0xa4e6f1ee, /// 0xb24
			 0x10db7aae, /// 0xb28
			 0x4cfe4203, /// 0xb2c
			 0xe31f34e8, /// 0xb30
			 0x47c90ec6, /// 0xb34
			 0x594a55ba, /// 0xb38
			 0x8ee22552, /// 0xb3c
			 0x313e6696, /// 0xb40
			 0x8d32e507, /// 0xb44
			 0xa086030c, /// 0xb48
			 0x1462688e, /// 0xb4c
			 0x14758864, /// 0xb50
			 0x090c517a, /// 0xb54
			 0x62429a2c, /// 0xb58
			 0x8ced5487, /// 0xb5c
			 0xe25578b6, /// 0xb60
			 0x715179c5, /// 0xb64
			 0x13924dcc, /// 0xb68
			 0x2f3059c1, /// 0xb6c
			 0x4a5fad7f, /// 0xb70
			 0x521fb3ca, /// 0xb74
			 0x2441989d, /// 0xb78
			 0xecf027c0, /// 0xb7c
			 0x722315d1, /// 0xb80
			 0xed9e6441, /// 0xb84
			 0x571cd414, /// 0xb88
			 0xabddebf4, /// 0xb8c
			 0x9f31519e, /// 0xb90
			 0xe8b3693c, /// 0xb94
			 0xf0054a9d, /// 0xb98
			 0x0eae0bd3, /// 0xb9c
			 0xa8bd3da9, /// 0xba0
			 0x2e4159ee, /// 0xba4
			 0xfc5ad981, /// 0xba8
			 0x10133846, /// 0xbac
			 0x49522947, /// 0xbb0
			 0x76b9fd5c, /// 0xbb4
			 0x502dd918, /// 0xbb8
			 0x7fa6733e, /// 0xbbc
			 0x27bd589f, /// 0xbc0
			 0x471b350d, /// 0xbc4
			 0x46374868, /// 0xbc8
			 0x59dde398, /// 0xbcc
			 0x301dc32e, /// 0xbd0
			 0xac43e7ac, /// 0xbd4
			 0xcafe4cb7, /// 0xbd8
			 0x7e5a2fe8, /// 0xbdc
			 0x2341da7a, /// 0xbe0
			 0xd033068b, /// 0xbe4
			 0x67c7f3f5, /// 0xbe8
			 0x27050f7c, /// 0xbec
			 0x81eaf897, /// 0xbf0
			 0x9348cd0c, /// 0xbf4
			 0x861fafba, /// 0xbf8
			 0x93622948, /// 0xbfc
			 0x4e4cfcc5, /// 0xc00
			 0x59685da5, /// 0xc04
			 0xb351a363, /// 0xc08
			 0xf83b7052, /// 0xc0c
			 0xc1d23770, /// 0xc10
			 0xf8343823, /// 0xc14
			 0xe10bda51, /// 0xc18
			 0x9365907f, /// 0xc1c
			 0xb3c52237, /// 0xc20
			 0xc9d41605, /// 0xc24
			 0xcaf17f73, /// 0xc28
			 0x31cc8f2d, /// 0xc2c
			 0x6ac0f21a, /// 0xc30
			 0x7e11e98e, /// 0xc34
			 0x90743bf8, /// 0xc38
			 0x363778cf, /// 0xc3c
			 0x70e8d800, /// 0xc40
			 0xff2a7d9b, /// 0xc44
			 0x5ca753bf, /// 0xc48
			 0xf0c0f7bd, /// 0xc4c
			 0xb6b1aa9f, /// 0xc50
			 0xa37da45f, /// 0xc54
			 0x625f48f7, /// 0xc58
			 0xf3274aa2, /// 0xc5c
			 0x0bcd4a4d, /// 0xc60
			 0x87fb0c9a, /// 0xc64
			 0x5176e6b5, /// 0xc68
			 0xb6a8640c, /// 0xc6c
			 0x61a5c97b, /// 0xc70
			 0xbbebcaf6, /// 0xc74
			 0x9ce1ab04, /// 0xc78
			 0x04995098, /// 0xc7c
			 0x0f2323b9, /// 0xc80
			 0x253569fe, /// 0xc84
			 0xae18bc7f, /// 0xc88
			 0x86747a46, /// 0xc8c
			 0x6b84a250, /// 0xc90
			 0xe13decc5, /// 0xc94
			 0x71c66333, /// 0xc98
			 0xec4e89fc, /// 0xc9c
			 0x666f45e0, /// 0xca0
			 0x9292ea85, /// 0xca4
			 0x20aea2b9, /// 0xca8
			 0xf8e81d65, /// 0xcac
			 0xfc2af443, /// 0xcb0
			 0xc99dd426, /// 0xcb4
			 0xb58a2f06, /// 0xcb8
			 0xa9bd0330, /// 0xcbc
			 0x04960f22, /// 0xcc0
			 0x741e1aab, /// 0xcc4
			 0xf56ecb88, /// 0xcc8
			 0xc79bcf83, /// 0xccc
			 0x5fb29cf0, /// 0xcd0
			 0xb72a9bcc, /// 0xcd4
			 0xb9ec28ed, /// 0xcd8
			 0x7201c17c, /// 0xcdc
			 0xbd27b55c, /// 0xce0
			 0xc38393d6, /// 0xce4
			 0x8798a7ef, /// 0xce8
			 0x0b454d00, /// 0xcec
			 0xc5980273, /// 0xcf0
			 0x53230d49, /// 0xcf4
			 0xdb5e592d, /// 0xcf8
			 0x05a90eed, /// 0xcfc
			 0xb9b8ba70, /// 0xd00
			 0x4bfce60a, /// 0xd04
			 0x430a77ee, /// 0xd08
			 0xb6182093, /// 0xd0c
			 0x320551a3, /// 0xd10
			 0xd3ffb0bc, /// 0xd14
			 0x8da688e6, /// 0xd18
			 0xf15072dd, /// 0xd1c
			 0x47981d76, /// 0xd20
			 0xb7f9cc1d, /// 0xd24
			 0xe31952e0, /// 0xd28
			 0x57f7a60e, /// 0xd2c
			 0x22a04877, /// 0xd30
			 0x1caf9b50, /// 0xd34
			 0xb9f38ac2, /// 0xd38
			 0xaf69823d, /// 0xd3c
			 0x5bae3ac2, /// 0xd40
			 0x992527c4, /// 0xd44
			 0x5beaf00b, /// 0xd48
			 0xd0136343, /// 0xd4c
			 0x970a4050, /// 0xd50
			 0xb919b244, /// 0xd54
			 0x53bdbb4a, /// 0xd58
			 0xc05ad389, /// 0xd5c
			 0x17ffc15b, /// 0xd60
			 0xf1c1b96a, /// 0xd64
			 0x980057b0, /// 0xd68
			 0x0f20b6e7, /// 0xd6c
			 0x4d795c71, /// 0xd70
			 0x22472e0b, /// 0xd74
			 0x58985204, /// 0xd78
			 0xf4ee344f, /// 0xd7c
			 0x62a904ea, /// 0xd80
			 0x1db71ad8, /// 0xd84
			 0xb50f5cd0, /// 0xd88
			 0xbd3fe49d, /// 0xd8c
			 0xfe22a33c, /// 0xd90
			 0x42e637a2, /// 0xd94
			 0x02bccdcd, /// 0xd98
			 0xc0bac054, /// 0xd9c
			 0x19c2f0a3, /// 0xda0
			 0x8b18f50e, /// 0xda4
			 0x61278ae4, /// 0xda8
			 0x65a784f9, /// 0xdac
			 0xca372eac, /// 0xdb0
			 0x72f635b9, /// 0xdb4
			 0x95f757c9, /// 0xdb8
			 0xc0deb35a, /// 0xdbc
			 0x56d57a61, /// 0xdc0
			 0x255fce45, /// 0xdc4
			 0xcdea058a, /// 0xdc8
			 0x9acdd1b3, /// 0xdcc
			 0x96291956, /// 0xdd0
			 0xcf3da1e7, /// 0xdd4
			 0x7c1ade19, /// 0xdd8
			 0x04704693, /// 0xddc
			 0x17b6cc7c, /// 0xde0
			 0x078ad485, /// 0xde4
			 0x23b3b302, /// 0xde8
			 0xe382f748, /// 0xdec
			 0x4903f949, /// 0xdf0
			 0x81590160, /// 0xdf4
			 0x88ca4ee4, /// 0xdf8
			 0xdcbddafe, /// 0xdfc
			 0xc898f673, /// 0xe00
			 0x0a6ed00f, /// 0xe04
			 0xabde38aa, /// 0xe08
			 0xe5076272, /// 0xe0c
			 0x59cf0251, /// 0xe10
			 0x66319f50, /// 0xe14
			 0x0e08be09, /// 0xe18
			 0x3c7fb7a8, /// 0xe1c
			 0x87387c17, /// 0xe20
			 0x7de880ab, /// 0xe24
			 0xc52702ee, /// 0xe28
			 0xf0a95c36, /// 0xe2c
			 0xdc45c330, /// 0xe30
			 0x283b7ff4, /// 0xe34
			 0xda2718ec, /// 0xe38
			 0xb06069a9, /// 0xe3c
			 0xbcf353f4, /// 0xe40
			 0x532ba77b, /// 0xe44
			 0x65711758, /// 0xe48
			 0xe4a5fedf, /// 0xe4c
			 0x23af7ca3, /// 0xe50
			 0x668f45bf, /// 0xe54
			 0xeb150196, /// 0xe58
			 0x99af53e3, /// 0xe5c
			 0x37a051ac, /// 0xe60
			 0xc58cbdc7, /// 0xe64
			 0x25c9fe42, /// 0xe68
			 0xcd01d3ec, /// 0xe6c
			 0x3e798b50, /// 0xe70
			 0x564a95b5, /// 0xe74
			 0x2b50f325, /// 0xe78
			 0xc04721a6, /// 0xe7c
			 0x8eaa4f48, /// 0xe80
			 0x93d8dac2, /// 0xe84
			 0x7482421f, /// 0xe88
			 0x1d4dd6bc, /// 0xe8c
			 0x0011a742, /// 0xe90
			 0xc035a225, /// 0xe94
			 0x043c2b71, /// 0xe98
			 0x4bb5f448, /// 0xe9c
			 0x7db2edd8, /// 0xea0
			 0x2c35b28f, /// 0xea4
			 0x9027d526, /// 0xea8
			 0x5e5f7dd9, /// 0xeac
			 0xe33671df, /// 0xeb0
			 0xa2ad52d7, /// 0xeb4
			 0xac1e11f7, /// 0xeb8
			 0x02f10d8a, /// 0xebc
			 0x9874e5a4, /// 0xec0
			 0xfd096ce5, /// 0xec4
			 0x6911d76d, /// 0xec8
			 0x6d4f3d14, /// 0xecc
			 0xd73fc67d, /// 0xed0
			 0x8725cd33, /// 0xed4
			 0x3ee87d5f, /// 0xed8
			 0x4fa4deb0, /// 0xedc
			 0xf73184d5, /// 0xee0
			 0x917c9d48, /// 0xee4
			 0x9294b0c7, /// 0xee8
			 0xfe66e7fc, /// 0xeec
			 0x9ae2a2c7, /// 0xef0
			 0x53874456, /// 0xef4
			 0x41e217a3, /// 0xef8
			 0x084a2597, /// 0xefc
			 0xe5132f07, /// 0xf00
			 0x3b95f8c2, /// 0xf04
			 0x2024c30b, /// 0xf08
			 0xae7e5401, /// 0xf0c
			 0x59fcf68d, /// 0xf10
			 0xbe075d0f, /// 0xf14
			 0xe1b34985, /// 0xf18
			 0x5de34ba3, /// 0xf1c
			 0xfeaee173, /// 0xf20
			 0xcb02834c, /// 0xf24
			 0x5ce814e0, /// 0xf28
			 0x4ea92666, /// 0xf2c
			 0x3078abcb, /// 0xf30
			 0xb8663940, /// 0xf34
			 0x2c8a98d7, /// 0xf38
			 0xc215950f, /// 0xf3c
			 0xcfe75743, /// 0xf40
			 0x64513087, /// 0xf44
			 0x177f7338, /// 0xf48
			 0x68e7ffdc, /// 0xf4c
			 0x3aee905a, /// 0xf50
			 0x6d23b2ee, /// 0xf54
			 0xc5212239, /// 0xf58
			 0xfa756105, /// 0xf5c
			 0x340e3b26, /// 0xf60
			 0x5e3145af, /// 0xf64
			 0xca4c056a, /// 0xf68
			 0xa3c764b0, /// 0xf6c
			 0xc64661bf, /// 0xf70
			 0x08ad11d1, /// 0xf74
			 0x00fb2dc1, /// 0xf78
			 0xe791a0f4, /// 0xf7c
			 0x93078aaa, /// 0xf80
			 0x0cf585c3, /// 0xf84
			 0x65c61d4a, /// 0xf88
			 0xe417d0ed, /// 0xf8c
			 0xbefd870c, /// 0xf90
			 0x6f47e88d, /// 0xf94
			 0xf8874c14, /// 0xf98
			 0xfb2aa396, /// 0xf9c
			 0x8747bd2d, /// 0xfa0
			 0xcd22b879, /// 0xfa4
			 0xf3749cce, /// 0xfa8
			 0xbe33ba9f, /// 0xfac
			 0x6e1c4abb, /// 0xfb0
			 0xb4d4557d, /// 0xfb4
			 0x4c64587f, /// 0xfb8
			 0xf1c24a4a, /// 0xfbc
			 0x7c685c66, /// 0xfc0
			 0x7b234877, /// 0xfc4
			 0x1667aced, /// 0xfc8
			 0xb48d1a7e, /// 0xfcc
			 0x1be28421, /// 0xfd0
			 0xbee1e6ae, /// 0xfd4
			 0xe4de823c, /// 0xfd8
			 0xb2d18019, /// 0xfdc
			 0xbce1b5bf, /// 0xfe0
			 0x70a84240, /// 0xfe4
			 0xa96a7034, /// 0xfe8
			 0xd884a3dc, /// 0xfec
			 0x7f36e958, /// 0xff0
			 0xf960b347, /// 0xff4
			 0x3d6f6770, /// 0xff8
			 0x8d867b68 /// last
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
			 0x00000088,
			 0x00000394,
			 0x00000510,
			 0x00000350,
			 0x000002ec,
			 0x0000074c,
			 0x0000026c,
			 0x00000440,

			 /// vpu register f2
			 0x41100000,
			 0x41e00000,
			 0x41400000,
			 0x41a00000,
			 0x41980000,
			 0x41800000,
			 0x40800000,
			 0x41700000,

			 /// vpu register f3
			 0x40000000,
			 0x40400000,
			 0x41d00000,
			 0x41a80000,
			 0x41e80000,
			 0x41600000,
			 0x41200000,
			 0x41980000,

			 /// vpu register f4
			 0x41c80000,
			 0x40c00000,
			 0x40e00000,
			 0x41b00000,
			 0x41400000,
			 0x40e00000,
			 0x41300000,
			 0x41c00000,

			 /// vpu register f5
			 0x41f80000,
			 0x41800000,
			 0x41000000,
			 0x41000000,
			 0x41400000,
			 0x41f80000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f6
			 0x41880000,
			 0x41a00000,
			 0x40e00000,
			 0x40c00000,
			 0x41d00000,
			 0x41c00000,
			 0x41300000,
			 0x40e00000,

			 /// vpu register f7
			 0x41d80000,
			 0x41a80000,
			 0x41f00000,
			 0x41f00000,
			 0x40a00000,
			 0x41880000,
			 0x41c80000,
			 0x41e80000,

			 /// vpu register f8
			 0x41b80000,
			 0x41f00000,
			 0x41e00000,
			 0x40400000,
			 0x41400000,
			 0x41200000,
			 0x41f80000,
			 0x41600000,

			 /// vpu register f9
			 0x40400000,
			 0x41a00000,
			 0x41e00000,
			 0x41880000,
			 0x41d00000,
			 0x3f800000,
			 0x41c80000,
			 0x41a80000,

			 /// vpu register f10
			 0x41900000,
			 0x41c00000,
			 0x41900000,
			 0x40e00000,
			 0x41900000,
			 0x41e00000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f11
			 0x42000000,
			 0x41100000,
			 0x41800000,
			 0x41f80000,
			 0x40000000,
			 0x41d00000,
			 0x40400000,
			 0x41980000,

			 /// vpu register f12
			 0x41f00000,
			 0x41a00000,
			 0x41700000,
			 0x41100000,
			 0x41000000,
			 0x40c00000,
			 0x41f80000,
			 0x41600000,

			 /// vpu register f13
			 0x41b80000,
			 0x41d80000,
			 0x41800000,
			 0x41880000,
			 0x41b80000,
			 0x41f80000,
			 0x41100000,
			 0x41e00000,

			 /// vpu register f14
			 0x41f80000,
			 0x40c00000,
			 0x41f00000,
			 0x41300000,
			 0x40400000,
			 0x41f80000,
			 0x41f80000,
			 0x40e00000,

			 /// vpu register f15
			 0x42000000,
			 0x41d80000,
			 0x41980000,
			 0x41e80000,
			 0x41d00000,
			 0x41d80000,
			 0x41c00000,
			 0x41200000,

			 /// vpu register f16
			 0x40800000,
			 0x41880000,
			 0x41000000,
			 0x41300000,
			 0x41f80000,
			 0x41880000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f17
			 0x41a80000,
			 0x41100000,
			 0x41300000,
			 0x41d00000,
			 0x41900000,
			 0x41100000,
			 0x41a80000,
			 0x41300000,

			 /// vpu register f18
			 0x41880000,
			 0x41880000,
			 0x40e00000,
			 0x41d00000,
			 0x41100000,
			 0x3f800000,
			 0x41000000,
			 0x40000000,

			 /// vpu register f19
			 0x40c00000,
			 0x3f800000,
			 0x41400000,
			 0x41e00000,
			 0x41a00000,
			 0x3f800000,
			 0x40000000,
			 0x41500000,

			 /// vpu register f20
			 0x41b00000,
			 0x41d00000,
			 0x41b00000,
			 0x40800000,
			 0x41b80000,
			 0x41e00000,
			 0x41c80000,
			 0x40000000,

			 /// vpu register f21
			 0x41f80000,
			 0x3f800000,
			 0x41d80000,
			 0x40a00000,
			 0x41700000,
			 0x41500000,
			 0x41500000,
			 0x41700000,

			 /// vpu register f22
			 0x41f80000,
			 0x41e80000,
			 0x41b80000,
			 0x40800000,
			 0x41e80000,
			 0x41b80000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f23
			 0x41c00000,
			 0x41400000,
			 0x40e00000,
			 0x41d80000,
			 0x40000000,
			 0x41c00000,
			 0x41a80000,
			 0x41c00000,

			 /// vpu register f24
			 0x41a00000,
			 0x41f80000,
			 0x41100000,
			 0x41d80000,
			 0x41a00000,
			 0x41e80000,
			 0x42000000,
			 0x41300000,

			 /// vpu register f25
			 0x40800000,
			 0x41400000,
			 0x41d00000,
			 0x41d80000,
			 0x41600000,
			 0x41980000,
			 0x41f80000,
			 0x40000000,

			 /// vpu register f26
			 0x41c80000,
			 0x40e00000,
			 0x41700000,
			 0x41d00000,
			 0x41f00000,
			 0x40e00000,
			 0x41100000,
			 0x41880000,

			 /// vpu register f27
			 0x41b00000,
			 0x41b80000,
			 0x41400000,
			 0x41000000,
			 0x41600000,
			 0x41200000,
			 0x42000000,
			 0x3f800000,

			 /// vpu register f28
			 0x41300000,
			 0x3f800000,
			 0x41e00000,
			 0x41000000,
			 0x41c00000,
			 0x41800000,
			 0x40e00000,
			 0x41f80000,

			 /// vpu register f29
			 0x41d80000,
			 0x40000000,
			 0x41880000,
			 0x41100000,
			 0x41d80000,
			 0x42000000,
			 0x41e80000,
			 0x41f80000,

			 /// vpu register f30
			 0x41e00000,
			 0x41900000,
			 0x41300000,
			 0x41a00000,
			 0x41800000,
			 0x41900000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f31
			 0x41d80000,
			 0x41b00000,
			 0x41d80000,
			 0x41d00000,
			 0x41000000,
			 0x40c00000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f28, f0, f7\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f27, f30, f21\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f22, f15, f5\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f6, f16, f24\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f5, f21, f21\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f4, f14, f6\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f28, f2, f11\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f19, f23, f15\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f18, f25, f11\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f4, f19, f2\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f22, f11, f17\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f6, f23, f30\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f21, f19, f19\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f3, f3, f6\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f7, f19, f30\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f16, f18, f15\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f25, f15, f23\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f24, f15, f18\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f17, f22, f14\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f12, f22, f12\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f24, f1, f18\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f27, f24, f21\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f6, f11, f7\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f25, f1, f25\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f18, f12, f10\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f9, f23, f24\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f24, f6, f24\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f19, f15, f9\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f10, f10, f8\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f29, f16, f28\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f28, f25, f24\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f11, f24, f19\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f8, f20, f30\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f14, f30, f0\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f6, f7, f19\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f16, f20, f5\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f7, f1, f16\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f22, f24, f15\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f10, f3, f5\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f7, f19, f25\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f3, f26, f16\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f25, f15, f9\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f8, f15, f10\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f8, f21, f6\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f26, f18, f9\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f22, f27, f22\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f16, f3, f0\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f2, f7, f13\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f25, f26, f16\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f14, f20, f13\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f29, f22, f29\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f20, f15, f22\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f26, f16, f22\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f19, f14, f28\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f29, f0, f1\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f7, f22, f2\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f17, f5, f17\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f5, f1, f17\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f14, f10, f14\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f16, f8, f7\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f9, f18, f1\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f29, f11, f1\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f3, f29, f0\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f3, f10, f14\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f11, f21, f24\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f5, f18, f14\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f26, f28, f13\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f18, f22, f22\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f9, f23, f10\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f10, f30, f26\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f22, f6, f18\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f30, f30, f9\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f11, f16, f15\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f21, f9, f28\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f12, f17, f23\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f14, f19, f26\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f9, f15, f1\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f20, f19, f1\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f24, f30, f29\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f11, f7, f14\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f18, f11, f29\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f2, f5, f2\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f11, f9, f10\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f10, f22, f26\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f25, f26, f29\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f8, f1, f12\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f18, f8, f3\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f13, f5, f17\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f6, f3, f26\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f26, f23, f26\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f26, f8, f26\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f14, f7, f26\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f30, f12, f17\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f9, f12, f16\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f14, f22, f23\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f16, f26, f24\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f11, f21, f27\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f18, f27, f5\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnj.s f7, f3, f17\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
