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
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00000
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00000000,                                                  // zero                                        /// 00008
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00010
			 0x80011111,                                                  // -9.7958E-41                                 /// 00014
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00018
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0001c
			 0x3f028f5c,                                                  // 0.51                                        /// 00020
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00024
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0003c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00044
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00050
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00054
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00058
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00060
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00064
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00068
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0006c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00070
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00074
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00078
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00080
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00084
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00088
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00090
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00094
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a8
			 0x4b000000,                                                  // 8388608.0                                   /// 000ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000b0
			 0x4b000000,                                                  // 8388608.0                                   /// 000b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0xffc00001,                                                  // -signaling NaN                              /// 000d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000e4
			 0xbf028f5c,                                                  // -0.51                                       /// 000e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00100
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00104
			 0x3f028f5c,                                                  // 0.51                                        /// 00108
			 0x0c400000,                                                  // Leading 1s:                                 /// 0010c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00110
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00114
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00118
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00124
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00128
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0012c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00130
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00140
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00144
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00148
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00154
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00158
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00160
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00164
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00170
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0017c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00180
			 0x0c780000,                                                  // Leading 1s:                                 /// 00184
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00188
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00190
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00198
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0019c
			 0x7fc00001,                                                  // signaling NaN                               /// 001a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00200
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00204
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00210
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0e000007,                                                  // Trailing 1s:                                /// 00218
			 0xffc00001,                                                  // -signaling NaN                              /// 0021c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00228
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0022c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00230
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00234
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00238
			 0xffc00001,                                                  // -signaling NaN                              /// 0023c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00240
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00244
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0024c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xcb000000,                                                  // -8388608.0                                  /// 00254
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00258
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0025c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00260
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00264
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00268
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0026c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00270
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00274
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00278
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0027c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00280
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00284
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00288
			 0xbf028f5c,                                                  // -0.51                                       /// 0028c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00290
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00294
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00298
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0029c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002a8
			 0x7fc00001,                                                  // signaling NaN                               /// 002ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0xffc00001,                                                  // -signaling NaN                              /// 002dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00304
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x80000000,                                                  // -zero                                       /// 00314
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00318
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00320
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00330
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00334
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00338
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0033c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00340
			 0x80000000,                                                  // -zero                                       /// 00344
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00348
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0034c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00350
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00354
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00358
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7f800000,                                                  // inf                                         /// 00364
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0036c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00374
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00384
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0038c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00394
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c600000,                                                  // Leading 1s:                                 /// 0039c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7fc00001,                                                  // signaling NaN                               /// 003d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00400
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00404
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00408
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00410
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00414
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00418
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00420
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00430
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00434
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00438
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0043c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00440
			 0x0c780000,                                                  // Leading 1s:                                 /// 00444
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0044c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00450
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00454
			 0x00000000,                                                  // zero                                        /// 00458
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0xcb000000,                                                  // -8388608.0                                  /// 00464
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x00011111,                                                  // 9.7958E-41                                  /// 00474
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00478
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0047c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00480
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00488
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0048c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00490
			 0x0e000001,                                                  // Trailing 1s:                                /// 00494
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00498
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0049c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00504
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00508
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0050c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00510
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00514
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00524
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00528
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0052c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00530
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00534
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0053c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00544
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00548
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0054c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00554
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00558
			 0x0c400000,                                                  // Leading 1s:                                 /// 0055c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00564
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00570
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00574
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00578
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0057c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00580
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00588
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00590
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00594
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0059c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c8
			 0x7fc00001,                                                  // signaling NaN                               /// 005cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7fc00001,                                                  // signaling NaN                               /// 005f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00600
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00604
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00608
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00610
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00618
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0061c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00620
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00624
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00628
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0062c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00634
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0063c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00640
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00644
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00648
			 0x0e000001,                                                  // Trailing 1s:                                /// 0064c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00650
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00658
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0065c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00660
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0066c
			 0xffc00001,                                                  // -signaling NaN                              /// 00670
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00674
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00678
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00680
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80000000,                                                  // -zero                                       /// 0068c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00690
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00694
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x33333333,                                                  // 4 random values                             /// 0069c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006a4
			 0x80000000,                                                  // -zero                                       /// 006a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00700
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00704
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0070c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00011111,                                                  // 9.7958E-41                                  /// 00718
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0071c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00724
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00730
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00734
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0073c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00740
			 0xffc00001,                                                  // -signaling NaN                              /// 00744
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0074c
			 0xbf028f5c,                                                  // -0.51                                       /// 00750
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00754
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00758
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80000000,                                                  // -zero                                       /// 00768
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00778
			 0x7fc00001,                                                  // signaling NaN                               /// 0077c
			 0xff800000,                                                  // -inf                                        /// 00780
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00784
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00788
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00790
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00794
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x4b000000,                                                  // 8388608.0                                   /// 007c8
			 0xffc00001,                                                  // -signaling NaN                              /// 007cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007d0
			 0x80000000,                                                  // -zero                                       /// 007d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00808
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0080c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00810
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00814
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00818
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00824
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00828
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0082c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00838
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0083c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00840
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00844
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00848
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00854
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00858
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0085c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00864
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00868
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00870
			 0x0c700000,                                                  // Leading 1s:                                 /// 00874
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00878
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00880
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00888
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x33333333,                                                  // 4 random values                             /// 00890
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00894
			 0x7f800000,                                                  // inf                                         /// 00898
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0089c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008b0
			 0x7fc00001,                                                  // signaling NaN                               /// 008b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x55555555,                                                  // 4 random values                             /// 008f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00904
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00908
			 0x00011111,                                                  // 9.7958E-41                                  /// 0090c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00910
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00914
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x0e000003,                                                  // Trailing 1s:                                /// 0091c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00920
			 0x00011111,                                                  // 9.7958E-41                                  /// 00924
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0092c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00930
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00934
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00940
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00948
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00950
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00954
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00964
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00968
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00970
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x00000000,                                                  // zero                                        /// 00978
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0097c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00984
			 0xbf028f5c,                                                  // -0.51                                       /// 00988
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0x3f028f5c,                                                  // 0.51                                        /// 00990
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00994
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0099c
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009a4
			 0xff800000,                                                  // -inf                                        /// 009a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c4
			 0x7fc00001,                                                  // signaling NaN                               /// 009c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00000000,                                                  // zero                                        /// 00a10
			 0xff800000,                                                  // -inf                                        /// 00a14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a18
			 0xffc00001,                                                  // -signaling NaN                              /// 00a1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x55555555,                                                  // 4 random values                             /// 00a40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00aa4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00abc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ac0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ac8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00acc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7f800000,                                                  // inf                                         /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00afc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x4b000000,                                                  // 8388608.0                                   /// 00b04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x7fc00001,                                                  // signaling NaN                               /// 00b14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80000000,                                                  // -zero                                       /// 00b30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b68
			 0x7f800000,                                                  // inf                                         /// 00b6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ba0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd8
			 0x00000000,                                                  // zero                                        /// 00bdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00be0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00be4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x33333333,                                                  // 4 random values                             /// 00bfc
			 0xffc00001,                                                  // -signaling NaN                              /// 00c00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c14
			 0x4b000000,                                                  // 8388608.0                                   /// 00c18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000000,                                                  // -zero                                       /// 00c70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c84
			 0x80000000,                                                  // -zero                                       /// 00c88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cc4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cf0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cf8
			 0x55555555,                                                  // 4 random values                             /// 00cfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d78
			 0x7f800000,                                                  // inf                                         /// 00d7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00db4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00db8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de4
			 0x3f028f5c,                                                  // 0.51                                        /// 00de8
			 0x33333333,                                                  // 4 random values                             /// 00dec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec0
			 0x00000000,                                                  // zero                                        /// 00ec4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ecc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ed0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xff800000,                                                  // -inf                                        /// 00f08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f14
			 0xcb000000,                                                  // -8388608.0                                  /// 00f18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fa4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fbc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fc4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fdc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ff0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7ffe00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00004
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00008
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00010
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00014
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0001c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00020
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00030
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00034
			 0x7fc00001,                                                  // signaling NaN                               /// 00038
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0003c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00044
			 0x00011111,                                                  // 9.7958E-41                                  /// 00048
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00050
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00058
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0005c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00068
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80000000,                                                  // -zero                                       /// 00074
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e000007,                                                  // Trailing 1s:                                /// 0007c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00080
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0008c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00090
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0009c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000e0
			 0x7fc00001,                                                  // signaling NaN                               /// 000e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00104
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00108
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00110
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00118
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00120
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00128
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00130
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00134
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0013c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00144
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00148
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0014c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00150
			 0x33333333,                                                  // 4 random values                             /// 00154
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00158
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0015c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00170
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00174
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00178
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0017c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00180
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00011111,                                                  // 9.7958E-41                                  /// 00188
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00190
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00198
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00204
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xffc00001,                                                  // -signaling NaN                              /// 0020c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00210
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00218
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0021c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00220
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00224
			 0x0c780000,                                                  // Leading 1s:                                 /// 00228
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0022c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00230
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00234
			 0x7f800000,                                                  // inf                                         /// 00238
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0023c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00248
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0024c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00250
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00254
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00258
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0025c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00260
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00264
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0026c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00274
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00278
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00000000,                                                  // zero                                        /// 00280
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00284
			 0x3f028f5c,                                                  // 0.51                                        /// 00288
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00290
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00294
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00298
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0029c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00304
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00308
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00310
			 0x00011111,                                                  // 9.7958E-41                                  /// 00314
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00318
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x33333333,                                                  // 4 random values                             /// 00320
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00328
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00334
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00338
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00340
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00344
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00348
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00354
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00358
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c600000,                                                  // Leading 1s:                                 /// 00364
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x33333333,                                                  // 4 random values                             /// 00374
			 0x0c600000,                                                  // Leading 1s:                                 /// 00378
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0037c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00380
			 0x0e000007,                                                  // Trailing 1s:                                /// 00384
			 0x00000000,                                                  // zero                                        /// 00388
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0038c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00390
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00394
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00398
			 0x33333333,                                                  // 4 random values                             /// 0039c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a0
			 0x80000000,                                                  // -zero                                       /// 003a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003c0
			 0xbf028f5c,                                                  // -0.51                                       /// 003c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00400
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00404
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00410
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00418
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0041c
			 0x7f800000,                                                  // inf                                         /// 00420
			 0x00011111,                                                  // 9.7958E-41                                  /// 00424
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00428
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00430
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00438
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00440
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00444
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00450
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00458
			 0x3f028f5c,                                                  // 0.51                                        /// 0045c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00460
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00464
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00468
			 0x0c400000,                                                  // Leading 1s:                                 /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00470
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00474
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0047c
			 0xff800000,                                                  // -inf                                        /// 00480
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00484
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00488
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0048c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00498
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0049c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xff800000,                                                  // -inf                                        /// 004b0
			 0xbf028f5c,                                                  // -0.51                                       /// 004b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004cc
			 0x33333333,                                                  // 4 random values                             /// 004d0
			 0xbf028f5c,                                                  // -0.51                                       /// 004d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00500
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00504
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80000000,                                                  // -zero                                       /// 0050c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00011111,                                                  // 9.7958E-41                                  /// 00518
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00524
			 0xcb000000,                                                  // -8388608.0                                  /// 00528
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0052c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00530
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00534
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00538
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00544
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00548
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00554
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00558
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80000000,                                                  // -zero                                       /// 00564
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0056c
			 0x4b000000,                                                  // 8388608.0                                   /// 00570
			 0x7fc00001,                                                  // signaling NaN                               /// 00574
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00578
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0057c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00580
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0058c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00590
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0059c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00608
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00614
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0061c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00620
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0062c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00630
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0064c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00650
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00660
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00664
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00668
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0066c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00670
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0067c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00684
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0068c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00694
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0069c
			 0x80000000,                                                  // -zero                                       /// 006a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ac
			 0x00000000,                                                  // zero                                        /// 006b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006e0
			 0xffc00001,                                                  // -signaling NaN                              /// 006e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00700
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0070c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00710
			 0x55555555,                                                  // 4 random values                             /// 00714
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00724
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00728
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00734
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00000000,                                                  // zero                                        /// 0073c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00744
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0074c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00750
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00754
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00758
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0075c
			 0x3f028f5c,                                                  // 0.51                                        /// 00760
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00768
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00770
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00774
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00778
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0077c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00780
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00790
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00794
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00798
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007ac
			 0x7f800000,                                                  // inf                                         /// 007b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00800
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x55555555,                                                  // 4 random values                             /// 0080c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00820
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00824
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00828
			 0x33333333,                                                  // 4 random values                             /// 0082c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00830
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00834
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00838
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x33333333,                                                  // 4 random values                             /// 00844
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00850
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00854
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00858
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0xff800000,                                                  // -inf                                        /// 00870
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00874
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0087c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00884
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00888
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0088c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00890
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00894
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00898
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f4
			 0xbf028f5c,                                                  // -0.51                                       /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00900
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xbf028f5c,                                                  // -0.51                                       /// 00908
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00914
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00918
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00924
			 0x0e000003,                                                  // Trailing 1s:                                /// 00928
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0092c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00934
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0093c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00940
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x00000000,                                                  // zero                                        /// 0094c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00950
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0095c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00960
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00964
			 0x0c400000,                                                  // Leading 1s:                                 /// 00968
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00970
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00978
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00980
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00984
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00988
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0098c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00990
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00998
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0099c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f4
			 0x55555555,                                                  // 4 random values                             /// 009f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a08
			 0x55555555,                                                  // 4 random values                             /// 00a0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0xff800000,                                                  // -inf                                        /// 00aa4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ab0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ac0
			 0x00000000,                                                  // zero                                        /// 00ac4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ac8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00acc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ad8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x33333333,                                                  // 4 random values                             /// 00af0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00af4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b10
			 0x7fc00001,                                                  // signaling NaN                               /// 00b14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b20
			 0x7f800000,                                                  // inf                                         /// 00b24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x55555555,                                                  // 4 random values                             /// 00b60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b74
			 0xcb000000,                                                  // -8388608.0                                  /// 00b78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00be4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bf8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c08
			 0x7fc00001,                                                  // signaling NaN                               /// 00c0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c64
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ca8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ccc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cd8
			 0x33333333,                                                  // 4 random values                             /// 00cdc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cf0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x7f800000,                                                  // inf                                         /// 00d10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x3f028f5c,                                                  // 0.51                                        /// 00d84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00da0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00db0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dc0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ddc
			 0x00000000,                                                  // zero                                        /// 00de0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00de8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e00
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e08
			 0x80000000,                                                  // -zero                                       /// 00e0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x7f800000,                                                  // inf                                         /// 00e30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x7f800000,                                                  // inf                                         /// 00e4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e70
			 0x80000000,                                                  // -zero                                       /// 00e74
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ebc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ec8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ecc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ed0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ee4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00efc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x55555555,                                                  // 4 random values                             /// 00f24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f2c
			 0x7f800000,                                                  // inf                                         /// 00f30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f54
			 0xbf028f5c,                                                  // -0.51                                       /// 00f58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0xbf028f5c,                                                  // -0.51                                       /// 00f74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fe8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff8
			 0x00800000                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00000
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00004
			 0x0e000003,                                                  // Trailing 1s:                                /// 00008
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00014
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00018
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00020
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0c700000,                                                  // Leading 1s:                                 /// 0002c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00030
			 0x00000000,                                                  // zero                                        /// 00034
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0003c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00040
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00044
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00050
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00054
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00058
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00064
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00074
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00078
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00084
			 0xff800000,                                                  // -inf                                        /// 00088
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0008c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00094
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00098
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0009c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x55555555,                                                  // 4 random values                             /// 000e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00104
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00110
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0011c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00124
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0012c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00130
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00134
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00138
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0013c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00140
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00144
			 0x0c600000,                                                  // Leading 1s:                                 /// 00148
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00154
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0015c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x4b000000,                                                  // 8388608.0                                   /// 00164
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00168
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00170
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00178
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00180
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00188
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0019c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00208
			 0x55555555,                                                  // 4 random values                             /// 0020c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00210
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00214
			 0xff800000,                                                  // -inf                                        /// 00218
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00220
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0c600000,                                                  // Leading 1s:                                 /// 00228
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00000000,                                                  // zero                                        /// 00238
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0023c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00244
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00250
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00254
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00258
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00260
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00264
			 0x7f800000,                                                  // inf                                         /// 00268
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00278
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0027c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00280
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00284
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00288
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0028c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00290
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00294
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002a8
			 0xbf028f5c,                                                  // -0.51                                       /// 002ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002e4
			 0x4b000000,                                                  // 8388608.0                                   /// 002e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00300
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00304
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00308
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0030c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00310
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00324
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00328
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00330
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00334
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00338
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00348
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00354
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00358
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0035c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00360
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00364
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00368
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0036c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00370
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00378
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00380
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00384
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00388
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00390
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00394
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003d8
			 0x80000000,                                                  // -zero                                       /// 003dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x4b000000,                                                  // 8388608.0                                   /// 003f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00400
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00408
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x3f028f5c,                                                  // 0.51                                        /// 00410
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00414
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00418
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0041c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00424
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00428
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00430
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00434
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00444
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00448
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00450
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00454
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00458
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0045c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00464
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0046c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00478
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00480
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00488
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00498
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0049c
			 0xbf028f5c,                                                  // -0.51                                       /// 004a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00504
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00508
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0050c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00510
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00514
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00518
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0051c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00520
			 0x0e000001,                                                  // Trailing 1s:                                /// 00524
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00528
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00530
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0053c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00558
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00560
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x7f800000,                                                  // inf                                         /// 00568
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0056c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00570
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00574
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00578
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0xbf028f5c,                                                  // -0.51                                       /// 00580
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00588
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00590
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00594
			 0x00011111,                                                  // 9.7958E-41                                  /// 00598
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005ec
			 0x55555555,                                                  // 4 random values                             /// 005f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00600
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00604
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0060c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00610
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00614
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00618
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00620
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00624
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00628
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0062c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00630
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00634
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00638
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0063c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00640
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00650
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00654
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00658
			 0xff800000,                                                  // -inf                                        /// 0065c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00660
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00664
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00668
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00678
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0067c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0068c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00690
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00694
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00698
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0069c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0xffc00001,                                                  // -signaling NaN                              /// 006f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0x7fc00001,                                                  // signaling NaN                               /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00704
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x7fc00001,                                                  // signaling NaN                               /// 0070c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00714
			 0x0c600000,                                                  // Leading 1s:                                 /// 00718
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0071c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00720
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00724
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00728
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00730
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00734
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x55555555,                                                  // 4 random values                             /// 0073c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00740
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00748
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00750
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00760
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00764
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00778
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00784
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00788
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00794
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0c600000,                                                  // Leading 1s:                                 /// 0079c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0xcb000000,                                                  // -8388608.0                                  /// 007c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00804
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00808
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00810
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00814
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00834
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00838
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00840
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00844
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00848
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0084c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00850
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00854
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00858
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0085c
			 0x80000000,                                                  // -zero                                       /// 00860
			 0x00000000,                                                  // zero                                        /// 00864
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00868
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0086c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00874
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x3f028f5c,                                                  // 0.51                                        /// 0087c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00880
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00888
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00890
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00894
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00898
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00900
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00904
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00908
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00910
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00918
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0091c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00924
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0092c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00934
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00938
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0093c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00940
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00944
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00948
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00950
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00958
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0095c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xbf028f5c,                                                  // -0.51                                       /// 00964
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00968
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0096c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00978
			 0x0e000007,                                                  // Trailing 1s:                                /// 0097c
			 0xff800000,                                                  // -inf                                        /// 00980
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0098c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00994
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c8
			 0x7f800000,                                                  // inf                                         /// 009cc
			 0x00000000,                                                  // zero                                        /// 009d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80000000,                                                  // -zero                                       /// 00a54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00aa4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aac
			 0x7f800000,                                                  // inf                                         /// 00ab0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00abc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ac0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ad0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00adc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00af4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00af8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00afc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b58
			 0xffc00001,                                                  // -signaling NaN                              /// 00b5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b60
			 0xffc00001,                                                  // -signaling NaN                              /// 00b64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0xcb000000,                                                  // -8388608.0                                  /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xffc00001,                                                  // -signaling NaN                              /// 00b8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bd4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xcb000000,                                                  // -8388608.0                                  /// 00c14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ccc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ce4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ce8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xff800000,                                                  // -inf                                        /// 00d44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0xff800000,                                                  // -inf                                        /// 00d98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00da4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x33333333,                                                  // 4 random values                             /// 00dc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00de0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00de4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00de8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dfc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xcb000000,                                                  // -8388608.0                                  /// 00e08
			 0x00000000,                                                  // zero                                        /// 00e0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x80000000,                                                  // -zero                                       /// 00e30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ebc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x7fc00001,                                                  // signaling NaN                               /// 00f00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x7f800000,                                                  // inf                                         /// 00f08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x80000000,                                                  // -zero                                       /// 00f1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0xff800000,                                                  // -inf                                        /// 00f28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f64
			 0x00000000,                                                  // zero                                        /// 00f68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fb4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fdc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ff4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ff8
			 0x80800002                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x3c11d4ee, /// 0x0
			 0x19a2e373, /// 0x4
			 0x142e07cb, /// 0x8
			 0xd0c86deb, /// 0xc
			 0x336c9dac, /// 0x10
			 0x5711d369, /// 0x14
			 0x894e6edc, /// 0x18
			 0x2fa249af, /// 0x1c
			 0xbbb1d136, /// 0x20
			 0xb275d098, /// 0x24
			 0xf5f9b907, /// 0x28
			 0x57f84b99, /// 0x2c
			 0xfb557f02, /// 0x30
			 0xcb9518e4, /// 0x34
			 0xaca0bc34, /// 0x38
			 0x44750360, /// 0x3c
			 0x79990172, /// 0x40
			 0x33900cbd, /// 0x44
			 0x60b93619, /// 0x48
			 0xf8b6645a, /// 0x4c
			 0x4fcb0d3a, /// 0x50
			 0xb03d4284, /// 0x54
			 0xd9334324, /// 0x58
			 0xc820d409, /// 0x5c
			 0x38b6a3cf, /// 0x60
			 0x5e5fc0c4, /// 0x64
			 0xc25660f2, /// 0x68
			 0x789bc98e, /// 0x6c
			 0x0b1bf3d0, /// 0x70
			 0xfa1cf065, /// 0x74
			 0x201c6820, /// 0x78
			 0xb5e63eb4, /// 0x7c
			 0x7f4da8f0, /// 0x80
			 0x0057883b, /// 0x84
			 0x4e43eed4, /// 0x88
			 0x52d5c66f, /// 0x8c
			 0x334532a6, /// 0x90
			 0x561c724c, /// 0x94
			 0x6d8bfeb2, /// 0x98
			 0xb1cd78e7, /// 0x9c
			 0x0bce0f1e, /// 0xa0
			 0x5801e5a9, /// 0xa4
			 0x1df167d6, /// 0xa8
			 0xdce4ffe9, /// 0xac
			 0xa6d2c639, /// 0xb0
			 0x9c1e5a55, /// 0xb4
			 0xaf752f52, /// 0xb8
			 0xaa77730b, /// 0xbc
			 0x3c4d0b78, /// 0xc0
			 0x452c5962, /// 0xc4
			 0x00536032, /// 0xc8
			 0xb205b942, /// 0xcc
			 0x76e05334, /// 0xd0
			 0xd3c2ac71, /// 0xd4
			 0x1d469b50, /// 0xd8
			 0xd48dcad9, /// 0xdc
			 0xb3a200e4, /// 0xe0
			 0x5a883d85, /// 0xe4
			 0xed027010, /// 0xe8
			 0xb1885141, /// 0xec
			 0xe9ef1160, /// 0xf0
			 0x94231c98, /// 0xf4
			 0xc0aee92f, /// 0xf8
			 0x050185dc, /// 0xfc
			 0xd61efbc8, /// 0x100
			 0xfc28c485, /// 0x104
			 0xae50ad68, /// 0x108
			 0xc2459873, /// 0x10c
			 0xf026ea44, /// 0x110
			 0x771d3414, /// 0x114
			 0x0f144a82, /// 0x118
			 0x5f122597, /// 0x11c
			 0x7d1104d7, /// 0x120
			 0x85a228b5, /// 0x124
			 0x9ec8f7fa, /// 0x128
			 0x4d8776d7, /// 0x12c
			 0x0ac5edf8, /// 0x130
			 0x26833b16, /// 0x134
			 0x51b1db66, /// 0x138
			 0x70370582, /// 0x13c
			 0xb7067659, /// 0x140
			 0x9b3627ae, /// 0x144
			 0x465b325f, /// 0x148
			 0xa4e25847, /// 0x14c
			 0x62fd2657, /// 0x150
			 0xb992292f, /// 0x154
			 0xc5be0633, /// 0x158
			 0x48f3ff72, /// 0x15c
			 0xdcaa2c52, /// 0x160
			 0x5f2bbc92, /// 0x164
			 0xbf4fca9c, /// 0x168
			 0xa517483f, /// 0x16c
			 0x4e1d54eb, /// 0x170
			 0x551f61fc, /// 0x174
			 0xf27fe4e0, /// 0x178
			 0x0db2f9a1, /// 0x17c
			 0xca0f9a54, /// 0x180
			 0x96abb8a5, /// 0x184
			 0x9ba80eca, /// 0x188
			 0xfeeaa9a2, /// 0x18c
			 0x1f097f41, /// 0x190
			 0x0cc33fa0, /// 0x194
			 0x8801d854, /// 0x198
			 0x8b8213c1, /// 0x19c
			 0x96bb6d8f, /// 0x1a0
			 0x843cbb7f, /// 0x1a4
			 0x6d07463e, /// 0x1a8
			 0x2ac825ec, /// 0x1ac
			 0x308d23d0, /// 0x1b0
			 0x8be88b83, /// 0x1b4
			 0x4084b30c, /// 0x1b8
			 0x27edf7c2, /// 0x1bc
			 0xf24d95a3, /// 0x1c0
			 0xf9435f65, /// 0x1c4
			 0xe1aaea65, /// 0x1c8
			 0x2ac02298, /// 0x1cc
			 0x7499a0d5, /// 0x1d0
			 0xcb1a0d89, /// 0x1d4
			 0x190232c0, /// 0x1d8
			 0xf2c86d11, /// 0x1dc
			 0x7814f670, /// 0x1e0
			 0xf9a60acb, /// 0x1e4
			 0x1dcd3841, /// 0x1e8
			 0xb4bf773e, /// 0x1ec
			 0x6691765c, /// 0x1f0
			 0xe100e80f, /// 0x1f4
			 0x9aef2ec8, /// 0x1f8
			 0xd2ebf53e, /// 0x1fc
			 0x92903ea8, /// 0x200
			 0x49ab86ab, /// 0x204
			 0x0d50a33d, /// 0x208
			 0x48e0a898, /// 0x20c
			 0xa8877f7a, /// 0x210
			 0xc29ff919, /// 0x214
			 0x2d6aabfe, /// 0x218
			 0x5e598f05, /// 0x21c
			 0x9c6a14aa, /// 0x220
			 0xa094975e, /// 0x224
			 0x9b9b4a82, /// 0x228
			 0xdbdfe82c, /// 0x22c
			 0xe430ef68, /// 0x230
			 0xbe6c74b6, /// 0x234
			 0x9e5c0381, /// 0x238
			 0x7465d8a4, /// 0x23c
			 0x010101ed, /// 0x240
			 0x76c0ca7a, /// 0x244
			 0x60fe132c, /// 0x248
			 0x86352997, /// 0x24c
			 0x60f6730c, /// 0x250
			 0x0d7aa77f, /// 0x254
			 0x78d287d2, /// 0x258
			 0xd7916953, /// 0x25c
			 0x6d77905c, /// 0x260
			 0x031ebf1e, /// 0x264
			 0xa1696e7d, /// 0x268
			 0x32df523d, /// 0x26c
			 0x6b275196, /// 0x270
			 0xc92d6858, /// 0x274
			 0xb60373e8, /// 0x278
			 0x6209c2b7, /// 0x27c
			 0xbe3b1a5a, /// 0x280
			 0x58bef11b, /// 0x284
			 0x8541918f, /// 0x288
			 0x89b06844, /// 0x28c
			 0x023d3d8b, /// 0x290
			 0xebe09f50, /// 0x294
			 0xf47f914e, /// 0x298
			 0x4fa50f1a, /// 0x29c
			 0x3425ff38, /// 0x2a0
			 0xe03841d7, /// 0x2a4
			 0x9dac69b2, /// 0x2a8
			 0x537f1a7c, /// 0x2ac
			 0x38427248, /// 0x2b0
			 0x4cc9b8eb, /// 0x2b4
			 0xf56a316f, /// 0x2b8
			 0xafb810b6, /// 0x2bc
			 0x2e7860b7, /// 0x2c0
			 0x6f72d41f, /// 0x2c4
			 0x6d125fa4, /// 0x2c8
			 0x29425877, /// 0x2cc
			 0x9572f082, /// 0x2d0
			 0x562c6239, /// 0x2d4
			 0x5c6fd9fa, /// 0x2d8
			 0xa50239c1, /// 0x2dc
			 0x7a19c417, /// 0x2e0
			 0xe81ac9f0, /// 0x2e4
			 0x7b6e0159, /// 0x2e8
			 0xb86b1734, /// 0x2ec
			 0x361aad68, /// 0x2f0
			 0xbd98d566, /// 0x2f4
			 0x7b3912e2, /// 0x2f8
			 0x4d924a5e, /// 0x2fc
			 0x348650c0, /// 0x300
			 0x17b38b0e, /// 0x304
			 0xfe4faf82, /// 0x308
			 0x91fc85d8, /// 0x30c
			 0xaa031497, /// 0x310
			 0xf550af73, /// 0x314
			 0xf901d50f, /// 0x318
			 0x1408269b, /// 0x31c
			 0x0ec79521, /// 0x320
			 0x29faf483, /// 0x324
			 0x51f9e9f2, /// 0x328
			 0x48007ebd, /// 0x32c
			 0x539f7d0c, /// 0x330
			 0xc22db84e, /// 0x334
			 0x045c6f3f, /// 0x338
			 0x403d7198, /// 0x33c
			 0xda883f89, /// 0x340
			 0x456d7bad, /// 0x344
			 0x915f28bd, /// 0x348
			 0x5e93456d, /// 0x34c
			 0x4501afab, /// 0x350
			 0x9f051cd6, /// 0x354
			 0x2ecaeceb, /// 0x358
			 0xb8fc16a2, /// 0x35c
			 0x9fec9c9b, /// 0x360
			 0xdcc364d1, /// 0x364
			 0x05e506e6, /// 0x368
			 0x7ea6f473, /// 0x36c
			 0x5f59485b, /// 0x370
			 0x425615eb, /// 0x374
			 0x5d4e237c, /// 0x378
			 0xae89ee24, /// 0x37c
			 0x0783f7d9, /// 0x380
			 0x83805c0c, /// 0x384
			 0x8727fa47, /// 0x388
			 0x5fddd055, /// 0x38c
			 0xcd986abd, /// 0x390
			 0x1e830bf0, /// 0x394
			 0x89a995da, /// 0x398
			 0xeafffc6c, /// 0x39c
			 0x63946257, /// 0x3a0
			 0x72eb8ad3, /// 0x3a4
			 0x952f13a3, /// 0x3a8
			 0x9c2b2895, /// 0x3ac
			 0x81bdb46d, /// 0x3b0
			 0xd7f15735, /// 0x3b4
			 0x857cfc0e, /// 0x3b8
			 0x7aadba22, /// 0x3bc
			 0xbf168a4d, /// 0x3c0
			 0x20185856, /// 0x3c4
			 0xd6f8ba78, /// 0x3c8
			 0x91d73e7a, /// 0x3cc
			 0x4c64aad3, /// 0x3d0
			 0xe1364502, /// 0x3d4
			 0xb1c93f53, /// 0x3d8
			 0x4e963eba, /// 0x3dc
			 0x34c6d6a5, /// 0x3e0
			 0xa825a622, /// 0x3e4
			 0x1a9339de, /// 0x3e8
			 0xc3204fa2, /// 0x3ec
			 0x95c612cb, /// 0x3f0
			 0x83c8886e, /// 0x3f4
			 0x9bbdb947, /// 0x3f8
			 0x81517d7a, /// 0x3fc
			 0x2f4483d3, /// 0x400
			 0xafae11bc, /// 0x404
			 0xbe28f53b, /// 0x408
			 0x248daf6d, /// 0x40c
			 0x961ee933, /// 0x410
			 0xcbaa6cf9, /// 0x414
			 0x8c2228e9, /// 0x418
			 0x6d21f13e, /// 0x41c
			 0x6e85e34d, /// 0x420
			 0x5832b941, /// 0x424
			 0xd4edd5d5, /// 0x428
			 0xccd8f620, /// 0x42c
			 0xfba3a354, /// 0x430
			 0x11fe5108, /// 0x434
			 0x65daf14a, /// 0x438
			 0xc4bbd419, /// 0x43c
			 0xd5480159, /// 0x440
			 0xc659e483, /// 0x444
			 0x6d073aa1, /// 0x448
			 0xea473c63, /// 0x44c
			 0xac13bbc4, /// 0x450
			 0x3013c8a7, /// 0x454
			 0xab7e263a, /// 0x458
			 0xf539fccf, /// 0x45c
			 0x7637d6db, /// 0x460
			 0xd8468b57, /// 0x464
			 0x27e9d0be, /// 0x468
			 0x7cbfb03a, /// 0x46c
			 0x104c818a, /// 0x470
			 0x107766b2, /// 0x474
			 0x0b2405c7, /// 0x478
			 0x67c23d16, /// 0x47c
			 0xb8ab9e06, /// 0x480
			 0x647a0579, /// 0x484
			 0x72a8b3e9, /// 0x488
			 0xf0a3dc8d, /// 0x48c
			 0x8419f8e0, /// 0x490
			 0x060391a0, /// 0x494
			 0xcfecb1b7, /// 0x498
			 0x32a1c6f6, /// 0x49c
			 0x73764a17, /// 0x4a0
			 0x2b192778, /// 0x4a4
			 0x761bdc50, /// 0x4a8
			 0x9eacfef7, /// 0x4ac
			 0x5958dd24, /// 0x4b0
			 0x9af69f4c, /// 0x4b4
			 0x31d202cc, /// 0x4b8
			 0xce8ccfa1, /// 0x4bc
			 0x2713f0aa, /// 0x4c0
			 0xc5b11010, /// 0x4c4
			 0x8428a243, /// 0x4c8
			 0xb7b8ba88, /// 0x4cc
			 0x76397341, /// 0x4d0
			 0xa3dd361e, /// 0x4d4
			 0xb376b51c, /// 0x4d8
			 0x0097ef33, /// 0x4dc
			 0xff209767, /// 0x4e0
			 0x5ad8c1de, /// 0x4e4
			 0x8718c5c3, /// 0x4e8
			 0xe65d5af4, /// 0x4ec
			 0x02bd6d73, /// 0x4f0
			 0xc1c36bf7, /// 0x4f4
			 0xb5bc8328, /// 0x4f8
			 0xa481cdf0, /// 0x4fc
			 0x35fde901, /// 0x500
			 0x3b057921, /// 0x504
			 0x9e57f247, /// 0x508
			 0xf45e3d12, /// 0x50c
			 0xab6eb1d0, /// 0x510
			 0x57b41e57, /// 0x514
			 0xae7f4d06, /// 0x518
			 0xb928b897, /// 0x51c
			 0xebccc9d1, /// 0x520
			 0x4ed97b9f, /// 0x524
			 0x2fbc7e9a, /// 0x528
			 0x36182bb5, /// 0x52c
			 0x6243a5b2, /// 0x530
			 0x4336add9, /// 0x534
			 0x54019bc7, /// 0x538
			 0x2cfa10a4, /// 0x53c
			 0x61d6b465, /// 0x540
			 0x5322fd35, /// 0x544
			 0x38a1db52, /// 0x548
			 0x9c5c20c8, /// 0x54c
			 0xc9ef9c7a, /// 0x550
			 0x94d85500, /// 0x554
			 0x61ced3d2, /// 0x558
			 0x5105f6b8, /// 0x55c
			 0x6ce1b03a, /// 0x560
			 0x28cb8f88, /// 0x564
			 0xd771643b, /// 0x568
			 0xe587328f, /// 0x56c
			 0xb8709c9f, /// 0x570
			 0x9479b89c, /// 0x574
			 0x3cafd80e, /// 0x578
			 0x2dff3066, /// 0x57c
			 0x893ce470, /// 0x580
			 0x0562273a, /// 0x584
			 0x471adfe4, /// 0x588
			 0xed6d39f2, /// 0x58c
			 0x32af2bc5, /// 0x590
			 0xcf1f5afa, /// 0x594
			 0x2ce72b53, /// 0x598
			 0xbf6122fc, /// 0x59c
			 0x60f44c00, /// 0x5a0
			 0x9d05ccee, /// 0x5a4
			 0xc1c1753b, /// 0x5a8
			 0x6792c2b3, /// 0x5ac
			 0x61aee678, /// 0x5b0
			 0x4d95669f, /// 0x5b4
			 0xe12651df, /// 0x5b8
			 0x71a8014a, /// 0x5bc
			 0xfd20a107, /// 0x5c0
			 0x0b8f0887, /// 0x5c4
			 0x946f78a1, /// 0x5c8
			 0x95aca861, /// 0x5cc
			 0x52d93fcd, /// 0x5d0
			 0xc42ef5d1, /// 0x5d4
			 0x5458cde5, /// 0x5d8
			 0x905e0568, /// 0x5dc
			 0xde51aad5, /// 0x5e0
			 0x593982dd, /// 0x5e4
			 0x88a6e835, /// 0x5e8
			 0x63f7b80a, /// 0x5ec
			 0x9b8cd527, /// 0x5f0
			 0xadd46d0c, /// 0x5f4
			 0x0ef52dca, /// 0x5f8
			 0x6663a0f8, /// 0x5fc
			 0x914ba9c2, /// 0x600
			 0x0d1a0b37, /// 0x604
			 0x8a85d860, /// 0x608
			 0xd5ceef06, /// 0x60c
			 0xafe9517c, /// 0x610
			 0x29acc14c, /// 0x614
			 0x7fd01f78, /// 0x618
			 0x8c173c70, /// 0x61c
			 0x28626e62, /// 0x620
			 0x5f01ba42, /// 0x624
			 0xb708529e, /// 0x628
			 0x060ac439, /// 0x62c
			 0x60143e51, /// 0x630
			 0xb3aa3aa4, /// 0x634
			 0x3286f97c, /// 0x638
			 0x34fe364d, /// 0x63c
			 0x563db848, /// 0x640
			 0x79ce043e, /// 0x644
			 0xdb209083, /// 0x648
			 0x82893f64, /// 0x64c
			 0xcd626a54, /// 0x650
			 0x8136c1f3, /// 0x654
			 0xf6b09ae2, /// 0x658
			 0x49a69860, /// 0x65c
			 0x363a9b85, /// 0x660
			 0x313c13a9, /// 0x664
			 0x8dc903c2, /// 0x668
			 0x8a86b75a, /// 0x66c
			 0xe54f076f, /// 0x670
			 0x9807c590, /// 0x674
			 0x83cbfa91, /// 0x678
			 0x5f25334c, /// 0x67c
			 0x29397983, /// 0x680
			 0x7deb7c64, /// 0x684
			 0xc9a2048f, /// 0x688
			 0x85fb7817, /// 0x68c
			 0x55a0488e, /// 0x690
			 0x58a1d244, /// 0x694
			 0xf787faa0, /// 0x698
			 0x8bd48c0b, /// 0x69c
			 0x4c9f9eea, /// 0x6a0
			 0xa520dc5d, /// 0x6a4
			 0xad59c1e7, /// 0x6a8
			 0xec137d20, /// 0x6ac
			 0xfc19c7bb, /// 0x6b0
			 0x6617f26b, /// 0x6b4
			 0xd2d292e7, /// 0x6b8
			 0x1dfb6c5b, /// 0x6bc
			 0x8e7ae687, /// 0x6c0
			 0xb4c81c5a, /// 0x6c4
			 0x825c61ea, /// 0x6c8
			 0x65e75079, /// 0x6cc
			 0x148d24e0, /// 0x6d0
			 0x55225cc4, /// 0x6d4
			 0x6fc95da9, /// 0x6d8
			 0xb1c3a448, /// 0x6dc
			 0x884036b5, /// 0x6e0
			 0x9f3ed16a, /// 0x6e4
			 0xf908975e, /// 0x6e8
			 0x2330471e, /// 0x6ec
			 0x4258983b, /// 0x6f0
			 0xbd97c62d, /// 0x6f4
			 0xf2b10a08, /// 0x6f8
			 0x11b68e7f, /// 0x6fc
			 0xda8eabfa, /// 0x700
			 0x2e773c18, /// 0x704
			 0xcb02f388, /// 0x708
			 0xfc729316, /// 0x70c
			 0xc956a99a, /// 0x710
			 0xfc924c53, /// 0x714
			 0x7e799ea9, /// 0x718
			 0xeb4e0398, /// 0x71c
			 0xa0d7f75d, /// 0x720
			 0x899b6165, /// 0x724
			 0xad491af1, /// 0x728
			 0xacaab231, /// 0x72c
			 0xddd95c81, /// 0x730
			 0xf1448982, /// 0x734
			 0x77eca017, /// 0x738
			 0x2a0eba19, /// 0x73c
			 0xa9aa0fed, /// 0x740
			 0x739c34d3, /// 0x744
			 0xa2dcf4a6, /// 0x748
			 0x35a9cedc, /// 0x74c
			 0x75f63e71, /// 0x750
			 0xabde6a48, /// 0x754
			 0xd10e6ddc, /// 0x758
			 0x0d2b31c3, /// 0x75c
			 0x30d79d5d, /// 0x760
			 0xcfa7ba72, /// 0x764
			 0x64b2b8f3, /// 0x768
			 0x45b97e25, /// 0x76c
			 0xed6413a6, /// 0x770
			 0x1fc05e0a, /// 0x774
			 0xf8c56368, /// 0x778
			 0x00c4439d, /// 0x77c
			 0xaecff9ca, /// 0x780
			 0x1c8c4510, /// 0x784
			 0x2cc5efa0, /// 0x788
			 0x667dbe7b, /// 0x78c
			 0xedf613e7, /// 0x790
			 0x2104cbf7, /// 0x794
			 0xf8ce315e, /// 0x798
			 0x8004ada1, /// 0x79c
			 0xc388a113, /// 0x7a0
			 0x00554bca, /// 0x7a4
			 0xe3935ad5, /// 0x7a8
			 0x70f19ec3, /// 0x7ac
			 0x8e627647, /// 0x7b0
			 0x5775b9a2, /// 0x7b4
			 0xfe2acb46, /// 0x7b8
			 0x938f465b, /// 0x7bc
			 0xeee8883a, /// 0x7c0
			 0x5a407c5b, /// 0x7c4
			 0xae6be6c7, /// 0x7c8
			 0x373b57e6, /// 0x7cc
			 0xe71a1955, /// 0x7d0
			 0x46a0cd36, /// 0x7d4
			 0x749e6e1a, /// 0x7d8
			 0xa3d44783, /// 0x7dc
			 0x770a8533, /// 0x7e0
			 0xb670c3c0, /// 0x7e4
			 0x3d984724, /// 0x7e8
			 0xd5d0caeb, /// 0x7ec
			 0xa05cacd2, /// 0x7f0
			 0xe6cbb408, /// 0x7f4
			 0x1cdc809f, /// 0x7f8
			 0xc5ba6d85, /// 0x7fc
			 0x9bd5079c, /// 0x800
			 0x7f100f6f, /// 0x804
			 0x03f5c48e, /// 0x808
			 0x0c464745, /// 0x80c
			 0x858ac8b1, /// 0x810
			 0xffbab978, /// 0x814
			 0xe3cb0df2, /// 0x818
			 0xc9f107a6, /// 0x81c
			 0x436e1e00, /// 0x820
			 0xd519decb, /// 0x824
			 0xdd8b29d5, /// 0x828
			 0xc2e40487, /// 0x82c
			 0x4334af9a, /// 0x830
			 0xe4de8fd5, /// 0x834
			 0xed85b763, /// 0x838
			 0x21a14522, /// 0x83c
			 0xc1d1f66f, /// 0x840
			 0xed724f10, /// 0x844
			 0xd7358426, /// 0x848
			 0xf99836f4, /// 0x84c
			 0xde9bf8ce, /// 0x850
			 0xd4ba8135, /// 0x854
			 0x78f2a524, /// 0x858
			 0x689cedf5, /// 0x85c
			 0x731d9359, /// 0x860
			 0x9f5ae717, /// 0x864
			 0x5b4fb98a, /// 0x868
			 0xf40a5001, /// 0x86c
			 0x93cc37b1, /// 0x870
			 0xe5feb25d, /// 0x874
			 0x809c8262, /// 0x878
			 0xdd3e9d84, /// 0x87c
			 0xe8db6bc8, /// 0x880
			 0xa0e02664, /// 0x884
			 0xb5938131, /// 0x888
			 0x098f570c, /// 0x88c
			 0x2ad44dcb, /// 0x890
			 0x23e2eb94, /// 0x894
			 0x7a80c298, /// 0x898
			 0x29c6e500, /// 0x89c
			 0xebbaf46f, /// 0x8a0
			 0xaa458747, /// 0x8a4
			 0xbd4420aa, /// 0x8a8
			 0x5969cdf5, /// 0x8ac
			 0x790c41b7, /// 0x8b0
			 0x5f4afeb8, /// 0x8b4
			 0x372f51ab, /// 0x8b8
			 0x1b824e1e, /// 0x8bc
			 0x5e0ec1c1, /// 0x8c0
			 0x548799e1, /// 0x8c4
			 0x5ba6b794, /// 0x8c8
			 0x3479262b, /// 0x8cc
			 0xa66d62e0, /// 0x8d0
			 0x8386d0ed, /// 0x8d4
			 0xe977119e, /// 0x8d8
			 0xc5b2af15, /// 0x8dc
			 0xda254da1, /// 0x8e0
			 0x12803f74, /// 0x8e4
			 0x05a91077, /// 0x8e8
			 0x7923ee76, /// 0x8ec
			 0xd0e77ea6, /// 0x8f0
			 0xe6925c61, /// 0x8f4
			 0xf67bbd5e, /// 0x8f8
			 0xa56a9f0e, /// 0x8fc
			 0x5b4cb2e8, /// 0x900
			 0xab3979c8, /// 0x904
			 0x35e35191, /// 0x908
			 0x0358ee0a, /// 0x90c
			 0x86e1aa08, /// 0x910
			 0x2a937a61, /// 0x914
			 0x452a6f6e, /// 0x918
			 0x77fde8b5, /// 0x91c
			 0x23763993, /// 0x920
			 0xa74440fa, /// 0x924
			 0x3ec674ec, /// 0x928
			 0x1d42544f, /// 0x92c
			 0xba34f1f5, /// 0x930
			 0x58dd4cac, /// 0x934
			 0x7152d514, /// 0x938
			 0xa9357de9, /// 0x93c
			 0x4aa33b39, /// 0x940
			 0x34016a92, /// 0x944
			 0x5c5b864a, /// 0x948
			 0x1c87136d, /// 0x94c
			 0xb344069b, /// 0x950
			 0x726328c7, /// 0x954
			 0xfdb51265, /// 0x958
			 0xaa9d4438, /// 0x95c
			 0x657bd4e3, /// 0x960
			 0x4f8e8af0, /// 0x964
			 0x71a48b93, /// 0x968
			 0x5f77c335, /// 0x96c
			 0xa94dbf86, /// 0x970
			 0xed4952e0, /// 0x974
			 0x4bf00a31, /// 0x978
			 0xe6b6f19b, /// 0x97c
			 0x22d23980, /// 0x980
			 0xf3ce537f, /// 0x984
			 0x73c8c8b9, /// 0x988
			 0xebc93c96, /// 0x98c
			 0xb2ee3567, /// 0x990
			 0x56184db1, /// 0x994
			 0x9ce84f69, /// 0x998
			 0x58bb5f1c, /// 0x99c
			 0xac76a1c8, /// 0x9a0
			 0x6e43cd54, /// 0x9a4
			 0x349fb782, /// 0x9a8
			 0xc79c6edd, /// 0x9ac
			 0xfc390d0a, /// 0x9b0
			 0x5ac55dfb, /// 0x9b4
			 0x8e0cc563, /// 0x9b8
			 0x350b6cf5, /// 0x9bc
			 0x5f1b5bd6, /// 0x9c0
			 0x6620eeb9, /// 0x9c4
			 0x1fdd4172, /// 0x9c8
			 0x9ae17047, /// 0x9cc
			 0xc95232e0, /// 0x9d0
			 0x679763f7, /// 0x9d4
			 0x1ab2532d, /// 0x9d8
			 0xaad42334, /// 0x9dc
			 0xb7295f6f, /// 0x9e0
			 0xfc41d300, /// 0x9e4
			 0x18e37c4d, /// 0x9e8
			 0x7d83edeb, /// 0x9ec
			 0xc4a811de, /// 0x9f0
			 0x049d563e, /// 0x9f4
			 0xed0e0ecb, /// 0x9f8
			 0xf4832e7d, /// 0x9fc
			 0x9f4504db, /// 0xa00
			 0xa0cce322, /// 0xa04
			 0x3c3c04e8, /// 0xa08
			 0x0efb702c, /// 0xa0c
			 0xa164b111, /// 0xa10
			 0xb4c34514, /// 0xa14
			 0xe22fac59, /// 0xa18
			 0xbbda7ac2, /// 0xa1c
			 0xcb92bd44, /// 0xa20
			 0x4ce6bf72, /// 0xa24
			 0x3ee10e37, /// 0xa28
			 0x6223cd5f, /// 0xa2c
			 0x3b048c73, /// 0xa30
			 0xbec0a9b5, /// 0xa34
			 0xc5c6c4c4, /// 0xa38
			 0x7ab6b612, /// 0xa3c
			 0x8b6a50c0, /// 0xa40
			 0xceb2340a, /// 0xa44
			 0x90c61a85, /// 0xa48
			 0xd3814d59, /// 0xa4c
			 0x2e7d951c, /// 0xa50
			 0x4bce27b3, /// 0xa54
			 0x4624c799, /// 0xa58
			 0xb21c7316, /// 0xa5c
			 0xd3e23cba, /// 0xa60
			 0x92f7ba5a, /// 0xa64
			 0x44b5d311, /// 0xa68
			 0xf6c59cad, /// 0xa6c
			 0x356afe14, /// 0xa70
			 0x1a434474, /// 0xa74
			 0x0913e17a, /// 0xa78
			 0xa90ecc6d, /// 0xa7c
			 0x2e419754, /// 0xa80
			 0xb4af1d56, /// 0xa84
			 0xcc7c3007, /// 0xa88
			 0x03cb2c7e, /// 0xa8c
			 0xe38cdca5, /// 0xa90
			 0xe9cdeb44, /// 0xa94
			 0xbb244d12, /// 0xa98
			 0xdc3267f3, /// 0xa9c
			 0x2e0bd247, /// 0xaa0
			 0x00e65a4c, /// 0xaa4
			 0xbe76d670, /// 0xaa8
			 0x67bbd0a8, /// 0xaac
			 0xc8723b40, /// 0xab0
			 0xb2b0a342, /// 0xab4
			 0xd8c2cb90, /// 0xab8
			 0x368c9884, /// 0xabc
			 0xb1b1ed73, /// 0xac0
			 0x97743807, /// 0xac4
			 0xced1a953, /// 0xac8
			 0x87fda62b, /// 0xacc
			 0xaf5230cb, /// 0xad0
			 0xecceaee2, /// 0xad4
			 0xf94bc7dd, /// 0xad8
			 0xcb617466, /// 0xadc
			 0xbed4358e, /// 0xae0
			 0xa60e88b3, /// 0xae4
			 0xc182d2bb, /// 0xae8
			 0x2ac85414, /// 0xaec
			 0x25f2c8ac, /// 0xaf0
			 0xe3bffe3d, /// 0xaf4
			 0xac28f66f, /// 0xaf8
			 0x323b0771, /// 0xafc
			 0x68fec8c2, /// 0xb00
			 0xa0d0ea22, /// 0xb04
			 0xb603587a, /// 0xb08
			 0xda2fb319, /// 0xb0c
			 0xc2e847b5, /// 0xb10
			 0x022de2d5, /// 0xb14
			 0x458fc5a3, /// 0xb18
			 0xf45fad1f, /// 0xb1c
			 0x076b33a0, /// 0xb20
			 0x9329892d, /// 0xb24
			 0x710111c9, /// 0xb28
			 0x4b773edf, /// 0xb2c
			 0x4fecc344, /// 0xb30
			 0xd0e9ef06, /// 0xb34
			 0xc20e38bb, /// 0xb38
			 0x17bf4991, /// 0xb3c
			 0xc03dd60d, /// 0xb40
			 0x5dfaeadb, /// 0xb44
			 0x002b9bcf, /// 0xb48
			 0xd9fa86f8, /// 0xb4c
			 0xd3235cee, /// 0xb50
			 0x67ec4f4d, /// 0xb54
			 0xb697103b, /// 0xb58
			 0x023f2900, /// 0xb5c
			 0x84bd6945, /// 0xb60
			 0xa792ae02, /// 0xb64
			 0xb7b814f0, /// 0xb68
			 0xfcea354e, /// 0xb6c
			 0x12fa7dd0, /// 0xb70
			 0x24d08d01, /// 0xb74
			 0x84c25de8, /// 0xb78
			 0xd1006d23, /// 0xb7c
			 0x4dc5555d, /// 0xb80
			 0x3f7ae5c9, /// 0xb84
			 0xd6ca880e, /// 0xb88
			 0x46209485, /// 0xb8c
			 0x5c707a54, /// 0xb90
			 0xfd475db8, /// 0xb94
			 0x92452a37, /// 0xb98
			 0x247df10f, /// 0xb9c
			 0x80ae020d, /// 0xba0
			 0x15a42315, /// 0xba4
			 0x4642944a, /// 0xba8
			 0x1da9016f, /// 0xbac
			 0xe0d2303f, /// 0xbb0
			 0x4e8094ec, /// 0xbb4
			 0xbad26073, /// 0xbb8
			 0x2b4af558, /// 0xbbc
			 0x6345dbc1, /// 0xbc0
			 0x12862d77, /// 0xbc4
			 0xb7cf889f, /// 0xbc8
			 0x13c2cf22, /// 0xbcc
			 0xe0713b20, /// 0xbd0
			 0x78835c27, /// 0xbd4
			 0xaf126609, /// 0xbd8
			 0x7db68c8d, /// 0xbdc
			 0x948b220e, /// 0xbe0
			 0x240c5f54, /// 0xbe4
			 0x23a11bdb, /// 0xbe8
			 0x188b8aa0, /// 0xbec
			 0xdf2ab6dd, /// 0xbf0
			 0x1c6ea06a, /// 0xbf4
			 0x70564101, /// 0xbf8
			 0x21cb3465, /// 0xbfc
			 0x825f67b4, /// 0xc00
			 0x4d9a37d3, /// 0xc04
			 0x34b94413, /// 0xc08
			 0x0c8fd233, /// 0xc0c
			 0xbd7b9cab, /// 0xc10
			 0x5cefd22d, /// 0xc14
			 0xd0cba3cd, /// 0xc18
			 0xb74ac2c8, /// 0xc1c
			 0x95f038c7, /// 0xc20
			 0x43c26e67, /// 0xc24
			 0x03964dbd, /// 0xc28
			 0x3c3f68b1, /// 0xc2c
			 0x8a127a62, /// 0xc30
			 0x67a5f4bb, /// 0xc34
			 0x8db78494, /// 0xc38
			 0x6ebabc4d, /// 0xc3c
			 0xdf4d23c6, /// 0xc40
			 0x0c228391, /// 0xc44
			 0xa95d00b8, /// 0xc48
			 0xe19f7e58, /// 0xc4c
			 0xda05dc9f, /// 0xc50
			 0xd4f9b74d, /// 0xc54
			 0x7164ea1c, /// 0xc58
			 0x21102b2a, /// 0xc5c
			 0x5aae7c5b, /// 0xc60
			 0x08cf8c8c, /// 0xc64
			 0x225cdda8, /// 0xc68
			 0xffc3a4d4, /// 0xc6c
			 0xa04e5857, /// 0xc70
			 0x97929a4a, /// 0xc74
			 0xdcda6662, /// 0xc78
			 0x6650f751, /// 0xc7c
			 0x7e4b2fd1, /// 0xc80
			 0xf045d3b7, /// 0xc84
			 0x929cd391, /// 0xc88
			 0x1d9ea53a, /// 0xc8c
			 0x743a00b8, /// 0xc90
			 0x3fa49d4c, /// 0xc94
			 0x3bbd4234, /// 0xc98
			 0xaac96033, /// 0xc9c
			 0x12869f1b, /// 0xca0
			 0x26868588, /// 0xca4
			 0x6c5e8c03, /// 0xca8
			 0xe4fe6a77, /// 0xcac
			 0x01e7eaa6, /// 0xcb0
			 0x5e4a5db8, /// 0xcb4
			 0x866307ee, /// 0xcb8
			 0xeb3867d2, /// 0xcbc
			 0x72c27b7f, /// 0xcc0
			 0x3f04ab81, /// 0xcc4
			 0x0b68c3e5, /// 0xcc8
			 0xa00370f6, /// 0xccc
			 0xc87b901c, /// 0xcd0
			 0xd6743101, /// 0xcd4
			 0x5165a9bf, /// 0xcd8
			 0x4ed4526b, /// 0xcdc
			 0x530b5e03, /// 0xce0
			 0x91931155, /// 0xce4
			 0xf0f6e3a9, /// 0xce8
			 0x02c65081, /// 0xcec
			 0xfd580b15, /// 0xcf0
			 0x107225dc, /// 0xcf4
			 0x58d04a95, /// 0xcf8
			 0x53aa4cc4, /// 0xcfc
			 0xea7c2527, /// 0xd00
			 0x3abd349f, /// 0xd04
			 0x1ba858c1, /// 0xd08
			 0xf56ddf47, /// 0xd0c
			 0xeaaecb48, /// 0xd10
			 0x05d62ca4, /// 0xd14
			 0xd7331359, /// 0xd18
			 0xe6f460f6, /// 0xd1c
			 0x630bcc9e, /// 0xd20
			 0x511724b1, /// 0xd24
			 0x528af19a, /// 0xd28
			 0x2a7d2f0c, /// 0xd2c
			 0xaeaa72d1, /// 0xd30
			 0x91c5dc85, /// 0xd34
			 0x66186927, /// 0xd38
			 0xd727230c, /// 0xd3c
			 0x622f3076, /// 0xd40
			 0xa25c237e, /// 0xd44
			 0xc4da2241, /// 0xd48
			 0xb5771541, /// 0xd4c
			 0x840cff84, /// 0xd50
			 0x0e3e8e7a, /// 0xd54
			 0xe0c838a5, /// 0xd58
			 0xb40a5de7, /// 0xd5c
			 0x904aa7b9, /// 0xd60
			 0xbb955a97, /// 0xd64
			 0xb695eb05, /// 0xd68
			 0xc483cf02, /// 0xd6c
			 0x324735fd, /// 0xd70
			 0x5deb9757, /// 0xd74
			 0x85bce19b, /// 0xd78
			 0x3d2bab61, /// 0xd7c
			 0x57faf9d2, /// 0xd80
			 0xca15d6d3, /// 0xd84
			 0x3757dc76, /// 0xd88
			 0x2dde509b, /// 0xd8c
			 0x2400d7e3, /// 0xd90
			 0x0b667f7b, /// 0xd94
			 0x1cf478af, /// 0xd98
			 0x7adeb7f5, /// 0xd9c
			 0x2e0b360b, /// 0xda0
			 0x343714e5, /// 0xda4
			 0xe741bc16, /// 0xda8
			 0xc80d66ad, /// 0xdac
			 0x22b86487, /// 0xdb0
			 0x9a5939c4, /// 0xdb4
			 0x14fe9e99, /// 0xdb8
			 0xb97c35bf, /// 0xdbc
			 0xb14acf8e, /// 0xdc0
			 0xacdb496c, /// 0xdc4
			 0x1a191e4f, /// 0xdc8
			 0x3d9ecc19, /// 0xdcc
			 0x2a56ef4e, /// 0xdd0
			 0x083cfda6, /// 0xdd4
			 0x5a8149a7, /// 0xdd8
			 0x6bc28dd4, /// 0xddc
			 0x589ccce9, /// 0xde0
			 0x4e82b78c, /// 0xde4
			 0x073958f0, /// 0xde8
			 0xc9d1f663, /// 0xdec
			 0x7545cce5, /// 0xdf0
			 0x6ef8fd33, /// 0xdf4
			 0x921494be, /// 0xdf8
			 0x9197e234, /// 0xdfc
			 0xc514af58, /// 0xe00
			 0x1d6de0f3, /// 0xe04
			 0x0c75e291, /// 0xe08
			 0x71f41af0, /// 0xe0c
			 0x6a3fb78b, /// 0xe10
			 0x3d82c972, /// 0xe14
			 0xc15ee34a, /// 0xe18
			 0x45f8cbba, /// 0xe1c
			 0x25925014, /// 0xe20
			 0x30593d0c, /// 0xe24
			 0xe8fcf8b8, /// 0xe28
			 0x01791334, /// 0xe2c
			 0x6d4115fd, /// 0xe30
			 0xb997e368, /// 0xe34
			 0x532237ec, /// 0xe38
			 0x3fb1dea1, /// 0xe3c
			 0x5022ac9d, /// 0xe40
			 0xf033cb9b, /// 0xe44
			 0x607a6265, /// 0xe48
			 0x82bf509d, /// 0xe4c
			 0x3ae81132, /// 0xe50
			 0xcd68f23f, /// 0xe54
			 0x7b0bccc1, /// 0xe58
			 0xd9453905, /// 0xe5c
			 0x0aa78642, /// 0xe60
			 0xf02dc559, /// 0xe64
			 0x24436b8d, /// 0xe68
			 0xcab1242f, /// 0xe6c
			 0xb6fbcbaa, /// 0xe70
			 0x015be700, /// 0xe74
			 0xc1843e96, /// 0xe78
			 0x3217cf10, /// 0xe7c
			 0x8722208e, /// 0xe80
			 0x2a20ac8b, /// 0xe84
			 0x194c4cdb, /// 0xe88
			 0x28675c68, /// 0xe8c
			 0x27429136, /// 0xe90
			 0x393e3ef5, /// 0xe94
			 0x433b9da2, /// 0xe98
			 0xb0269a9b, /// 0xe9c
			 0x6f627204, /// 0xea0
			 0xed859c68, /// 0xea4
			 0x4dd905b1, /// 0xea8
			 0x1e7257d9, /// 0xeac
			 0x3266b248, /// 0xeb0
			 0xc9ca2af2, /// 0xeb4
			 0xe0994854, /// 0xeb8
			 0x8e9a1a69, /// 0xebc
			 0x0db310a0, /// 0xec0
			 0x4b0ef6bf, /// 0xec4
			 0x0a800901, /// 0xec8
			 0xbde0ddb7, /// 0xecc
			 0xa108333a, /// 0xed0
			 0x7813573a, /// 0xed4
			 0xa9ff20fe, /// 0xed8
			 0xfcbc0e6b, /// 0xedc
			 0xd859a282, /// 0xee0
			 0x6fa15b12, /// 0xee4
			 0x1c75498d, /// 0xee8
			 0x40c89484, /// 0xeec
			 0xb9b67ffc, /// 0xef0
			 0xca001429, /// 0xef4
			 0xff0712de, /// 0xef8
			 0xc1a43e8a, /// 0xefc
			 0xcbeb304f, /// 0xf00
			 0x8eb057e6, /// 0xf04
			 0xbca295ef, /// 0xf08
			 0xb3192252, /// 0xf0c
			 0x96b0fb85, /// 0xf10
			 0x4937e6f7, /// 0xf14
			 0x3dc648a6, /// 0xf18
			 0xff5aa99a, /// 0xf1c
			 0xb4d2705b, /// 0xf20
			 0x86159101, /// 0xf24
			 0xa3ccf7fc, /// 0xf28
			 0x67627c95, /// 0xf2c
			 0x0b8187e6, /// 0xf30
			 0xadc6cc09, /// 0xf34
			 0xb80b3fb0, /// 0xf38
			 0x57faba8f, /// 0xf3c
			 0x732154be, /// 0xf40
			 0x56896832, /// 0xf44
			 0x84a56e06, /// 0xf48
			 0x635ebce1, /// 0xf4c
			 0xadefafba, /// 0xf50
			 0x848e830d, /// 0xf54
			 0xa9593133, /// 0xf58
			 0x47d9f99a, /// 0xf5c
			 0x32cbd675, /// 0xf60
			 0xe2a18d5f, /// 0xf64
			 0x791ec625, /// 0xf68
			 0x365afa34, /// 0xf6c
			 0x3b1493f7, /// 0xf70
			 0xf53b27bc, /// 0xf74
			 0xdb007e67, /// 0xf78
			 0xcecba71b, /// 0xf7c
			 0x9ae49036, /// 0xf80
			 0xb65ec38d, /// 0xf84
			 0x3f11613b, /// 0xf88
			 0xc0d97b91, /// 0xf8c
			 0x4c206477, /// 0xf90
			 0x39b0d2ef, /// 0xf94
			 0x6255a2af, /// 0xf98
			 0x09c82c4a, /// 0xf9c
			 0x883953ea, /// 0xfa0
			 0xb324859f, /// 0xfa4
			 0x2438a378, /// 0xfa8
			 0xe9cd5ed4, /// 0xfac
			 0xc6f2edbd, /// 0xfb0
			 0x17464d5a, /// 0xfb4
			 0x1c4cfe64, /// 0xfb8
			 0xbd7b8e8d, /// 0xfbc
			 0xc88ea4cb, /// 0xfc0
			 0x7d6380b5, /// 0xfc4
			 0xe5c29154, /// 0xfc8
			 0xddc90896, /// 0xfcc
			 0xb88a9c14, /// 0xfd0
			 0xceb72e7f, /// 0xfd4
			 0x1c77c3dd, /// 0xfd8
			 0x3fcf1520, /// 0xfdc
			 0x6af9794b, /// 0xfe0
			 0xd23b18cd, /// 0xfe4
			 0xfe7dce7b, /// 0xfe8
			 0xa2323372, /// 0xfec
			 0xe4735873, /// 0xff0
			 0x4ea446c9, /// 0xff4
			 0xa95f90bb, /// 0xff8
			 0xc3712cef /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c700000,                                                  // Leading 1s:                                 /// 00000
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00008
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0000c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00014
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c600000,                                                  // Leading 1s:                                 /// 00024
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00028
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0002c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00030
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00034
			 0x80000000,                                                  // -zero                                       /// 00038
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00040
			 0xffc00001,                                                  // -signaling NaN                              /// 00044
			 0xbf028f5c,                                                  // -0.51                                       /// 00048
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0004c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00054
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0005c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00060
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00064
			 0x0e000007,                                                  // Trailing 1s:                                /// 00068
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00070
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0c780000,                                                  // Leading 1s:                                 /// 00078
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00080
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00088
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00090
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00094
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00000000,                                                  // zero                                        /// 0009c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00100
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00104
			 0x7fc00001,                                                  // signaling NaN                               /// 00108
			 0xffc00001,                                                  // -signaling NaN                              /// 0010c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00114
			 0x0c600000,                                                  // Leading 1s:                                 /// 00118
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0011c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00120
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00124
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00130
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00138
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00140
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00144
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00148
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0014c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00150
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00158
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0015c
			 0xff800000,                                                  // -inf                                        /// 00160
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00164
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00168
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x55555555,                                                  // 4 random values                             /// 00178
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00180
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00184
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0018c
			 0x3f028f5c,                                                  // 0.51                                        /// 00190
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00198
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x33333333,                                                  // 4 random values                             /// 001b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x3f028f5c,                                                  // 0.51                                        /// 001d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e8
			 0xff800000,                                                  // -inf                                        /// 001ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00204
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00208
			 0x7f800000,                                                  // inf                                         /// 0020c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00210
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00214
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x3f028f5c,                                                  // 0.51                                        /// 0021c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00220
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00224
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00228
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00230
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00234
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00244
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00248
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0024c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00250
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00258
			 0xbf028f5c,                                                  // -0.51                                       /// 0025c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0026c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00011111,                                                  // 9.7958E-41                                  /// 00274
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00278
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0027c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00280
			 0x4b000000,                                                  // 8388608.0                                   /// 00284
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00288
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0028c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00290
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00298
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0029c
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a8
			 0x80000000,                                                  // -zero                                       /// 002ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80000000,                                                  // -zero                                       /// 002c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 002e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0030c
			 0x80000000,                                                  // -zero                                       /// 00310
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00314
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0xcb000000,                                                  // -8388608.0                                  /// 0031c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00320
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00324
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00328
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0032c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00330
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00334
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0033c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00348
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0034c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x7f800000,                                                  // inf                                         /// 00358
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0035c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00368
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0036c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00370
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7f800000,                                                  // inf                                         /// 00378
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00384
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80000000,                                                  // -zero                                       /// 0038c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x7f800000,                                                  // inf                                         /// 00394
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00398
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0039c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b4
			 0xffc00001,                                                  // -signaling NaN                              /// 003b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003dc
			 0x7f800000,                                                  // inf                                         /// 003e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003f4
			 0x80000000,                                                  // -zero                                       /// 003f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00408
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00414
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00424
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0042c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00430
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00434
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00448
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00450
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00454
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0045c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00464
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0046c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00470
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00474
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00478
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00480
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00484
			 0x80000000,                                                  // -zero                                       /// 00488
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00490
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00494
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00498
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0049c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e4
			 0x7f800000,                                                  // inf                                         /// 004e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00504
			 0x00011111,                                                  // 9.7958E-41                                  /// 00508
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00514
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00520
			 0x33333333,                                                  // 4 random values                             /// 00524
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00528
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0e000001,                                                  // Trailing 1s:                                /// 00534
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00548
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0054c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00550
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00558
			 0x0c600000,                                                  // Leading 1s:                                 /// 0055c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00560
			 0x0c400000,                                                  // Leading 1s:                                 /// 00564
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00568
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0056c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00574
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00578
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0057c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00580
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00584
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00588
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0058c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e0
			 0x55555555,                                                  // 4 random values                             /// 005e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00608
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00610
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00614
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00624
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00628
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0062c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00638
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0063c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00648
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0064c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00650
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00658
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00660
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00664
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0066c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00670
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00674
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00688
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0068c
			 0x3f028f5c,                                                  // 0.51                                        /// 00690
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00698
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0069c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006a0
			 0xff800000,                                                  // -inf                                        /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00700
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00704
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00710
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00714
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00718
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00720
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00728
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00734
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00738
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00740
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7f800000,                                                  // inf                                         /// 00748
			 0x0e000007,                                                  // Trailing 1s:                                /// 0074c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xcb000000,                                                  // -8388608.0                                  /// 00754
			 0x0e000003,                                                  // Trailing 1s:                                /// 00758
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00764
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0xcb000000,                                                  // -8388608.0                                  /// 0076c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00770
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0077c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00780
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00784
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00788
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0078c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00790
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00794
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00798
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0079c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007d8
			 0x7f800000,                                                  // inf                                         /// 007dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00804
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00810
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00814
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00818
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0081c
			 0xffc00001,                                                  // -signaling NaN                              /// 00820
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00828
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00830
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00834
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0083c
			 0x80000000,                                                  // -zero                                       /// 00840
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00848
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0084c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00850
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00854
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00858
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0085c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00860
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00864
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00868
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00874
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00878
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00880
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00884
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00888
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0088c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00894
			 0xff800000,                                                  // -inf                                        /// 00898
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008a0
			 0xbf028f5c,                                                  // -0.51                                       /// 008a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008dc
			 0x33333333,                                                  // 4 random values                             /// 008e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00900
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000000,                                                  // zero                                        /// 00908
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0090c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00914
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00918
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0091c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80000000,                                                  // -zero                                       /// 00928
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0092c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00934
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00938
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0093c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00940
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0094c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00950
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00954
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00958
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0095c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00960
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00974
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00978
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0097c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00984
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00990
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00994
			 0xffc00001,                                                  // -signaling NaN                              /// 00998
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0099c
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a48
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a78
			 0x80000000,                                                  // -zero                                       /// 00a7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ab0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00acc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ad8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00adc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00afc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ba0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bd8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00be8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c10
			 0x7f800000,                                                  // inf                                         /// 00c14
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c24
			 0xbf028f5c,                                                  // -0.51                                       /// 00c28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c30
			 0x3f028f5c,                                                  // 0.51                                        /// 00c34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c50
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c68
			 0xffc00001,                                                  // -signaling NaN                              /// 00c6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c80
			 0x7f800000,                                                  // inf                                         /// 00c84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c98
			 0x00000000,                                                  // zero                                        /// 00c9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cb4
			 0xff800000,                                                  // -inf                                        /// 00cb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ccc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x80000000,                                                  // -zero                                       /// 00cf4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x7f800000,                                                  // inf                                         /// 00d54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d7c
			 0x00000000,                                                  // zero                                        /// 00d80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00da4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00db8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dcc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dfc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e24
			 0x3f028f5c,                                                  // 0.51                                        /// 00e28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e44
			 0x33333333,                                                  // 4 random values                             /// 00e48
			 0xcb000000,                                                  // -8388608.0                                  /// 00e4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ea0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ea8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00eb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ebc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ecc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00edc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ef8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00efc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f14
			 0x00000000,                                                  // zero                                        /// 00f18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x33333333,                                                  // 4 random values                             /// 00f2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x4b000000,                                                  // 8388608.0                                   /// 00f54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x7f800000,                                                  // inf                                         /// 00f70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x7fc00001,                                                  // signaling NaN                               /// 00f7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa4
			 0x80000000,                                                  // -zero                                       /// 00fa8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fac
			 0x55555555,                                                  // 4 random values                             /// 00fb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc0
			 0x80000000,                                                  // -zero                                       /// 00fc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fd4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff8
			 0xff7ffffe // max norm - 1ulp                               // max norm -ve                                /// last
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
			 0x00000370,
			 0x0000069c,
			 0x00000564,
			 0x0000035c,
			 0x00000798,
			 0x0000036c,
			 0x000002bc,
			 0x0000070c,

			 /// vpu register f2
			 0x41900000,
			 0x41a00000,
			 0x40c00000,
			 0x41700000,
			 0x41c80000,
			 0x41800000,
			 0x41400000,
			 0x41900000,

			 /// vpu register f3
			 0x41600000,
			 0x41980000,
			 0x41c00000,
			 0x41100000,
			 0x41880000,
			 0x41200000,
			 0x41e80000,
			 0x41f00000,

			 /// vpu register f4
			 0x41a00000,
			 0x41000000,
			 0x41700000,
			 0x41700000,
			 0x41c80000,
			 0x41f00000,
			 0x41b00000,
			 0x40e00000,

			 /// vpu register f5
			 0x41500000,
			 0x40400000,
			 0x41f00000,
			 0x3f800000,
			 0x41b80000,
			 0x40a00000,
			 0x41b80000,
			 0x41980000,

			 /// vpu register f6
			 0x41e80000,
			 0x40000000,
			 0x3f800000,
			 0x40c00000,
			 0x41c00000,
			 0x41b80000,
			 0x41900000,
			 0x41b00000,

			 /// vpu register f7
			 0x40e00000,
			 0x41500000,
			 0x41a80000,
			 0x41600000,
			 0x41a80000,
			 0x40800000,
			 0x40e00000,
			 0x41400000,

			 /// vpu register f8
			 0x40000000,
			 0x41e00000,
			 0x41c00000,
			 0x41f80000,
			 0x41b00000,
			 0x41980000,
			 0x41f80000,
			 0x41a00000,

			 /// vpu register f9
			 0x41100000,
			 0x41a80000,
			 0x41000000,
			 0x40e00000,
			 0x41c80000,
			 0x42000000,
			 0x42000000,
			 0x41a00000,

			 /// vpu register f10
			 0x40000000,
			 0x41880000,
			 0x3f800000,
			 0x41800000,
			 0x3f800000,
			 0x41700000,
			 0x41e80000,
			 0x40c00000,

			 /// vpu register f11
			 0x41800000,
			 0x41100000,
			 0x40c00000,
			 0x41980000,
			 0x41d80000,
			 0x41980000,
			 0x41a00000,
			 0x41d80000,

			 /// vpu register f12
			 0x40800000,
			 0x41800000,
			 0x41f80000,
			 0x41700000,
			 0x41e80000,
			 0x41800000,
			 0x41e00000,
			 0x40a00000,

			 /// vpu register f13
			 0x41100000,
			 0x41200000,
			 0x3f800000,
			 0x42000000,
			 0x41f00000,
			 0x41e00000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f14
			 0x41300000,
			 0x40800000,
			 0x41e00000,
			 0x40000000,
			 0x41400000,
			 0x40000000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f15
			 0x41f80000,
			 0x41300000,
			 0x41000000,
			 0x41880000,
			 0x40800000,
			 0x41880000,
			 0x41200000,
			 0x40c00000,

			 /// vpu register f16
			 0x41900000,
			 0x40400000,
			 0x40400000,
			 0x41000000,
			 0x41000000,
			 0x41a00000,
			 0x41200000,
			 0x41600000,

			 /// vpu register f17
			 0x41700000,
			 0x41d00000,
			 0x41300000,
			 0x41f00000,
			 0x40400000,
			 0x41d00000,
			 0x41980000,
			 0x41700000,

			 /// vpu register f18
			 0x41c80000,
			 0x40e00000,
			 0x40a00000,
			 0x41e80000,
			 0x41f00000,
			 0x41c00000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f19
			 0x42000000,
			 0x41200000,
			 0x41400000,
			 0x41b00000,
			 0x41880000,
			 0x41a00000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f20
			 0x42000000,
			 0x41c80000,
			 0x40c00000,
			 0x40000000,
			 0x41e80000,
			 0x41400000,
			 0x41600000,
			 0x41800000,

			 /// vpu register f21
			 0x41980000,
			 0x40a00000,
			 0x42000000,
			 0x40400000,
			 0x42000000,
			 0x41a00000,
			 0x41900000,
			 0x40800000,

			 /// vpu register f22
			 0x40c00000,
			 0x41000000,
			 0x3f800000,
			 0x41600000,
			 0x41880000,
			 0x41d80000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f23
			 0x41b00000,
			 0x41000000,
			 0x41880000,
			 0x40000000,
			 0x41400000,
			 0x41600000,
			 0x41100000,
			 0x41f00000,

			 /// vpu register f24
			 0x41f00000,
			 0x41400000,
			 0x41f80000,
			 0x41500000,
			 0x41b00000,
			 0x41c80000,
			 0x41400000,
			 0x41000000,

			 /// vpu register f25
			 0x3f800000,
			 0x41c00000,
			 0x41800000,
			 0x41a00000,
			 0x41600000,
			 0x41500000,
			 0x41e00000,
			 0x42000000,

			 /// vpu register f26
			 0x40800000,
			 0x41400000,
			 0x41400000,
			 0x41100000,
			 0x41d00000,
			 0x41a80000,
			 0x41c00000,
			 0x40e00000,

			 /// vpu register f27
			 0x41e80000,
			 0x40400000,
			 0x40000000,
			 0x41f80000,
			 0x41b80000,
			 0x41600000,
			 0x41980000,
			 0x40c00000,

			 /// vpu register f28
			 0x41700000,
			 0x41700000,
			 0x42000000,
			 0x41c00000,
			 0x41100000,
			 0x41c00000,
			 0x40000000,
			 0x41400000,

			 /// vpu register f29
			 0x41b80000,
			 0x41300000,
			 0x41880000,
			 0x41100000,
			 0x41900000,
			 0x41c80000,
			 0x41d80000,
			 0x41b00000,

			 /// vpu register f30
			 0x41600000,
			 0x41800000,
			 0x40000000,
			 0x40000000,
			 0x41800000,
			 0x41980000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f31
			 0x41880000,
			 0x41b00000,
			 0x41a80000,
			 0x41400000,
			 0x42000000,
			 0x41d80000,
			 0x41d00000,
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
		"fltm.pi m3, f14, f13\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f19, f18\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f4, f16\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f29, f9\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f22, f26\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f13, f18\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f5, f7\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f25, f11\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f10, f15\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f0, f27\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f17, f30\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f3, f7\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f5, f3\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f28, f18\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f15, f29\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f24, f28\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f20, f11\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f26, f10\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f25, f10\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f17, f26\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f0, f2\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f29, f3\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f26, f15\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f9, f27\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f1, f21\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f21, f29\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f25, f26\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f8, f30\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f26, f27\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f4, f2\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f5, f16\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f4, f8\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f9, f13\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f22, f12\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f29, f24\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f13, f25\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f11, f14\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f6, f30\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f24, f21\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f11, f26\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f16, f9\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f23, f15\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f22, f29\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f27, f20\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f17, f7\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f14, f24\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f0, f5\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f13, f5\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f26, f4\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f0, f8\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f9, f30\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f9, f16\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f24, f11\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f24, f14\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f12, f28\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f17, f28\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f24, f28\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f11, f5\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f19, f7\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f25, f0\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f17, f5\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f2, f26\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f14, f21\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f23, f15\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f30, f22\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f19, f7\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f6, f13\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m6, f0, f3\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f30, f15\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f2, f23\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f18, f25\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f12, f15\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f9, f18\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f8, f28\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f24, f17\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f11, f16\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f22, f8\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f19, f2\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f16, f25\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f19, f14\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m2, f5, f24\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f8, f29\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f26, f20\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f18, f2\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f16, f7\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f21, f25\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f11, f28\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f1, f25\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f6, f27\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f28, f4\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f10, f16\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f4, f20\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m3, f15, f26\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m0, f6, f25\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m5, f2, f11\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m1, f1, f28\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f21, f12\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m4, f6, f21\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fltm.pi m7, f4, f20\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
