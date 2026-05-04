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
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00000
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x7fc00001,                                                  // signaling NaN                               /// 00010
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00014
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00018
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00024
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0002c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00038
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00048
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00054
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0005c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00060
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00064
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00068
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0006c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00070
			 0xff800000,                                                  // -inf                                        /// 00074
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0007c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00080
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0008c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x55555555,                                                  // 4 random values                             /// 00094
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00098
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0009c
			 0x7fc00001,                                                  // signaling NaN                               /// 000a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000b0
			 0x80000000,                                                  // -zero                                       /// 000b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000f4
			 0xbf028f5c,                                                  // -0.51                                       /// 000f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00104
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00110
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00114
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00118
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00124
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00128
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00130
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00134
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00138
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0013c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00140
			 0x55555555,                                                  // 4 random values                             /// 00144
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00148
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00000000,                                                  // zero                                        /// 00150
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00158
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x7f800000,                                                  // inf                                         /// 00160
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0016c
			 0x7f800000,                                                  // inf                                         /// 00170
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00178
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00180
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00184
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00188
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00190
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00194
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0019c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001e0
			 0x3f028f5c,                                                  // 0.51                                        /// 001e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00200
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00208
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0020c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00210
			 0x0c780000,                                                  // Leading 1s:                                 /// 00214
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0021c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00224
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00228
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0022c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00230
			 0xbf028f5c,                                                  // -0.51                                       /// 00234
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00238
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0023c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00240
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00244
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0024c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00250
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00254
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00258
			 0x0c400000,                                                  // Leading 1s:                                 /// 0025c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00268
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0026c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00270
			 0x0e000001,                                                  // Trailing 1s:                                /// 00274
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0027c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00284
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00288
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0028c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00298
			 0xbf028f5c,                                                  // -0.51                                       /// 0029c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002c8
			 0x3f028f5c,                                                  // 0.51                                        /// 002cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00300
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00304
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0030c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00314
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00318
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0031c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00320
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x7fc00001,                                                  // signaling NaN                               /// 00328
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0032c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00334
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00340
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00344
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0034c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00350
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00354
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00358
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00360
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00364
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00368
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0036c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00370
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00374
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00378
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0037c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00380
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00388
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0038c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00390
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00394
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003c8
			 0x4b000000,                                                  // 8388608.0                                   /// 003cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d4
			 0xff800000,                                                  // -inf                                        /// 003d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0e000007,                                                  // Trailing 1s:                                /// 00404
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00408
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00414
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00418
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00420
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0042c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00430
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00434
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00438
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0043c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00440
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00444
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00448
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00450
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00454
			 0x7f800000,                                                  // inf                                         /// 00458
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0045c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00460
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00464
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00468
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00474
			 0x0e000001,                                                  // Trailing 1s:                                /// 00478
			 0x0c600000,                                                  // Leading 1s:                                 /// 0047c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00480
			 0x4b000000,                                                  // 8388608.0                                   /// 00484
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00488
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00490
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c8
			 0x4b000000,                                                  // 8388608.0                                   /// 004cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00500
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00504
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0050c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00518
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0051c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00520
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00524
			 0x3f028f5c,                                                  // 0.51                                        /// 00528
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00530
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00534
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00538
			 0x3f028f5c,                                                  // 0.51                                        /// 0053c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00540
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00544
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00550
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00558
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00560
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0056c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00570
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00574
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80000000,                                                  // -zero                                       /// 0057c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00580
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00584
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00588
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0058c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xff800000,                                                  // -inf                                        /// 00594
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x3f028f5c,                                                  // 0.51                                        /// 005b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00604
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x7f800000,                                                  // inf                                         /// 0060c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00610
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0061c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00620
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x4b000000,                                                  // 8388608.0                                   /// 00628
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0062c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00630
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x3f028f5c,                                                  // 0.51                                        /// 0063c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00640
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00648
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00650
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00660
			 0x4b000000,                                                  // 8388608.0                                   /// 00664
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00668
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00684
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00690
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00698
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80000000,                                                  // -zero                                       /// 006dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00704
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00708
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0070c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00710
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00720
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00724
			 0x00011111,                                                  // 9.7958E-41                                  /// 00728
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0072c
			 0x80000000,                                                  // -zero                                       /// 00730
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x55555555,                                                  // 4 random values                             /// 00738
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0073c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00744
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00748
			 0x80000000,                                                  // -zero                                       /// 0074c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00750
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00754
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0075c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0xffc00001,                                                  // -signaling NaN                              /// 00764
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00770
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00774
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00000000,                                                  // zero                                        /// 00780
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0078c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00790
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00798
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00800
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00804
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00808
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0080c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00818
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0081c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00820
			 0x80011111,                                                  // -9.7958E-41                                 /// 00824
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00830
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00838
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0xbf028f5c,                                                  // -0.51                                       /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00848
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0084c
			 0xbf028f5c,                                                  // -0.51                                       /// 00850
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00854
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00858
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00868
			 0x0e000007,                                                  // Trailing 1s:                                /// 0086c
			 0x4b000000,                                                  // 8388608.0                                   /// 00870
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00874
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0088c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00890
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a0
			 0x00000000,                                                  // zero                                        /// 008a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x7fc00001,                                                  // signaling NaN                               /// 008d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00900
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00908
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0090c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00914
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0091c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00920
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00924
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00928
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0092c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00930
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00938
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00950
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00954
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0095c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c600000,                                                  // Leading 1s:                                 /// 00964
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0096c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00974
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x0e000007,                                                  // Trailing 1s:                                /// 0097c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00980
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00984
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00988
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0098c
			 0x3f028f5c,                                                  // 0.51                                        /// 00990
			 0x7fc00001,                                                  // signaling NaN                               /// 00994
			 0x80000000,                                                  // -zero                                       /// 00998
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009dc
			 0x3f028f5c,                                                  // 0.51                                        /// 009e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x55555555,                                                  // 4 random values                             /// 00a04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0xff800000,                                                  // -inf                                        /// 00a14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a18
			 0x00000000,                                                  // zero                                        /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000000,                                                  // zero                                        /// 00a34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x55555555,                                                  // 4 random values                             /// 00a40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a6c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x55555555,                                                  // 4 random values                             /// 00a88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00aa4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00abc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00acc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00adc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ae0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00afc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x7f800000,                                                  // inf                                         /// 00b74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b8c
			 0x33333333,                                                  // 4 random values                             /// 00b90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bbc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bf0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf4
			 0x33333333,                                                  // 4 random values                             /// 00bf8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bfc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xffc00001,                                                  // -signaling NaN                              /// 00c14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c50
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00ca0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ca8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ccc
			 0xff800000,                                                  // -inf                                        /// 00cd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80000000,                                                  // -zero                                       /// 00ce8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cfc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d38
			 0x33333333,                                                  // 4 random values                             /// 00d3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00da0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00da4
			 0xcb000000,                                                  // -8388608.0                                  /// 00da8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00db4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dfc
			 0x00000000,                                                  // zero                                        /// 00e00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea4
			 0x80000000,                                                  // -zero                                       /// 00ea8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ebc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ed8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ee4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00efc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x3f028f5c,                                                  // 0.51                                        /// 00f14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f58
			 0x7f800000,                                                  // inf                                         /// 00f5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fac
			 0x00000000,                                                  // zero                                        /// 00fb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ff0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff4
			 0x7f800000,                                                  // inf                                         /// 00ff8
			 0x80800000 // min norm                                      // subnormals -ve                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00000
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c400000,                                                  // Leading 1s:                                 /// 0000c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00014
			 0x0e000007,                                                  // Trailing 1s:                                /// 00018
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0001c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0002c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00034
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00038
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0003c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0c700000,                                                  // Leading 1s:                                 /// 00044
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00048
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00050
			 0x0c600000,                                                  // Leading 1s:                                 /// 00054
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00058
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0xff800000,                                                  // -inf                                        /// 00064
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00068
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0006c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00070
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0007c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00080
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00090
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00094
			 0x80011111,                                                  // -9.7958E-41                                 /// 00098
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0009c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000dc
			 0xbf028f5c,                                                  // -0.51                                       /// 000e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00100
			 0x80000000,                                                  // -zero                                       /// 00104
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00110
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00114
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00118
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0011c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00120
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00128
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0012c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00130
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00138
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0013c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00140
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00144
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00148
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0014c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00154
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00158
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0015c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00160
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00164
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00168
			 0x80000000,                                                  // -zero                                       /// 0016c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00170
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00178
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0017c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00180
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00184
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x33333333,                                                  // 4 random values                             /// 00190
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00194
			 0x0c600000,                                                  // Leading 1s:                                 /// 00198
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d0
			 0x3f028f5c,                                                  // 0.51                                        /// 001d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0xbf028f5c,                                                  // -0.51                                       /// 001fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00200
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00204
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0020c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00210
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00214
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80000000,                                                  // -zero                                       /// 0021c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00220
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00224
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0022c
			 0x80000000,                                                  // -zero                                       /// 00230
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00234
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00238
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0023c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00240
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00248
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00258
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0025c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00260
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00264
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00268
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0026c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00278
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00280
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00288
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00294
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00298
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0029c
			 0xbf028f5c,                                                  // -0.51                                       /// 002a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00304
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00308
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0030c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00310
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00314
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00318
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0031c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00320
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00324
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0032c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00330
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00338
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00340
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0034c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x4b000000,                                                  // 8388608.0                                   /// 0035c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00360
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00364
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00374
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0037c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00380
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00384
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0038c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00394
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c4
			 0xbf028f5c,                                                  // -0.51                                       /// 003c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00400
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00404
			 0x33333333,                                                  // 4 random values                             /// 00408
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00410
			 0x4b000000,                                                  // 8388608.0                                   /// 00414
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00420
			 0x3f028f5c,                                                  // 0.51                                        /// 00424
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0042c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00430
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00434
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00438
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00440
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0044c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00450
			 0x0c700000,                                                  // Leading 1s:                                 /// 00454
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x0c600000,                                                  // Leading 1s:                                 /// 00468
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0046c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00470
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0047c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00480
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00484
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00000000,                                                  // zero                                        /// 0048c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00490
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00494
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00498
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00500
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00514
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0051c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00520
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x55555555,                                                  // 4 random values                             /// 0052c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00530
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0053c
			 0x7f800000,                                                  // inf                                         /// 00540
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00548
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00550
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00558
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0055c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00564
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0057c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00580
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00584
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00588
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0058c
			 0x55555555,                                                  // 4 random values                             /// 00590
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00594
			 0x0e000001,                                                  // Trailing 1s:                                /// 00598
			 0x0c780000,                                                  // Leading 1s:                                 /// 0059c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e4
			 0x7f800000,                                                  // inf                                         /// 005e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 005f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005f8
			 0x80000000,                                                  // -zero                                       /// 005fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00600
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00604
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0060c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00614
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00618
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00620
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00628
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0062c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00630
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00634
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00644
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0064c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00654
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00658
			 0x80000000,                                                  // -zero                                       /// 0065c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00660
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00664
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00668
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00674
			 0x33333333,                                                  // 4 random values                             /// 00678
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00680
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0068c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x55555555,                                                  // 4 random values                             /// 00698
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0069c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b8
			 0xbf028f5c,                                                  // -0.51                                       /// 006bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006dc
			 0x3f028f5c,                                                  // 0.51                                        /// 006e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f0
			 0x33333333,                                                  // 4 random values                             /// 006f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006f8
			 0x33333333,                                                  // 4 random values                             /// 006fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00700
			 0x0c700000,                                                  // Leading 1s:                                 /// 00704
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00708
			 0x0c400000,                                                  // Leading 1s:                                 /// 0070c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00710
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00714
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00718
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0071c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00724
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0072c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00738
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0073c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0074c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00750
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00754
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00764
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00768
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0076c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00774
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0077c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x0c780000,                                                  // Leading 1s:                                 /// 0078c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00794
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0079c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007a8
			 0xffc00001,                                                  // -signaling NaN                              /// 007ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00800
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0080c
			 0x7fc00001,                                                  // signaling NaN                               /// 00810
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00814
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0081c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00824
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0082c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00830
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00834
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00838
			 0x0e000001,                                                  // Trailing 1s:                                /// 0083c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00844
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00848
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x4b000000,                                                  // 8388608.0                                   /// 00850
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x33333333,                                                  // 4 random values                             /// 00858
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0085c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00860
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00864
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00868
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0086c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00870
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00878
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0087c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00888
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00890
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00898
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0089c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008c8
			 0x55555555,                                                  // 4 random values                             /// 008cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00904
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00908
			 0x0c400000,                                                  // Leading 1s:                                 /// 0090c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00910
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00914
			 0x00011111,                                                  // 9.7958E-41                                  /// 00918
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00930
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00934
			 0x00011111,                                                  // 9.7958E-41                                  /// 00938
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00954
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00964
			 0x4b000000,                                                  // 8388608.0                                   /// 00968
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0096c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00970
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xff800000,                                                  // -inf                                        /// 00978
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0097c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00984
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0098c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00990
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00998
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f4
			 0x3f028f5c,                                                  // 0.51                                        /// 009f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a20
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a24
			 0xcb000000,                                                  // -8388608.0                                  /// 00a28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a48
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a90
			 0xbf028f5c,                                                  // -0.51                                       /// 00a94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00aac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab8
			 0xffc00001,                                                  // -signaling NaN                              /// 00abc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ac4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ad0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00adc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00afc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b00
			 0xff800000,                                                  // -inf                                        /// 00b04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ba4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ba8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bcc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00be4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bf8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xbf028f5c,                                                  // -0.51                                       /// 00c04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c10
			 0x80000000,                                                  // -zero                                       /// 00c14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c18
			 0xbf028f5c,                                                  // -0.51                                       /// 00c1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c60
			 0xcb000000,                                                  // -8388608.0                                  /// 00c64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ca0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x55555555,                                                  // 4 random values                             /// 00cbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce8
			 0x33333333,                                                  // 4 random values                             /// 00cec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cfc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d04
			 0xffc00001,                                                  // -signaling NaN                              /// 00d08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d20
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x00000000,                                                  // zero                                        /// 00d54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00da0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xffc00001,                                                  // -signaling NaN                              /// 00db0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ddc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00df0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e68
			 0x33333333,                                                  // 4 random values                             /// 00e6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00eac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb4
			 0xff800000,                                                  // -inf                                        /// 00eb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ec0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ecc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ed0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ee0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7f800000,                                                  // inf                                         /// 00ee8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ef0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00efc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f40
			 0x7fc00001,                                                  // signaling NaN                               /// 00f44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f70
			 0x4b000000,                                                  // 8388608.0                                   /// 00f74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fa4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fcc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fe8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0xbf028f5c                                                  // -0.51                                       /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x94795756, /// 0x0
			 0x7ee073d0, /// 0x4
			 0xfef8d302, /// 0x8
			 0x54367c41, /// 0xc
			 0x904c236f, /// 0x10
			 0xb352d2d1, /// 0x14
			 0x1328d62e, /// 0x18
			 0x4e77036e, /// 0x1c
			 0xc0abe037, /// 0x20
			 0xf5d8d267, /// 0x24
			 0x03746ef4, /// 0x28
			 0x9fd00ebb, /// 0x2c
			 0xbe51c093, /// 0x30
			 0xcc869ec1, /// 0x34
			 0x5c829aec, /// 0x38
			 0x40030822, /// 0x3c
			 0x29dba76d, /// 0x40
			 0xbb51ec3e, /// 0x44
			 0xdd2d9dd9, /// 0x48
			 0xefe9bc8e, /// 0x4c
			 0x5edd051b, /// 0x50
			 0x315a19d7, /// 0x54
			 0x89fdb27b, /// 0x58
			 0x43fc40c2, /// 0x5c
			 0xf4509e6d, /// 0x60
			 0xbaad04a3, /// 0x64
			 0x98bb405c, /// 0x68
			 0x05a905f6, /// 0x6c
			 0x7d429e1e, /// 0x70
			 0x02bb848f, /// 0x74
			 0x0cbffb5b, /// 0x78
			 0x61e648f0, /// 0x7c
			 0x24ef5da7, /// 0x80
			 0xbae73ece, /// 0x84
			 0x1566989e, /// 0x88
			 0x0784f1f1, /// 0x8c
			 0xfa45cfc5, /// 0x90
			 0xff617db7, /// 0x94
			 0xfaad63eb, /// 0x98
			 0xbc217ff6, /// 0x9c
			 0xf2328fc7, /// 0xa0
			 0x068f1615, /// 0xa4
			 0xb773b710, /// 0xa8
			 0x3f217656, /// 0xac
			 0x97bd5f76, /// 0xb0
			 0xb0d139dd, /// 0xb4
			 0xc08a7602, /// 0xb8
			 0x88f88024, /// 0xbc
			 0xe3250a65, /// 0xc0
			 0xda8cbea5, /// 0xc4
			 0x185c65e4, /// 0xc8
			 0xc0434ab9, /// 0xcc
			 0x978baeab, /// 0xd0
			 0xc04fa13f, /// 0xd4
			 0x7b96f343, /// 0xd8
			 0xa2720211, /// 0xdc
			 0x38b219fc, /// 0xe0
			 0x4ffba9bf, /// 0xe4
			 0x08cf552b, /// 0xe8
			 0x19ada00e, /// 0xec
			 0x1d9f56b5, /// 0xf0
			 0xb8e8e7c1, /// 0xf4
			 0x7c64ade9, /// 0xf8
			 0xbd1c5db5, /// 0xfc
			 0x7c7b05ed, /// 0x100
			 0x687f5569, /// 0x104
			 0x1694f008, /// 0x108
			 0x829fced4, /// 0x10c
			 0xb3b6b9c1, /// 0x110
			 0x0b75526b, /// 0x114
			 0x8e4bd5e7, /// 0x118
			 0x1f4cdb58, /// 0x11c
			 0x8807b5f3, /// 0x120
			 0x8bcd0461, /// 0x124
			 0x66a98040, /// 0x128
			 0x8ed7ccb2, /// 0x12c
			 0x1fee78d5, /// 0x130
			 0x3e0386c4, /// 0x134
			 0x744db8a3, /// 0x138
			 0x52508f83, /// 0x13c
			 0xddf9f6b7, /// 0x140
			 0x72aed30f, /// 0x144
			 0x302855de, /// 0x148
			 0xd0e9a9f4, /// 0x14c
			 0x9fb68869, /// 0x150
			 0xfcc6a7fd, /// 0x154
			 0x61543c67, /// 0x158
			 0xc5a4373e, /// 0x15c
			 0x59f03261, /// 0x160
			 0xd5aea310, /// 0x164
			 0xf59eb6d6, /// 0x168
			 0x755d0297, /// 0x16c
			 0xd36410b9, /// 0x170
			 0xb2f5b55d, /// 0x174
			 0x05e554b6, /// 0x178
			 0xb8118ed4, /// 0x17c
			 0xb165ac26, /// 0x180
			 0x3ec17ea4, /// 0x184
			 0xd2437e15, /// 0x188
			 0x707834a0, /// 0x18c
			 0xd8e566ab, /// 0x190
			 0x5e06daf6, /// 0x194
			 0xd5832087, /// 0x198
			 0xf28429d7, /// 0x19c
			 0x6a962f96, /// 0x1a0
			 0xbc273bec, /// 0x1a4
			 0x8e353729, /// 0x1a8
			 0xee6ac27d, /// 0x1ac
			 0x65a3aada, /// 0x1b0
			 0xf8a160ba, /// 0x1b4
			 0x64d7f425, /// 0x1b8
			 0x8ff99ab2, /// 0x1bc
			 0x2a80d5fa, /// 0x1c0
			 0x861a3f6a, /// 0x1c4
			 0x65597985, /// 0x1c8
			 0x2b877f6a, /// 0x1cc
			 0x522194ad, /// 0x1d0
			 0x703347e5, /// 0x1d4
			 0xe0964f9d, /// 0x1d8
			 0x3c355a7f, /// 0x1dc
			 0x5576424f, /// 0x1e0
			 0x2cd77ec9, /// 0x1e4
			 0x9125a288, /// 0x1e8
			 0x8436e422, /// 0x1ec
			 0x9f9a1086, /// 0x1f0
			 0xf5f4739d, /// 0x1f4
			 0x75b674ea, /// 0x1f8
			 0xdf493679, /// 0x1fc
			 0x7e13cf58, /// 0x200
			 0x713e9538, /// 0x204
			 0x8b5086d2, /// 0x208
			 0xd684e509, /// 0x20c
			 0x22e6bd00, /// 0x210
			 0x2ce84a1e, /// 0x214
			 0xac7b27bb, /// 0x218
			 0x2e1d7de1, /// 0x21c
			 0x5b4554e1, /// 0x220
			 0xb63e2a95, /// 0x224
			 0x6a5ccf1c, /// 0x228
			 0x8178d9af, /// 0x22c
			 0x319d4a8f, /// 0x230
			 0x388fc831, /// 0x234
			 0xb142daa3, /// 0x238
			 0xdfd653b6, /// 0x23c
			 0x9b9dfae4, /// 0x240
			 0x627b255e, /// 0x244
			 0x8cb109e5, /// 0x248
			 0x9c5deb73, /// 0x24c
			 0xbf09318e, /// 0x250
			 0x36816a51, /// 0x254
			 0x64ea60d1, /// 0x258
			 0x6aeb84b4, /// 0x25c
			 0xde35b28a, /// 0x260
			 0x75104fa6, /// 0x264
			 0x6395cbb0, /// 0x268
			 0x4d596eaf, /// 0x26c
			 0x3137738f, /// 0x270
			 0xbe91f832, /// 0x274
			 0x2bbd02a1, /// 0x278
			 0xcbf3ccea, /// 0x27c
			 0xca68658e, /// 0x280
			 0x6806ddae, /// 0x284
			 0x2806cb3d, /// 0x288
			 0x97de7e32, /// 0x28c
			 0x1e6969a0, /// 0x290
			 0xf4f34209, /// 0x294
			 0x2f1e9ad9, /// 0x298
			 0x63048552, /// 0x29c
			 0xf5305d89, /// 0x2a0
			 0x7c4512d1, /// 0x2a4
			 0x64df784e, /// 0x2a8
			 0x9aa2c1d5, /// 0x2ac
			 0xdaab58ba, /// 0x2b0
			 0x0dabe64d, /// 0x2b4
			 0x4d2c683d, /// 0x2b8
			 0x65756359, /// 0x2bc
			 0x1d5d19ed, /// 0x2c0
			 0xd2d051c1, /// 0x2c4
			 0x7cc80415, /// 0x2c8
			 0xef8ec92a, /// 0x2cc
			 0x7f8e0e43, /// 0x2d0
			 0x22759fa7, /// 0x2d4
			 0x0fc69ed4, /// 0x2d8
			 0xb792cb52, /// 0x2dc
			 0x42b1fe97, /// 0x2e0
			 0x472b6fdd, /// 0x2e4
			 0x93294530, /// 0x2e8
			 0x11648913, /// 0x2ec
			 0xa6ddf76a, /// 0x2f0
			 0xf8b64f8e, /// 0x2f4
			 0x847291a1, /// 0x2f8
			 0xa16c0656, /// 0x2fc
			 0xeb09557b, /// 0x300
			 0xb28f254d, /// 0x304
			 0x46dfd32e, /// 0x308
			 0x1275ac11, /// 0x30c
			 0x75dacafa, /// 0x310
			 0x44c264e6, /// 0x314
			 0x9cf8cdd7, /// 0x318
			 0xb645a282, /// 0x31c
			 0xfcda9ff3, /// 0x320
			 0xc4b792a4, /// 0x324
			 0xc4956399, /// 0x328
			 0x9471c18d, /// 0x32c
			 0xb33c2fa6, /// 0x330
			 0xb5b5db39, /// 0x334
			 0x31f30070, /// 0x338
			 0xcee64864, /// 0x33c
			 0x0ce6038d, /// 0x340
			 0x9ca2d3f3, /// 0x344
			 0x62b5fd89, /// 0x348
			 0x61c6263d, /// 0x34c
			 0x07108bce, /// 0x350
			 0x6bc49195, /// 0x354
			 0x1068e735, /// 0x358
			 0x57930330, /// 0x35c
			 0x2cb26c0d, /// 0x360
			 0x98292220, /// 0x364
			 0x1f02a2b4, /// 0x368
			 0x38ec0f60, /// 0x36c
			 0x335fa886, /// 0x370
			 0xdea27667, /// 0x374
			 0x2c265223, /// 0x378
			 0xfce801c5, /// 0x37c
			 0x73339865, /// 0x380
			 0x9fc251af, /// 0x384
			 0x1efc1ec9, /// 0x388
			 0xda126d0e, /// 0x38c
			 0xca7059fb, /// 0x390
			 0xf09dbb74, /// 0x394
			 0x224800a3, /// 0x398
			 0x238e0d3e, /// 0x39c
			 0xfeec5136, /// 0x3a0
			 0xe55f5c34, /// 0x3a4
			 0xf6a32514, /// 0x3a8
			 0xe91cde33, /// 0x3ac
			 0x3c881e34, /// 0x3b0
			 0x69b02d94, /// 0x3b4
			 0x18efe404, /// 0x3b8
			 0x0b7a7d54, /// 0x3bc
			 0xb8e4d2b5, /// 0x3c0
			 0x7999bb4a, /// 0x3c4
			 0x0d2319a0, /// 0x3c8
			 0xa24ab410, /// 0x3cc
			 0xce62605a, /// 0x3d0
			 0x711d1dec, /// 0x3d4
			 0x4c33d7ff, /// 0x3d8
			 0x1378a7c8, /// 0x3dc
			 0x6b4c2b6c, /// 0x3e0
			 0xc3c54d37, /// 0x3e4
			 0x8e315d2f, /// 0x3e8
			 0xf0af1703, /// 0x3ec
			 0xd7a5bbcc, /// 0x3f0
			 0x0f39497b, /// 0x3f4
			 0x4e69110d, /// 0x3f8
			 0xb340327c, /// 0x3fc
			 0xe2de25f9, /// 0x400
			 0x9bfdd2b6, /// 0x404
			 0xfe64830a, /// 0x408
			 0xf48d2a5b, /// 0x40c
			 0x53335e4e, /// 0x410
			 0xa6ba148e, /// 0x414
			 0xc60d9de8, /// 0x418
			 0x4fe6d3f4, /// 0x41c
			 0xae80eb75, /// 0x420
			 0x2ead1956, /// 0x424
			 0xdf642afa, /// 0x428
			 0x92b61c2c, /// 0x42c
			 0x7a73e226, /// 0x430
			 0x928895d3, /// 0x434
			 0x2380a1ad, /// 0x438
			 0x3f37e8a6, /// 0x43c
			 0xa841a53d, /// 0x440
			 0x2443ed0b, /// 0x444
			 0x043f903a, /// 0x448
			 0x34096584, /// 0x44c
			 0x340f0168, /// 0x450
			 0xf936fe84, /// 0x454
			 0x3c1d47ef, /// 0x458
			 0x6dad4bd0, /// 0x45c
			 0xd4a297b9, /// 0x460
			 0xac62c3aa, /// 0x464
			 0x4c1c97af, /// 0x468
			 0xd642e860, /// 0x46c
			 0x222ea55d, /// 0x470
			 0xe1078142, /// 0x474
			 0xc52c6a00, /// 0x478
			 0x3ba1ca12, /// 0x47c
			 0xc6f7b78c, /// 0x480
			 0x0a7cd2b2, /// 0x484
			 0x69abc057, /// 0x488
			 0xa27c0bd9, /// 0x48c
			 0x849470b9, /// 0x490
			 0x1b300f7e, /// 0x494
			 0xb0cb07fe, /// 0x498
			 0xa2eea806, /// 0x49c
			 0x4bcc1cbb, /// 0x4a0
			 0xd5a8fd78, /// 0x4a4
			 0x895de4dc, /// 0x4a8
			 0xe90e835f, /// 0x4ac
			 0x6d1d48a6, /// 0x4b0
			 0x483fd187, /// 0x4b4
			 0x1a521855, /// 0x4b8
			 0x2a596ae7, /// 0x4bc
			 0x9a973993, /// 0x4c0
			 0x3d4513eb, /// 0x4c4
			 0x3118d7c4, /// 0x4c8
			 0x5f465d99, /// 0x4cc
			 0x34e82272, /// 0x4d0
			 0x0a741915, /// 0x4d4
			 0x4b09a909, /// 0x4d8
			 0xe2f89b9f, /// 0x4dc
			 0xba238694, /// 0x4e0
			 0xdb2f861b, /// 0x4e4
			 0x28ab6d73, /// 0x4e8
			 0x570a4382, /// 0x4ec
			 0x395f1298, /// 0x4f0
			 0x630e9a16, /// 0x4f4
			 0x3d82ebef, /// 0x4f8
			 0x45f2cc71, /// 0x4fc
			 0xef7b1087, /// 0x500
			 0xec19f518, /// 0x504
			 0xa579d366, /// 0x508
			 0xba733e68, /// 0x50c
			 0x77c2c04c, /// 0x510
			 0xccec390e, /// 0x514
			 0x4209e709, /// 0x518
			 0x892ebc52, /// 0x51c
			 0x1ac9b20e, /// 0x520
			 0x84beb581, /// 0x524
			 0xfcbe667d, /// 0x528
			 0x7f5c1e9c, /// 0x52c
			 0xe14d50e8, /// 0x530
			 0xfcac5b1e, /// 0x534
			 0xfc3c9ff2, /// 0x538
			 0x9bed7bec, /// 0x53c
			 0x0304f64b, /// 0x540
			 0x4e6485d3, /// 0x544
			 0x8e21ddc2, /// 0x548
			 0x6d3169d4, /// 0x54c
			 0x230c599e, /// 0x550
			 0x3303db7f, /// 0x554
			 0xbbe46696, /// 0x558
			 0x1a183a2f, /// 0x55c
			 0xd97c6633, /// 0x560
			 0x7dfe8621, /// 0x564
			 0xd7eb1b1b, /// 0x568
			 0x64d10c52, /// 0x56c
			 0x70e09437, /// 0x570
			 0x20cf20d2, /// 0x574
			 0x5b99999e, /// 0x578
			 0x07e1b435, /// 0x57c
			 0xc3aeaf8f, /// 0x580
			 0x94018f0d, /// 0x584
			 0xed62bee3, /// 0x588
			 0xc9ae92d9, /// 0x58c
			 0x835929c2, /// 0x590
			 0x94f634e8, /// 0x594
			 0xc221648c, /// 0x598
			 0x8e26fd9f, /// 0x59c
			 0x2e6e7d63, /// 0x5a0
			 0xc8781b96, /// 0x5a4
			 0xc776e8ab, /// 0x5a8
			 0x27ad7acb, /// 0x5ac
			 0x294ec6d6, /// 0x5b0
			 0x38a3d054, /// 0x5b4
			 0xfaaad0d6, /// 0x5b8
			 0xbe95e986, /// 0x5bc
			 0x0a989bc1, /// 0x5c0
			 0x189ffe91, /// 0x5c4
			 0xa0ca5919, /// 0x5c8
			 0x3493ff35, /// 0x5cc
			 0xd7e10859, /// 0x5d0
			 0x68818b2d, /// 0x5d4
			 0xdfb865ee, /// 0x5d8
			 0x01b463e8, /// 0x5dc
			 0x850778e1, /// 0x5e0
			 0xeaae3f69, /// 0x5e4
			 0x34764aec, /// 0x5e8
			 0x9b86ad48, /// 0x5ec
			 0x08ec24ae, /// 0x5f0
			 0x7bfab53c, /// 0x5f4
			 0xb28d512e, /// 0x5f8
			 0xfa4336d7, /// 0x5fc
			 0x9deb0271, /// 0x600
			 0x964a5036, /// 0x604
			 0x69685453, /// 0x608
			 0x08085a1c, /// 0x60c
			 0xc2e00649, /// 0x610
			 0x94af06b4, /// 0x614
			 0x1661d96c, /// 0x618
			 0x620e0a63, /// 0x61c
			 0x846f25bd, /// 0x620
			 0x5a64d21c, /// 0x624
			 0x271c59a5, /// 0x628
			 0x61e52b3a, /// 0x62c
			 0xc08a928a, /// 0x630
			 0x81f9fbc3, /// 0x634
			 0xec61e2fd, /// 0x638
			 0x97f032ff, /// 0x63c
			 0x3cd24608, /// 0x640
			 0xf17053dd, /// 0x644
			 0xf4fa6efb, /// 0x648
			 0x25c7315e, /// 0x64c
			 0x05b47ae3, /// 0x650
			 0x010bf831, /// 0x654
			 0xfb3bb556, /// 0x658
			 0x5d925886, /// 0x65c
			 0x26b18c60, /// 0x660
			 0x461367c0, /// 0x664
			 0x5c8b3266, /// 0x668
			 0x06c64919, /// 0x66c
			 0x655b8dbf, /// 0x670
			 0x71c8f7d9, /// 0x674
			 0x7e2e1eaa, /// 0x678
			 0x37281dfe, /// 0x67c
			 0xc26d2735, /// 0x680
			 0x2aba7d82, /// 0x684
			 0x7df90c74, /// 0x688
			 0x18c49c43, /// 0x68c
			 0xe420304d, /// 0x690
			 0xeaaea3f8, /// 0x694
			 0x5e5c3caf, /// 0x698
			 0x79d8ef2f, /// 0x69c
			 0x1efd9193, /// 0x6a0
			 0x9cd081d7, /// 0x6a4
			 0x0d7be818, /// 0x6a8
			 0x0f190646, /// 0x6ac
			 0x2d82c7e4, /// 0x6b0
			 0x30d975e1, /// 0x6b4
			 0xdfca02e8, /// 0x6b8
			 0xb11df1d2, /// 0x6bc
			 0x8abf4fd3, /// 0x6c0
			 0xc1db5ce9, /// 0x6c4
			 0xa5074b27, /// 0x6c8
			 0xc2d9aa58, /// 0x6cc
			 0x761d99de, /// 0x6d0
			 0x113a0cda, /// 0x6d4
			 0x0dee3aee, /// 0x6d8
			 0x039c1f15, /// 0x6dc
			 0xd4e1bfe2, /// 0x6e0
			 0x6dd4b1b3, /// 0x6e4
			 0xae474ef2, /// 0x6e8
			 0xadc3f6ab, /// 0x6ec
			 0x3aeda525, /// 0x6f0
			 0x528455e7, /// 0x6f4
			 0xca2a0aad, /// 0x6f8
			 0x128238e5, /// 0x6fc
			 0x4e34e84b, /// 0x700
			 0xa40a22e9, /// 0x704
			 0xe24399cf, /// 0x708
			 0x20605ef8, /// 0x70c
			 0x9bc9ec57, /// 0x710
			 0xcaf8da66, /// 0x714
			 0x6f638138, /// 0x718
			 0xb4112093, /// 0x71c
			 0x46f81329, /// 0x720
			 0x7d3b035c, /// 0x724
			 0x57f504d3, /// 0x728
			 0x3f203c5b, /// 0x72c
			 0x3cfa6d30, /// 0x730
			 0xfdbe2473, /// 0x734
			 0xf4240a74, /// 0x738
			 0x508d9b91, /// 0x73c
			 0xa0a27ff9, /// 0x740
			 0x7109cc1d, /// 0x744
			 0xf4054ac0, /// 0x748
			 0x3aa11034, /// 0x74c
			 0xa1c1c6b3, /// 0x750
			 0x1ab27f02, /// 0x754
			 0x79dc6c39, /// 0x758
			 0x5d3aaa60, /// 0x75c
			 0x808a8afa, /// 0x760
			 0xcc3981cb, /// 0x764
			 0x0a34bbf0, /// 0x768
			 0xb1c9b0df, /// 0x76c
			 0x08f5a36b, /// 0x770
			 0x8c03a266, /// 0x774
			 0xa830aed6, /// 0x778
			 0xaa10070f, /// 0x77c
			 0x9615ecae, /// 0x780
			 0xe1bacf4e, /// 0x784
			 0x6cb19320, /// 0x788
			 0x9b957d3f, /// 0x78c
			 0xc24cde7e, /// 0x790
			 0xe2bb91b3, /// 0x794
			 0x71b09149, /// 0x798
			 0x7c5532c4, /// 0x79c
			 0x9b3c06f7, /// 0x7a0
			 0xb8bfcb64, /// 0x7a4
			 0x81547956, /// 0x7a8
			 0xd81de551, /// 0x7ac
			 0x5991b49f, /// 0x7b0
			 0x428c7dab, /// 0x7b4
			 0xea820c6a, /// 0x7b8
			 0x53924016, /// 0x7bc
			 0x19e9f122, /// 0x7c0
			 0xae73ded5, /// 0x7c4
			 0xd0e2dda6, /// 0x7c8
			 0xa1f0b94a, /// 0x7cc
			 0x96c3d60e, /// 0x7d0
			 0x82a7243f, /// 0x7d4
			 0xe2dd5842, /// 0x7d8
			 0x9c643abc, /// 0x7dc
			 0x50b92293, /// 0x7e0
			 0x37e34efe, /// 0x7e4
			 0x825848f9, /// 0x7e8
			 0x916dddc2, /// 0x7ec
			 0x6a764b91, /// 0x7f0
			 0xb502de5f, /// 0x7f4
			 0x6a4727d1, /// 0x7f8
			 0xbcb31cb9, /// 0x7fc
			 0xdd04265a, /// 0x800
			 0x1530edf3, /// 0x804
			 0x53e03957, /// 0x808
			 0x5cc313a3, /// 0x80c
			 0x8b32702d, /// 0x810
			 0x3757834c, /// 0x814
			 0x2a736e14, /// 0x818
			 0xba700c36, /// 0x81c
			 0x1ab7475a, /// 0x820
			 0x04e5e982, /// 0x824
			 0xe4afc025, /// 0x828
			 0x794126bb, /// 0x82c
			 0xddedb9ca, /// 0x830
			 0x6e1b57dd, /// 0x834
			 0x351792b4, /// 0x838
			 0xfa27cb99, /// 0x83c
			 0x18946143, /// 0x840
			 0x010ea3c3, /// 0x844
			 0x44ca4fc7, /// 0x848
			 0xfef6ab2e, /// 0x84c
			 0x9be09182, /// 0x850
			 0x9d672944, /// 0x854
			 0xb9d8a760, /// 0x858
			 0x252dcbc2, /// 0x85c
			 0x5623818a, /// 0x860
			 0xfb91e877, /// 0x864
			 0xb29a1342, /// 0x868
			 0x76866887, /// 0x86c
			 0xdb513327, /// 0x870
			 0x96af1f0e, /// 0x874
			 0xc9eaa95c, /// 0x878
			 0x4c82b746, /// 0x87c
			 0x921851a2, /// 0x880
			 0x10e81e47, /// 0x884
			 0x5c5119f0, /// 0x888
			 0x235e274f, /// 0x88c
			 0xfe666fec, /// 0x890
			 0xb625c1e1, /// 0x894
			 0x6f60c0df, /// 0x898
			 0x3d7f214a, /// 0x89c
			 0xba770120, /// 0x8a0
			 0x5056eb18, /// 0x8a4
			 0x96202a88, /// 0x8a8
			 0x58ea3bae, /// 0x8ac
			 0x2a552a72, /// 0x8b0
			 0xe7802419, /// 0x8b4
			 0xcf0dfed0, /// 0x8b8
			 0x71168f10, /// 0x8bc
			 0x55511dc8, /// 0x8c0
			 0x344445a4, /// 0x8c4
			 0xd0dd0b0d, /// 0x8c8
			 0x8fa77367, /// 0x8cc
			 0x600736db, /// 0x8d0
			 0xf0941e09, /// 0x8d4
			 0x27685ac7, /// 0x8d8
			 0xe8e29c86, /// 0x8dc
			 0xc65b48fe, /// 0x8e0
			 0xbac2ef74, /// 0x8e4
			 0x674030f5, /// 0x8e8
			 0x55d1214d, /// 0x8ec
			 0x18b565cd, /// 0x8f0
			 0x9456587d, /// 0x8f4
			 0x11fabc39, /// 0x8f8
			 0xf3351632, /// 0x8fc
			 0x7a8aa448, /// 0x900
			 0xecf60d51, /// 0x904
			 0xa3adeb74, /// 0x908
			 0x5bfac592, /// 0x90c
			 0x9c4476cf, /// 0x910
			 0xe0ac23b9, /// 0x914
			 0x29c7c6ad, /// 0x918
			 0x4003b03b, /// 0x91c
			 0x91d5fc49, /// 0x920
			 0xea2a2055, /// 0x924
			 0xab3e830a, /// 0x928
			 0xc2aa3af3, /// 0x92c
			 0xed338163, /// 0x930
			 0xf7c68281, /// 0x934
			 0xf3cfda0d, /// 0x938
			 0xd622c2e0, /// 0x93c
			 0x66abcf3d, /// 0x940
			 0x7672b108, /// 0x944
			 0x0d186875, /// 0x948
			 0xb159ec36, /// 0x94c
			 0xc9de0a44, /// 0x950
			 0x27cffae9, /// 0x954
			 0xd0da614d, /// 0x958
			 0xe077727c, /// 0x95c
			 0xa5051d0a, /// 0x960
			 0xa89f4841, /// 0x964
			 0x77b857b3, /// 0x968
			 0xebffe391, /// 0x96c
			 0x3375e948, /// 0x970
			 0x85191945, /// 0x974
			 0x726cb116, /// 0x978
			 0x624de3d9, /// 0x97c
			 0x5cf6a766, /// 0x980
			 0x5d1d1862, /// 0x984
			 0x63fea3dc, /// 0x988
			 0x39224075, /// 0x98c
			 0xc6e7b37f, /// 0x990
			 0xf63930e2, /// 0x994
			 0x484101ea, /// 0x998
			 0xb04d7fde, /// 0x99c
			 0xdcd65ca7, /// 0x9a0
			 0x30fb2298, /// 0x9a4
			 0x116019bf, /// 0x9a8
			 0x20173bd3, /// 0x9ac
			 0x1d0a59ad, /// 0x9b0
			 0xcaf3782a, /// 0x9b4
			 0x9389fe78, /// 0x9b8
			 0x68af9711, /// 0x9bc
			 0x2fc818fe, /// 0x9c0
			 0x611411e6, /// 0x9c4
			 0xf43a6cb1, /// 0x9c8
			 0x1b0d1f35, /// 0x9cc
			 0x04b1d8ca, /// 0x9d0
			 0x537c7793, /// 0x9d4
			 0xca27b1df, /// 0x9d8
			 0x172d7c5c, /// 0x9dc
			 0x1be28b6b, /// 0x9e0
			 0x3c161819, /// 0x9e4
			 0xa3141856, /// 0x9e8
			 0xfa72b5c4, /// 0x9ec
			 0x92ca5f1c, /// 0x9f0
			 0x0c073fba, /// 0x9f4
			 0x0a84846c, /// 0x9f8
			 0x0944b811, /// 0x9fc
			 0xb62ec251, /// 0xa00
			 0xa304c196, /// 0xa04
			 0x0e0741ed, /// 0xa08
			 0x35368e3e, /// 0xa0c
			 0x668ad3eb, /// 0xa10
			 0x57c62d5f, /// 0xa14
			 0xe32925c2, /// 0xa18
			 0x1a866565, /// 0xa1c
			 0xd2603e89, /// 0xa20
			 0x92e0eafb, /// 0xa24
			 0xc065be32, /// 0xa28
			 0xd363cab7, /// 0xa2c
			 0x861c8fe3, /// 0xa30
			 0x4378e463, /// 0xa34
			 0xb6d45e58, /// 0xa38
			 0x9c49e5e4, /// 0xa3c
			 0x72f6ea2c, /// 0xa40
			 0xeb9928f9, /// 0xa44
			 0x88da269c, /// 0xa48
			 0x0b89a855, /// 0xa4c
			 0x7e6f15fa, /// 0xa50
			 0xd98801f0, /// 0xa54
			 0xde78e286, /// 0xa58
			 0xdfc8b6c3, /// 0xa5c
			 0x31077889, /// 0xa60
			 0x0da36724, /// 0xa64
			 0xb745d77d, /// 0xa68
			 0xbee24df3, /// 0xa6c
			 0xc6b21228, /// 0xa70
			 0xb918c6ef, /// 0xa74
			 0x3de3b3a7, /// 0xa78
			 0x91e66c05, /// 0xa7c
			 0x300eb4f8, /// 0xa80
			 0x24a8c08a, /// 0xa84
			 0x8d939e66, /// 0xa88
			 0x4812c512, /// 0xa8c
			 0x9f7e5c2a, /// 0xa90
			 0x2df4a356, /// 0xa94
			 0x8e04bece, /// 0xa98
			 0x73e84eae, /// 0xa9c
			 0xe8554936, /// 0xaa0
			 0xdacbf147, /// 0xaa4
			 0xd550fb5e, /// 0xaa8
			 0xc4837a5b, /// 0xaac
			 0x16d1d860, /// 0xab0
			 0xa96c66ee, /// 0xab4
			 0xe95fb952, /// 0xab8
			 0x0ea3972a, /// 0xabc
			 0x93529f5d, /// 0xac0
			 0xd24b5308, /// 0xac4
			 0x1ade0391, /// 0xac8
			 0x7c02edc1, /// 0xacc
			 0x35d4491f, /// 0xad0
			 0x59df5ea2, /// 0xad4
			 0xdd2a7838, /// 0xad8
			 0x5ba2245c, /// 0xadc
			 0xd0e1ba18, /// 0xae0
			 0xde7b032b, /// 0xae4
			 0x0c3d8994, /// 0xae8
			 0x569af200, /// 0xaec
			 0xc6e0b8cf, /// 0xaf0
			 0x45680d2c, /// 0xaf4
			 0x33c57996, /// 0xaf8
			 0x3ca4bff8, /// 0xafc
			 0x90ab8825, /// 0xb00
			 0xa43fa03f, /// 0xb04
			 0xe711e6fc, /// 0xb08
			 0x637639d1, /// 0xb0c
			 0x96789ac4, /// 0xb10
			 0xfcc89798, /// 0xb14
			 0x6149a3bd, /// 0xb18
			 0xb2426c28, /// 0xb1c
			 0x43b00fc3, /// 0xb20
			 0xdb403b21, /// 0xb24
			 0x7bd5e526, /// 0xb28
			 0x614f60a0, /// 0xb2c
			 0x0244ff28, /// 0xb30
			 0x757a57a5, /// 0xb34
			 0x442855ec, /// 0xb38
			 0x934ba0fb, /// 0xb3c
			 0xc28c751f, /// 0xb40
			 0xb83d0598, /// 0xb44
			 0x6add9495, /// 0xb48
			 0x4dc319ec, /// 0xb4c
			 0x1fcb86af, /// 0xb50
			 0x035cd3dd, /// 0xb54
			 0xa086732b, /// 0xb58
			 0x1d085d06, /// 0xb5c
			 0xe0307def, /// 0xb60
			 0x7a74fae1, /// 0xb64
			 0x1631807a, /// 0xb68
			 0x5f1758f6, /// 0xb6c
			 0xd109ff6f, /// 0xb70
			 0x481f4e0d, /// 0xb74
			 0xecf12941, /// 0xb78
			 0x35b4c1ae, /// 0xb7c
			 0xb223e6e2, /// 0xb80
			 0x1140bf48, /// 0xb84
			 0x7a217690, /// 0xb88
			 0xe044de01, /// 0xb8c
			 0x1db2dfa8, /// 0xb90
			 0xe5d3138c, /// 0xb94
			 0x6d15514c, /// 0xb98
			 0x29a686b1, /// 0xb9c
			 0x16b11aad, /// 0xba0
			 0xdf04bbb2, /// 0xba4
			 0x8292ea84, /// 0xba8
			 0xd177aff7, /// 0xbac
			 0x34f7007d, /// 0xbb0
			 0xd9794454, /// 0xbb4
			 0x6b2dc881, /// 0xbb8
			 0x0ffc5b1e, /// 0xbbc
			 0xfee00e2e, /// 0xbc0
			 0xc83c035d, /// 0xbc4
			 0x3fd8ea92, /// 0xbc8
			 0xa702964f, /// 0xbcc
			 0x073d7c83, /// 0xbd0
			 0xc32522dc, /// 0xbd4
			 0x34ca816a, /// 0xbd8
			 0xcd88cffa, /// 0xbdc
			 0x06db4854, /// 0xbe0
			 0xfe72aeae, /// 0xbe4
			 0x36d7b42c, /// 0xbe8
			 0x2b0adaf4, /// 0xbec
			 0x13a9bf19, /// 0xbf0
			 0x68043c02, /// 0xbf4
			 0x42015c90, /// 0xbf8
			 0x688e1d1f, /// 0xbfc
			 0x027d5144, /// 0xc00
			 0x2940e224, /// 0xc04
			 0x4378f572, /// 0xc08
			 0x3d22851b, /// 0xc0c
			 0x5b3056a7, /// 0xc10
			 0x61a6e208, /// 0xc14
			 0xd52040c8, /// 0xc18
			 0xffcb33d7, /// 0xc1c
			 0x1b21f311, /// 0xc20
			 0x50464e07, /// 0xc24
			 0xfe6dd7c6, /// 0xc28
			 0x5fdf1c97, /// 0xc2c
			 0xa34d485f, /// 0xc30
			 0x424636bd, /// 0xc34
			 0x08fc40e4, /// 0xc38
			 0xeb08a328, /// 0xc3c
			 0x64fdf086, /// 0xc40
			 0xce067aa2, /// 0xc44
			 0xde1d0d19, /// 0xc48
			 0x3afd9f6d, /// 0xc4c
			 0x0ee7cb63, /// 0xc50
			 0x022a2a71, /// 0xc54
			 0x0db21bc5, /// 0xc58
			 0xb9f2a2e1, /// 0xc5c
			 0x9b6fa466, /// 0xc60
			 0xce291dcb, /// 0xc64
			 0x820543a3, /// 0xc68
			 0x34e917ed, /// 0xc6c
			 0x8fe8acd3, /// 0xc70
			 0xf1470bec, /// 0xc74
			 0x02ac0586, /// 0xc78
			 0x2d72feaf, /// 0xc7c
			 0xc1c24d23, /// 0xc80
			 0x52be7b79, /// 0xc84
			 0xc4123e44, /// 0xc88
			 0x04c0f2c3, /// 0xc8c
			 0xfc765c3a, /// 0xc90
			 0x59ca231d, /// 0xc94
			 0x77ef5e6f, /// 0xc98
			 0xa54110ec, /// 0xc9c
			 0x026e7d42, /// 0xca0
			 0x4ddc2400, /// 0xca4
			 0xb1a12c72, /// 0xca8
			 0x157abefe, /// 0xcac
			 0xb00b9aaa, /// 0xcb0
			 0xf4deebbe, /// 0xcb4
			 0x138bc9a7, /// 0xcb8
			 0x2a8ac8ba, /// 0xcbc
			 0x544a5535, /// 0xcc0
			 0xe90cdc5c, /// 0xcc4
			 0x80f16d2e, /// 0xcc8
			 0x81c3b5f8, /// 0xccc
			 0xe65e843e, /// 0xcd0
			 0x85ad4bde, /// 0xcd4
			 0x1463dc66, /// 0xcd8
			 0x619ae40f, /// 0xcdc
			 0x5ba30a34, /// 0xce0
			 0x24eb85e0, /// 0xce4
			 0x5e734f07, /// 0xce8
			 0xc8f1911e, /// 0xcec
			 0x068a2d09, /// 0xcf0
			 0x797780ea, /// 0xcf4
			 0x632deb19, /// 0xcf8
			 0xe472f03f, /// 0xcfc
			 0x8601790d, /// 0xd00
			 0x29fb0e7a, /// 0xd04
			 0xf8e99bec, /// 0xd08
			 0x734ffc47, /// 0xd0c
			 0x3f6f1d1e, /// 0xd10
			 0x94edd073, /// 0xd14
			 0xd9153f05, /// 0xd18
			 0x12c264d3, /// 0xd1c
			 0xb4ce80bc, /// 0xd20
			 0xa7c3f7a4, /// 0xd24
			 0x11b305a4, /// 0xd28
			 0xdc875e32, /// 0xd2c
			 0x03702ce8, /// 0xd30
			 0x590b8c0b, /// 0xd34
			 0x1d085a10, /// 0xd38
			 0x04fcddae, /// 0xd3c
			 0x57c74f7e, /// 0xd40
			 0x52bd78ba, /// 0xd44
			 0xb643d303, /// 0xd48
			 0xb55321a0, /// 0xd4c
			 0x0ba15054, /// 0xd50
			 0x360fbdce, /// 0xd54
			 0x67d9e859, /// 0xd58
			 0x9cfdda1e, /// 0xd5c
			 0xce61ac1c, /// 0xd60
			 0x811b2a8e, /// 0xd64
			 0x52f71605, /// 0xd68
			 0x09d23fac, /// 0xd6c
			 0xbe1f9b66, /// 0xd70
			 0x80cf7645, /// 0xd74
			 0xe0c72523, /// 0xd78
			 0x7d37502a, /// 0xd7c
			 0xceaab24f, /// 0xd80
			 0x9c26875d, /// 0xd84
			 0x85646ff6, /// 0xd88
			 0xa95bf81e, /// 0xd8c
			 0xeff556e5, /// 0xd90
			 0xd0f1c7af, /// 0xd94
			 0xc2b1b09b, /// 0xd98
			 0x8b3786e8, /// 0xd9c
			 0x9ae3323b, /// 0xda0
			 0x51168ee6, /// 0xda4
			 0x04ba93b5, /// 0xda8
			 0x4ecd7fae, /// 0xdac
			 0x914c5b38, /// 0xdb0
			 0x972cd27c, /// 0xdb4
			 0x49b8e62d, /// 0xdb8
			 0xd731d386, /// 0xdbc
			 0xa8a1ef3a, /// 0xdc0
			 0xe68c0fa0, /// 0xdc4
			 0x5b203034, /// 0xdc8
			 0xd8b3e246, /// 0xdcc
			 0x3839efbe, /// 0xdd0
			 0x66bf6e44, /// 0xdd4
			 0x47691d3e, /// 0xdd8
			 0xf3fe21dd, /// 0xddc
			 0x16f949b1, /// 0xde0
			 0x9b66b4bb, /// 0xde4
			 0x211f1780, /// 0xde8
			 0xb45e82e7, /// 0xdec
			 0xf4b9b3fa, /// 0xdf0
			 0x5c0e93ab, /// 0xdf4
			 0x0b9f2f54, /// 0xdf8
			 0x432f55e7, /// 0xdfc
			 0x1e5b161d, /// 0xe00
			 0xb5041eb8, /// 0xe04
			 0x960c4c60, /// 0xe08
			 0x157fef25, /// 0xe0c
			 0xbe05de47, /// 0xe10
			 0x6f27adec, /// 0xe14
			 0x4f981d6e, /// 0xe18
			 0x0f4024f2, /// 0xe1c
			 0x8f1ccdf7, /// 0xe20
			 0xc62c13b3, /// 0xe24
			 0xb8e525b5, /// 0xe28
			 0xbeb5724b, /// 0xe2c
			 0xf01cfab5, /// 0xe30
			 0xba2ec5ee, /// 0xe34
			 0x84a46bc7, /// 0xe38
			 0xc6883b4f, /// 0xe3c
			 0xd37dd7d3, /// 0xe40
			 0x20222097, /// 0xe44
			 0xee98850d, /// 0xe48
			 0x797d0cae, /// 0xe4c
			 0x361573c8, /// 0xe50
			 0x8ff2d2b3, /// 0xe54
			 0x9a25177a, /// 0xe58
			 0x2cd482e6, /// 0xe5c
			 0xfe736199, /// 0xe60
			 0x89568235, /// 0xe64
			 0xf43cd3b7, /// 0xe68
			 0xe2ca3ac9, /// 0xe6c
			 0xd3aeb181, /// 0xe70
			 0xf3a657d1, /// 0xe74
			 0xd28b5f09, /// 0xe78
			 0x3403e477, /// 0xe7c
			 0x4a5c5273, /// 0xe80
			 0x86070e51, /// 0xe84
			 0xe38a55ab, /// 0xe88
			 0xad4e28b3, /// 0xe8c
			 0x798a8823, /// 0xe90
			 0x0d254afc, /// 0xe94
			 0x150b2ca9, /// 0xe98
			 0x77370f0b, /// 0xe9c
			 0xbdccc9f4, /// 0xea0
			 0x8b43ed4f, /// 0xea4
			 0xc886f554, /// 0xea8
			 0x79751d3a, /// 0xeac
			 0x7861096d, /// 0xeb0
			 0x3b5ae73f, /// 0xeb4
			 0x19c71b22, /// 0xeb8
			 0xcfffd386, /// 0xebc
			 0x2d99e9b7, /// 0xec0
			 0x7b1c76f7, /// 0xec4
			 0x9581e555, /// 0xec8
			 0xb912718d, /// 0xecc
			 0xa062fadb, /// 0xed0
			 0x94be7dcd, /// 0xed4
			 0x9abce516, /// 0xed8
			 0xaf1532f8, /// 0xedc
			 0xb8f15122, /// 0xee0
			 0xfd2956b4, /// 0xee4
			 0xd2c6a8bb, /// 0xee8
			 0x78bd03bb, /// 0xeec
			 0x4fbe787b, /// 0xef0
			 0xff992986, /// 0xef4
			 0x2befbdb6, /// 0xef8
			 0x7277aa89, /// 0xefc
			 0x64b1f441, /// 0xf00
			 0x124bf8fe, /// 0xf04
			 0xac2b58a6, /// 0xf08
			 0x3dced0a5, /// 0xf0c
			 0xb79cb4d3, /// 0xf10
			 0xb73494e6, /// 0xf14
			 0x55724da6, /// 0xf18
			 0xa3612848, /// 0xf1c
			 0xaf4ebf14, /// 0xf20
			 0xe6a39c35, /// 0xf24
			 0x95e0c9de, /// 0xf28
			 0xe42817ec, /// 0xf2c
			 0xf4ce07a5, /// 0xf30
			 0x1f3ff6db, /// 0xf34
			 0x351f4d29, /// 0xf38
			 0x5e207030, /// 0xf3c
			 0xcddcaf5e, /// 0xf40
			 0x1f6c4e43, /// 0xf44
			 0x3280d7f6, /// 0xf48
			 0xf416a3dd, /// 0xf4c
			 0x7b79c1a9, /// 0xf50
			 0x9216366d, /// 0xf54
			 0xfce9ff51, /// 0xf58
			 0x8d3febd6, /// 0xf5c
			 0xe56a5d9d, /// 0xf60
			 0x15d42785, /// 0xf64
			 0x5db3bf3d, /// 0xf68
			 0x4fd956e2, /// 0xf6c
			 0xf34fefb1, /// 0xf70
			 0xab16cc8a, /// 0xf74
			 0x3322db18, /// 0xf78
			 0x57f37709, /// 0xf7c
			 0xd83c4e97, /// 0xf80
			 0x00812dca, /// 0xf84
			 0x88edee17, /// 0xf88
			 0x9b5f808d, /// 0xf8c
			 0x0e5bdab6, /// 0xf90
			 0xf7c97ba4, /// 0xf94
			 0xd52b965d, /// 0xf98
			 0xa4215795, /// 0xf9c
			 0x85d7e8d7, /// 0xfa0
			 0x36ed51a1, /// 0xfa4
			 0xe0b74661, /// 0xfa8
			 0xa7a82b12, /// 0xfac
			 0x48a69d89, /// 0xfb0
			 0x55a492e1, /// 0xfb4
			 0x673ceb55, /// 0xfb8
			 0xb1a532a1, /// 0xfbc
			 0x8ba1b194, /// 0xfc0
			 0x0ae0aecb, /// 0xfc4
			 0x39d33185, /// 0xfc8
			 0x19084822, /// 0xfcc
			 0x7bbcfbab, /// 0xfd0
			 0x6c303a6d, /// 0xfd4
			 0x2cafa0a4, /// 0xfd8
			 0x09814909, /// 0xfdc
			 0x6332aade, /// 0xfe0
			 0xe34acf7c, /// 0xfe4
			 0x46d6b18b, /// 0xfe8
			 0x999aa870, /// 0xfec
			 0x5fc07e15, /// 0xff0
			 0x8a066347, /// 0xff4
			 0x121e21ea, /// 0xff8
			 0xee14ae16 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xfee7900b, /// 0x0
			 0x333df021, /// 0x4
			 0x51728d41, /// 0x8
			 0x4bcfa9ab, /// 0xc
			 0x36645565, /// 0x10
			 0x65980e23, /// 0x14
			 0xd9c3910a, /// 0x18
			 0xf5ab5729, /// 0x1c
			 0x83760ff3, /// 0x20
			 0x72d9433d, /// 0x24
			 0xd64cc3b5, /// 0x28
			 0xc779938f, /// 0x2c
			 0x1337272c, /// 0x30
			 0xbbdb4a9b, /// 0x34
			 0x017adc16, /// 0x38
			 0xbd6673b4, /// 0x3c
			 0x3996f00a, /// 0x40
			 0x1d476281, /// 0x44
			 0x11965fbe, /// 0x48
			 0x69e69146, /// 0x4c
			 0x98f9117f, /// 0x50
			 0x6fecc2f9, /// 0x54
			 0xc4a70941, /// 0x58
			 0x32cf1486, /// 0x5c
			 0x92718117, /// 0x60
			 0x0ae01d41, /// 0x64
			 0x389c1d1d, /// 0x68
			 0xf062253d, /// 0x6c
			 0x5afb08d9, /// 0x70
			 0xb08b7b0b, /// 0x74
			 0xb25a90d5, /// 0x78
			 0x614ff4e5, /// 0x7c
			 0xc38f70e7, /// 0x80
			 0xbcf3396c, /// 0x84
			 0xb49fc3a2, /// 0x88
			 0x27032e1e, /// 0x8c
			 0xb1093ecc, /// 0x90
			 0x572f0027, /// 0x94
			 0x6bd7b5de, /// 0x98
			 0x59b34623, /// 0x9c
			 0x10d95955, /// 0xa0
			 0xaf2aa530, /// 0xa4
			 0xd9592168, /// 0xa8
			 0x45d60fc6, /// 0xac
			 0xf83c69d4, /// 0xb0
			 0x4d004109, /// 0xb4
			 0xa5568b71, /// 0xb8
			 0x4930fe54, /// 0xbc
			 0xc9e2ff5a, /// 0xc0
			 0xb0471ae2, /// 0xc4
			 0xbcbae608, /// 0xc8
			 0x69be667a, /// 0xcc
			 0x68eeb116, /// 0xd0
			 0x177a9549, /// 0xd4
			 0x0f6ffa2c, /// 0xd8
			 0x00c071ae, /// 0xdc
			 0xeeb91db6, /// 0xe0
			 0xf06dff57, /// 0xe4
			 0x60dc4457, /// 0xe8
			 0x81f6ad8d, /// 0xec
			 0x17c6e3ac, /// 0xf0
			 0x08ffb58c, /// 0xf4
			 0x5d0a0e4e, /// 0xf8
			 0x3f2de181, /// 0xfc
			 0x681067de, /// 0x100
			 0x8b180d8b, /// 0x104
			 0x0f83e3d1, /// 0x108
			 0x87b4c794, /// 0x10c
			 0x220b7104, /// 0x110
			 0x891842ff, /// 0x114
			 0xdf4ae58a, /// 0x118
			 0x0df6a039, /// 0x11c
			 0x1e2d3c4d, /// 0x120
			 0x83e80f1a, /// 0x124
			 0xd9347bb5, /// 0x128
			 0x4a86be44, /// 0x12c
			 0x061c267f, /// 0x130
			 0x0ec40ed5, /// 0x134
			 0xff71712e, /// 0x138
			 0x758abcbe, /// 0x13c
			 0xd5a75910, /// 0x140
			 0x44928050, /// 0x144
			 0x430d7ba6, /// 0x148
			 0xb19de102, /// 0x14c
			 0xfc7d1c1e, /// 0x150
			 0x039db39e, /// 0x154
			 0x46d389e0, /// 0x158
			 0x5823e3eb, /// 0x15c
			 0xfd6b306c, /// 0x160
			 0xfad5c5c1, /// 0x164
			 0x7187277a, /// 0x168
			 0x3d30eae9, /// 0x16c
			 0xf8e83630, /// 0x170
			 0xf75b164d, /// 0x174
			 0xe5b06ae6, /// 0x178
			 0x515fc3f5, /// 0x17c
			 0xa3a0b46a, /// 0x180
			 0x6f1846a7, /// 0x184
			 0x299c7540, /// 0x188
			 0x24972694, /// 0x18c
			 0x39490248, /// 0x190
			 0xff5fb0aa, /// 0x194
			 0x65958535, /// 0x198
			 0x593b8841, /// 0x19c
			 0xb0c3fdeb, /// 0x1a0
			 0xae726b76, /// 0x1a4
			 0xb79b051a, /// 0x1a8
			 0xc2f5dc4f, /// 0x1ac
			 0xa054259e, /// 0x1b0
			 0x681e297c, /// 0x1b4
			 0x927e5beb, /// 0x1b8
			 0x3920d1ae, /// 0x1bc
			 0x6e1aaaae, /// 0x1c0
			 0xb8858aec, /// 0x1c4
			 0xd02dc88d, /// 0x1c8
			 0xad6375a6, /// 0x1cc
			 0x3d13da90, /// 0x1d0
			 0x37f756a0, /// 0x1d4
			 0x062c0945, /// 0x1d8
			 0x7e7d4685, /// 0x1dc
			 0x0092f1aa, /// 0x1e0
			 0xb8abb2e1, /// 0x1e4
			 0x840c2967, /// 0x1e8
			 0xa881ea75, /// 0x1ec
			 0x19cf11db, /// 0x1f0
			 0x350456cf, /// 0x1f4
			 0xdae0df7d, /// 0x1f8
			 0x2fb6f136, /// 0x1fc
			 0x959ced87, /// 0x200
			 0x0fa41cf8, /// 0x204
			 0xf471f8fe, /// 0x208
			 0x7e840711, /// 0x20c
			 0xde4c9f10, /// 0x210
			 0x4cdef1d7, /// 0x214
			 0xb9a4d537, /// 0x218
			 0xa9f7edff, /// 0x21c
			 0x8ccc43b7, /// 0x220
			 0x14612654, /// 0x224
			 0xf62594fb, /// 0x228
			 0x48d51faa, /// 0x22c
			 0x0238470a, /// 0x230
			 0x75936467, /// 0x234
			 0x178d277c, /// 0x238
			 0xca80cd87, /// 0x23c
			 0xcf717ed5, /// 0x240
			 0xa49d0233, /// 0x244
			 0x3f321d0c, /// 0x248
			 0x09aac504, /// 0x24c
			 0xf74dee36, /// 0x250
			 0x664d2137, /// 0x254
			 0x30827c97, /// 0x258
			 0xb29a8c09, /// 0x25c
			 0xad6b13e5, /// 0x260
			 0xc8b5d3a6, /// 0x264
			 0x69416971, /// 0x268
			 0x015c9b49, /// 0x26c
			 0x3a30e82b, /// 0x270
			 0x98fc71a3, /// 0x274
			 0xbe2e2aff, /// 0x278
			 0xb2ba1205, /// 0x27c
			 0x37dc00cd, /// 0x280
			 0x5905cf99, /// 0x284
			 0x6f5f5297, /// 0x288
			 0xaa40de4f, /// 0x28c
			 0x9c458f13, /// 0x290
			 0xd238ec3e, /// 0x294
			 0x30cf353d, /// 0x298
			 0xd1499cce, /// 0x29c
			 0x900d4b88, /// 0x2a0
			 0x9f6bd9f9, /// 0x2a4
			 0xe5dd8abc, /// 0x2a8
			 0x0181e80d, /// 0x2ac
			 0x803e8b43, /// 0x2b0
			 0xc7f9fdf2, /// 0x2b4
			 0x1aec48db, /// 0x2b8
			 0xf47fa6e8, /// 0x2bc
			 0xbeadd627, /// 0x2c0
			 0xb0a0bf1f, /// 0x2c4
			 0x3109d1f9, /// 0x2c8
			 0x3749a3af, /// 0x2cc
			 0xced4e12b, /// 0x2d0
			 0x5ead25f1, /// 0x2d4
			 0xa371cfac, /// 0x2d8
			 0xe032d8e8, /// 0x2dc
			 0xff117067, /// 0x2e0
			 0x97ec7acc, /// 0x2e4
			 0x83c832e5, /// 0x2e8
			 0x66ef2f0e, /// 0x2ec
			 0xbea2baff, /// 0x2f0
			 0xf1a8582b, /// 0x2f4
			 0xda62e808, /// 0x2f8
			 0x0fb0c32e, /// 0x2fc
			 0x8c16371b, /// 0x300
			 0xd319359d, /// 0x304
			 0x3d5a040d, /// 0x308
			 0x13cf458e, /// 0x30c
			 0x54f5c761, /// 0x310
			 0x2001bca5, /// 0x314
			 0xcfe28668, /// 0x318
			 0xc0f55e6a, /// 0x31c
			 0x865de9b3, /// 0x320
			 0x3a7c7b05, /// 0x324
			 0x476d1bf8, /// 0x328
			 0x719d3ee6, /// 0x32c
			 0xc83f4455, /// 0x330
			 0x3b28e56d, /// 0x334
			 0x4964a6c1, /// 0x338
			 0xc0f2f206, /// 0x33c
			 0xa5ffb439, /// 0x340
			 0x0c807548, /// 0x344
			 0xe0cb0f21, /// 0x348
			 0xda79137b, /// 0x34c
			 0x3788ff8d, /// 0x350
			 0xabf059c9, /// 0x354
			 0xdc71e9e6, /// 0x358
			 0x6696572e, /// 0x35c
			 0xfb4c858d, /// 0x360
			 0x778baa7c, /// 0x364
			 0x482d1959, /// 0x368
			 0x30852f01, /// 0x36c
			 0xb595f358, /// 0x370
			 0xa4af280b, /// 0x374
			 0x2e6d7422, /// 0x378
			 0xf23a4add, /// 0x37c
			 0x22e6eece, /// 0x380
			 0x6506ecb2, /// 0x384
			 0x75892662, /// 0x388
			 0x6987890f, /// 0x38c
			 0x011b328d, /// 0x390
			 0x8026bb97, /// 0x394
			 0x54822ebf, /// 0x398
			 0x9557b3b4, /// 0x39c
			 0xfc0ab54c, /// 0x3a0
			 0x48dcb36d, /// 0x3a4
			 0x9babb3bf, /// 0x3a8
			 0x20e02ed8, /// 0x3ac
			 0xfe30c586, /// 0x3b0
			 0x176d8c7e, /// 0x3b4
			 0xe0a968e2, /// 0x3b8
			 0x5b4a50f1, /// 0x3bc
			 0xa8fb28a9, /// 0x3c0
			 0x1688b9de, /// 0x3c4
			 0x97112993, /// 0x3c8
			 0x9b3fe067, /// 0x3cc
			 0xfdf0d9ec, /// 0x3d0
			 0xff88a478, /// 0x3d4
			 0xcfd1d37f, /// 0x3d8
			 0x9db9e5ad, /// 0x3dc
			 0x5b3f16ac, /// 0x3e0
			 0x4c72f2c5, /// 0x3e4
			 0xdc0e9420, /// 0x3e8
			 0xd748dcd3, /// 0x3ec
			 0xcad1a0a0, /// 0x3f0
			 0xf93e19db, /// 0x3f4
			 0xe1b34828, /// 0x3f8
			 0x262ac4f1, /// 0x3fc
			 0x2edc5a70, /// 0x400
			 0xcc376b38, /// 0x404
			 0x6874d83f, /// 0x408
			 0x6713fe39, /// 0x40c
			 0xd6a39b7e, /// 0x410
			 0xf20267c9, /// 0x414
			 0x64e94522, /// 0x418
			 0xb62c298c, /// 0x41c
			 0xd73e780f, /// 0x420
			 0xc9c9a901, /// 0x424
			 0x9e5cd62b, /// 0x428
			 0x8d3d0ce7, /// 0x42c
			 0x35dd9127, /// 0x430
			 0xbe3ae5c7, /// 0x434
			 0x6bc8fd36, /// 0x438
			 0xff9fe4cd, /// 0x43c
			 0x3015c166, /// 0x440
			 0x7e7de772, /// 0x444
			 0x03c86733, /// 0x448
			 0x3f040c6a, /// 0x44c
			 0xc03b3f14, /// 0x450
			 0x72f01315, /// 0x454
			 0xa3666241, /// 0x458
			 0xf9a8ebf4, /// 0x45c
			 0xf52b8203, /// 0x460
			 0xcd279be7, /// 0x464
			 0x652e573d, /// 0x468
			 0xc236a5f1, /// 0x46c
			 0x5412ed15, /// 0x470
			 0x2e033e02, /// 0x474
			 0x4b0d978e, /// 0x478
			 0x19efac22, /// 0x47c
			 0x67ccbe5c, /// 0x480
			 0x3c1248a5, /// 0x484
			 0x8a2ea09b, /// 0x488
			 0xe3dae209, /// 0x48c
			 0x7142563c, /// 0x490
			 0x9b4de441, /// 0x494
			 0x1fd7d071, /// 0x498
			 0xa5326581, /// 0x49c
			 0x3cd2ff4c, /// 0x4a0
			 0xc3ef13a4, /// 0x4a4
			 0x9645a86c, /// 0x4a8
			 0xe044acfc, /// 0x4ac
			 0x6907e183, /// 0x4b0
			 0xf14f8477, /// 0x4b4
			 0x08061c73, /// 0x4b8
			 0x83ba4f72, /// 0x4bc
			 0xb3b4aae4, /// 0x4c0
			 0xab281e5d, /// 0x4c4
			 0x4ffbc2fc, /// 0x4c8
			 0x8f04d455, /// 0x4cc
			 0x15259729, /// 0x4d0
			 0x969a431a, /// 0x4d4
			 0xaf87ce58, /// 0x4d8
			 0x071f3e48, /// 0x4dc
			 0x6fb3d936, /// 0x4e0
			 0xf5f66f2a, /// 0x4e4
			 0xdb3c9175, /// 0x4e8
			 0x7b336e72, /// 0x4ec
			 0xa55db8e7, /// 0x4f0
			 0x961fbb17, /// 0x4f4
			 0xce2156af, /// 0x4f8
			 0x5f655969, /// 0x4fc
			 0xaa8662e2, /// 0x500
			 0x801fc4f5, /// 0x504
			 0x0e2277a0, /// 0x508
			 0x11dad79c, /// 0x50c
			 0x9a307e65, /// 0x510
			 0xdea33828, /// 0x514
			 0x2688492e, /// 0x518
			 0x9ae9bcf2, /// 0x51c
			 0x71c18179, /// 0x520
			 0xdc93a4ce, /// 0x524
			 0xf3ae8c32, /// 0x528
			 0x7a705e03, /// 0x52c
			 0x1c9779df, /// 0x530
			 0x3d8a87ac, /// 0x534
			 0xe68be509, /// 0x538
			 0x68167856, /// 0x53c
			 0xe3df2cd7, /// 0x540
			 0x2c8fe509, /// 0x544
			 0x66fe4ede, /// 0x548
			 0xb6bbcd85, /// 0x54c
			 0x0c539372, /// 0x550
			 0x4b3e8c80, /// 0x554
			 0x62d8bf4b, /// 0x558
			 0x2e2790dc, /// 0x55c
			 0x39147fbf, /// 0x560
			 0x225b220b, /// 0x564
			 0xd2d8680e, /// 0x568
			 0xeb5dbd13, /// 0x56c
			 0x57acab71, /// 0x570
			 0xd7a0eafc, /// 0x574
			 0xbb4e37e5, /// 0x578
			 0x4c51fba6, /// 0x57c
			 0x4c4567f7, /// 0x580
			 0xe4958113, /// 0x584
			 0x3fa52bfa, /// 0x588
			 0xe8df7e24, /// 0x58c
			 0x6cc2b18a, /// 0x590
			 0xf10e7f15, /// 0x594
			 0xe4e6caab, /// 0x598
			 0xde0344c7, /// 0x59c
			 0x07bc3fd1, /// 0x5a0
			 0x1f6acb1d, /// 0x5a4
			 0x060545e3, /// 0x5a8
			 0xf32fc5f5, /// 0x5ac
			 0xf9db1888, /// 0x5b0
			 0xe74055aa, /// 0x5b4
			 0xfc5e56db, /// 0x5b8
			 0x3f23b533, /// 0x5bc
			 0xfd3376fe, /// 0x5c0
			 0xc846b3a1, /// 0x5c4
			 0x50c0133a, /// 0x5c8
			 0x0abd7d1f, /// 0x5cc
			 0x5aed0e15, /// 0x5d0
			 0x937ddf59, /// 0x5d4
			 0x40879167, /// 0x5d8
			 0x69871ac5, /// 0x5dc
			 0x0cde2462, /// 0x5e0
			 0xe76bf4b4, /// 0x5e4
			 0x56ed4c8b, /// 0x5e8
			 0xbb431f66, /// 0x5ec
			 0x38210131, /// 0x5f0
			 0x5aaf5915, /// 0x5f4
			 0x24ae7442, /// 0x5f8
			 0x2293fd21, /// 0x5fc
			 0xe1532aed, /// 0x600
			 0x0755c096, /// 0x604
			 0xd0e4f4e7, /// 0x608
			 0x72f215a4, /// 0x60c
			 0x875c242d, /// 0x610
			 0x35b8ca61, /// 0x614
			 0xfc6a0c8f, /// 0x618
			 0x1e343c40, /// 0x61c
			 0xeaa8c114, /// 0x620
			 0x9db01e3b, /// 0x624
			 0x53d4f335, /// 0x628
			 0xf569a7c2, /// 0x62c
			 0xc2e60db0, /// 0x630
			 0xb789fd0b, /// 0x634
			 0xf197c7f9, /// 0x638
			 0xbad46988, /// 0x63c
			 0x59f0ce8b, /// 0x640
			 0x3a35378a, /// 0x644
			 0x6f1d1384, /// 0x648
			 0x78817bc0, /// 0x64c
			 0xe718209d, /// 0x650
			 0xaa4381ca, /// 0x654
			 0xe2d925ec, /// 0x658
			 0x937d697d, /// 0x65c
			 0x7eb15820, /// 0x660
			 0x20704a74, /// 0x664
			 0x41e4536b, /// 0x668
			 0x915b25dc, /// 0x66c
			 0xfa4088cb, /// 0x670
			 0xbeecf9a5, /// 0x674
			 0x135f9a8b, /// 0x678
			 0xa40ca185, /// 0x67c
			 0x57a51504, /// 0x680
			 0xc39d5684, /// 0x684
			 0xe91760e7, /// 0x688
			 0xa8f7ca87, /// 0x68c
			 0x335575ce, /// 0x690
			 0x6e010ad7, /// 0x694
			 0x3316636d, /// 0x698
			 0x85676fa1, /// 0x69c
			 0x712f410f, /// 0x6a0
			 0x96eb3e3a, /// 0x6a4
			 0x3d6ac1ae, /// 0x6a8
			 0x8be4c6e9, /// 0x6ac
			 0xcad02d4f, /// 0x6b0
			 0x67651def, /// 0x6b4
			 0x6ee4b74d, /// 0x6b8
			 0x9b51e5fb, /// 0x6bc
			 0xc50f7eaf, /// 0x6c0
			 0xcd3d0ddb, /// 0x6c4
			 0x75d177b8, /// 0x6c8
			 0x8312e743, /// 0x6cc
			 0xab9f5fbc, /// 0x6d0
			 0x11a6d90c, /// 0x6d4
			 0xf66e9d91, /// 0x6d8
			 0xc8cdf8e7, /// 0x6dc
			 0x5ca49b3c, /// 0x6e0
			 0xd3eb4212, /// 0x6e4
			 0x7f0b0490, /// 0x6e8
			 0xeee05570, /// 0x6ec
			 0x93f22be5, /// 0x6f0
			 0x544e2e3f, /// 0x6f4
			 0x0f41c74d, /// 0x6f8
			 0xeb782a0c, /// 0x6fc
			 0x3088f5f2, /// 0x700
			 0xb3c86dcd, /// 0x704
			 0xc842774b, /// 0x708
			 0xc1850ada, /// 0x70c
			 0x3394c21c, /// 0x710
			 0xd2f36ea3, /// 0x714
			 0xcc29a349, /// 0x718
			 0x033c4644, /// 0x71c
			 0xc6c5cfd5, /// 0x720
			 0x4a0292c9, /// 0x724
			 0x1f726d19, /// 0x728
			 0xf0b5baa9, /// 0x72c
			 0x9509eb17, /// 0x730
			 0x54b04ffc, /// 0x734
			 0xc83fe8a6, /// 0x738
			 0x3eaee168, /// 0x73c
			 0x312d16e2, /// 0x740
			 0x8599c509, /// 0x744
			 0x71c1d12c, /// 0x748
			 0x611b72a5, /// 0x74c
			 0xccfe0df6, /// 0x750
			 0xa7cbe6e7, /// 0x754
			 0xe9001d6b, /// 0x758
			 0x638a81f8, /// 0x75c
			 0x242311ca, /// 0x760
			 0xbae228b9, /// 0x764
			 0x410c7c0c, /// 0x768
			 0x372b5cf0, /// 0x76c
			 0x86e9343a, /// 0x770
			 0x3fa82388, /// 0x774
			 0x1d57dbd7, /// 0x778
			 0x75cb2b8f, /// 0x77c
			 0xfde0f734, /// 0x780
			 0xa0bd3d01, /// 0x784
			 0x08976743, /// 0x788
			 0xc3948fc9, /// 0x78c
			 0x2cf469e0, /// 0x790
			 0x708f1e6b, /// 0x794
			 0x710ce9d8, /// 0x798
			 0x26600a17, /// 0x79c
			 0xb3c70186, /// 0x7a0
			 0xdbca9c0d, /// 0x7a4
			 0x2a5ecf01, /// 0x7a8
			 0x61c4df0e, /// 0x7ac
			 0x93ade799, /// 0x7b0
			 0x454e0b9a, /// 0x7b4
			 0x4e4c62c3, /// 0x7b8
			 0x451b4180, /// 0x7bc
			 0xfaa7e442, /// 0x7c0
			 0xc9c42ea4, /// 0x7c4
			 0xfca6a219, /// 0x7c8
			 0x25bc5418, /// 0x7cc
			 0xba9ecd41, /// 0x7d0
			 0x328a587c, /// 0x7d4
			 0x797a0da2, /// 0x7d8
			 0xd7c4530b, /// 0x7dc
			 0x9687a681, /// 0x7e0
			 0x5c8c8094, /// 0x7e4
			 0x57dfd554, /// 0x7e8
			 0x24e31036, /// 0x7ec
			 0x2ca58fa2, /// 0x7f0
			 0xe2b11b2d, /// 0x7f4
			 0xebb537fb, /// 0x7f8
			 0xb8afee34, /// 0x7fc
			 0x40a39572, /// 0x800
			 0x3ba770c1, /// 0x804
			 0x17871889, /// 0x808
			 0x0dd4ab5a, /// 0x80c
			 0x77f40619, /// 0x810
			 0x4f427e2f, /// 0x814
			 0xf9a671a2, /// 0x818
			 0xb0d94896, /// 0x81c
			 0x19d664cf, /// 0x820
			 0xfc6ff0b4, /// 0x824
			 0x901ba9f6, /// 0x828
			 0x4fb1370f, /// 0x82c
			 0x1890b29e, /// 0x830
			 0xc9eac1b7, /// 0x834
			 0xedf36520, /// 0x838
			 0x15315727, /// 0x83c
			 0xfce271c6, /// 0x840
			 0x0837b32d, /// 0x844
			 0xebca7e4c, /// 0x848
			 0xc37b717b, /// 0x84c
			 0xdcf052ff, /// 0x850
			 0x355d13d3, /// 0x854
			 0xb30c3391, /// 0x858
			 0xd907d8a9, /// 0x85c
			 0xb708da7e, /// 0x860
			 0x7d55a27c, /// 0x864
			 0xbe02177c, /// 0x868
			 0xe5d4df43, /// 0x86c
			 0xcdd41df5, /// 0x870
			 0x0d014e93, /// 0x874
			 0xbcbbe4d9, /// 0x878
			 0x4776f042, /// 0x87c
			 0xd59b0553, /// 0x880
			 0x84a26d12, /// 0x884
			 0x5e54795b, /// 0x888
			 0xb85664f1, /// 0x88c
			 0xe1d782ce, /// 0x890
			 0xe4472b4e, /// 0x894
			 0x514eb4c5, /// 0x898
			 0x3aaec438, /// 0x89c
			 0x1dea5661, /// 0x8a0
			 0xb6d2ffb2, /// 0x8a4
			 0x5f0b6338, /// 0x8a8
			 0x16bf92a5, /// 0x8ac
			 0xc5b8e38b, /// 0x8b0
			 0x9b2bbb38, /// 0x8b4
			 0x6951562c, /// 0x8b8
			 0xbc53c3a8, /// 0x8bc
			 0x7234b449, /// 0x8c0
			 0x4e9852f2, /// 0x8c4
			 0x978a7586, /// 0x8c8
			 0x270ea637, /// 0x8cc
			 0x1f5a24be, /// 0x8d0
			 0x75e076c4, /// 0x8d4
			 0x4198a5e8, /// 0x8d8
			 0xeacfb137, /// 0x8dc
			 0x83156deb, /// 0x8e0
			 0x55fd308c, /// 0x8e4
			 0x310ad7e9, /// 0x8e8
			 0x951df733, /// 0x8ec
			 0xed4aa629, /// 0x8f0
			 0x5d1b462e, /// 0x8f4
			 0x2ad7347f, /// 0x8f8
			 0xa0300c32, /// 0x8fc
			 0x4e73d83b, /// 0x900
			 0xdb7d8182, /// 0x904
			 0x7371c039, /// 0x908
			 0x1f44b6e4, /// 0x90c
			 0x68fc20d9, /// 0x910
			 0x76f0fec0, /// 0x914
			 0x6737e68f, /// 0x918
			 0xa837a496, /// 0x91c
			 0x23e6fe79, /// 0x920
			 0x02e70ecd, /// 0x924
			 0x612af5e3, /// 0x928
			 0x01707cf8, /// 0x92c
			 0xf24460b0, /// 0x930
			 0xfb28daa8, /// 0x934
			 0xb44286d0, /// 0x938
			 0x4ee257a4, /// 0x93c
			 0x1742a65f, /// 0x940
			 0xaa221611, /// 0x944
			 0xbb079fc8, /// 0x948
			 0xed47269b, /// 0x94c
			 0xa314cd54, /// 0x950
			 0x599c5d66, /// 0x954
			 0x16cc5e22, /// 0x958
			 0x5990e327, /// 0x95c
			 0xd7511277, /// 0x960
			 0x67f4af4a, /// 0x964
			 0x1ff1e28c, /// 0x968
			 0x9afba7a1, /// 0x96c
			 0x590db06a, /// 0x970
			 0x553d74c9, /// 0x974
			 0xeef20599, /// 0x978
			 0x17975ad1, /// 0x97c
			 0x435954b5, /// 0x980
			 0xc085a7f7, /// 0x984
			 0xb8050f1a, /// 0x988
			 0x6fd257ed, /// 0x98c
			 0xbe6424df, /// 0x990
			 0xb79e3f56, /// 0x994
			 0x99a68c75, /// 0x998
			 0x3a9ebebe, /// 0x99c
			 0xd90b635b, /// 0x9a0
			 0x4c46e604, /// 0x9a4
			 0x9495e9fa, /// 0x9a8
			 0x2933f473, /// 0x9ac
			 0xa1e59f58, /// 0x9b0
			 0x4ac2935a, /// 0x9b4
			 0x23d057c4, /// 0x9b8
			 0x4053760f, /// 0x9bc
			 0x6de95bab, /// 0x9c0
			 0x2ddcd4d1, /// 0x9c4
			 0xa125a0ab, /// 0x9c8
			 0x0b25bf0d, /// 0x9cc
			 0x78f7ec7c, /// 0x9d0
			 0x332b3dda, /// 0x9d4
			 0x9f1137f5, /// 0x9d8
			 0xf8d4fc5c, /// 0x9dc
			 0x0db7985d, /// 0x9e0
			 0xc5cc8c36, /// 0x9e4
			 0x92c5d724, /// 0x9e8
			 0x10f65394, /// 0x9ec
			 0x58d6300d, /// 0x9f0
			 0x35ebe033, /// 0x9f4
			 0xeb938143, /// 0x9f8
			 0xc72628f5, /// 0x9fc
			 0x302dba7c, /// 0xa00
			 0x65b9b2a2, /// 0xa04
			 0x3ba95a31, /// 0xa08
			 0xec01e340, /// 0xa0c
			 0x3405b043, /// 0xa10
			 0xa1d8feca, /// 0xa14
			 0xd2d58217, /// 0xa18
			 0xdcc8d7dd, /// 0xa1c
			 0x7ce71fc1, /// 0xa20
			 0x9b9ce547, /// 0xa24
			 0x26ca080d, /// 0xa28
			 0x468fdfe1, /// 0xa2c
			 0x502bcd7c, /// 0xa30
			 0x87221c16, /// 0xa34
			 0xd112cd8f, /// 0xa38
			 0xb05e84e4, /// 0xa3c
			 0x2a495733, /// 0xa40
			 0x5dee3c6b, /// 0xa44
			 0x18c7198e, /// 0xa48
			 0x74b16cbc, /// 0xa4c
			 0x815c9020, /// 0xa50
			 0x4658a2c0, /// 0xa54
			 0xa4119427, /// 0xa58
			 0xb7bcb3db, /// 0xa5c
			 0x26089ab0, /// 0xa60
			 0x3c0ea261, /// 0xa64
			 0x622e7540, /// 0xa68
			 0x40ef8b7b, /// 0xa6c
			 0x2605f47b, /// 0xa70
			 0xe297616c, /// 0xa74
			 0xa8e73ffd, /// 0xa78
			 0x9b4f2838, /// 0xa7c
			 0xea0fdd16, /// 0xa80
			 0xa5fa29df, /// 0xa84
			 0x4d344828, /// 0xa88
			 0xd31bed93, /// 0xa8c
			 0x06e36abf, /// 0xa90
			 0xb4f39b0c, /// 0xa94
			 0x67608e4b, /// 0xa98
			 0x8901b810, /// 0xa9c
			 0x4be00fd9, /// 0xaa0
			 0x1ffa5ac9, /// 0xaa4
			 0xe8620377, /// 0xaa8
			 0xeaa5fae4, /// 0xaac
			 0xf0ffaf24, /// 0xab0
			 0x9ccffc7b, /// 0xab4
			 0x8e0f60a4, /// 0xab8
			 0x4035de05, /// 0xabc
			 0x449a73fe, /// 0xac0
			 0xc742854a, /// 0xac4
			 0xc9186e79, /// 0xac8
			 0xfce20cde, /// 0xacc
			 0xcf3f712e, /// 0xad0
			 0x4edeb4d7, /// 0xad4
			 0x572457ad, /// 0xad8
			 0x51eb4828, /// 0xadc
			 0x23d15724, /// 0xae0
			 0x7cf67515, /// 0xae4
			 0x145069c8, /// 0xae8
			 0x8a202a49, /// 0xaec
			 0x44c1586c, /// 0xaf0
			 0xaf230c62, /// 0xaf4
			 0x3defb3cb, /// 0xaf8
			 0x52fc7ae4, /// 0xafc
			 0xcfcec708, /// 0xb00
			 0xcec3c036, /// 0xb04
			 0xf3027a5e, /// 0xb08
			 0xec81b952, /// 0xb0c
			 0x2fe2b963, /// 0xb10
			 0x1053cd9f, /// 0xb14
			 0xb1e9d1ce, /// 0xb18
			 0x0fee2759, /// 0xb1c
			 0xad1a261b, /// 0xb20
			 0xf5ca3fef, /// 0xb24
			 0xa80ffdcc, /// 0xb28
			 0x1d1d6990, /// 0xb2c
			 0xd7b3c7d9, /// 0xb30
			 0xc69b8dd9, /// 0xb34
			 0x4a601b75, /// 0xb38
			 0x0d9538cd, /// 0xb3c
			 0x7eb8dad6, /// 0xb40
			 0x338ec212, /// 0xb44
			 0x9372f3e4, /// 0xb48
			 0x8f672937, /// 0xb4c
			 0x68d1f34f, /// 0xb50
			 0xa11c97e5, /// 0xb54
			 0xdc77df7a, /// 0xb58
			 0x1c44ccae, /// 0xb5c
			 0x6bf04e73, /// 0xb60
			 0x77a1e304, /// 0xb64
			 0x35705dd3, /// 0xb68
			 0x149a46e3, /// 0xb6c
			 0x4e98488c, /// 0xb70
			 0x84aba9ef, /// 0xb74
			 0x43d41888, /// 0xb78
			 0x5e66b890, /// 0xb7c
			 0x31abb8f2, /// 0xb80
			 0x76d4a22c, /// 0xb84
			 0xd35a3ffe, /// 0xb88
			 0x5f676011, /// 0xb8c
			 0x096fd3f7, /// 0xb90
			 0xf6924899, /// 0xb94
			 0x8b4ce084, /// 0xb98
			 0xe0480b4f, /// 0xb9c
			 0xf689ea1e, /// 0xba0
			 0xb7d1cd5a, /// 0xba4
			 0x272bb165, /// 0xba8
			 0x234c58c3, /// 0xbac
			 0x23aa9088, /// 0xbb0
			 0xefe93267, /// 0xbb4
			 0x05d73ae1, /// 0xbb8
			 0x995e0d4b, /// 0xbbc
			 0xbd598333, /// 0xbc0
			 0x2f833805, /// 0xbc4
			 0xb52442d4, /// 0xbc8
			 0xdae594af, /// 0xbcc
			 0x01f4f77d, /// 0xbd0
			 0x7b1f7ba2, /// 0xbd4
			 0x697e59de, /// 0xbd8
			 0x63222f35, /// 0xbdc
			 0xf5293c57, /// 0xbe0
			 0x57d2539c, /// 0xbe4
			 0x69f23de0, /// 0xbe8
			 0xc678a321, /// 0xbec
			 0xd1366715, /// 0xbf0
			 0xa9eb97cd, /// 0xbf4
			 0xafdbad16, /// 0xbf8
			 0xb92086c1, /// 0xbfc
			 0x1ddbb5fa, /// 0xc00
			 0x21f1757f, /// 0xc04
			 0xc4769dcb, /// 0xc08
			 0xe21ef357, /// 0xc0c
			 0xd1a07025, /// 0xc10
			 0xf27208bc, /// 0xc14
			 0x3456d87f, /// 0xc18
			 0x6add07a9, /// 0xc1c
			 0xa75a507a, /// 0xc20
			 0x44b97f7d, /// 0xc24
			 0x81f98e82, /// 0xc28
			 0x828e97b6, /// 0xc2c
			 0x7068f0c8, /// 0xc30
			 0xbdf38035, /// 0xc34
			 0xaaa9d6c5, /// 0xc38
			 0x0e29e245, /// 0xc3c
			 0xcb000700, /// 0xc40
			 0x66d5d59f, /// 0xc44
			 0x0a8b999c, /// 0xc48
			 0x9f216de8, /// 0xc4c
			 0x4c47ff24, /// 0xc50
			 0x511e0882, /// 0xc54
			 0x7179da7a, /// 0xc58
			 0xfe106528, /// 0xc5c
			 0xaec4e275, /// 0xc60
			 0x28f2935c, /// 0xc64
			 0x90141427, /// 0xc68
			 0x60957688, /// 0xc6c
			 0xeb6a3e8f, /// 0xc70
			 0x0cb41a2f, /// 0xc74
			 0xe9619c2a, /// 0xc78
			 0xa3ed587f, /// 0xc7c
			 0x2a0caefd, /// 0xc80
			 0x69418985, /// 0xc84
			 0xfb544bae, /// 0xc88
			 0x59723560, /// 0xc8c
			 0x8c1e9385, /// 0xc90
			 0xa92bfff5, /// 0xc94
			 0x364e4840, /// 0xc98
			 0xfc95bdab, /// 0xc9c
			 0xb1ae60c2, /// 0xca0
			 0x673faacf, /// 0xca4
			 0x2726d0b3, /// 0xca8
			 0x2dcf8b4b, /// 0xcac
			 0x7c424974, /// 0xcb0
			 0x2fd326e5, /// 0xcb4
			 0x07496e84, /// 0xcb8
			 0xd7355d82, /// 0xcbc
			 0x122fee0d, /// 0xcc0
			 0xe0d0f1ef, /// 0xcc4
			 0x2b4326fa, /// 0xcc8
			 0x8735c931, /// 0xccc
			 0xeb07ad0c, /// 0xcd0
			 0x8dbb3909, /// 0xcd4
			 0xf92e7397, /// 0xcd8
			 0x90e97e8d, /// 0xcdc
			 0x4fce4f91, /// 0xce0
			 0x58db7ffb, /// 0xce4
			 0xab57e2ea, /// 0xce8
			 0x70a29484, /// 0xcec
			 0x3a87a1c8, /// 0xcf0
			 0x9bdaf410, /// 0xcf4
			 0x58b7a204, /// 0xcf8
			 0x15eed034, /// 0xcfc
			 0x3794895a, /// 0xd00
			 0x41cac696, /// 0xd04
			 0x993021c0, /// 0xd08
			 0xe2740a12, /// 0xd0c
			 0x9fd3f49b, /// 0xd10
			 0x72b4f0db, /// 0xd14
			 0x7b024b59, /// 0xd18
			 0x1ff9ee7d, /// 0xd1c
			 0xdff8dd6f, /// 0xd20
			 0xd4cacac3, /// 0xd24
			 0x99d755af, /// 0xd28
			 0x234ca660, /// 0xd2c
			 0x268276e7, /// 0xd30
			 0xa534072e, /// 0xd34
			 0x2fcc68cb, /// 0xd38
			 0x278637cd, /// 0xd3c
			 0x3243fd3f, /// 0xd40
			 0x13235caf, /// 0xd44
			 0xd8231c5e, /// 0xd48
			 0x772e94ac, /// 0xd4c
			 0xbdec175d, /// 0xd50
			 0x3899fcf8, /// 0xd54
			 0x123047f8, /// 0xd58
			 0x7f135830, /// 0xd5c
			 0x6358a3de, /// 0xd60
			 0x6676c2ed, /// 0xd64
			 0xc3507cab, /// 0xd68
			 0xa6ebd0ba, /// 0xd6c
			 0x5170f9c1, /// 0xd70
			 0x4dcee7b2, /// 0xd74
			 0xe018c4d1, /// 0xd78
			 0x6668dd07, /// 0xd7c
			 0xd4c3e352, /// 0xd80
			 0x7c34a881, /// 0xd84
			 0xf9d9519f, /// 0xd88
			 0x8b635717, /// 0xd8c
			 0x979d2ff8, /// 0xd90
			 0x92270c84, /// 0xd94
			 0x9ebbbcb9, /// 0xd98
			 0xabdc0329, /// 0xd9c
			 0xc91aba06, /// 0xda0
			 0xc8a044b8, /// 0xda4
			 0x90d1611b, /// 0xda8
			 0x1dbeeaa9, /// 0xdac
			 0xfd9f10d5, /// 0xdb0
			 0x4dd65d65, /// 0xdb4
			 0x5f0e6ed1, /// 0xdb8
			 0x94642d7b, /// 0xdbc
			 0x478c4a26, /// 0xdc0
			 0x6321cc58, /// 0xdc4
			 0x42a77ab9, /// 0xdc8
			 0x56a683bd, /// 0xdcc
			 0xe7457c0f, /// 0xdd0
			 0xcc8f4149, /// 0xdd4
			 0x907ea442, /// 0xdd8
			 0xa4037eb8, /// 0xddc
			 0x781c7b19, /// 0xde0
			 0xfd36d728, /// 0xde4
			 0x3f3c293d, /// 0xde8
			 0x2f359a5a, /// 0xdec
			 0x6dce0881, /// 0xdf0
			 0x11a9f026, /// 0xdf4
			 0x784d2a5a, /// 0xdf8
			 0x1b2df6cb, /// 0xdfc
			 0x85ce9ce2, /// 0xe00
			 0xe88d8f29, /// 0xe04
			 0xfce7288d, /// 0xe08
			 0x88315e20, /// 0xe0c
			 0x8059445b, /// 0xe10
			 0x1944e18f, /// 0xe14
			 0x185ee3eb, /// 0xe18
			 0x59295684, /// 0xe1c
			 0x15528a9b, /// 0xe20
			 0x64c52f1a, /// 0xe24
			 0xfdece157, /// 0xe28
			 0xa20fb09f, /// 0xe2c
			 0x9d0454cf, /// 0xe30
			 0xd901afc9, /// 0xe34
			 0xcef92c75, /// 0xe38
			 0xfa12ef40, /// 0xe3c
			 0x783cae59, /// 0xe40
			 0xa05b9c9c, /// 0xe44
			 0x201995b4, /// 0xe48
			 0x51aab920, /// 0xe4c
			 0xbe5d66fd, /// 0xe50
			 0xa7da94af, /// 0xe54
			 0xc6499a27, /// 0xe58
			 0x20e676c1, /// 0xe5c
			 0x4c553b07, /// 0xe60
			 0x9de95e29, /// 0xe64
			 0xf630acfb, /// 0xe68
			 0xe39883f3, /// 0xe6c
			 0x57be288c, /// 0xe70
			 0x00f3903d, /// 0xe74
			 0x5617d804, /// 0xe78
			 0x711621dc, /// 0xe7c
			 0x935c8eb0, /// 0xe80
			 0xc2f34c67, /// 0xe84
			 0x4f8401fd, /// 0xe88
			 0xb576bf78, /// 0xe8c
			 0x04017849, /// 0xe90
			 0x2ccbe11d, /// 0xe94
			 0x3119d3e5, /// 0xe98
			 0x7ec1906b, /// 0xe9c
			 0xcf023b8a, /// 0xea0
			 0x7e002a8b, /// 0xea4
			 0xbe5fdf38, /// 0xea8
			 0xa687ce37, /// 0xeac
			 0x1df59f18, /// 0xeb0
			 0xf59aef5b, /// 0xeb4
			 0x74e3a92e, /// 0xeb8
			 0x82fc6ec7, /// 0xebc
			 0x887789f0, /// 0xec0
			 0x67e8bdf5, /// 0xec4
			 0xb9179234, /// 0xec8
			 0x6ace1bc6, /// 0xecc
			 0x101463cc, /// 0xed0
			 0xa025a5dd, /// 0xed4
			 0x1509c16f, /// 0xed8
			 0xd29a27ba, /// 0xedc
			 0x824f4a82, /// 0xee0
			 0x39ede1d7, /// 0xee4
			 0xbb9fefe9, /// 0xee8
			 0x16513225, /// 0xeec
			 0xf30c71c6, /// 0xef0
			 0x15cb96cf, /// 0xef4
			 0x43d89c6a, /// 0xef8
			 0xca3e6331, /// 0xefc
			 0x700fa6c4, /// 0xf00
			 0x5e80920f, /// 0xf04
			 0x9b96eeeb, /// 0xf08
			 0xe1fdf53a, /// 0xf0c
			 0xb232d76c, /// 0xf10
			 0x3d2215c7, /// 0xf14
			 0x37d4e8c1, /// 0xf18
			 0x3921fb1c, /// 0xf1c
			 0x867e31b1, /// 0xf20
			 0x397c471e, /// 0xf24
			 0x303dee7b, /// 0xf28
			 0xcd884469, /// 0xf2c
			 0xc69dcb27, /// 0xf30
			 0x8a99559c, /// 0xf34
			 0x43cb6e6b, /// 0xf38
			 0x0c71b14b, /// 0xf3c
			 0x66196450, /// 0xf40
			 0xb6dab14e, /// 0xf44
			 0xe5cb3560, /// 0xf48
			 0x3a31d295, /// 0xf4c
			 0xdcd55873, /// 0xf50
			 0x498915e5, /// 0xf54
			 0xaaed38b0, /// 0xf58
			 0xf3c239ce, /// 0xf5c
			 0x1832354b, /// 0xf60
			 0xd63fd29b, /// 0xf64
			 0xfe2796b4, /// 0xf68
			 0x80481106, /// 0xf6c
			 0x11bb518b, /// 0xf70
			 0xd8c37e5a, /// 0xf74
			 0xb744a5e5, /// 0xf78
			 0xc191c42a, /// 0xf7c
			 0xcfb87121, /// 0xf80
			 0xa45c3d9e, /// 0xf84
			 0xeeb88dd3, /// 0xf88
			 0x4a323e5f, /// 0xf8c
			 0xdc09fa5b, /// 0xf90
			 0x4730e2f9, /// 0xf94
			 0x946da8f3, /// 0xf98
			 0xde345a41, /// 0xf9c
			 0x952db234, /// 0xfa0
			 0xbb7dc834, /// 0xfa4
			 0xafa75940, /// 0xfa8
			 0x07dd6a0d, /// 0xfac
			 0x134cca55, /// 0xfb0
			 0x29f10eec, /// 0xfb4
			 0x7b0affa8, /// 0xfb8
			 0x8cbf2272, /// 0xfbc
			 0xdc0ec98e, /// 0xfc0
			 0x74c6ec64, /// 0xfc4
			 0x24c1092a, /// 0xfc8
			 0xc5ce7c4e, /// 0xfcc
			 0x67869cbb, /// 0xfd0
			 0xe1e4cd50, /// 0xfd4
			 0x972b996c, /// 0xfd8
			 0x8786399b, /// 0xfdc
			 0xcf2048f6, /// 0xfe0
			 0xfad1dd98, /// 0xfe4
			 0xcf94a116, /// 0xfe8
			 0xcaa451ce, /// 0xfec
			 0xa710a078, /// 0xff0
			 0xdf173c3e, /// 0xff4
			 0x853acba3, /// 0xff8
			 0xf28fcc70 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x138ee09f, /// 0x0
			 0xf9cfe006, /// 0x4
			 0x63a62c19, /// 0x8
			 0x306433e3, /// 0xc
			 0xcf47da95, /// 0x10
			 0x9a873f8f, /// 0x14
			 0x5e9cdc2a, /// 0x18
			 0x16e4d9c0, /// 0x1c
			 0x192f659f, /// 0x20
			 0x1f089c47, /// 0x24
			 0xffafa219, /// 0x28
			 0x2f7b4f2e, /// 0x2c
			 0x98c81902, /// 0x30
			 0x97a02263, /// 0x34
			 0xecd34ff4, /// 0x38
			 0xcb2621e3, /// 0x3c
			 0x903e603d, /// 0x40
			 0xd46c67da, /// 0x44
			 0xb2339235, /// 0x48
			 0x58735d4a, /// 0x4c
			 0x274aa30c, /// 0x50
			 0x4cbf6772, /// 0x54
			 0xbdd2bd91, /// 0x58
			 0xc9fe84e1, /// 0x5c
			 0x605ec0fc, /// 0x60
			 0x1d4a647c, /// 0x64
			 0x65986f06, /// 0x68
			 0x86c923de, /// 0x6c
			 0x915592ca, /// 0x70
			 0xb22dc7e3, /// 0x74
			 0x1022d3f0, /// 0x78
			 0x85ef1df0, /// 0x7c
			 0x9a7ff553, /// 0x80
			 0xfeca87eb, /// 0x84
			 0x36d1b845, /// 0x88
			 0x2ceb2e1b, /// 0x8c
			 0x59abf37c, /// 0x90
			 0x526cf97d, /// 0x94
			 0xb219cb35, /// 0x98
			 0x477d4ce6, /// 0x9c
			 0xf0db72d7, /// 0xa0
			 0x3079268d, /// 0xa4
			 0xc827701e, /// 0xa8
			 0x8d681e33, /// 0xac
			 0x77813837, /// 0xb0
			 0x942f68b7, /// 0xb4
			 0xc779107e, /// 0xb8
			 0xd0804149, /// 0xbc
			 0xb353242e, /// 0xc0
			 0xfdca41e4, /// 0xc4
			 0x079e12fe, /// 0xc8
			 0x76d7ea02, /// 0xcc
			 0x4062b2bd, /// 0xd0
			 0xc3ae3d0f, /// 0xd4
			 0x67208439, /// 0xd8
			 0x15a7463c, /// 0xdc
			 0xb998fbaa, /// 0xe0
			 0x5127ed84, /// 0xe4
			 0xed2d53a2, /// 0xe8
			 0x8a92016d, /// 0xec
			 0xe33a8ee7, /// 0xf0
			 0x6cda212a, /// 0xf4
			 0x76a278ed, /// 0xf8
			 0x2c74fa6f, /// 0xfc
			 0x0cf58840, /// 0x100
			 0xc81bd0eb, /// 0x104
			 0x5a856fc1, /// 0x108
			 0xb6b86d24, /// 0x10c
			 0x33bf4f92, /// 0x110
			 0x0c957155, /// 0x114
			 0xa2438131, /// 0x118
			 0x2af76ab1, /// 0x11c
			 0xa2f9b1c2, /// 0x120
			 0xcfdd5c41, /// 0x124
			 0x9307ce11, /// 0x128
			 0xdf04536a, /// 0x12c
			 0xa8ce27db, /// 0x130
			 0x504b0f8c, /// 0x134
			 0xfd18a6e7, /// 0x138
			 0xa6bc8f60, /// 0x13c
			 0xd19229a7, /// 0x140
			 0x084aa5fe, /// 0x144
			 0xa09288b3, /// 0x148
			 0xa0fe7394, /// 0x14c
			 0x248c742d, /// 0x150
			 0xa6da3a43, /// 0x154
			 0x82094785, /// 0x158
			 0x3d46cc9a, /// 0x15c
			 0xe5368e7c, /// 0x160
			 0x5b6d7d45, /// 0x164
			 0xaf94f9a5, /// 0x168
			 0xebe32996, /// 0x16c
			 0xe0e4d3d3, /// 0x170
			 0x976bfada, /// 0x174
			 0xa0e16108, /// 0x178
			 0x0ffb2aee, /// 0x17c
			 0x49e82a1c, /// 0x180
			 0xd266c5c5, /// 0x184
			 0x3be4d496, /// 0x188
			 0xd0bec2ff, /// 0x18c
			 0xa7430c4f, /// 0x190
			 0xe996031d, /// 0x194
			 0x1f104007, /// 0x198
			 0xb0abf7c4, /// 0x19c
			 0x9a2f1d5e, /// 0x1a0
			 0x68bcc6df, /// 0x1a4
			 0x7f051f76, /// 0x1a8
			 0xfe375392, /// 0x1ac
			 0xd296a7aa, /// 0x1b0
			 0x8d96f2b7, /// 0x1b4
			 0x0aad21fd, /// 0x1b8
			 0xc1e88822, /// 0x1bc
			 0x19c2fbde, /// 0x1c0
			 0x07973103, /// 0x1c4
			 0xf3609891, /// 0x1c8
			 0x0563a83b, /// 0x1cc
			 0x29a92077, /// 0x1d0
			 0xd678559d, /// 0x1d4
			 0x3edd92d4, /// 0x1d8
			 0xba0fe429, /// 0x1dc
			 0x80106d54, /// 0x1e0
			 0x34d529dc, /// 0x1e4
			 0x8d02e18b, /// 0x1e8
			 0x10439364, /// 0x1ec
			 0x8acdb107, /// 0x1f0
			 0xd1598d3a, /// 0x1f4
			 0xbf99ab13, /// 0x1f8
			 0x080b8c8e, /// 0x1fc
			 0x7510b1f9, /// 0x200
			 0x46a3a544, /// 0x204
			 0x8631ef28, /// 0x208
			 0x7fb09ad1, /// 0x20c
			 0xe8c2ab7c, /// 0x210
			 0x6f291544, /// 0x214
			 0xa75bf715, /// 0x218
			 0x678925db, /// 0x21c
			 0xfaa2228c, /// 0x220
			 0x0b0b4721, /// 0x224
			 0x28620c8b, /// 0x228
			 0x204aca8d, /// 0x22c
			 0x2039166c, /// 0x230
			 0xcf01ee85, /// 0x234
			 0xd2a181fd, /// 0x238
			 0xdf3d9559, /// 0x23c
			 0x00af6cc4, /// 0x240
			 0xa0ec20af, /// 0x244
			 0x75789dc1, /// 0x248
			 0x72e70d2d, /// 0x24c
			 0x2b8ebd05, /// 0x250
			 0xc128b77e, /// 0x254
			 0xf0411e24, /// 0x258
			 0x17315b7e, /// 0x25c
			 0xc12a3e7d, /// 0x260
			 0xb342cba7, /// 0x264
			 0xd99ce0ec, /// 0x268
			 0x7e3cc96f, /// 0x26c
			 0x54bda1dc, /// 0x270
			 0x5ea7a64d, /// 0x274
			 0x4fb83854, /// 0x278
			 0x23c8beb3, /// 0x27c
			 0x23b4677a, /// 0x280
			 0xe2a16cbf, /// 0x284
			 0x2bdf0d52, /// 0x288
			 0xb086af25, /// 0x28c
			 0x2e82d9eb, /// 0x290
			 0x32f7558e, /// 0x294
			 0x5b880ad2, /// 0x298
			 0xb9adf32b, /// 0x29c
			 0xced3d988, /// 0x2a0
			 0x8ddabb85, /// 0x2a4
			 0x0cae2b72, /// 0x2a8
			 0xda5e00d5, /// 0x2ac
			 0xea83ac3d, /// 0x2b0
			 0x175ee273, /// 0x2b4
			 0x3cd78ffb, /// 0x2b8
			 0x874487ce, /// 0x2bc
			 0x8014911e, /// 0x2c0
			 0x1d82ad7b, /// 0x2c4
			 0xb1327331, /// 0x2c8
			 0xe8f73971, /// 0x2cc
			 0x37bb67fd, /// 0x2d0
			 0x71879ae3, /// 0x2d4
			 0x6929717a, /// 0x2d8
			 0x18efc8bd, /// 0x2dc
			 0xb7abf987, /// 0x2e0
			 0x1b986ba7, /// 0x2e4
			 0xef9418ec, /// 0x2e8
			 0xcd6dead3, /// 0x2ec
			 0x379ac782, /// 0x2f0
			 0xb3ec098e, /// 0x2f4
			 0x06f0b2c3, /// 0x2f8
			 0x07dc9791, /// 0x2fc
			 0xb273c3db, /// 0x300
			 0x0387d892, /// 0x304
			 0xa3bdcf12, /// 0x308
			 0x33f189e6, /// 0x30c
			 0xb1e6d656, /// 0x310
			 0x364e0c5b, /// 0x314
			 0xd40f9636, /// 0x318
			 0xb92dc613, /// 0x31c
			 0xe4883a1b, /// 0x320
			 0x1b1991c6, /// 0x324
			 0x1d832251, /// 0x328
			 0xc1a7e51c, /// 0x32c
			 0x38f2d86f, /// 0x330
			 0xd6126981, /// 0x334
			 0x195b68a5, /// 0x338
			 0xea2d7c7b, /// 0x33c
			 0x394bf17e, /// 0x340
			 0x8accac93, /// 0x344
			 0x422dbb1d, /// 0x348
			 0x2f8f1f98, /// 0x34c
			 0xf538251a, /// 0x350
			 0x98f39a62, /// 0x354
			 0xdcbe30a1, /// 0x358
			 0x026b775d, /// 0x35c
			 0x125ea35e, /// 0x360
			 0xcd08f41b, /// 0x364
			 0x671d2de7, /// 0x368
			 0x26614fed, /// 0x36c
			 0x0d47fc9e, /// 0x370
			 0x24cf240b, /// 0x374
			 0x91e13346, /// 0x378
			 0x578ab03b, /// 0x37c
			 0x2f77ed8f, /// 0x380
			 0x36a7194c, /// 0x384
			 0x1ead1a86, /// 0x388
			 0x5d6fcade, /// 0x38c
			 0xacf55310, /// 0x390
			 0x6f97759e, /// 0x394
			 0xd0b4a8d3, /// 0x398
			 0xb42e8537, /// 0x39c
			 0x03312ffb, /// 0x3a0
			 0x957fbd8c, /// 0x3a4
			 0xd502e216, /// 0x3a8
			 0x1e707fcc, /// 0x3ac
			 0x93b82505, /// 0x3b0
			 0xa592ee1d, /// 0x3b4
			 0xa1aee60e, /// 0x3b8
			 0x4902d68b, /// 0x3bc
			 0x23963a84, /// 0x3c0
			 0xf4de6a6b, /// 0x3c4
			 0x3303dcc2, /// 0x3c8
			 0x067cedc8, /// 0x3cc
			 0x62c8c2bf, /// 0x3d0
			 0x93f78530, /// 0x3d4
			 0x9afe540e, /// 0x3d8
			 0x716e9392, /// 0x3dc
			 0xae6f1941, /// 0x3e0
			 0x626ce2d8, /// 0x3e4
			 0x0eb98c7d, /// 0x3e8
			 0x8c1eb92b, /// 0x3ec
			 0x0d70e4f8, /// 0x3f0
			 0xc0448062, /// 0x3f4
			 0x6b28601f, /// 0x3f8
			 0xe2c0733e, /// 0x3fc
			 0xecd4fca8, /// 0x400
			 0xb5257c46, /// 0x404
			 0x35aa261f, /// 0x408
			 0x523f9f55, /// 0x40c
			 0x703d9b71, /// 0x410
			 0x5cf8a40f, /// 0x414
			 0x66147f93, /// 0x418
			 0x66314395, /// 0x41c
			 0x256897a4, /// 0x420
			 0x6297df68, /// 0x424
			 0xc8d4bc0e, /// 0x428
			 0xa2e1f198, /// 0x42c
			 0x45f326a8, /// 0x430
			 0x5dcdee16, /// 0x434
			 0xd618c590, /// 0x438
			 0x24906526, /// 0x43c
			 0x622c7a5f, /// 0x440
			 0x4712cfe2, /// 0x444
			 0x37495f64, /// 0x448
			 0x49ea8f23, /// 0x44c
			 0xdcf99cc2, /// 0x450
			 0x634341b5, /// 0x454
			 0xf34dbbc4, /// 0x458
			 0xe3ca7d11, /// 0x45c
			 0xc5bcbf4d, /// 0x460
			 0xa4e0d553, /// 0x464
			 0xfe718415, /// 0x468
			 0xa29413bc, /// 0x46c
			 0x9fb1c640, /// 0x470
			 0x109cc2b0, /// 0x474
			 0x30fbfa0f, /// 0x478
			 0x3dc1e527, /// 0x47c
			 0xbeafff65, /// 0x480
			 0xac6ddb1b, /// 0x484
			 0xd3070531, /// 0x488
			 0x869253c1, /// 0x48c
			 0x370f2d01, /// 0x490
			 0xcbcf0022, /// 0x494
			 0x33d56752, /// 0x498
			 0xb0da9a5b, /// 0x49c
			 0xf8e50120, /// 0x4a0
			 0xb18b1b23, /// 0x4a4
			 0xe28c31f7, /// 0x4a8
			 0x72c12301, /// 0x4ac
			 0x3f62669a, /// 0x4b0
			 0x2032ee36, /// 0x4b4
			 0x68efbd2c, /// 0x4b8
			 0x6d069437, /// 0x4bc
			 0x2c41d4af, /// 0x4c0
			 0x33122709, /// 0x4c4
			 0xa9acd261, /// 0x4c8
			 0x7d95863d, /// 0x4cc
			 0x9ade3de7, /// 0x4d0
			 0x2313aa89, /// 0x4d4
			 0x826960fc, /// 0x4d8
			 0x1741307e, /// 0x4dc
			 0x0f8377c6, /// 0x4e0
			 0x48acd875, /// 0x4e4
			 0x65e7b84f, /// 0x4e8
			 0x7e02e5a8, /// 0x4ec
			 0xa043591a, /// 0x4f0
			 0xe76ec923, /// 0x4f4
			 0xc9a9cfa9, /// 0x4f8
			 0x9a41d0bd, /// 0x4fc
			 0x489b2f51, /// 0x500
			 0xd49d78c2, /// 0x504
			 0xa8f0ce31, /// 0x508
			 0x920790cc, /// 0x50c
			 0xa2114c0d, /// 0x510
			 0x053ed87e, /// 0x514
			 0x02bf9aea, /// 0x518
			 0x5600777f, /// 0x51c
			 0x24598de6, /// 0x520
			 0xdbb99377, /// 0x524
			 0x4c086e16, /// 0x528
			 0x2195ef86, /// 0x52c
			 0xb5b4a619, /// 0x530
			 0xaaf0fce1, /// 0x534
			 0x8b9daa3c, /// 0x538
			 0x7b7be349, /// 0x53c
			 0x139c573c, /// 0x540
			 0x53bea2e9, /// 0x544
			 0x1d21727e, /// 0x548
			 0x391d9f5a, /// 0x54c
			 0x8134b811, /// 0x550
			 0x5edfeafb, /// 0x554
			 0x0a2114aa, /// 0x558
			 0xd9c2224b, /// 0x55c
			 0x3cd73451, /// 0x560
			 0xd1b25e0a, /// 0x564
			 0x3038da9f, /// 0x568
			 0x34cd1d90, /// 0x56c
			 0x512513dc, /// 0x570
			 0x87107f90, /// 0x574
			 0xc8d883d8, /// 0x578
			 0xb7aee6fa, /// 0x57c
			 0xe5f9e36b, /// 0x580
			 0x3ab6d3ca, /// 0x584
			 0xe8ff4598, /// 0x588
			 0xeb670cc5, /// 0x58c
			 0x70c57baf, /// 0x590
			 0x423d2be7, /// 0x594
			 0x372fbaf7, /// 0x598
			 0xaf3a8ee7, /// 0x59c
			 0xf00c2a1a, /// 0x5a0
			 0x4f118fe4, /// 0x5a4
			 0x263f4850, /// 0x5a8
			 0x86c02817, /// 0x5ac
			 0x6fd0417b, /// 0x5b0
			 0x6136cbfa, /// 0x5b4
			 0x2b92de28, /// 0x5b8
			 0x7a20288f, /// 0x5bc
			 0xa3fc90d2, /// 0x5c0
			 0xf8cb0166, /// 0x5c4
			 0x1e7ec2c8, /// 0x5c8
			 0x5efb7d34, /// 0x5cc
			 0x2c6419fd, /// 0x5d0
			 0x7ccba529, /// 0x5d4
			 0x51ce9878, /// 0x5d8
			 0xa7935e96, /// 0x5dc
			 0x7e1a3cad, /// 0x5e0
			 0x2ffdaa99, /// 0x5e4
			 0xfcaba851, /// 0x5e8
			 0xab7404d0, /// 0x5ec
			 0x25a56b85, /// 0x5f0
			 0x08875012, /// 0x5f4
			 0x6b8fd60b, /// 0x5f8
			 0x3f430a56, /// 0x5fc
			 0x981c1928, /// 0x600
			 0x24807999, /// 0x604
			 0x90638041, /// 0x608
			 0x0e9fb81c, /// 0x60c
			 0xdb43460b, /// 0x610
			 0x17aff79c, /// 0x614
			 0xb137a429, /// 0x618
			 0xe4536632, /// 0x61c
			 0x416f8d5d, /// 0x620
			 0x689f0cf9, /// 0x624
			 0xcf8ac344, /// 0x628
			 0xec6cd599, /// 0x62c
			 0xcfe8a7a1, /// 0x630
			 0x0b88c63f, /// 0x634
			 0x231766f5, /// 0x638
			 0xf8c4e545, /// 0x63c
			 0x69c025ab, /// 0x640
			 0x30d002e2, /// 0x644
			 0xea55c137, /// 0x648
			 0x29bfe772, /// 0x64c
			 0x0a41f26a, /// 0x650
			 0x280e4063, /// 0x654
			 0x9d800dcd, /// 0x658
			 0x90ee8891, /// 0x65c
			 0x65c9e6f9, /// 0x660
			 0x89f1bebd, /// 0x664
			 0x8101ae52, /// 0x668
			 0x5e5df73d, /// 0x66c
			 0xfe892b5e, /// 0x670
			 0x2cc4cf39, /// 0x674
			 0xb5f33934, /// 0x678
			 0x4c4b05ee, /// 0x67c
			 0xae4d66cb, /// 0x680
			 0x7f21d0fd, /// 0x684
			 0x36f2cceb, /// 0x688
			 0x135964de, /// 0x68c
			 0xeb6ad196, /// 0x690
			 0x2608887a, /// 0x694
			 0x685d5df7, /// 0x698
			 0x17c8e147, /// 0x69c
			 0x1647ae78, /// 0x6a0
			 0x9f5237ee, /// 0x6a4
			 0xc0d76529, /// 0x6a8
			 0x05febf43, /// 0x6ac
			 0x78e2f930, /// 0x6b0
			 0x9507e971, /// 0x6b4
			 0xbc0934b8, /// 0x6b8
			 0x80b74632, /// 0x6bc
			 0x047041d0, /// 0x6c0
			 0x914f7853, /// 0x6c4
			 0x9e051261, /// 0x6c8
			 0x2ec7efb3, /// 0x6cc
			 0x96d476ed, /// 0x6d0
			 0x544e196f, /// 0x6d4
			 0xc0019c3b, /// 0x6d8
			 0xacfc6e70, /// 0x6dc
			 0x0af0b481, /// 0x6e0
			 0xe896d240, /// 0x6e4
			 0xf36e1e94, /// 0x6e8
			 0x1fa941dc, /// 0x6ec
			 0xfe6c132c, /// 0x6f0
			 0xe9f01718, /// 0x6f4
			 0xa71d58fd, /// 0x6f8
			 0x10a7d060, /// 0x6fc
			 0x7b466ab5, /// 0x700
			 0x80af1171, /// 0x704
			 0x0e6b182e, /// 0x708
			 0x3073dabf, /// 0x70c
			 0xd90973ba, /// 0x710
			 0x78435759, /// 0x714
			 0x09a15d38, /// 0x718
			 0xd23d94ce, /// 0x71c
			 0x325cff6c, /// 0x720
			 0x64cabdbb, /// 0x724
			 0x926ad1e2, /// 0x728
			 0x170d6083, /// 0x72c
			 0x424fe3cb, /// 0x730
			 0x7aa03ee5, /// 0x734
			 0xc9f182c2, /// 0x738
			 0x34178ed9, /// 0x73c
			 0xbdc6c12f, /// 0x740
			 0x12ac4af7, /// 0x744
			 0x8aa263f7, /// 0x748
			 0xc2c43022, /// 0x74c
			 0xd452be87, /// 0x750
			 0xb56c138d, /// 0x754
			 0x861161db, /// 0x758
			 0xce781684, /// 0x75c
			 0x793c917a, /// 0x760
			 0xb49957a2, /// 0x764
			 0xc44511c4, /// 0x768
			 0x9d0de43e, /// 0x76c
			 0xfdb4d7b3, /// 0x770
			 0xc1e604cc, /// 0x774
			 0x3206608b, /// 0x778
			 0xa9712eb2, /// 0x77c
			 0x1aafe077, /// 0x780
			 0x3efdd3c9, /// 0x784
			 0xc5d2b9a3, /// 0x788
			 0x1e5e4509, /// 0x78c
			 0x6ec7988c, /// 0x790
			 0x3fec9817, /// 0x794
			 0xe7d2d672, /// 0x798
			 0x18c77352, /// 0x79c
			 0x41f0807d, /// 0x7a0
			 0x5a582616, /// 0x7a4
			 0x6c84923e, /// 0x7a8
			 0x3ab84691, /// 0x7ac
			 0xadcd1f43, /// 0x7b0
			 0x9a20b2b9, /// 0x7b4
			 0x163ffc0d, /// 0x7b8
			 0x95bf4f2e, /// 0x7bc
			 0x3b7925b7, /// 0x7c0
			 0xe9b88212, /// 0x7c4
			 0x61cd2a16, /// 0x7c8
			 0x832a15b5, /// 0x7cc
			 0x248c716d, /// 0x7d0
			 0x727cb1e1, /// 0x7d4
			 0xe9a07408, /// 0x7d8
			 0x5448ad6f, /// 0x7dc
			 0xe3ada202, /// 0x7e0
			 0x4c6b84cd, /// 0x7e4
			 0x5bd0fa81, /// 0x7e8
			 0x5f55f969, /// 0x7ec
			 0x50016209, /// 0x7f0
			 0x71f7eb86, /// 0x7f4
			 0x44ff9b9d, /// 0x7f8
			 0xfaaf4e56, /// 0x7fc
			 0x44ceb5f5, /// 0x800
			 0x2a5d687a, /// 0x804
			 0x695b0f2e, /// 0x808
			 0x00d1c9ac, /// 0x80c
			 0x00b90a37, /// 0x810
			 0xda9a09b8, /// 0x814
			 0x3d63a4f2, /// 0x818
			 0xb58ddf7b, /// 0x81c
			 0xc6c2aaf3, /// 0x820
			 0xf7d8c26c, /// 0x824
			 0x31f4d542, /// 0x828
			 0x2cd24a61, /// 0x82c
			 0xdfd62051, /// 0x830
			 0x707db09b, /// 0x834
			 0x29ae8360, /// 0x838
			 0x88d7edda, /// 0x83c
			 0x6b072c6a, /// 0x840
			 0x6a4539d8, /// 0x844
			 0xb40fea97, /// 0x848
			 0xeab6752d, /// 0x84c
			 0xe7aa5492, /// 0x850
			 0x78b5061c, /// 0x854
			 0x1785e977, /// 0x858
			 0x5c3eba1a, /// 0x85c
			 0x876decfe, /// 0x860
			 0x680d4110, /// 0x864
			 0x7b1c4ad0, /// 0x868
			 0xbbe5e23e, /// 0x86c
			 0xf185c759, /// 0x870
			 0x2199df59, /// 0x874
			 0x691fa3bb, /// 0x878
			 0x988ef0a3, /// 0x87c
			 0xfeb8c904, /// 0x880
			 0xc46338ae, /// 0x884
			 0x73914243, /// 0x888
			 0xfa0f3607, /// 0x88c
			 0xb49bcde4, /// 0x890
			 0x5f4ed6eb, /// 0x894
			 0xa5cfab4c, /// 0x898
			 0x85452ef0, /// 0x89c
			 0x8dcd574b, /// 0x8a0
			 0x179cf4a6, /// 0x8a4
			 0x714d8f46, /// 0x8a8
			 0x9120fe75, /// 0x8ac
			 0xe93b319a, /// 0x8b0
			 0x409a0ff2, /// 0x8b4
			 0x0b82a682, /// 0x8b8
			 0xfa67d2a6, /// 0x8bc
			 0x87efa3d4, /// 0x8c0
			 0x7b301cc6, /// 0x8c4
			 0xed3d56d6, /// 0x8c8
			 0x0b0e038a, /// 0x8cc
			 0x683c5bca, /// 0x8d0
			 0x493d4099, /// 0x8d4
			 0x8e8efc8a, /// 0x8d8
			 0x386d42ea, /// 0x8dc
			 0xb7995ca1, /// 0x8e0
			 0x76077ca9, /// 0x8e4
			 0xaff9647d, /// 0x8e8
			 0xd1f3fa4b, /// 0x8ec
			 0x3243e79f, /// 0x8f0
			 0xcc32a5e9, /// 0x8f4
			 0x5301ece2, /// 0x8f8
			 0x9548365d, /// 0x8fc
			 0xe0fe18d5, /// 0x900
			 0x21a70b5d, /// 0x904
			 0x8718637d, /// 0x908
			 0x7ac93350, /// 0x90c
			 0xbc93f04f, /// 0x910
			 0xc8d2a02e, /// 0x914
			 0x4e490526, /// 0x918
			 0xa8bdf94f, /// 0x91c
			 0xc4f22195, /// 0x920
			 0x5a83af9c, /// 0x924
			 0xdda71280, /// 0x928
			 0xba631674, /// 0x92c
			 0x6a6100f5, /// 0x930
			 0x79932901, /// 0x934
			 0xddc89fff, /// 0x938
			 0x2674de0e, /// 0x93c
			 0x021b4ced, /// 0x940
			 0xc3f866b3, /// 0x944
			 0xee83f714, /// 0x948
			 0x1071ed9a, /// 0x94c
			 0x724494e7, /// 0x950
			 0x3fd918e7, /// 0x954
			 0x388a3da2, /// 0x958
			 0x2e1a8394, /// 0x95c
			 0x17a0f04c, /// 0x960
			 0xaed8062c, /// 0x964
			 0xea41a545, /// 0x968
			 0x1068055a, /// 0x96c
			 0x23512c6e, /// 0x970
			 0xc2301683, /// 0x974
			 0x5efd1192, /// 0x978
			 0x4609b392, /// 0x97c
			 0x079e28fc, /// 0x980
			 0x56c5bfb2, /// 0x984
			 0xe14956dd, /// 0x988
			 0xf5595b35, /// 0x98c
			 0x136754e8, /// 0x990
			 0x546642a1, /// 0x994
			 0xc2a53a7a, /// 0x998
			 0x02737111, /// 0x99c
			 0xbab723d9, /// 0x9a0
			 0xbe394448, /// 0x9a4
			 0xfcfd7970, /// 0x9a8
			 0x01365b47, /// 0x9ac
			 0x6a856c91, /// 0x9b0
			 0xb5b9906b, /// 0x9b4
			 0x6c1ee2e2, /// 0x9b8
			 0x2cef474f, /// 0x9bc
			 0x00f222e1, /// 0x9c0
			 0x752d0b66, /// 0x9c4
			 0x36a9a078, /// 0x9c8
			 0xc52395df, /// 0x9cc
			 0xc6ead0cc, /// 0x9d0
			 0x344d6f81, /// 0x9d4
			 0xba48779a, /// 0x9d8
			 0x1d5dd479, /// 0x9dc
			 0x7eb529bf, /// 0x9e0
			 0x081d9386, /// 0x9e4
			 0xdd318b7b, /// 0x9e8
			 0xea042214, /// 0x9ec
			 0x982efe26, /// 0x9f0
			 0xa6240aeb, /// 0x9f4
			 0xdced2c3b, /// 0x9f8
			 0x54526243, /// 0x9fc
			 0x34c0afa0, /// 0xa00
			 0xe4292022, /// 0xa04
			 0x3403655e, /// 0xa08
			 0x76e12d1c, /// 0xa0c
			 0x18daa802, /// 0xa10
			 0x0ed4c1d2, /// 0xa14
			 0x5099e007, /// 0xa18
			 0xf4de9c64, /// 0xa1c
			 0x7f0d5569, /// 0xa20
			 0x818b73ab, /// 0xa24
			 0x2ebb278a, /// 0xa28
			 0x51efba17, /// 0xa2c
			 0xcf6a6cfc, /// 0xa30
			 0xe98c00ce, /// 0xa34
			 0xcf1dcd90, /// 0xa38
			 0xf85617c9, /// 0xa3c
			 0xac39b7c7, /// 0xa40
			 0x36dcca20, /// 0xa44
			 0x22c8b91a, /// 0xa48
			 0x16dc0994, /// 0xa4c
			 0x811d1953, /// 0xa50
			 0xcbb67234, /// 0xa54
			 0x57a2b94c, /// 0xa58
			 0x7dec94b7, /// 0xa5c
			 0x120e0623, /// 0xa60
			 0x9e715254, /// 0xa64
			 0x459f5ab1, /// 0xa68
			 0x49c07d5c, /// 0xa6c
			 0x78420254, /// 0xa70
			 0x50043d99, /// 0xa74
			 0xe47042bc, /// 0xa78
			 0x0874a769, /// 0xa7c
			 0xa83aacf9, /// 0xa80
			 0x95eb2b69, /// 0xa84
			 0xfaefccd8, /// 0xa88
			 0xd79c074b, /// 0xa8c
			 0x4756f046, /// 0xa90
			 0xff2393c4, /// 0xa94
			 0x60331488, /// 0xa98
			 0x84cc9b2f, /// 0xa9c
			 0xa87a6cea, /// 0xaa0
			 0x4c86175d, /// 0xaa4
			 0xd750a844, /// 0xaa8
			 0x26b7660c, /// 0xaac
			 0x1986f7d2, /// 0xab0
			 0xcf616cc7, /// 0xab4
			 0x1ac7b0c8, /// 0xab8
			 0x804b8b97, /// 0xabc
			 0x48d234e1, /// 0xac0
			 0x74aab73c, /// 0xac4
			 0xd674efb5, /// 0xac8
			 0xe70849af, /// 0xacc
			 0x304fbd6a, /// 0xad0
			 0xf4a9ca16, /// 0xad4
			 0xa36cc188, /// 0xad8
			 0x0301ad21, /// 0xadc
			 0x603b501b, /// 0xae0
			 0x467cc5b5, /// 0xae4
			 0x35d029cc, /// 0xae8
			 0x54984a0f, /// 0xaec
			 0x4c2bdfd0, /// 0xaf0
			 0x225233b3, /// 0xaf4
			 0x729ff8f4, /// 0xaf8
			 0xf148f704, /// 0xafc
			 0xffd37059, /// 0xb00
			 0x8bee35a9, /// 0xb04
			 0x9e2cfa55, /// 0xb08
			 0x4e1dfc9f, /// 0xb0c
			 0xd0ddf46d, /// 0xb10
			 0x88cc1132, /// 0xb14
			 0x60eb7539, /// 0xb18
			 0x7c4391fd, /// 0xb1c
			 0x6a21e087, /// 0xb20
			 0x16bbb65f, /// 0xb24
			 0xfa2fa736, /// 0xb28
			 0x838a0b7c, /// 0xb2c
			 0xcf2bc221, /// 0xb30
			 0x2d4266a6, /// 0xb34
			 0x70700b25, /// 0xb38
			 0xd7bad1af, /// 0xb3c
			 0xeb4b4f4a, /// 0xb40
			 0xfcb1921f, /// 0xb44
			 0x5bd16d6f, /// 0xb48
			 0xd1727cb8, /// 0xb4c
			 0xb6b46f4b, /// 0xb50
			 0x117dccbe, /// 0xb54
			 0xc8c92dc1, /// 0xb58
			 0x2fabc16b, /// 0xb5c
			 0x4769a387, /// 0xb60
			 0x7dc49e1f, /// 0xb64
			 0x23d412af, /// 0xb68
			 0x42ae30c2, /// 0xb6c
			 0xd415bb96, /// 0xb70
			 0x088d5bec, /// 0xb74
			 0x6f83a879, /// 0xb78
			 0x055cba77, /// 0xb7c
			 0xa956fcdd, /// 0xb80
			 0x2854f1ce, /// 0xb84
			 0x387f7c99, /// 0xb88
			 0xa7ef9723, /// 0xb8c
			 0x6eee5e20, /// 0xb90
			 0x96de2b4c, /// 0xb94
			 0xe4c51100, /// 0xb98
			 0xa1ca6d44, /// 0xb9c
			 0x32be168b, /// 0xba0
			 0xd9480351, /// 0xba4
			 0xb74fb89d, /// 0xba8
			 0xa8c0c1ae, /// 0xbac
			 0x23789899, /// 0xbb0
			 0x44ccb0e9, /// 0xbb4
			 0xa97e769d, /// 0xbb8
			 0xcc3b7ec7, /// 0xbbc
			 0xdbf4cdef, /// 0xbc0
			 0xb180dd8a, /// 0xbc4
			 0x3c0fc6a7, /// 0xbc8
			 0xef5d273b, /// 0xbcc
			 0xc68a37f5, /// 0xbd0
			 0x9b03dc89, /// 0xbd4
			 0x737fbfc9, /// 0xbd8
			 0x215de736, /// 0xbdc
			 0xcbb6183c, /// 0xbe0
			 0x3a13bf7d, /// 0xbe4
			 0xae431c94, /// 0xbe8
			 0x3f29f672, /// 0xbec
			 0x8a06c4f5, /// 0xbf0
			 0x841e7cac, /// 0xbf4
			 0xe28355b2, /// 0xbf8
			 0x21bc66a6, /// 0xbfc
			 0x9a4af5f6, /// 0xc00
			 0xebda2756, /// 0xc04
			 0xb0e7f678, /// 0xc08
			 0x88a97dd6, /// 0xc0c
			 0xffe3d636, /// 0xc10
			 0x1b3f10d7, /// 0xc14
			 0x8b25f2d4, /// 0xc18
			 0xb11dbaef, /// 0xc1c
			 0x96085961, /// 0xc20
			 0x2814025d, /// 0xc24
			 0xda7e89d2, /// 0xc28
			 0xe868cbd2, /// 0xc2c
			 0x643d5d73, /// 0xc30
			 0xdca31491, /// 0xc34
			 0x6573e553, /// 0xc38
			 0x0da344df, /// 0xc3c
			 0xfefa1838, /// 0xc40
			 0xaba2c761, /// 0xc44
			 0x0bd313c2, /// 0xc48
			 0xe22c0fb1, /// 0xc4c
			 0x849a30e4, /// 0xc50
			 0x14bd2d70, /// 0xc54
			 0x1554187d, /// 0xc58
			 0x8f9b8d89, /// 0xc5c
			 0x0b0f62b3, /// 0xc60
			 0x45f82cff, /// 0xc64
			 0xca418e60, /// 0xc68
			 0xd96b2dcc, /// 0xc6c
			 0xdd22882a, /// 0xc70
			 0x3ad7cfef, /// 0xc74
			 0x993c44a5, /// 0xc78
			 0x028b5795, /// 0xc7c
			 0x74757bd5, /// 0xc80
			 0x9b59242e, /// 0xc84
			 0xaf014734, /// 0xc88
			 0xaefab434, /// 0xc8c
			 0x75158681, /// 0xc90
			 0x56d9401a, /// 0xc94
			 0xc3e35b90, /// 0xc98
			 0xad5dd9ee, /// 0xc9c
			 0x7fc3fc1a, /// 0xca0
			 0xb8fa73d3, /// 0xca4
			 0x720f576e, /// 0xca8
			 0xb8ea91c4, /// 0xcac
			 0xd20c9b92, /// 0xcb0
			 0x42c72c27, /// 0xcb4
			 0x1b49c6e7, /// 0xcb8
			 0x7882942c, /// 0xcbc
			 0x97828b04, /// 0xcc0
			 0x2d6bb635, /// 0xcc4
			 0x1f4b3e43, /// 0xcc8
			 0x665c0dc1, /// 0xccc
			 0x409c922f, /// 0xcd0
			 0x943e9980, /// 0xcd4
			 0xcb10602b, /// 0xcd8
			 0x501c4dbf, /// 0xcdc
			 0xc9c23e98, /// 0xce0
			 0x4fe7353a, /// 0xce4
			 0x55f1b05d, /// 0xce8
			 0xc070239d, /// 0xcec
			 0x6fac718b, /// 0xcf0
			 0xa28fd7e6, /// 0xcf4
			 0x5310cddf, /// 0xcf8
			 0x79419dee, /// 0xcfc
			 0x5a2bff07, /// 0xd00
			 0xdc0d982e, /// 0xd04
			 0x8b38ea1a, /// 0xd08
			 0xcca36746, /// 0xd0c
			 0xe38735f3, /// 0xd10
			 0xc2f2aec2, /// 0xd14
			 0x8d8b7f2e, /// 0xd18
			 0x46313745, /// 0xd1c
			 0xf7a52de9, /// 0xd20
			 0xfde4285d, /// 0xd24
			 0xb5e16b7d, /// 0xd28
			 0x02f1865a, /// 0xd2c
			 0x1bf732f0, /// 0xd30
			 0x15a7209d, /// 0xd34
			 0xf49cec7b, /// 0xd38
			 0x58fb57a8, /// 0xd3c
			 0x75206e53, /// 0xd40
			 0x4ab194b4, /// 0xd44
			 0xa8229202, /// 0xd48
			 0x2bc6471b, /// 0xd4c
			 0x2c123ccc, /// 0xd50
			 0x06474f28, /// 0xd54
			 0x367bad61, /// 0xd58
			 0xa471e043, /// 0xd5c
			 0xe489bfe3, /// 0xd60
			 0x608b0a6e, /// 0xd64
			 0x8e2f8892, /// 0xd68
			 0xfcf5dbd2, /// 0xd6c
			 0xee272584, /// 0xd70
			 0xd9daa1f6, /// 0xd74
			 0xa8711107, /// 0xd78
			 0x1ec70261, /// 0xd7c
			 0x76dd40e7, /// 0xd80
			 0xd610b7e0, /// 0xd84
			 0xe415a53a, /// 0xd88
			 0x9b5eda32, /// 0xd8c
			 0x48e11638, /// 0xd90
			 0x9a7d0bf4, /// 0xd94
			 0xd3cc3d98, /// 0xd98
			 0xcee6bc19, /// 0xd9c
			 0xdfc4814f, /// 0xda0
			 0x6527bbf6, /// 0xda4
			 0xbd795fe5, /// 0xda8
			 0xe846a103, /// 0xdac
			 0x59b4cb35, /// 0xdb0
			 0x4c401bf9, /// 0xdb4
			 0xd615cfcd, /// 0xdb8
			 0x9e2a94c1, /// 0xdbc
			 0xfec2cd80, /// 0xdc0
			 0x4e197244, /// 0xdc4
			 0x368dd5d7, /// 0xdc8
			 0xc78e333d, /// 0xdcc
			 0xc2b219fc, /// 0xdd0
			 0x1337a048, /// 0xdd4
			 0x29e54189, /// 0xdd8
			 0x4addfcde, /// 0xddc
			 0x05e10106, /// 0xde0
			 0x18673392, /// 0xde4
			 0x53e98928, /// 0xde8
			 0x5aafe4b3, /// 0xdec
			 0x943fe620, /// 0xdf0
			 0xb29787d2, /// 0xdf4
			 0xef804c28, /// 0xdf8
			 0xec1b0fe8, /// 0xdfc
			 0x5227b885, /// 0xe00
			 0x337bec6a, /// 0xe04
			 0x38fcb760, /// 0xe08
			 0xa1d06e73, /// 0xe0c
			 0xe518b201, /// 0xe10
			 0x102cac3f, /// 0xe14
			 0x74fcf12b, /// 0xe18
			 0xc008f0e3, /// 0xe1c
			 0xd7e95de4, /// 0xe20
			 0x54e9999b, /// 0xe24
			 0xb27babd2, /// 0xe28
			 0xb085b8bc, /// 0xe2c
			 0x4bb5cf26, /// 0xe30
			 0x5aa712ec, /// 0xe34
			 0xa296115f, /// 0xe38
			 0x45211e33, /// 0xe3c
			 0x2cb74b48, /// 0xe40
			 0x200e4737, /// 0xe44
			 0x05996334, /// 0xe48
			 0xd67792d7, /// 0xe4c
			 0x5482ad09, /// 0xe50
			 0xa7b4a27e, /// 0xe54
			 0x40a3886a, /// 0xe58
			 0xa590460f, /// 0xe5c
			 0xb21f9caa, /// 0xe60
			 0x8bbd56ea, /// 0xe64
			 0xf7b4ceff, /// 0xe68
			 0xc3347e42, /// 0xe6c
			 0x7568a282, /// 0xe70
			 0x112bb0d7, /// 0xe74
			 0xc73d8179, /// 0xe78
			 0x0e9643f3, /// 0xe7c
			 0xce6cdac1, /// 0xe80
			 0xbabb4e06, /// 0xe84
			 0x8f8af9a2, /// 0xe88
			 0x78f73520, /// 0xe8c
			 0x499c94a8, /// 0xe90
			 0x727876f9, /// 0xe94
			 0xd993c6c5, /// 0xe98
			 0x31c5208b, /// 0xe9c
			 0x0fce33d3, /// 0xea0
			 0xf63ba858, /// 0xea4
			 0x04eb22d7, /// 0xea8
			 0xbbb6f15b, /// 0xeac
			 0x4344ade1, /// 0xeb0
			 0x347f2550, /// 0xeb4
			 0xa335c789, /// 0xeb8
			 0xdeb614a4, /// 0xebc
			 0x3eb5d26f, /// 0xec0
			 0x1904d5ae, /// 0xec4
			 0x9364502f, /// 0xec8
			 0x9ccf2461, /// 0xecc
			 0x5327f34e, /// 0xed0
			 0x006ff235, /// 0xed4
			 0xcfd42624, /// 0xed8
			 0x64ac0947, /// 0xedc
			 0xf846baeb, /// 0xee0
			 0x904f8d77, /// 0xee4
			 0xdbe9f4be, /// 0xee8
			 0x403b4728, /// 0xeec
			 0x847a985c, /// 0xef0
			 0xa212ba9d, /// 0xef4
			 0x932f1c75, /// 0xef8
			 0xdc11b83e, /// 0xefc
			 0xc8f795ca, /// 0xf00
			 0x399f6ba9, /// 0xf04
			 0x17496176, /// 0xf08
			 0xbb113652, /// 0xf0c
			 0x030cb21f, /// 0xf10
			 0x917d1ac3, /// 0xf14
			 0xe7ff96b3, /// 0xf18
			 0x7aa3ea1c, /// 0xf1c
			 0x3268407c, /// 0xf20
			 0x34cea366, /// 0xf24
			 0xaf63ed7e, /// 0xf28
			 0xf42d6150, /// 0xf2c
			 0x591e635e, /// 0xf30
			 0xdccb1936, /// 0xf34
			 0x364d2dd6, /// 0xf38
			 0xab72d511, /// 0xf3c
			 0x146cb4f1, /// 0xf40
			 0x00108767, /// 0xf44
			 0xefc3e45c, /// 0xf48
			 0x93cc1b63, /// 0xf4c
			 0xefe1fbef, /// 0xf50
			 0xbd902745, /// 0xf54
			 0x056f2b86, /// 0xf58
			 0x54573dd5, /// 0xf5c
			 0x2a5f6b93, /// 0xf60
			 0x06a50a38, /// 0xf64
			 0x91f13f69, /// 0xf68
			 0xc299684f, /// 0xf6c
			 0xd20f15f6, /// 0xf70
			 0x25e33f42, /// 0xf74
			 0x84a16073, /// 0xf78
			 0xb523fcd5, /// 0xf7c
			 0xb0b23335, /// 0xf80
			 0xe918f58b, /// 0xf84
			 0x4b659d96, /// 0xf88
			 0x293747fa, /// 0xf8c
			 0xc1f51e65, /// 0xf90
			 0x54faf0d9, /// 0xf94
			 0x1903e7df, /// 0xf98
			 0x47c9aacc, /// 0xf9c
			 0x14adfe6f, /// 0xfa0
			 0x6dd6e8d5, /// 0xfa4
			 0x6a4a63ac, /// 0xfa8
			 0x2424e19d, /// 0xfac
			 0xa1625919, /// 0xfb0
			 0x5b3a1fa2, /// 0xfb4
			 0x300086d4, /// 0xfb8
			 0xdb8fd7e0, /// 0xfbc
			 0x79a97525, /// 0xfc0
			 0x11891a6c, /// 0xfc4
			 0x6b0afdcb, /// 0xfc8
			 0xd43a8fbb, /// 0xfcc
			 0xdbb545f5, /// 0xfd0
			 0xbca37d57, /// 0xfd4
			 0x04087302, /// 0xfd8
			 0x2ad28554, /// 0xfdc
			 0x7687f60a, /// 0xfe0
			 0x84fd8b51, /// 0xfe4
			 0xab4070c4, /// 0xfe8
			 0xe4aa249c, /// 0xfec
			 0x154c493d, /// 0xff0
			 0xdfad499a, /// 0xff4
			 0x10f8efba, /// 0xff8
			 0x2df60383 /// last
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
			 0x00000254,
			 0x000005d4,
			 0x00000254,
			 0x000001cc,
			 0x000005bc,
			 0x00000440,
			 0x000001c0,
			 0x00000364,

			 /// vpu register f2
			 0x41880000,
			 0x41900000,
			 0x41980000,
			 0x40400000,
			 0x40e00000,
			 0x41c80000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f3
			 0x41b80000,
			 0x3f800000,
			 0x41e00000,
			 0x41000000,
			 0x42000000,
			 0x41c80000,
			 0x3f800000,
			 0x41800000,

			 /// vpu register f4
			 0x41200000,
			 0x41200000,
			 0x41200000,
			 0x42000000,
			 0x41500000,
			 0x41500000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f5
			 0x41d00000,
			 0x41300000,
			 0x41100000,
			 0x41900000,
			 0x41d00000,
			 0x41d80000,
			 0x40400000,
			 0x41600000,

			 /// vpu register f6
			 0x41d80000,
			 0x41100000,
			 0x3f800000,
			 0x41500000,
			 0x41a80000,
			 0x41f80000,
			 0x3f800000,
			 0x41d00000,

			 /// vpu register f7
			 0x41500000,
			 0x41f80000,
			 0x41700000,
			 0x41c00000,
			 0x41c00000,
			 0x40800000,
			 0x41400000,
			 0x41800000,

			 /// vpu register f8
			 0x41900000,
			 0x3f800000,
			 0x40e00000,
			 0x41e80000,
			 0x41900000,
			 0x41e80000,
			 0x41e00000,
			 0x40c00000,

			 /// vpu register f9
			 0x41f80000,
			 0x40400000,
			 0x41200000,
			 0x3f800000,
			 0x41b00000,
			 0x41400000,
			 0x40000000,
			 0x41600000,

			 /// vpu register f10
			 0x40400000,
			 0x41e00000,
			 0x41880000,
			 0x41200000,
			 0x41d00000,
			 0x41b80000,
			 0x41b80000,
			 0x3f800000,

			 /// vpu register f11
			 0x40c00000,
			 0x41880000,
			 0x42000000,
			 0x41a80000,
			 0x3f800000,
			 0x41c80000,
			 0x41600000,
			 0x41700000,

			 /// vpu register f12
			 0x40c00000,
			 0x41b80000,
			 0x41a00000,
			 0x41d00000,
			 0x41100000,
			 0x41980000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f13
			 0x41d80000,
			 0x40400000,
			 0x41900000,
			 0x41c80000,
			 0x41900000,
			 0x41600000,
			 0x40400000,
			 0x40000000,

			 /// vpu register f14
			 0x41900000,
			 0x41000000,
			 0x41300000,
			 0x41300000,
			 0x40c00000,
			 0x41b80000,
			 0x40400000,
			 0x41500000,

			 /// vpu register f15
			 0x41000000,
			 0x41c00000,
			 0x41d00000,
			 0x41500000,
			 0x41d00000,
			 0x41b00000,
			 0x40800000,
			 0x41f00000,

			 /// vpu register f16
			 0x41a00000,
			 0x41f00000,
			 0x40400000,
			 0x41d80000,
			 0x41500000,
			 0x41e80000,
			 0x40000000,
			 0x41200000,

			 /// vpu register f17
			 0x41700000,
			 0x41980000,
			 0x41600000,
			 0x41800000,
			 0x40c00000,
			 0x41500000,
			 0x41b00000,
			 0x41700000,

			 /// vpu register f18
			 0x41900000,
			 0x41300000,
			 0x41900000,
			 0x41d00000,
			 0x42000000,
			 0x41700000,
			 0x41b80000,
			 0x41200000,

			 /// vpu register f19
			 0x41300000,
			 0x41880000,
			 0x40e00000,
			 0x41880000,
			 0x41300000,
			 0x41600000,
			 0x40400000,
			 0x41b00000,

			 /// vpu register f20
			 0x41600000,
			 0x41a00000,
			 0x41300000,
			 0x41e80000,
			 0x41200000,
			 0x40400000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f21
			 0x41d00000,
			 0x41f80000,
			 0x42000000,
			 0x41700000,
			 0x41900000,
			 0x40e00000,
			 0x40c00000,
			 0x41300000,

			 /// vpu register f22
			 0x41c00000,
			 0x41f80000,
			 0x41f80000,
			 0x40000000,
			 0x40400000,
			 0x3f800000,
			 0x41d80000,
			 0x41d80000,

			 /// vpu register f23
			 0x41a80000,
			 0x40e00000,
			 0x41a80000,
			 0x41c00000,
			 0x41700000,
			 0x40000000,
			 0x40a00000,
			 0x40800000,

			 /// vpu register f24
			 0x41980000,
			 0x41e00000,
			 0x41b00000,
			 0x40e00000,
			 0x41600000,
			 0x41400000,
			 0x41700000,
			 0x40800000,

			 /// vpu register f25
			 0x41a00000,
			 0x41300000,
			 0x40c00000,
			 0x41a00000,
			 0x41f00000,
			 0x41600000,
			 0x3f800000,
			 0x41d80000,

			 /// vpu register f26
			 0x41000000,
			 0x41a80000,
			 0x41900000,
			 0x41600000,
			 0x41600000,
			 0x41c00000,
			 0x40400000,
			 0x40c00000,

			 /// vpu register f27
			 0x40800000,
			 0x41c80000,
			 0x42000000,
			 0x41000000,
			 0x41a00000,
			 0x40400000,
			 0x40e00000,
			 0x41e00000,

			 /// vpu register f28
			 0x41f00000,
			 0x41a00000,
			 0x41d00000,
			 0x40000000,
			 0x41b80000,
			 0x41d00000,
			 0x41e80000,
			 0x41a00000,

			 /// vpu register f29
			 0x41800000,
			 0x41d00000,
			 0x41e80000,
			 0x41600000,
			 0x41880000,
			 0x40c00000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f30
			 0x41400000,
			 0x41700000,
			 0x42000000,
			 0x41600000,
			 0x41300000,
			 0x41200000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f31
			 0x41c00000,
			 0x40a00000,
			 0x41500000,
			 0x41e80000,
			 0x41980000,
			 0x41d00000,
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
		"fmsub.ps f16, f21, f5, f22, dyn\n"                   ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f25, f24, f24, f25, rtz\n"                  ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f8, f15, f25, f22, dyn\n"                   ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f0, f17, f7, rmm\n"                    ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f29, f28, f11, f16, rtz\n"                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f16, f18, f29, f12, rne\n"                  ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f26, f13, f11, f18, rtz\n"                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f24, f12, f4, f1, rne\n"                    ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f26, f14, f17, f10, rmm\n"                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f7, f16, f30, f13, rtz\n"                   ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f22, f17, f6, f23, rtz\n"                   ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f2, f13, f12, f14, rne\n"                   ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f8, f17, f21, f12, rmm\n"                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f13, f9, f18, f5, dyn\n"                    ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f16, f10, f26, f23, rne\n"                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f17, f10, f2, f25, rne\n"                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f9, f29, f21, f8, rdn\n"                    ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f8, f3, f6, f28, rtz\n"                     ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f21, f18, f29, f14, rdn\n"                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f4, f22, f2, f10, rup\n"                    ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f29, f21, f9, f18, rmm\n"                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f15, f18, f13, f24, rne\n"                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f14, f4, f26, rne\n"                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f2, f30, f22, f30, dyn\n"                   ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f2, f18, f14, f24, rmm\n"                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f12, f23, f5, rtz\n"                   ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f25, f15, f10, f17, rup\n"                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f19, f15, f0, f3, rdn\n"                    ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f25, f26, f13, rne\n"                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f16, f10, f5, f22, dyn\n"                   ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f6, f21, f16, dyn\n"                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f17, f20, f6, dyn\n"                   ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f15, f2, f1, f5, rdn\n"                     ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f28, f11, f7, f24, rdn\n"                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f10, f18, f28, f5, rup\n"                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f28, f4, f5, f10, rdn\n"                    ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f17, f21, f30, f26, dyn\n"                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f3, f7, f21, f30, rdn\n"                    ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f19, f12, f7, f17, rne\n"                   ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f30, f10, f14, f24, rne\n"                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f24, f9, f22, f25, rup\n"                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f16, f21, f19, f17, rne\n"                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f21, f29, f24, f22, dyn\n"                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f29, f4, f0, rmm\n"                    ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f30, f6, f0, f25, rne\n"                    ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f5, f7, f4, f17, rmm\n"                     ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f29, f2, f19, f15, rmm\n"                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f13, f5, f4, f21, rmm\n"                    ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f2, f4, f20, f28, rup\n"                    ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f15, f24, f15, f0, rtz\n"                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f6, f2, f7, f15, rdn\n"                     ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f11, f4, f20, f21, rtz\n"                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f20, f2, f12, f20, rmm\n"                   ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f26, f9, f26, f1, rne\n"                    ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f4, f7, f19, f18, dyn\n"                    ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f10, f20, f4, f2, rdn\n"                    ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f9, f21, f25, rmm\n"                   ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f29, f22, f29, f1, rdn\n"                   ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f14, f26, f29, f27, dyn\n"                  ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f6, f1, f16, f21, rne\n"                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f29, f5, f15, f20, rtz\n"                   ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f10, f12, f29, f25, rup\n"                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f24, f4, f1, f22, dyn\n"                    ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f27, f7, f26, rmm\n"                   ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f4, f0, f13, f2, rmm\n"                     ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f7, f21, f7, rmm\n"                    ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f15, f22, f6, f19, rup\n"                   ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f29, f24, f8, f18, dyn\n"                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f4, f26, f24, f7, rdn\n"                    ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f4, f11, f30, rne\n"                   ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f19, f12, f28, f23, rup\n"                  ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f26, f9, f5, f15, dyn\n"                    ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f8, f4, f26, f15, rne\n"                    ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f2, f3, f27, f25, rtz\n"                    ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f20, f8, f21, f5, rne\n"                    ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f17, f6, f8, f10, rtz\n"                    ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f26, f18, f19, f9, rup\n"                   ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f3, f18, f8, f20, rdn\n"                    ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f11, f7, f14, f14, rne\n"                   ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f30, f4, f30, f24, dyn\n"                   ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f15, f21, f22, f30, rmm\n"                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f5, f19, f16, f6, rdn\n"                    ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f6, f17, f28, f23, rne\n"                   ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f17, f18, f13, f8, dyn\n"                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f5, f10, f25, f21, dyn\n"                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f6, f16, f23, f6, rne\n"                    ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f13, f16, f30, f1, rmm\n"                   ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f6, f27, f13, f0, rne\n"                    ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f17, f3, f14, f2, rmm\n"                    ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f24, f11, f30, f22, rtz\n"                  ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f5, f21, f7, f15, rmm\n"                    ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f4, f24, f26, f29, rmm\n"                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f17, f30, f12, f6, dyn\n"                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f15, f15, f16, f30, rmm\n"                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f10, f12, f7, f8, rmm\n"                    ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f18, f5, f17, rtz\n"                   ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f25, f4, f24, f15, rdn\n"                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f27, f20, f8, f29, dyn\n"                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmsub.ps f23, f6, f12, f0, rtz\n"                    ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
