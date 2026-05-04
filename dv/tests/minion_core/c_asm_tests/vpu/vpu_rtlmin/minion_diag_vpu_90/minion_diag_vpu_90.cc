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
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00000
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00004
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00008
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0000c
			 0xbf028f5c,                                                  // -0.51                                       /// 00010
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00014
			 0x0c700000,                                                  // Leading 1s:                                 /// 00018
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00020
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00024
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00030
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00038
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0003c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00048
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0004c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00050
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00054
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00058
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0005c
			 0xff800000,                                                  // -inf                                        /// 00060
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00068
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00070
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00080
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00084
			 0x0c600000,                                                  // Leading 1s:                                 /// 00088
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00090
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00094
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0009c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x55555555,                                                  // 4 random values                             /// 000b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00104
			 0xffc00001,                                                  // -signaling NaN                              /// 00108
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00114
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00118
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00120
			 0x55555555,                                                  // 4 random values                             /// 00124
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00128
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00130
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00138
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00150
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00164
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00168
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0016c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00174
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00178
			 0x7fc00001,                                                  // signaling NaN                               /// 0017c
			 0x4b000000,                                                  // 8388608.0                                   /// 00180
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00184
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00190
			 0xcb000000,                                                  // -8388608.0                                  /// 00194
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00198
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0019c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001bc
			 0xcb000000,                                                  // -8388608.0                                  /// 001c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c4
			 0x80000000,                                                  // -zero                                       /// 001c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xbf028f5c,                                                  // -0.51                                       /// 00204
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00208
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00210
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00214
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00218
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00224
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00228
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00000000,                                                  // zero                                        /// 00238
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0023c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00244
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00248
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00250
			 0x80011111,                                                  // -9.7958E-41                                 /// 00254
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00258
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00260
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00264
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00268
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00270
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c400000,                                                  // Leading 1s:                                 /// 00278
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0027c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00280
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00284
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0028c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00290
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00294
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00298
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0029c
			 0x7f800000,                                                  // inf                                         /// 002a0
			 0xcb000000,                                                  // -8388608.0                                  /// 002a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002e0
			 0x80000000,                                                  // -zero                                       /// 002e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00300
			 0x0c700000,                                                  // Leading 1s:                                 /// 00304
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00308
			 0xcb000000,                                                  // -8388608.0                                  /// 0030c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00314
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00320
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00324
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00328
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0032c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00334
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00338
			 0x7fc00001,                                                  // signaling NaN                               /// 0033c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00340
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00344
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00348
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00350
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00354
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00358
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0xffc00001,                                                  // -signaling NaN                              /// 00360
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00368
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00370
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00378
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00388
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0038c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00390
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00394
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00398
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0039c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000000,                                                  // zero                                        /// 00404
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00408
			 0xcb000000,                                                  // -8388608.0                                  /// 0040c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00410
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00420
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00424
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00430
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00438
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0043c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00444
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00448
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00450
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00454
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00458
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00470
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00474
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00480
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00484
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00488
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0048c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00490
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00494
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0049c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80000000,                                                  // -zero                                       /// 004f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00500
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00504
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00508
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0050c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00518
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0051c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00520
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0052c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00534
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00538
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0053c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00544
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00548
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0054c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00550
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00558
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0055c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00560
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0056c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00570
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00574
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00578
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0e000007,                                                  // Trailing 1s:                                /// 00584
			 0x55555555,                                                  // 4 random values                             /// 00588
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00590
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00594
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005bc
			 0xbf028f5c,                                                  // -0.51                                       /// 005c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c4
			 0x3f028f5c,                                                  // 0.51                                        /// 005c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005e8
			 0x55555555,                                                  // 4 random values                             /// 005ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00000000,                                                  // zero                                        /// 005f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00600
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00610
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0xcb000000,                                                  // -8388608.0                                  /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00620
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00624
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0xffc00001,                                                  // -signaling NaN                              /// 00630
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00634
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0063c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00640
			 0xbf028f5c,                                                  // -0.51                                       /// 00644
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0xcb000000,                                                  // -8388608.0                                  /// 00650
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00654
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0065c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00660
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00664
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00670
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00674
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00678
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0067c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00680
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00684
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00690
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00698
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0069c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00700
			 0xff800000,                                                  // -inf                                        /// 00704
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00708
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0070c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00710
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00718
			 0x7f800000,                                                  // inf                                         /// 0071c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00724
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00728
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00734
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00740
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00744
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00750
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00754
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0075c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00760
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00764
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00768
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00770
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00774
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00778
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0077c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00780
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0078c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00790
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00794
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00798
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0079c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000000,                                                  // -zero                                       /// 007a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007dc
			 0x33333333,                                                  // 4 random values                             /// 007e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00800
			 0x0e000007,                                                  // Trailing 1s:                                /// 00804
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0080c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00810
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00814
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00818
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00820
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00824
			 0x0c400000,                                                  // Leading 1s:                                 /// 00828
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0082c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x33333333,                                                  // 4 random values                             /// 00834
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00838
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0083c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00844
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00848
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0084c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00854
			 0x7f800000,                                                  // inf                                         /// 00858
			 0x00011111,                                                  // 9.7958E-41                                  /// 0085c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00860
			 0x0c600000,                                                  // Leading 1s:                                 /// 00864
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0086c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00870
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00874
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00878
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0087c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00880
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00884
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0088c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00890
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00898
			 0x7fc00001,                                                  // signaling NaN                               /// 0089c
			 0x80000000,                                                  // -zero                                       /// 008a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b8
			 0xffc00001,                                                  // -signaling NaN                              /// 008bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x00000000,                                                  // zero                                        /// 008fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00900
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00904
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0090c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00914
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00928
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0092c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00930
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00934
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00938
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00944
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00948
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0094c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00950
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00960
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00964
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00968
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0096c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00970
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x55555555,                                                  // 4 random values                             /// 00978
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0097c
			 0x7fc00001,                                                  // signaling NaN                               /// 00980
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0098c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00990
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00994
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x7fc00001,                                                  // signaling NaN                               /// 009ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009d0
			 0x7fc00001,                                                  // signaling NaN                               /// 009d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 009f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x4b000000,                                                  // 8388608.0                                   /// 00a18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a58
			 0x80000000,                                                  // -zero                                       /// 00a5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aa0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x7f800000,                                                  // inf                                         /// 00aa8
			 0x80000000,                                                  // -zero                                       /// 00aac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ab0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ab4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000000,                                                  // -zero                                       /// 00ac0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ac8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00acc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad8
			 0xff800000,                                                  // -inf                                        /// 00adc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00afc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b40
			 0x55555555,                                                  // 4 random values                             /// 00b44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b48
			 0x80000000,                                                  // -zero                                       /// 00b4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b58
			 0x7fc00001,                                                  // signaling NaN                               /// 00b5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bbc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00be4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00be8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ca4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cdc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ce8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cf0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d18
			 0x7f800000,                                                  // inf                                         /// 00d1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x7fc00001,                                                  // signaling NaN                               /// 00d3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d40
			 0x33333333,                                                  // 4 random values                             /// 00d44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d68
			 0x33333333,                                                  // 4 random values                             /// 00d6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da0
			 0xcb000000,                                                  // -8388608.0                                  /// 00da4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00db0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e24
			 0xff800000,                                                  // -inf                                        /// 00e28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e50
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x3f028f5c,                                                  // 0.51                                        /// 00e68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x00000000,                                                  // zero                                        /// 00e7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ebc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec8
			 0x7f800000,                                                  // inf                                         /// 00ecc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ed8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00edc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee8
			 0x55555555,                                                  // 4 random values                             /// 00eec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ef8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x3f028f5c,                                                  // 0.51                                        /// 00f58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f84
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fa8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x7f800000,                                                  // inf                                         /// 00fb0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x7f800000,                                                  // inf                                         /// 00fdc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fe4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ff0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x0c7fffc0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xdb567143, /// 0x0
			 0xe6d9c92d, /// 0x4
			 0xe6f58ca6, /// 0x8
			 0x90a908ba, /// 0xc
			 0x48b0204f, /// 0x10
			 0x9bc0229a, /// 0x14
			 0xb16cce8a, /// 0x18
			 0x778da21a, /// 0x1c
			 0x09189791, /// 0x20
			 0x72fef44a, /// 0x24
			 0x4de7382a, /// 0x28
			 0x140c9067, /// 0x2c
			 0x882716da, /// 0x30
			 0xf6cef65f, /// 0x34
			 0xaea9a2ac, /// 0x38
			 0x6109639a, /// 0x3c
			 0x57045757, /// 0x40
			 0x58b293ef, /// 0x44
			 0x46da54bd, /// 0x48
			 0x9f0b2302, /// 0x4c
			 0x9524a412, /// 0x50
			 0x9a69ae19, /// 0x54
			 0xbce63c86, /// 0x58
			 0x4f31d586, /// 0x5c
			 0x438f5d5a, /// 0x60
			 0xa2762923, /// 0x64
			 0x3f9ffb04, /// 0x68
			 0x1a8bd252, /// 0x6c
			 0xd69d7a50, /// 0x70
			 0x5cc0e7e6, /// 0x74
			 0x6ef3583e, /// 0x78
			 0x9a7f74f2, /// 0x7c
			 0x2dd45515, /// 0x80
			 0x1f0559d9, /// 0x84
			 0x5a3163a8, /// 0x88
			 0x50ef63e0, /// 0x8c
			 0x4fd16f40, /// 0x90
			 0xc47be62f, /// 0x94
			 0x82c38796, /// 0x98
			 0xaaf11e28, /// 0x9c
			 0xb53904a9, /// 0xa0
			 0x19e65def, /// 0xa4
			 0xe0db605e, /// 0xa8
			 0x0a8600dc, /// 0xac
			 0xe674368d, /// 0xb0
			 0xc3bbf084, /// 0xb4
			 0x016e5679, /// 0xb8
			 0xa8658b96, /// 0xbc
			 0x8d504dd4, /// 0xc0
			 0x5c133de1, /// 0xc4
			 0x271cf7cd, /// 0xc8
			 0xe2b40360, /// 0xcc
			 0x2010074e, /// 0xd0
			 0xbc464a3d, /// 0xd4
			 0xba9974c4, /// 0xd8
			 0x30539b59, /// 0xdc
			 0x20371a36, /// 0xe0
			 0x9eb95621, /// 0xe4
			 0x444c5a0c, /// 0xe8
			 0x7d01c27d, /// 0xec
			 0xe92dd4c7, /// 0xf0
			 0xf63fd725, /// 0xf4
			 0x72bb15c7, /// 0xf8
			 0x57006a69, /// 0xfc
			 0x871f6c5c, /// 0x100
			 0xf1a01398, /// 0x104
			 0xbeaf8498, /// 0x108
			 0xbe8dc365, /// 0x10c
			 0x33bbd28e, /// 0x110
			 0x8dea17ad, /// 0x114
			 0x24fffcb1, /// 0x118
			 0x36f2b9b1, /// 0x11c
			 0xd6c794b6, /// 0x120
			 0xa18d6c1e, /// 0x124
			 0xcab25463, /// 0x128
			 0x9b42659d, /// 0x12c
			 0xd8c81392, /// 0x130
			 0xc0f85ec3, /// 0x134
			 0xfea36221, /// 0x138
			 0x01b7fcf1, /// 0x13c
			 0x686c9ac2, /// 0x140
			 0x272a88a1, /// 0x144
			 0xc35a7bc8, /// 0x148
			 0x2cb7cf97, /// 0x14c
			 0x91f6c988, /// 0x150
			 0x437e453a, /// 0x154
			 0xeb6d4ee8, /// 0x158
			 0x8ece7f7b, /// 0x15c
			 0x72c4ee4a, /// 0x160
			 0xad1c1483, /// 0x164
			 0xbb6e461a, /// 0x168
			 0x26a46627, /// 0x16c
			 0x844e9021, /// 0x170
			 0x6ebfba81, /// 0x174
			 0x65294520, /// 0x178
			 0x97df2f91, /// 0x17c
			 0x658151eb, /// 0x180
			 0x855e0d44, /// 0x184
			 0x75d0e0a8, /// 0x188
			 0xbe72f3b4, /// 0x18c
			 0xd9bc8044, /// 0x190
			 0xdf9cd8d4, /// 0x194
			 0x56d00e2e, /// 0x198
			 0x52363bd6, /// 0x19c
			 0xb768f306, /// 0x1a0
			 0xf823751d, /// 0x1a4
			 0xd783597e, /// 0x1a8
			 0xac434a5e, /// 0x1ac
			 0xc5af3fb2, /// 0x1b0
			 0x6572dcc8, /// 0x1b4
			 0x0555b81c, /// 0x1b8
			 0x07360fed, /// 0x1bc
			 0x7b13fdf8, /// 0x1c0
			 0x7979e005, /// 0x1c4
			 0x3aba6cc0, /// 0x1c8
			 0x43e3dc95, /// 0x1cc
			 0x382f3891, /// 0x1d0
			 0x70da0842, /// 0x1d4
			 0xb514aad2, /// 0x1d8
			 0x7ed1dca2, /// 0x1dc
			 0x73286f44, /// 0x1e0
			 0xefdff5be, /// 0x1e4
			 0xfad2918d, /// 0x1e8
			 0x743da486, /// 0x1ec
			 0x606a080a, /// 0x1f0
			 0xd952d233, /// 0x1f4
			 0xcbc922c5, /// 0x1f8
			 0x0be89458, /// 0x1fc
			 0x505dbf59, /// 0x200
			 0xe4387637, /// 0x204
			 0x1c91b866, /// 0x208
			 0x6210e09c, /// 0x20c
			 0xffbf2089, /// 0x210
			 0x4e2f4ed2, /// 0x214
			 0xf8eec413, /// 0x218
			 0xe7e48210, /// 0x21c
			 0x452aac22, /// 0x220
			 0x5655cec6, /// 0x224
			 0x15bb277c, /// 0x228
			 0x3a40c70a, /// 0x22c
			 0x8b3cef34, /// 0x230
			 0x27a5ee71, /// 0x234
			 0xef59d2e2, /// 0x238
			 0xb264597c, /// 0x23c
			 0xaba2680d, /// 0x240
			 0x6fc8078e, /// 0x244
			 0xf6021e01, /// 0x248
			 0xad7b424b, /// 0x24c
			 0xefa26cd8, /// 0x250
			 0x3f15c534, /// 0x254
			 0x56abefdf, /// 0x258
			 0xcfac7e2f, /// 0x25c
			 0xb3f5bc7e, /// 0x260
			 0x4eceee7b, /// 0x264
			 0xf2f8283e, /// 0x268
			 0xc23b2caa, /// 0x26c
			 0xa8f2be15, /// 0x270
			 0x2052c464, /// 0x274
			 0x38a0e150, /// 0x278
			 0x74ac2289, /// 0x27c
			 0x602d9b24, /// 0x280
			 0x9716f96c, /// 0x284
			 0xa606cd51, /// 0x288
			 0x3ffd7ce9, /// 0x28c
			 0xdba2117b, /// 0x290
			 0x78b66ca7, /// 0x294
			 0x68685fe0, /// 0x298
			 0x0eabc431, /// 0x29c
			 0xab470220, /// 0x2a0
			 0xe2d2c192, /// 0x2a4
			 0x005c94ce, /// 0x2a8
			 0xd9038e82, /// 0x2ac
			 0x49154ab1, /// 0x2b0
			 0x5878fc8a, /// 0x2b4
			 0xf7eac354, /// 0x2b8
			 0x8bc251a9, /// 0x2bc
			 0x1fc145e5, /// 0x2c0
			 0xebc04b5f, /// 0x2c4
			 0x884ca153, /// 0x2c8
			 0x4e9c64f2, /// 0x2cc
			 0x99ec4f60, /// 0x2d0
			 0xd2cfd6f1, /// 0x2d4
			 0xc7003918, /// 0x2d8
			 0x186edac6, /// 0x2dc
			 0xedabb334, /// 0x2e0
			 0x6ea10fb4, /// 0x2e4
			 0x88347112, /// 0x2e8
			 0xc840d775, /// 0x2ec
			 0x2edf07a6, /// 0x2f0
			 0xcce1e865, /// 0x2f4
			 0xfcd08ccc, /// 0x2f8
			 0xfedbdfa0, /// 0x2fc
			 0xc87224b5, /// 0x300
			 0xf6fc30a7, /// 0x304
			 0x776fc6b8, /// 0x308
			 0x30fd21a0, /// 0x30c
			 0xa0474f5a, /// 0x310
			 0xf001ef0b, /// 0x314
			 0x22085354, /// 0x318
			 0xfe3bebc5, /// 0x31c
			 0xd4f2af6f, /// 0x320
			 0x18fe2869, /// 0x324
			 0xe7e02903, /// 0x328
			 0xff861864, /// 0x32c
			 0x4d7a796c, /// 0x330
			 0xd299e6a9, /// 0x334
			 0xe6e1bd20, /// 0x338
			 0x03616499, /// 0x33c
			 0x78b71b7d, /// 0x340
			 0xa8e213d7, /// 0x344
			 0x6e042cc3, /// 0x348
			 0x40e2a8ac, /// 0x34c
			 0xe14d6507, /// 0x350
			 0x235adc70, /// 0x354
			 0x4d11e53a, /// 0x358
			 0xf581c373, /// 0x35c
			 0xbbb9203b, /// 0x360
			 0x84640d88, /// 0x364
			 0x1b32817e, /// 0x368
			 0xbe7e853e, /// 0x36c
			 0x6c5eaa2d, /// 0x370
			 0x108511e7, /// 0x374
			 0xd151988b, /// 0x378
			 0xe04de4d8, /// 0x37c
			 0x50516b81, /// 0x380
			 0xe782279b, /// 0x384
			 0xab9bf5ef, /// 0x388
			 0xe038801a, /// 0x38c
			 0xb51bacb5, /// 0x390
			 0x0f2ae942, /// 0x394
			 0x02eced8f, /// 0x398
			 0xdbfaa4be, /// 0x39c
			 0x3e330e9c, /// 0x3a0
			 0x5dae19a0, /// 0x3a4
			 0x970e26ed, /// 0x3a8
			 0xd407ce06, /// 0x3ac
			 0xa2948774, /// 0x3b0
			 0x9ceb65e7, /// 0x3b4
			 0xc10003f0, /// 0x3b8
			 0x0bf27c1b, /// 0x3bc
			 0x97ab0832, /// 0x3c0
			 0xf053c9a4, /// 0x3c4
			 0xd91fe8c7, /// 0x3c8
			 0x75c216fc, /// 0x3cc
			 0x6f5fbd16, /// 0x3d0
			 0x760c3f0b, /// 0x3d4
			 0x1fd24bbd, /// 0x3d8
			 0x30cd54e7, /// 0x3dc
			 0xef9c8514, /// 0x3e0
			 0x5ea7c1aa, /// 0x3e4
			 0xa4df30ed, /// 0x3e8
			 0xaed5cd87, /// 0x3ec
			 0x864c9486, /// 0x3f0
			 0x0e48b686, /// 0x3f4
			 0x6f420a48, /// 0x3f8
			 0xfb86b32c, /// 0x3fc
			 0xcc6971fc, /// 0x400
			 0x57b0c445, /// 0x404
			 0x04902ec1, /// 0x408
			 0x328136e9, /// 0x40c
			 0x27ecc306, /// 0x410
			 0x66402184, /// 0x414
			 0x7f293a80, /// 0x418
			 0x7aca0573, /// 0x41c
			 0x89cb9b09, /// 0x420
			 0x9696945f, /// 0x424
			 0x5f9e33b7, /// 0x428
			 0xde88748d, /// 0x42c
			 0x0b699625, /// 0x430
			 0x6733a098, /// 0x434
			 0xdeefdca5, /// 0x438
			 0x22bb4929, /// 0x43c
			 0xe594de6c, /// 0x440
			 0x8a796661, /// 0x444
			 0x17755b6f, /// 0x448
			 0xcc1be31d, /// 0x44c
			 0xe1a46d86, /// 0x450
			 0x9d900a5d, /// 0x454
			 0x17a0c283, /// 0x458
			 0x5580ef0c, /// 0x45c
			 0x59ccf9e1, /// 0x460
			 0xd2140f51, /// 0x464
			 0x56102093, /// 0x468
			 0x55ddfa12, /// 0x46c
			 0x7e5c39a8, /// 0x470
			 0x7ab2eb2c, /// 0x474
			 0x2f73b86b, /// 0x478
			 0xd69b2ead, /// 0x47c
			 0x54b7b142, /// 0x480
			 0x4f2f8722, /// 0x484
			 0xc7a39687, /// 0x488
			 0x74655e3d, /// 0x48c
			 0xfc78f2a6, /// 0x490
			 0x90a944d5, /// 0x494
			 0x6ce5b475, /// 0x498
			 0x1479a3f3, /// 0x49c
			 0x97371346, /// 0x4a0
			 0x180b42a4, /// 0x4a4
			 0x88435455, /// 0x4a8
			 0xf25349fa, /// 0x4ac
			 0x91e8147f, /// 0x4b0
			 0xdc033615, /// 0x4b4
			 0x0921ab92, /// 0x4b8
			 0xd372a522, /// 0x4bc
			 0xd31d1771, /// 0x4c0
			 0xee708ef9, /// 0x4c4
			 0xae2ece80, /// 0x4c8
			 0xf7e77f88, /// 0x4cc
			 0x2d71cc04, /// 0x4d0
			 0x35d14a9f, /// 0x4d4
			 0x41f43551, /// 0x4d8
			 0xd2417919, /// 0x4dc
			 0xc7896730, /// 0x4e0
			 0x47013f55, /// 0x4e4
			 0xe282c9aa, /// 0x4e8
			 0xb634b0cc, /// 0x4ec
			 0xfa674300, /// 0x4f0
			 0x9d389e75, /// 0x4f4
			 0x99c4277a, /// 0x4f8
			 0x41b7cef2, /// 0x4fc
			 0xeb2317b7, /// 0x500
			 0xd370797c, /// 0x504
			 0x571f4ba6, /// 0x508
			 0x458cbab8, /// 0x50c
			 0xc4fc03e5, /// 0x510
			 0xda77e447, /// 0x514
			 0x323138be, /// 0x518
			 0xfdcd8273, /// 0x51c
			 0xd8a8087f, /// 0x520
			 0xf46aaf21, /// 0x524
			 0x7b53bdea, /// 0x528
			 0x247b3b35, /// 0x52c
			 0xde22fb43, /// 0x530
			 0x430565d1, /// 0x534
			 0xcb790b66, /// 0x538
			 0xbffbfdca, /// 0x53c
			 0x9d74b76d, /// 0x540
			 0xe68b01c8, /// 0x544
			 0xb0e1a55f, /// 0x548
			 0xa9bbe79f, /// 0x54c
			 0x09c3f7de, /// 0x550
			 0x4a80e98d, /// 0x554
			 0xbd34b3ae, /// 0x558
			 0x5efa8b67, /// 0x55c
			 0x6006e00d, /// 0x560
			 0xc2529252, /// 0x564
			 0xd56c74c1, /// 0x568
			 0x461ee8c6, /// 0x56c
			 0xbd488480, /// 0x570
			 0x8be3f2d5, /// 0x574
			 0x7d7eb61e, /// 0x578
			 0xa6111075, /// 0x57c
			 0x96327294, /// 0x580
			 0xb6496dc6, /// 0x584
			 0x9ce819e2, /// 0x588
			 0xda81a042, /// 0x58c
			 0x03e3e7fc, /// 0x590
			 0xea2f6cca, /// 0x594
			 0x781fe2d4, /// 0x598
			 0x5fe6b410, /// 0x59c
			 0x878ddbf4, /// 0x5a0
			 0x9b568fa5, /// 0x5a4
			 0x13cef395, /// 0x5a8
			 0x7fefa206, /// 0x5ac
			 0x9819bdf2, /// 0x5b0
			 0xc6769247, /// 0x5b4
			 0x302528cf, /// 0x5b8
			 0x90cfaa05, /// 0x5bc
			 0x2e31494b, /// 0x5c0
			 0x06991d27, /// 0x5c4
			 0xdaa9ca07, /// 0x5c8
			 0x4cc12798, /// 0x5cc
			 0x330dd5ee, /// 0x5d0
			 0x99b31b52, /// 0x5d4
			 0xef1c618d, /// 0x5d8
			 0x3f6ea53a, /// 0x5dc
			 0xd0c77d04, /// 0x5e0
			 0x840d6e89, /// 0x5e4
			 0x22f51117, /// 0x5e8
			 0x75e25407, /// 0x5ec
			 0x6bb54fc6, /// 0x5f0
			 0x99bbfdc1, /// 0x5f4
			 0x908c0668, /// 0x5f8
			 0x14b06fe6, /// 0x5fc
			 0xe793134b, /// 0x600
			 0xe7462dce, /// 0x604
			 0x40a47ccb, /// 0x608
			 0xe9d4f568, /// 0x60c
			 0x63f13891, /// 0x610
			 0x8b846734, /// 0x614
			 0xc03d17b9, /// 0x618
			 0xa8314860, /// 0x61c
			 0xc24e9da6, /// 0x620
			 0x2738e150, /// 0x624
			 0x054ccce5, /// 0x628
			 0xe45c6ac5, /// 0x62c
			 0xdcbaca8d, /// 0x630
			 0xfaacb6d4, /// 0x634
			 0x305ceea8, /// 0x638
			 0xb9413c94, /// 0x63c
			 0x685a72c4, /// 0x640
			 0xa4834782, /// 0x644
			 0x93c1822f, /// 0x648
			 0x199598e3, /// 0x64c
			 0x79a628e6, /// 0x650
			 0xe038ee5b, /// 0x654
			 0xa9185865, /// 0x658
			 0x7715635c, /// 0x65c
			 0x14f6fd2e, /// 0x660
			 0x077c1365, /// 0x664
			 0xdcecfdb5, /// 0x668
			 0x0fc9c080, /// 0x66c
			 0x879c9c15, /// 0x670
			 0x7a018912, /// 0x674
			 0x9efb8f89, /// 0x678
			 0x30eccc91, /// 0x67c
			 0xf76d841d, /// 0x680
			 0xbbf1659e, /// 0x684
			 0x2bff4d29, /// 0x688
			 0x6cc5ff49, /// 0x68c
			 0x773b36e8, /// 0x690
			 0x04f90603, /// 0x694
			 0x1c055676, /// 0x698
			 0xa2748986, /// 0x69c
			 0xe22200f1, /// 0x6a0
			 0x0f57a920, /// 0x6a4
			 0x13e76d0e, /// 0x6a8
			 0xa1321b07, /// 0x6ac
			 0x02018358, /// 0x6b0
			 0x6d10c8e0, /// 0x6b4
			 0x22f6fd88, /// 0x6b8
			 0x74d7613e, /// 0x6bc
			 0x75a83fa5, /// 0x6c0
			 0x7b0b7eb3, /// 0x6c4
			 0x5a27b414, /// 0x6c8
			 0xef6a2f2f, /// 0x6cc
			 0x3fffa665, /// 0x6d0
			 0x16a82e73, /// 0x6d4
			 0xc09871d6, /// 0x6d8
			 0x958a5aed, /// 0x6dc
			 0xde9ca326, /// 0x6e0
			 0xd969cc8c, /// 0x6e4
			 0x0a5b0062, /// 0x6e8
			 0x2a63df00, /// 0x6ec
			 0xde920c16, /// 0x6f0
			 0xf109a139, /// 0x6f4
			 0xd588e187, /// 0x6f8
			 0xd09e667f, /// 0x6fc
			 0x5c4d72bb, /// 0x700
			 0x140a3cd7, /// 0x704
			 0xa298b41d, /// 0x708
			 0xf0a34f2b, /// 0x70c
			 0x994b8f38, /// 0x710
			 0x1138f8c5, /// 0x714
			 0xafb1c7ea, /// 0x718
			 0xd988111b, /// 0x71c
			 0x3fa1232a, /// 0x720
			 0xab840336, /// 0x724
			 0xa5bc70fa, /// 0x728
			 0xbf62610b, /// 0x72c
			 0x2148e9e1, /// 0x730
			 0x4b257bae, /// 0x734
			 0xcda85392, /// 0x738
			 0x9cd3203a, /// 0x73c
			 0x3bf2a352, /// 0x740
			 0xe9741550, /// 0x744
			 0xcffd74a2, /// 0x748
			 0xf267114e, /// 0x74c
			 0xa6e2c169, /// 0x750
			 0x75c21cc9, /// 0x754
			 0xb3205796, /// 0x758
			 0x4d4d6718, /// 0x75c
			 0x7d60573c, /// 0x760
			 0x0284b5b4, /// 0x764
			 0x53cb21f6, /// 0x768
			 0xb4db142c, /// 0x76c
			 0x671bbb27, /// 0x770
			 0x0fe0af98, /// 0x774
			 0xe2f0f054, /// 0x778
			 0x575d9e97, /// 0x77c
			 0x46078545, /// 0x780
			 0xd473071d, /// 0x784
			 0x49acc233, /// 0x788
			 0xa2cb9fd1, /// 0x78c
			 0xc7468ed8, /// 0x790
			 0x720258df, /// 0x794
			 0xe3615279, /// 0x798
			 0x07ddd235, /// 0x79c
			 0x6e310cdd, /// 0x7a0
			 0x6b941dd7, /// 0x7a4
			 0x98cfce8f, /// 0x7a8
			 0x4a404b0d, /// 0x7ac
			 0xbc40957e, /// 0x7b0
			 0x97fcdf5b, /// 0x7b4
			 0x7b2e9a29, /// 0x7b8
			 0xf669eb34, /// 0x7bc
			 0x459f7474, /// 0x7c0
			 0xa5e42c7a, /// 0x7c4
			 0x2fe3be1a, /// 0x7c8
			 0x0fef8dfe, /// 0x7cc
			 0xf97d2fd1, /// 0x7d0
			 0x65183a6b, /// 0x7d4
			 0x8fd50a90, /// 0x7d8
			 0x770b9081, /// 0x7dc
			 0x9cd9ab75, /// 0x7e0
			 0x1528fae1, /// 0x7e4
			 0xd3d77f5a, /// 0x7e8
			 0x146cacce, /// 0x7ec
			 0x01a74172, /// 0x7f0
			 0x22e31f12, /// 0x7f4
			 0xf2505afc, /// 0x7f8
			 0x1ba54610, /// 0x7fc
			 0x53260e50, /// 0x800
			 0x1adc0047, /// 0x804
			 0x1d5ba774, /// 0x808
			 0x3361eb2c, /// 0x80c
			 0xd4f34da5, /// 0x810
			 0x01513a31, /// 0x814
			 0xb815a137, /// 0x818
			 0x3220f271, /// 0x81c
			 0x0732150d, /// 0x820
			 0xc2a05462, /// 0x824
			 0x49eee5ec, /// 0x828
			 0x7cb18962, /// 0x82c
			 0x93e3c40f, /// 0x830
			 0xd43b16b1, /// 0x834
			 0xea6d9d07, /// 0x838
			 0x7a4105e5, /// 0x83c
			 0xcf893a66, /// 0x840
			 0x20fb414c, /// 0x844
			 0xb0683f3d, /// 0x848
			 0x14ea475d, /// 0x84c
			 0x5ba566bf, /// 0x850
			 0x5aa63638, /// 0x854
			 0x4025dba8, /// 0x858
			 0xd93a1c46, /// 0x85c
			 0xc7052a92, /// 0x860
			 0xb2f3a1aa, /// 0x864
			 0x0ff3d2fb, /// 0x868
			 0xa2e1954a, /// 0x86c
			 0xde678700, /// 0x870
			 0xbabe2d5d, /// 0x874
			 0x6880c550, /// 0x878
			 0x64a41520, /// 0x87c
			 0xbc16a815, /// 0x880
			 0x27a89f0a, /// 0x884
			 0xcda2a7b6, /// 0x888
			 0x3dc874ed, /// 0x88c
			 0xe6fd06f5, /// 0x890
			 0x2a2abcb5, /// 0x894
			 0xb1e8955f, /// 0x898
			 0xd1477f8d, /// 0x89c
			 0x5b024eb8, /// 0x8a0
			 0x2511f8d6, /// 0x8a4
			 0x674be18b, /// 0x8a8
			 0x4e05d876, /// 0x8ac
			 0x2f3f82e8, /// 0x8b0
			 0x016e75ce, /// 0x8b4
			 0x82e9b6ca, /// 0x8b8
			 0x3df57381, /// 0x8bc
			 0x62762fc6, /// 0x8c0
			 0x7295ecb8, /// 0x8c4
			 0xa0d5c99f, /// 0x8c8
			 0xff3b18d7, /// 0x8cc
			 0xeabc9ffc, /// 0x8d0
			 0x3b68122e, /// 0x8d4
			 0xcbcd9bff, /// 0x8d8
			 0x248542ef, /// 0x8dc
			 0x98b2a50e, /// 0x8e0
			 0xbd32655a, /// 0x8e4
			 0xc581814b, /// 0x8e8
			 0xf5bb010d, /// 0x8ec
			 0x3ff09a56, /// 0x8f0
			 0x3803d8cd, /// 0x8f4
			 0x9f913c2c, /// 0x8f8
			 0xa7a3374e, /// 0x8fc
			 0x4c508dbe, /// 0x900
			 0x46da1600, /// 0x904
			 0xebbe6bb8, /// 0x908
			 0x757a8e18, /// 0x90c
			 0x0e6e6c1f, /// 0x910
			 0x1f34e52f, /// 0x914
			 0x0486f73f, /// 0x918
			 0x784699bb, /// 0x91c
			 0x92da7123, /// 0x920
			 0xa42c31e2, /// 0x924
			 0xcb77b0f6, /// 0x928
			 0x77007a02, /// 0x92c
			 0xd1e0cc7f, /// 0x930
			 0x203185dd, /// 0x934
			 0x56d2daf6, /// 0x938
			 0xbaa0333e, /// 0x93c
			 0x523c0ccd, /// 0x940
			 0x4aa50d66, /// 0x944
			 0x3bd67be8, /// 0x948
			 0xd40636b5, /// 0x94c
			 0x9d6187d0, /// 0x950
			 0xe79149ef, /// 0x954
			 0xa33d8c2e, /// 0x958
			 0x1e52c8f7, /// 0x95c
			 0x9a1583b6, /// 0x960
			 0x0b373aea, /// 0x964
			 0x7e7d3e76, /// 0x968
			 0x81fedd46, /// 0x96c
			 0x11be751e, /// 0x970
			 0x71c68675, /// 0x974
			 0x3271294c, /// 0x978
			 0x3e91780e, /// 0x97c
			 0xb95472ff, /// 0x980
			 0xd0715f42, /// 0x984
			 0x610aecde, /// 0x988
			 0x310a5337, /// 0x98c
			 0xe54763f4, /// 0x990
			 0x84285dbc, /// 0x994
			 0xd7675e42, /// 0x998
			 0x37e59e4d, /// 0x99c
			 0x0c7268e8, /// 0x9a0
			 0xdddc43dd, /// 0x9a4
			 0x1bda9c79, /// 0x9a8
			 0xbc81981d, /// 0x9ac
			 0x08b7c6d2, /// 0x9b0
			 0x2e813183, /// 0x9b4
			 0xb7d847cb, /// 0x9b8
			 0xec0ff173, /// 0x9bc
			 0x8c005a65, /// 0x9c0
			 0xa298b4a3, /// 0x9c4
			 0x372ed6fb, /// 0x9c8
			 0xdc0e359b, /// 0x9cc
			 0xb04c04cf, /// 0x9d0
			 0x4f39c5d3, /// 0x9d4
			 0xbd67f84d, /// 0x9d8
			 0x1515bfba, /// 0x9dc
			 0x0cd7c025, /// 0x9e0
			 0x911f4231, /// 0x9e4
			 0x532b440f, /// 0x9e8
			 0x062a0618, /// 0x9ec
			 0xb8be363a, /// 0x9f0
			 0xae091412, /// 0x9f4
			 0xa6eb97cd, /// 0x9f8
			 0x33403000, /// 0x9fc
			 0x8b150e0e, /// 0xa00
			 0xe6644cf3, /// 0xa04
			 0xe3650d04, /// 0xa08
			 0x89c0ea1b, /// 0xa0c
			 0x9cb2a83a, /// 0xa10
			 0xa11f676f, /// 0xa14
			 0x1f555d7f, /// 0xa18
			 0xd8ed321c, /// 0xa1c
			 0xe7c2a3b6, /// 0xa20
			 0x5d019026, /// 0xa24
			 0xb649262a, /// 0xa28
			 0x2191fb35, /// 0xa2c
			 0x37ece805, /// 0xa30
			 0x088460f1, /// 0xa34
			 0xb80bc7a3, /// 0xa38
			 0x178e3d24, /// 0xa3c
			 0x2742c37b, /// 0xa40
			 0x08e4d951, /// 0xa44
			 0x51f43633, /// 0xa48
			 0x7ed946dd, /// 0xa4c
			 0xc99c2d58, /// 0xa50
			 0xe3c8b946, /// 0xa54
			 0xcae33a17, /// 0xa58
			 0xa6706105, /// 0xa5c
			 0x491f4240, /// 0xa60
			 0xd9bc3fee, /// 0xa64
			 0x2788fcfc, /// 0xa68
			 0x94c7e839, /// 0xa6c
			 0xba6f3573, /// 0xa70
			 0x358f8ec4, /// 0xa74
			 0x7b721821, /// 0xa78
			 0xa501d1ba, /// 0xa7c
			 0xcae8dec7, /// 0xa80
			 0xcd4aeddc, /// 0xa84
			 0x813ecbee, /// 0xa88
			 0x1659e631, /// 0xa8c
			 0x8a832fc0, /// 0xa90
			 0x9cc433e9, /// 0xa94
			 0x1876b840, /// 0xa98
			 0x3e670027, /// 0xa9c
			 0x75d338fd, /// 0xaa0
			 0xd65693de, /// 0xaa4
			 0xd2a9098b, /// 0xaa8
			 0x6f828c3c, /// 0xaac
			 0x4811c798, /// 0xab0
			 0xc60026d2, /// 0xab4
			 0xfb130b24, /// 0xab8
			 0x3cd79bcd, /// 0xabc
			 0xbf29a682, /// 0xac0
			 0x73b899e2, /// 0xac4
			 0x18699bdf, /// 0xac8
			 0x2d4ff54c, /// 0xacc
			 0x5763f294, /// 0xad0
			 0x2967240b, /// 0xad4
			 0xcb45ed69, /// 0xad8
			 0x2bed73cb, /// 0xadc
			 0x2c970121, /// 0xae0
			 0x97e5c451, /// 0xae4
			 0x27dba5ab, /// 0xae8
			 0xc62c7ff1, /// 0xaec
			 0x7da4e02a, /// 0xaf0
			 0xf5797be8, /// 0xaf4
			 0x729e46ec, /// 0xaf8
			 0x99af7d0f, /// 0xafc
			 0xe97d8b19, /// 0xb00
			 0xfe1d12c7, /// 0xb04
			 0xfc066c33, /// 0xb08
			 0x8def5f4a, /// 0xb0c
			 0x0ff6e353, /// 0xb10
			 0x73f90f5d, /// 0xb14
			 0x371020d7, /// 0xb18
			 0x422f07b9, /// 0xb1c
			 0x9bbb42da, /// 0xb20
			 0xe7c98c45, /// 0xb24
			 0x64a35215, /// 0xb28
			 0x8991f9ba, /// 0xb2c
			 0xa3f74ee3, /// 0xb30
			 0x74161132, /// 0xb34
			 0xc5972afc, /// 0xb38
			 0xdbd5c69f, /// 0xb3c
			 0x16e54277, /// 0xb40
			 0x9fc23b1b, /// 0xb44
			 0x766c6341, /// 0xb48
			 0x24bd2d9f, /// 0xb4c
			 0x40206139, /// 0xb50
			 0x276bc6fb, /// 0xb54
			 0xb72aadd8, /// 0xb58
			 0x0219f6bd, /// 0xb5c
			 0x7fd37518, /// 0xb60
			 0xc04e4481, /// 0xb64
			 0x89014bed, /// 0xb68
			 0xc0a6d903, /// 0xb6c
			 0x215f1b73, /// 0xb70
			 0xd2b32fba, /// 0xb74
			 0x6d6d5a33, /// 0xb78
			 0x2d9cebf6, /// 0xb7c
			 0x7d0b7257, /// 0xb80
			 0x0727f030, /// 0xb84
			 0x238c0905, /// 0xb88
			 0x8d6c1b86, /// 0xb8c
			 0x116dad48, /// 0xb90
			 0xdb2e0cc4, /// 0xb94
			 0xcbddfc9f, /// 0xb98
			 0xa2f2247f, /// 0xb9c
			 0xcc3817f2, /// 0xba0
			 0x8695608b, /// 0xba4
			 0x13a9760f, /// 0xba8
			 0x27abb826, /// 0xbac
			 0xdd62e3e8, /// 0xbb0
			 0xa81dba75, /// 0xbb4
			 0x1dc92f5a, /// 0xbb8
			 0x1ff75c0a, /// 0xbbc
			 0xcd9b448c, /// 0xbc0
			 0x5d723a87, /// 0xbc4
			 0x69452667, /// 0xbc8
			 0x4a12afc2, /// 0xbcc
			 0xeb9a1218, /// 0xbd0
			 0xa027ce28, /// 0xbd4
			 0x4338e88e, /// 0xbd8
			 0x574ce405, /// 0xbdc
			 0x4006c9f4, /// 0xbe0
			 0x197e6cca, /// 0xbe4
			 0x3ece12f3, /// 0xbe8
			 0x06b3b39c, /// 0xbec
			 0x4bff5637, /// 0xbf0
			 0x25aae26e, /// 0xbf4
			 0x17801d0a, /// 0xbf8
			 0x44303248, /// 0xbfc
			 0xc815e92f, /// 0xc00
			 0x47c5de29, /// 0xc04
			 0x4462b714, /// 0xc08
			 0xa9bbcc74, /// 0xc0c
			 0x2a6eb27e, /// 0xc10
			 0x21cb27c9, /// 0xc14
			 0x3f4b5fcb, /// 0xc18
			 0x03fd70fb, /// 0xc1c
			 0x1d298370, /// 0xc20
			 0x7165fda3, /// 0xc24
			 0x1ace1956, /// 0xc28
			 0x99f52988, /// 0xc2c
			 0x5f4ac29d, /// 0xc30
			 0x75b4db1d, /// 0xc34
			 0x51939652, /// 0xc38
			 0x825d3157, /// 0xc3c
			 0x8a7ded48, /// 0xc40
			 0xb5f94392, /// 0xc44
			 0xb5db4fd4, /// 0xc48
			 0x5b7db01e, /// 0xc4c
			 0x881eb6f2, /// 0xc50
			 0x1cd2d530, /// 0xc54
			 0xe1fa0afe, /// 0xc58
			 0x7606edf0, /// 0xc5c
			 0x9034870e, /// 0xc60
			 0x853f623d, /// 0xc64
			 0xa2332480, /// 0xc68
			 0xdcba0c09, /// 0xc6c
			 0xda132483, /// 0xc70
			 0x85f724cb, /// 0xc74
			 0xfb55dabe, /// 0xc78
			 0x2c11b8cc, /// 0xc7c
			 0x77947f21, /// 0xc80
			 0x47ecd4c3, /// 0xc84
			 0xe142d11a, /// 0xc88
			 0x23865f48, /// 0xc8c
			 0xcdbdd5d2, /// 0xc90
			 0x3527e915, /// 0xc94
			 0xe052e489, /// 0xc98
			 0xa00cc0dd, /// 0xc9c
			 0x161361d0, /// 0xca0
			 0x48139b0a, /// 0xca4
			 0xbd5e0d46, /// 0xca8
			 0x514b3130, /// 0xcac
			 0x5a291159, /// 0xcb0
			 0x5270a08d, /// 0xcb4
			 0x9322d8d8, /// 0xcb8
			 0xf19dc968, /// 0xcbc
			 0x4c30053a, /// 0xcc0
			 0xc24f0eb7, /// 0xcc4
			 0x8560d1ca, /// 0xcc8
			 0x71b5871e, /// 0xccc
			 0xf034ab6d, /// 0xcd0
			 0x808b7a5a, /// 0xcd4
			 0x2eaa8a7f, /// 0xcd8
			 0x9bea8065, /// 0xcdc
			 0xd82cb5a9, /// 0xce0
			 0x74721e42, /// 0xce4
			 0x04d31418, /// 0xce8
			 0xf256e147, /// 0xcec
			 0x206f93a3, /// 0xcf0
			 0xcd535099, /// 0xcf4
			 0xba13134b, /// 0xcf8
			 0xe2df564f, /// 0xcfc
			 0xfc610c62, /// 0xd00
			 0x5bdfa8e6, /// 0xd04
			 0xc93b7531, /// 0xd08
			 0x5b8762a0, /// 0xd0c
			 0xa965f6cd, /// 0xd10
			 0x35e0b493, /// 0xd14
			 0x7c458400, /// 0xd18
			 0x9bd976ca, /// 0xd1c
			 0xa7fa951d, /// 0xd20
			 0x8eb7a048, /// 0xd24
			 0xd9107b84, /// 0xd28
			 0xf86d3469, /// 0xd2c
			 0x41fdb77b, /// 0xd30
			 0xdc1e4e3f, /// 0xd34
			 0xc9fd23ad, /// 0xd38
			 0xdc0a3997, /// 0xd3c
			 0xc9118bf6, /// 0xd40
			 0x2419aba6, /// 0xd44
			 0x0d6bde22, /// 0xd48
			 0x074dc063, /// 0xd4c
			 0xf9aa4072, /// 0xd50
			 0x0cca9bbd, /// 0xd54
			 0xf1ecbf32, /// 0xd58
			 0x02af4457, /// 0xd5c
			 0x1cd1ac62, /// 0xd60
			 0x40a46c5a, /// 0xd64
			 0xcafa7fe5, /// 0xd68
			 0x5e42d712, /// 0xd6c
			 0x83ec04db, /// 0xd70
			 0xa595ea52, /// 0xd74
			 0x0c13f12e, /// 0xd78
			 0x4e298b90, /// 0xd7c
			 0x60593468, /// 0xd80
			 0xf4523b9e, /// 0xd84
			 0x79b50013, /// 0xd88
			 0x3b382087, /// 0xd8c
			 0xfad1c113, /// 0xd90
			 0x3df0e909, /// 0xd94
			 0x6b4f844a, /// 0xd98
			 0xf1fecc04, /// 0xd9c
			 0x4c4ccb64, /// 0xda0
			 0xa7b5ccf4, /// 0xda4
			 0x8b4d0fb6, /// 0xda8
			 0x326f032b, /// 0xdac
			 0x6ed1b929, /// 0xdb0
			 0x0cdf33c6, /// 0xdb4
			 0x973b2a49, /// 0xdb8
			 0x7937450f, /// 0xdbc
			 0x934fc04e, /// 0xdc0
			 0x0f34f1c3, /// 0xdc4
			 0xd2d41b2c, /// 0xdc8
			 0x82f136fd, /// 0xdcc
			 0xa5b962c0, /// 0xdd0
			 0xcdf0db24, /// 0xdd4
			 0xc5661cf1, /// 0xdd8
			 0x8302dc26, /// 0xddc
			 0x7024f10a, /// 0xde0
			 0x80b0e35a, /// 0xde4
			 0xfd4ee28b, /// 0xde8
			 0x702163f2, /// 0xdec
			 0x7c9dd1fc, /// 0xdf0
			 0xa441a7b9, /// 0xdf4
			 0x5d15df69, /// 0xdf8
			 0x53ed9917, /// 0xdfc
			 0x3236056d, /// 0xe00
			 0x169729a9, /// 0xe04
			 0xd57b4d7f, /// 0xe08
			 0x5741c7f5, /// 0xe0c
			 0xf8cb8aef, /// 0xe10
			 0x4d32204a, /// 0xe14
			 0x29310d5d, /// 0xe18
			 0x0444f28c, /// 0xe1c
			 0x5bd4c578, /// 0xe20
			 0x3b05f76e, /// 0xe24
			 0x04d01fc5, /// 0xe28
			 0xee6f1f88, /// 0xe2c
			 0x94dd3290, /// 0xe30
			 0x652d3af7, /// 0xe34
			 0x2919e435, /// 0xe38
			 0xb43b4bbd, /// 0xe3c
			 0xb14f38bc, /// 0xe40
			 0xf83fda3b, /// 0xe44
			 0xf37e15e7, /// 0xe48
			 0x89e13510, /// 0xe4c
			 0x98538574, /// 0xe50
			 0x9616e0f7, /// 0xe54
			 0xbf2d7d6c, /// 0xe58
			 0x8b8cc391, /// 0xe5c
			 0x065a1f27, /// 0xe60
			 0x8250a48e, /// 0xe64
			 0x7b2834f4, /// 0xe68
			 0x4df8f58a, /// 0xe6c
			 0x640f7e05, /// 0xe70
			 0xb8614d5d, /// 0xe74
			 0xb9734fc3, /// 0xe78
			 0x01ccf86b, /// 0xe7c
			 0x6ad4e132, /// 0xe80
			 0x18e53be1, /// 0xe84
			 0x1bc4177e, /// 0xe88
			 0xb2d2f3ee, /// 0xe8c
			 0x9741098d, /// 0xe90
			 0x091d672a, /// 0xe94
			 0x6d13a6a5, /// 0xe98
			 0x7cf1c47e, /// 0xe9c
			 0xf71a6bfc, /// 0xea0
			 0x45d68b4a, /// 0xea4
			 0x4c6ccc75, /// 0xea8
			 0xcda4b6c7, /// 0xeac
			 0xe623ed28, /// 0xeb0
			 0xd941c0b7, /// 0xeb4
			 0x640f23a9, /// 0xeb8
			 0x6baee10c, /// 0xebc
			 0x816cb1f3, /// 0xec0
			 0xaddbcd8a, /// 0xec4
			 0xad53d0ea, /// 0xec8
			 0x866f9f27, /// 0xecc
			 0x108b73e7, /// 0xed0
			 0x220654fa, /// 0xed4
			 0x8fb9aaa4, /// 0xed8
			 0x0718f7ca, /// 0xedc
			 0x701f7258, /// 0xee0
			 0x268cea88, /// 0xee4
			 0x0c749db7, /// 0xee8
			 0xd2127480, /// 0xeec
			 0xffd3f770, /// 0xef0
			 0xfdeb79ab, /// 0xef4
			 0x3cec2989, /// 0xef8
			 0x29221afa, /// 0xefc
			 0xed6088e7, /// 0xf00
			 0xf7c7599a, /// 0xf04
			 0x34a93990, /// 0xf08
			 0xab1453e3, /// 0xf0c
			 0xa4e62de1, /// 0xf10
			 0xb9d17ace, /// 0xf14
			 0x36fd2792, /// 0xf18
			 0xcfc6912f, /// 0xf1c
			 0x706d393e, /// 0xf20
			 0x7408d28f, /// 0xf24
			 0x810430d0, /// 0xf28
			 0x76c8cf64, /// 0xf2c
			 0xe63f0355, /// 0xf30
			 0x27de706e, /// 0xf34
			 0xd0187268, /// 0xf38
			 0x293b305b, /// 0xf3c
			 0x2f4b2151, /// 0xf40
			 0x0b409f34, /// 0xf44
			 0x191e2cb5, /// 0xf48
			 0xa9f9861b, /// 0xf4c
			 0xb91f3f99, /// 0xf50
			 0x1a174a3b, /// 0xf54
			 0xc77e28fa, /// 0xf58
			 0xbf24fd7a, /// 0xf5c
			 0xcf88e470, /// 0xf60
			 0xb80dbd5b, /// 0xf64
			 0x6822b5c0, /// 0xf68
			 0xef0412d7, /// 0xf6c
			 0x5e4f7dd6, /// 0xf70
			 0xe5a13035, /// 0xf74
			 0xe23a60f0, /// 0xf78
			 0xaff7eb86, /// 0xf7c
			 0xb84c8aae, /// 0xf80
			 0x161ac015, /// 0xf84
			 0xc5e5827e, /// 0xf88
			 0x7d140e36, /// 0xf8c
			 0xafad745d, /// 0xf90
			 0x714f10d4, /// 0xf94
			 0x26318784, /// 0xf98
			 0xab51b9f9, /// 0xf9c
			 0xa3f0c22a, /// 0xfa0
			 0x1d7a163f, /// 0xfa4
			 0xa6bd2953, /// 0xfa8
			 0x66076913, /// 0xfac
			 0xf796a596, /// 0xfb0
			 0x725f5edc, /// 0xfb4
			 0x1f29fc91, /// 0xfb8
			 0x7885748c, /// 0xfbc
			 0x148915eb, /// 0xfc0
			 0xc96fd99a, /// 0xfc4
			 0x6a64f2fe, /// 0xfc8
			 0xab5f354f, /// 0xfcc
			 0x151f42f4, /// 0xfd0
			 0xcfffe317, /// 0xfd4
			 0x36e4e96c, /// 0xfd8
			 0x51778219, /// 0xfdc
			 0xdbd7bd19, /// 0xfe0
			 0x3fc1ce27, /// 0xfe4
			 0x225884a8, /// 0xfe8
			 0xeb4d5947, /// 0xfec
			 0x7104f1a8, /// 0xff0
			 0xd0434802, /// 0xff4
			 0x26548432, /// 0xff8
			 0x1a07d198 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xbb3987c8, /// 0x0
			 0x6ca8e9b0, /// 0x4
			 0xdb2453fa, /// 0x8
			 0x8f4161c0, /// 0xc
			 0xb2cb8ff4, /// 0x10
			 0x24e3ed7b, /// 0x14
			 0x31e42091, /// 0x18
			 0x5f92f63b, /// 0x1c
			 0xc3a7a97a, /// 0x20
			 0x73121526, /// 0x24
			 0xd9a56fa3, /// 0x28
			 0x90ed8e7a, /// 0x2c
			 0xce70efec, /// 0x30
			 0x8171cae9, /// 0x34
			 0x43fca4c5, /// 0x38
			 0xfa68c0d6, /// 0x3c
			 0xe0aa8800, /// 0x40
			 0x3c5dbd78, /// 0x44
			 0x6efe9038, /// 0x48
			 0x6785d13c, /// 0x4c
			 0xbd763f4f, /// 0x50
			 0x303fe57b, /// 0x54
			 0x33d9c693, /// 0x58
			 0x328cb95e, /// 0x5c
			 0x0459af08, /// 0x60
			 0x2bd5341a, /// 0x64
			 0x76524d17, /// 0x68
			 0xb805d089, /// 0x6c
			 0x63bb75a9, /// 0x70
			 0xc552c1ec, /// 0x74
			 0x913ea9c7, /// 0x78
			 0x4e999767, /// 0x7c
			 0xf34967ef, /// 0x80
			 0xc70b09ae, /// 0x84
			 0x107e31c8, /// 0x88
			 0x3e274622, /// 0x8c
			 0xc4576436, /// 0x90
			 0x9f90ab79, /// 0x94
			 0x3894e061, /// 0x98
			 0x989d5b30, /// 0x9c
			 0x5dd3b158, /// 0xa0
			 0x9c73fa28, /// 0xa4
			 0x42f03f42, /// 0xa8
			 0x89d329d9, /// 0xac
			 0x38e67032, /// 0xb0
			 0x7000c526, /// 0xb4
			 0xa2cba754, /// 0xb8
			 0xea71a313, /// 0xbc
			 0x2e299c56, /// 0xc0
			 0x4d465df4, /// 0xc4
			 0x4af5b8bf, /// 0xc8
			 0x02d30af8, /// 0xcc
			 0x7636bfa3, /// 0xd0
			 0xea7562a7, /// 0xd4
			 0xab94e098, /// 0xd8
			 0x538b21ce, /// 0xdc
			 0xa1163fca, /// 0xe0
			 0x99bf0429, /// 0xe4
			 0x8768e3a6, /// 0xe8
			 0xe1066a36, /// 0xec
			 0xf7f35dc0, /// 0xf0
			 0xf718164f, /// 0xf4
			 0x0e7dcf67, /// 0xf8
			 0x5b50eb74, /// 0xfc
			 0x088c786d, /// 0x100
			 0x5cff288a, /// 0x104
			 0xe7297950, /// 0x108
			 0xe97f8b5b, /// 0x10c
			 0x6dbccd22, /// 0x110
			 0xea3b8711, /// 0x114
			 0x9f8d2341, /// 0x118
			 0xe273e292, /// 0x11c
			 0x22a08e43, /// 0x120
			 0x70e03e96, /// 0x124
			 0x90adc510, /// 0x128
			 0xfdb16266, /// 0x12c
			 0xc4541801, /// 0x130
			 0x03e81af6, /// 0x134
			 0x9ca0c3c7, /// 0x138
			 0x4b167750, /// 0x13c
			 0x86889c9d, /// 0x140
			 0xa0583bd0, /// 0x144
			 0x7504982b, /// 0x148
			 0x0660f866, /// 0x14c
			 0xa46fca7e, /// 0x150
			 0x617cd279, /// 0x154
			 0x8aff0e40, /// 0x158
			 0xbfa2894f, /// 0x15c
			 0x4bf52fe2, /// 0x160
			 0xf6956254, /// 0x164
			 0x45b88597, /// 0x168
			 0xe6a00f7e, /// 0x16c
			 0xc85ee17e, /// 0x170
			 0x910410ab, /// 0x174
			 0x65c3057c, /// 0x178
			 0x90a8b78e, /// 0x17c
			 0x3f70bffa, /// 0x180
			 0xf05c7ed3, /// 0x184
			 0x18031983, /// 0x188
			 0x4a97981d, /// 0x18c
			 0x0c8ad4fc, /// 0x190
			 0x964b02a0, /// 0x194
			 0x2b92c93d, /// 0x198
			 0x67cfff03, /// 0x19c
			 0x2ace252e, /// 0x1a0
			 0x532d34ad, /// 0x1a4
			 0xfc9717e8, /// 0x1a8
			 0x3eab6cb5, /// 0x1ac
			 0xa71ac5a6, /// 0x1b0
			 0x9495dd8d, /// 0x1b4
			 0x03775e2f, /// 0x1b8
			 0x4c304e40, /// 0x1bc
			 0x050ad823, /// 0x1c0
			 0x04686e6a, /// 0x1c4
			 0xa51f0c78, /// 0x1c8
			 0xa0ea7369, /// 0x1cc
			 0x491bd5ec, /// 0x1d0
			 0x27a30fc3, /// 0x1d4
			 0x754e4923, /// 0x1d8
			 0xf685800d, /// 0x1dc
			 0x8600dcb7, /// 0x1e0
			 0x6c2e1dd8, /// 0x1e4
			 0x9f3e0f4c, /// 0x1e8
			 0x01b00e7f, /// 0x1ec
			 0x50d18707, /// 0x1f0
			 0xde2d8d9e, /// 0x1f4
			 0x22291b23, /// 0x1f8
			 0x68c00073, /// 0x1fc
			 0x6cd8a8e8, /// 0x200
			 0xe6c79a8b, /// 0x204
			 0x7ad50a63, /// 0x208
			 0x63151c77, /// 0x20c
			 0x0f673541, /// 0x210
			 0xff2d2a7d, /// 0x214
			 0x743e954f, /// 0x218
			 0xc0327b52, /// 0x21c
			 0x737283c3, /// 0x220
			 0x9ba05421, /// 0x224
			 0x0853199c, /// 0x228
			 0xedcc2484, /// 0x22c
			 0xa198f938, /// 0x230
			 0x2d3f5b65, /// 0x234
			 0xc7032ebc, /// 0x238
			 0x345d0743, /// 0x23c
			 0x797cb051, /// 0x240
			 0x16ab467a, /// 0x244
			 0xf82c1ccc, /// 0x248
			 0xe9fdcf59, /// 0x24c
			 0x70600174, /// 0x250
			 0xa740fc53, /// 0x254
			 0xb36ca97b, /// 0x258
			 0xbac18f94, /// 0x25c
			 0x3aacd38e, /// 0x260
			 0xbc2d4650, /// 0x264
			 0x2335e99a, /// 0x268
			 0x449a473b, /// 0x26c
			 0xc896072c, /// 0x270
			 0x97a06283, /// 0x274
			 0xf8187e63, /// 0x278
			 0x17ed3da7, /// 0x27c
			 0x0ffbc7d0, /// 0x280
			 0x3723edbc, /// 0x284
			 0x3bd7b0bb, /// 0x288
			 0x7f60a4ba, /// 0x28c
			 0xe152bd62, /// 0x290
			 0x8b50b181, /// 0x294
			 0x1c4ac91e, /// 0x298
			 0x6424e823, /// 0x29c
			 0xc751ff76, /// 0x2a0
			 0xedc24092, /// 0x2a4
			 0x3c8656a7, /// 0x2a8
			 0x73209859, /// 0x2ac
			 0xa932e1a6, /// 0x2b0
			 0x4471f570, /// 0x2b4
			 0xa2beddd9, /// 0x2b8
			 0x040f58ae, /// 0x2bc
			 0x5a4ca0de, /// 0x2c0
			 0x9877b330, /// 0x2c4
			 0xc048f6e6, /// 0x2c8
			 0x92c370ef, /// 0x2cc
			 0xd165e024, /// 0x2d0
			 0xeb249430, /// 0x2d4
			 0xd61ab769, /// 0x2d8
			 0x2460f41e, /// 0x2dc
			 0xd7fb22a0, /// 0x2e0
			 0x19a137df, /// 0x2e4
			 0x76052da3, /// 0x2e8
			 0x50bb8d15, /// 0x2ec
			 0x487a6c65, /// 0x2f0
			 0x0d455c18, /// 0x2f4
			 0xc5b5780d, /// 0x2f8
			 0x6016221c, /// 0x2fc
			 0xb57395fe, /// 0x300
			 0xc4dd441c, /// 0x304
			 0xc5d51a0f, /// 0x308
			 0x1beaafd2, /// 0x30c
			 0x3777b35a, /// 0x310
			 0xb787082e, /// 0x314
			 0xe4b233ab, /// 0x318
			 0xd0e5de13, /// 0x31c
			 0x95dc2682, /// 0x320
			 0xa0eb61c4, /// 0x324
			 0xe4d69de6, /// 0x328
			 0x002029ba, /// 0x32c
			 0xb868fba3, /// 0x330
			 0x5e7e50d7, /// 0x334
			 0x52335cd2, /// 0x338
			 0xbcd1a677, /// 0x33c
			 0xe655a973, /// 0x340
			 0x7c43039d, /// 0x344
			 0xf7e946ab, /// 0x348
			 0x4455c034, /// 0x34c
			 0xbb7745ff, /// 0x350
			 0x769355f1, /// 0x354
			 0x5ddee711, /// 0x358
			 0x2d9a53f2, /// 0x35c
			 0x77aada7a, /// 0x360
			 0x2e15c49f, /// 0x364
			 0x2485a4a9, /// 0x368
			 0xdc143361, /// 0x36c
			 0x035f19b7, /// 0x370
			 0x16e50a25, /// 0x374
			 0x75b69fa4, /// 0x378
			 0xc982fd77, /// 0x37c
			 0xc7fd03a7, /// 0x380
			 0xc38d4305, /// 0x384
			 0x7d5655b1, /// 0x388
			 0x5a64dd9c, /// 0x38c
			 0xe192c69c, /// 0x390
			 0xc6be67ca, /// 0x394
			 0xeef00762, /// 0x398
			 0xeebd7175, /// 0x39c
			 0x2e0a6d36, /// 0x3a0
			 0x1ad47ab8, /// 0x3a4
			 0xacfed958, /// 0x3a8
			 0x819de71b, /// 0x3ac
			 0x40e4b63b, /// 0x3b0
			 0xdcf8d140, /// 0x3b4
			 0xf254ebfa, /// 0x3b8
			 0xeec09cec, /// 0x3bc
			 0x59676b79, /// 0x3c0
			 0x4da774ae, /// 0x3c4
			 0x3d468a1a, /// 0x3c8
			 0xdd362283, /// 0x3cc
			 0xe98856ee, /// 0x3d0
			 0x3415cf8b, /// 0x3d4
			 0x5d1adf06, /// 0x3d8
			 0x90353fb8, /// 0x3dc
			 0x757c3c48, /// 0x3e0
			 0x538bf83d, /// 0x3e4
			 0x2bb02a62, /// 0x3e8
			 0xae30eb37, /// 0x3ec
			 0x4d95d439, /// 0x3f0
			 0x7c0eb18e, /// 0x3f4
			 0x6ec05c02, /// 0x3f8
			 0x30881e8e, /// 0x3fc
			 0xdfd4e2af, /// 0x400
			 0x6da4c7ac, /// 0x404
			 0x47fe098d, /// 0x408
			 0x6b22294a, /// 0x40c
			 0x78e52dfb, /// 0x410
			 0xded09f15, /// 0x414
			 0xae878173, /// 0x418
			 0xccc841c3, /// 0x41c
			 0xdd3ef5d1, /// 0x420
			 0x7954fbfc, /// 0x424
			 0x4e70892d, /// 0x428
			 0x18e6c470, /// 0x42c
			 0xb50d50cf, /// 0x430
			 0x6a72b03a, /// 0x434
			 0xb81f2ec3, /// 0x438
			 0x6579b948, /// 0x43c
			 0x78d5d553, /// 0x440
			 0x5ff5d308, /// 0x444
			 0xc38fbdb2, /// 0x448
			 0x6afdc368, /// 0x44c
			 0xbae456fa, /// 0x450
			 0xa0129b06, /// 0x454
			 0xb13c0271, /// 0x458
			 0x634ed119, /// 0x45c
			 0xc4e7c46b, /// 0x460
			 0xb854cc2c, /// 0x464
			 0x761fb74b, /// 0x468
			 0xed594889, /// 0x46c
			 0x4d895f32, /// 0x470
			 0x81760953, /// 0x474
			 0x7071bead, /// 0x478
			 0xe66db082, /// 0x47c
			 0x8f967408, /// 0x480
			 0xe66ff0ee, /// 0x484
			 0x5ad38709, /// 0x488
			 0xc01e0fbd, /// 0x48c
			 0x936a576f, /// 0x490
			 0x0b10af1f, /// 0x494
			 0xe23a822b, /// 0x498
			 0xd0ec8894, /// 0x49c
			 0xae072101, /// 0x4a0
			 0x7393282a, /// 0x4a4
			 0x6656a442, /// 0x4a8
			 0x0027dbf9, /// 0x4ac
			 0x6498e9db, /// 0x4b0
			 0xaa993036, /// 0x4b4
			 0x2dd3362c, /// 0x4b8
			 0xca5f5288, /// 0x4bc
			 0x94711fd2, /// 0x4c0
			 0x88e8b405, /// 0x4c4
			 0xf2baae70, /// 0x4c8
			 0xc94cfeed, /// 0x4cc
			 0x890e42f9, /// 0x4d0
			 0x407dee0f, /// 0x4d4
			 0x46c916cc, /// 0x4d8
			 0xfb72b43e, /// 0x4dc
			 0x99c78786, /// 0x4e0
			 0xa6be2709, /// 0x4e4
			 0x65af02d3, /// 0x4e8
			 0x1e358e52, /// 0x4ec
			 0x3f4dbc0e, /// 0x4f0
			 0x1a0fce4e, /// 0x4f4
			 0x9fa379d5, /// 0x4f8
			 0xc83de9b4, /// 0x4fc
			 0x904d2909, /// 0x500
			 0x0055b925, /// 0x504
			 0x5bfc3023, /// 0x508
			 0x4926d038, /// 0x50c
			 0x36ee0aaf, /// 0x510
			 0x0aa9225e, /// 0x514
			 0x927aa9b5, /// 0x518
			 0xf9feb188, /// 0x51c
			 0x12b51d18, /// 0x520
			 0x58ad1496, /// 0x524
			 0xa17c53f7, /// 0x528
			 0x914dce9e, /// 0x52c
			 0xdb881385, /// 0x530
			 0x8302e32a, /// 0x534
			 0x53bbb399, /// 0x538
			 0x9f6f87e3, /// 0x53c
			 0x2df5c73e, /// 0x540
			 0x218ed255, /// 0x544
			 0xef88437d, /// 0x548
			 0xfdba9f9e, /// 0x54c
			 0x72afe4f8, /// 0x550
			 0xab8917af, /// 0x554
			 0x5b3e43e3, /// 0x558
			 0xa3c2589b, /// 0x55c
			 0x408132d6, /// 0x560
			 0xa5dce8ef, /// 0x564
			 0xd0de3c5a, /// 0x568
			 0xcd3fc145, /// 0x56c
			 0x9b372dc0, /// 0x570
			 0x53e9706b, /// 0x574
			 0xf90f785e, /// 0x578
			 0x02ea7ab0, /// 0x57c
			 0x5bf579c4, /// 0x580
			 0xa873c75d, /// 0x584
			 0xd62e3aae, /// 0x588
			 0x30f2b207, /// 0x58c
			 0x6f912e2b, /// 0x590
			 0xadb89bbd, /// 0x594
			 0x1a61776a, /// 0x598
			 0x1f3fa611, /// 0x59c
			 0x62d6b36c, /// 0x5a0
			 0xa0da7d2c, /// 0x5a4
			 0x7a0df928, /// 0x5a8
			 0x442a2305, /// 0x5ac
			 0x0d2ad597, /// 0x5b0
			 0x9e56b6e2, /// 0x5b4
			 0x0d506baf, /// 0x5b8
			 0x47571e0c, /// 0x5bc
			 0x8000fa22, /// 0x5c0
			 0xcfb66ef4, /// 0x5c4
			 0xf1ce34a5, /// 0x5c8
			 0xdede1582, /// 0x5cc
			 0xb2b457ed, /// 0x5d0
			 0x624533ea, /// 0x5d4
			 0xe7d53487, /// 0x5d8
			 0x12a34472, /// 0x5dc
			 0x0df651dc, /// 0x5e0
			 0x25e881d1, /// 0x5e4
			 0xd5e51066, /// 0x5e8
			 0xd93c5e56, /// 0x5ec
			 0x088b4d21, /// 0x5f0
			 0xffc10c2d, /// 0x5f4
			 0x103b8f02, /// 0x5f8
			 0x58c4a43e, /// 0x5fc
			 0xc1bb97c2, /// 0x600
			 0xf4bff331, /// 0x604
			 0x6e83039e, /// 0x608
			 0xc0275dc4, /// 0x60c
			 0x06a27aa3, /// 0x610
			 0x8bb0fdf8, /// 0x614
			 0x50129450, /// 0x618
			 0xad729e42, /// 0x61c
			 0x16ca56ea, /// 0x620
			 0x8726ce5b, /// 0x624
			 0xba4edaae, /// 0x628
			 0x306a57e1, /// 0x62c
			 0x1b3ba879, /// 0x630
			 0xfdeab17b, /// 0x634
			 0x17c80bc3, /// 0x638
			 0xa79a45b7, /// 0x63c
			 0x9242fe80, /// 0x640
			 0x949a06cd, /// 0x644
			 0xf8fd0b46, /// 0x648
			 0x0e7791f1, /// 0x64c
			 0x0a127117, /// 0x650
			 0x37581e68, /// 0x654
			 0x1f4b1511, /// 0x658
			 0x0d9b4df6, /// 0x65c
			 0x5abf03d4, /// 0x660
			 0xcad8e61f, /// 0x664
			 0xfcb08629, /// 0x668
			 0x8e83a514, /// 0x66c
			 0xb4b532f8, /// 0x670
			 0xb72e8b8e, /// 0x674
			 0x07b790d7, /// 0x678
			 0xba8b076a, /// 0x67c
			 0xc434cc93, /// 0x680
			 0x7d042c1c, /// 0x684
			 0x2dcd61eb, /// 0x688
			 0xc8985691, /// 0x68c
			 0x97b9e37b, /// 0x690
			 0xa329a9d7, /// 0x694
			 0x6757c4a9, /// 0x698
			 0x8ad1e41c, /// 0x69c
			 0x5824f783, /// 0x6a0
			 0xba0d4ec1, /// 0x6a4
			 0x54060ef6, /// 0x6a8
			 0xd5a73035, /// 0x6ac
			 0x4707051d, /// 0x6b0
			 0x030478f7, /// 0x6b4
			 0x0832d246, /// 0x6b8
			 0x5ea9eaba, /// 0x6bc
			 0xe18cf5d7, /// 0x6c0
			 0x44ce4a38, /// 0x6c4
			 0x57a87618, /// 0x6c8
			 0x4faf1216, /// 0x6cc
			 0x6d1af245, /// 0x6d0
			 0x3fa176cf, /// 0x6d4
			 0xc7fe7342, /// 0x6d8
			 0x8586c2ea, /// 0x6dc
			 0x28156d38, /// 0x6e0
			 0x28438a3f, /// 0x6e4
			 0x9302f5ed, /// 0x6e8
			 0xa8a88d32, /// 0x6ec
			 0x7cfbd7b8, /// 0x6f0
			 0x77f28716, /// 0x6f4
			 0xc53c34f9, /// 0x6f8
			 0xf735e7dc, /// 0x6fc
			 0x37218fc7, /// 0x700
			 0xcf741c8d, /// 0x704
			 0x8efdd833, /// 0x708
			 0xdf28efa0, /// 0x70c
			 0x52cdb391, /// 0x710
			 0x17e59473, /// 0x714
			 0x30478b75, /// 0x718
			 0xd24e0f85, /// 0x71c
			 0xce6edd04, /// 0x720
			 0x19fac563, /// 0x724
			 0xf13a0223, /// 0x728
			 0xb9acd4df, /// 0x72c
			 0x28790ffc, /// 0x730
			 0x4b30e9d0, /// 0x734
			 0xecc98fca, /// 0x738
			 0x1c781e12, /// 0x73c
			 0x9505126f, /// 0x740
			 0x0faaf0d0, /// 0x744
			 0x2df1c6b7, /// 0x748
			 0xdf99c5a3, /// 0x74c
			 0x034496dc, /// 0x750
			 0x2b0906bb, /// 0x754
			 0xa7d072d4, /// 0x758
			 0x16dcca40, /// 0x75c
			 0x004e1408, /// 0x760
			 0xace02eb2, /// 0x764
			 0x51f7d8ff, /// 0x768
			 0x05e23825, /// 0x76c
			 0xed3a1b8d, /// 0x770
			 0xc5331d68, /// 0x774
			 0xa72661a2, /// 0x778
			 0x8c51d8f9, /// 0x77c
			 0x0ad7afe6, /// 0x780
			 0xd9e1f97b, /// 0x784
			 0x35436e25, /// 0x788
			 0xa5ae7398, /// 0x78c
			 0x2c2f0624, /// 0x790
			 0x34dede2c, /// 0x794
			 0x241ff6c1, /// 0x798
			 0xd338b1bc, /// 0x79c
			 0xf32f0b59, /// 0x7a0
			 0x00031be4, /// 0x7a4
			 0xef286881, /// 0x7a8
			 0xb64b432a, /// 0x7ac
			 0x87201886, /// 0x7b0
			 0xb4729624, /// 0x7b4
			 0xf15ba7ab, /// 0x7b8
			 0x327e2a21, /// 0x7bc
			 0xbac6103d, /// 0x7c0
			 0x60b39b2e, /// 0x7c4
			 0xaf916431, /// 0x7c8
			 0x4e1ac6fa, /// 0x7cc
			 0xed0cfd11, /// 0x7d0
			 0x23faea8c, /// 0x7d4
			 0x5372e6eb, /// 0x7d8
			 0x7a875b76, /// 0x7dc
			 0x434d3a5c, /// 0x7e0
			 0xd43e5a38, /// 0x7e4
			 0x4508ae09, /// 0x7e8
			 0xb1e8bb57, /// 0x7ec
			 0x01fde3a6, /// 0x7f0
			 0xa0c2202f, /// 0x7f4
			 0xc9b8b808, /// 0x7f8
			 0x0684c1d6, /// 0x7fc
			 0xcf87f4c6, /// 0x800
			 0x0476be96, /// 0x804
			 0x6e547dd0, /// 0x808
			 0xa14b9d6b, /// 0x80c
			 0x9c9066d2, /// 0x810
			 0xf566f570, /// 0x814
			 0xe1dbfdb2, /// 0x818
			 0x26412608, /// 0x81c
			 0x87b1bd52, /// 0x820
			 0x7d6d916f, /// 0x824
			 0xe987b09e, /// 0x828
			 0x1cf6b825, /// 0x82c
			 0xc05c5481, /// 0x830
			 0x9449bead, /// 0x834
			 0x35b7822c, /// 0x838
			 0x5d0514d3, /// 0x83c
			 0x77f5129a, /// 0x840
			 0xab23e378, /// 0x844
			 0x2a4cc573, /// 0x848
			 0x82e2e092, /// 0x84c
			 0x9c0a675e, /// 0x850
			 0x6a6cc559, /// 0x854
			 0xf3d4449f, /// 0x858
			 0x6f77f23e, /// 0x85c
			 0x907ac622, /// 0x860
			 0xb5adc7fd, /// 0x864
			 0x888cf54c, /// 0x868
			 0x90541ca6, /// 0x86c
			 0x06ef96c6, /// 0x870
			 0xa34f569e, /// 0x874
			 0x3d6c353a, /// 0x878
			 0x773e0eaa, /// 0x87c
			 0x7e7f69df, /// 0x880
			 0xccc9e084, /// 0x884
			 0x7056d9be, /// 0x888
			 0xa3dfe01b, /// 0x88c
			 0x4adb0e55, /// 0x890
			 0x0035acc7, /// 0x894
			 0x97d64700, /// 0x898
			 0xead21858, /// 0x89c
			 0x8c58f355, /// 0x8a0
			 0x665b7858, /// 0x8a4
			 0x2af28c90, /// 0x8a8
			 0x238a7778, /// 0x8ac
			 0xee1b0c97, /// 0x8b0
			 0x0597b9ca, /// 0x8b4
			 0x471ac268, /// 0x8b8
			 0x76cec3e8, /// 0x8bc
			 0x7b8033da, /// 0x8c0
			 0x91f839a5, /// 0x8c4
			 0xa2f41f70, /// 0x8c8
			 0xaa419401, /// 0x8cc
			 0x725452f2, /// 0x8d0
			 0x1d22e4f7, /// 0x8d4
			 0x143933d1, /// 0x8d8
			 0xa0475fd1, /// 0x8dc
			 0x61f83f42, /// 0x8e0
			 0xa781675a, /// 0x8e4
			 0xdb61c35d, /// 0x8e8
			 0x0aa2dad5, /// 0x8ec
			 0xd138fe53, /// 0x8f0
			 0xd3bbab24, /// 0x8f4
			 0x4ab1f649, /// 0x8f8
			 0x8cfac065, /// 0x8fc
			 0xdce74a87, /// 0x900
			 0x57c42305, /// 0x904
			 0x3854ddf5, /// 0x908
			 0x2fc80d4e, /// 0x90c
			 0x147017f9, /// 0x910
			 0x695253d6, /// 0x914
			 0xf94def37, /// 0x918
			 0x140fe28a, /// 0x91c
			 0x4c296f76, /// 0x920
			 0x50ae2a3a, /// 0x924
			 0x53d53335, /// 0x928
			 0xb9dccafe, /// 0x92c
			 0xcd3c701b, /// 0x930
			 0x57a9641c, /// 0x934
			 0x39be4f20, /// 0x938
			 0x842ce039, /// 0x93c
			 0x7beaadc2, /// 0x940
			 0xffb614c7, /// 0x944
			 0xa3acf5e3, /// 0x948
			 0x1d52153d, /// 0x94c
			 0xa46f3fd4, /// 0x950
			 0x025f1f94, /// 0x954
			 0x3b7b80f5, /// 0x958
			 0x86f469f7, /// 0x95c
			 0x78cab51f, /// 0x960
			 0xa25edca1, /// 0x964
			 0x3b0668dd, /// 0x968
			 0xe842258f, /// 0x96c
			 0xd1d0dc84, /// 0x970
			 0xee950a69, /// 0x974
			 0xe0c48189, /// 0x978
			 0xa355a108, /// 0x97c
			 0x7b5b52f6, /// 0x980
			 0x53574fb0, /// 0x984
			 0xf14aeeeb, /// 0x988
			 0x9859d485, /// 0x98c
			 0xa3604e62, /// 0x990
			 0x03a32258, /// 0x994
			 0x43c3f585, /// 0x998
			 0x2fecb0e7, /// 0x99c
			 0x65bb2bd0, /// 0x9a0
			 0x0bf51fbc, /// 0x9a4
			 0xc3c1d860, /// 0x9a8
			 0xb1994d63, /// 0x9ac
			 0xb9b995f7, /// 0x9b0
			 0x32dbb265, /// 0x9b4
			 0xaee34098, /// 0x9b8
			 0x4d5703a8, /// 0x9bc
			 0x5e837341, /// 0x9c0
			 0xa3103480, /// 0x9c4
			 0xd6057334, /// 0x9c8
			 0xc1d5178c, /// 0x9cc
			 0x1bf1a4f6, /// 0x9d0
			 0x62147b4f, /// 0x9d4
			 0xf602f58d, /// 0x9d8
			 0xec67d6b9, /// 0x9dc
			 0xb742bd83, /// 0x9e0
			 0xcac4eea6, /// 0x9e4
			 0xc09f4733, /// 0x9e8
			 0xbb516ba5, /// 0x9ec
			 0x20713148, /// 0x9f0
			 0xfdfe22f9, /// 0x9f4
			 0xe919d3e8, /// 0x9f8
			 0x0e120df2, /// 0x9fc
			 0x268d1beb, /// 0xa00
			 0xc0349db4, /// 0xa04
			 0xaef974d8, /// 0xa08
			 0x9308ea5a, /// 0xa0c
			 0xdad8ec25, /// 0xa10
			 0x18b02a50, /// 0xa14
			 0x236a1098, /// 0xa18
			 0xa684b07e, /// 0xa1c
			 0x3374afc2, /// 0xa20
			 0x9526f31e, /// 0xa24
			 0x5964f19b, /// 0xa28
			 0x0416f555, /// 0xa2c
			 0x7b2e859c, /// 0xa30
			 0xd85dcb7b, /// 0xa34
			 0x45d81cd3, /// 0xa38
			 0x9241c52d, /// 0xa3c
			 0xd4475e48, /// 0xa40
			 0xb5d904ec, /// 0xa44
			 0xb227fefd, /// 0xa48
			 0xc3f0a741, /// 0xa4c
			 0xb8908338, /// 0xa50
			 0xbf967238, /// 0xa54
			 0x80dfffce, /// 0xa58
			 0x3e32e78a, /// 0xa5c
			 0xf2abda60, /// 0xa60
			 0xa3dd2075, /// 0xa64
			 0x019b3908, /// 0xa68
			 0x6b67bb23, /// 0xa6c
			 0x7cc4834d, /// 0xa70
			 0x7f4da143, /// 0xa74
			 0x7b3cbb9d, /// 0xa78
			 0x2d61edfe, /// 0xa7c
			 0x393ca6fe, /// 0xa80
			 0xdef48468, /// 0xa84
			 0x916ed01a, /// 0xa88
			 0x92d98e1d, /// 0xa8c
			 0x75b7c07f, /// 0xa90
			 0xb6b707d8, /// 0xa94
			 0x5ee8adff, /// 0xa98
			 0x1f880d5b, /// 0xa9c
			 0x32d93b41, /// 0xaa0
			 0xdb50592d, /// 0xaa4
			 0x25f0a31d, /// 0xaa8
			 0x67c120c1, /// 0xaac
			 0xbc9e3172, /// 0xab0
			 0xa402b95a, /// 0xab4
			 0xde3e5248, /// 0xab8
			 0x1bb44b07, /// 0xabc
			 0xd647d16d, /// 0xac0
			 0xf902dc14, /// 0xac4
			 0x1fb88e44, /// 0xac8
			 0xad981af8, /// 0xacc
			 0x7835faf3, /// 0xad0
			 0xcd588770, /// 0xad4
			 0x9ad4b8f0, /// 0xad8
			 0x6dce8ebf, /// 0xadc
			 0x4456469e, /// 0xae0
			 0xe8b258f8, /// 0xae4
			 0xfdd08e73, /// 0xae8
			 0x05e3301f, /// 0xaec
			 0xad46d275, /// 0xaf0
			 0x51875b71, /// 0xaf4
			 0xe721a894, /// 0xaf8
			 0xc7283bdb, /// 0xafc
			 0x88e395eb, /// 0xb00
			 0xbc31d27f, /// 0xb04
			 0xd095f3d8, /// 0xb08
			 0xa9071ef6, /// 0xb0c
			 0xf1d5952e, /// 0xb10
			 0x2b42257c, /// 0xb14
			 0xbad1f408, /// 0xb18
			 0xb8f8fe39, /// 0xb1c
			 0xd1686804, /// 0xb20
			 0xbe920f00, /// 0xb24
			 0x81579492, /// 0xb28
			 0x1f9f410f, /// 0xb2c
			 0x04cca1b8, /// 0xb30
			 0x6722d248, /// 0xb34
			 0xd758af16, /// 0xb38
			 0xb3747ac3, /// 0xb3c
			 0x75d68339, /// 0xb40
			 0x5f2b8a1a, /// 0xb44
			 0x80797499, /// 0xb48
			 0xca7e2b1b, /// 0xb4c
			 0xc8a90bdf, /// 0xb50
			 0x32bead42, /// 0xb54
			 0x68885617, /// 0xb58
			 0x308084cf, /// 0xb5c
			 0xcc8375f1, /// 0xb60
			 0xaa240040, /// 0xb64
			 0x901dea0d, /// 0xb68
			 0x2b2a111b, /// 0xb6c
			 0x27c4bd95, /// 0xb70
			 0x8b9e7d39, /// 0xb74
			 0xa4fc8d38, /// 0xb78
			 0x7ad39bdd, /// 0xb7c
			 0x0c625962, /// 0xb80
			 0xcbc2c361, /// 0xb84
			 0x68e974d3, /// 0xb88
			 0xb1dafa90, /// 0xb8c
			 0xa02a8eca, /// 0xb90
			 0x87ca8562, /// 0xb94
			 0x04bd9db2, /// 0xb98
			 0x07fcf7b4, /// 0xb9c
			 0xbef42c3b, /// 0xba0
			 0x7916a050, /// 0xba4
			 0x781ccc6e, /// 0xba8
			 0x4e85d322, /// 0xbac
			 0x34725871, /// 0xbb0
			 0x4920c28c, /// 0xbb4
			 0x4f4d5607, /// 0xbb8
			 0xf833c44b, /// 0xbbc
			 0xeeca5881, /// 0xbc0
			 0x20c93011, /// 0xbc4
			 0x90134097, /// 0xbc8
			 0x1b74e5e9, /// 0xbcc
			 0xea3cfa10, /// 0xbd0
			 0x986714c6, /// 0xbd4
			 0x038e096f, /// 0xbd8
			 0x27300522, /// 0xbdc
			 0x8e2f2a4e, /// 0xbe0
			 0x06a10cf3, /// 0xbe4
			 0xc4e811db, /// 0xbe8
			 0x7efd6fa0, /// 0xbec
			 0xe76119ca, /// 0xbf0
			 0x09283727, /// 0xbf4
			 0xb7c81ff5, /// 0xbf8
			 0x19bb3eef, /// 0xbfc
			 0xbe260932, /// 0xc00
			 0x259ba4e2, /// 0xc04
			 0xec29ad15, /// 0xc08
			 0x3ddf52f6, /// 0xc0c
			 0x0f876279, /// 0xc10
			 0x38e8fde3, /// 0xc14
			 0x9d8d027a, /// 0xc18
			 0xf154a311, /// 0xc1c
			 0x58d32ab3, /// 0xc20
			 0x43fddf30, /// 0xc24
			 0x8b46ec33, /// 0xc28
			 0xf8439b6a, /// 0xc2c
			 0x8635d31d, /// 0xc30
			 0xda8b65a2, /// 0xc34
			 0x6bff4b16, /// 0xc38
			 0x693b0749, /// 0xc3c
			 0x71af822f, /// 0xc40
			 0xc706cba7, /// 0xc44
			 0xca33a67a, /// 0xc48
			 0xe8f504dc, /// 0xc4c
			 0x9ec042d7, /// 0xc50
			 0x69dbace2, /// 0xc54
			 0x1c8de4fb, /// 0xc58
			 0xfbcb003e, /// 0xc5c
			 0x53211973, /// 0xc60
			 0x2fbc8929, /// 0xc64
			 0x091e1d2f, /// 0xc68
			 0xf87cc14a, /// 0xc6c
			 0x479aa9e9, /// 0xc70
			 0x468b5a04, /// 0xc74
			 0xdc727411, /// 0xc78
			 0xa5c0823a, /// 0xc7c
			 0x1fded437, /// 0xc80
			 0x5cf63af4, /// 0xc84
			 0x2bfabb11, /// 0xc88
			 0xb2e0737d, /// 0xc8c
			 0xcb0d582c, /// 0xc90
			 0x2e8da466, /// 0xc94
			 0xa494ab73, /// 0xc98
			 0x133a3774, /// 0xc9c
			 0xef88cad6, /// 0xca0
			 0xc02e59c4, /// 0xca4
			 0x067626ef, /// 0xca8
			 0xf7176edc, /// 0xcac
			 0x81bb5d87, /// 0xcb0
			 0x0d1ab40b, /// 0xcb4
			 0x50e3d77a, /// 0xcb8
			 0xd8cc7d91, /// 0xcbc
			 0xb42589c7, /// 0xcc0
			 0x10f085e5, /// 0xcc4
			 0x21cbd6fa, /// 0xcc8
			 0xdb99143f, /// 0xccc
			 0x5bbb7f4d, /// 0xcd0
			 0x6e18ec6c, /// 0xcd4
			 0xb734d82f, /// 0xcd8
			 0x5bc3f6a9, /// 0xcdc
			 0x122f0fab, /// 0xce0
			 0x62d0b213, /// 0xce4
			 0x62ed576b, /// 0xce8
			 0x2b8525cd, /// 0xcec
			 0xa034a411, /// 0xcf0
			 0x755d8281, /// 0xcf4
			 0x34dcf0d9, /// 0xcf8
			 0x53d9b992, /// 0xcfc
			 0x0fbe9b88, /// 0xd00
			 0x2d053236, /// 0xd04
			 0x700c190e, /// 0xd08
			 0x6f33b289, /// 0xd0c
			 0x380c167f, /// 0xd10
			 0xa3305822, /// 0xd14
			 0xdeb2283c, /// 0xd18
			 0x77c8e9f1, /// 0xd1c
			 0xc61f9b66, /// 0xd20
			 0x037765d3, /// 0xd24
			 0x00d5640c, /// 0xd28
			 0x0ed17f09, /// 0xd2c
			 0xac1261c5, /// 0xd30
			 0x3739f30b, /// 0xd34
			 0x7a9a07a2, /// 0xd38
			 0x7b4289fd, /// 0xd3c
			 0x272254a4, /// 0xd40
			 0x63c4e882, /// 0xd44
			 0x077ccc5a, /// 0xd48
			 0xe6d12b98, /// 0xd4c
			 0x8d092e19, /// 0xd50
			 0x1bf23f4e, /// 0xd54
			 0x986188c4, /// 0xd58
			 0x20c314ec, /// 0xd5c
			 0xcdae2576, /// 0xd60
			 0xd0e13bc8, /// 0xd64
			 0x75596eda, /// 0xd68
			 0xdb25898d, /// 0xd6c
			 0x2ea4a401, /// 0xd70
			 0xdbcae5bc, /// 0xd74
			 0x49bdd0f3, /// 0xd78
			 0x6d509f59, /// 0xd7c
			 0x2b5feb30, /// 0xd80
			 0x513588a3, /// 0xd84
			 0x1370a2a3, /// 0xd88
			 0x3811bd6b, /// 0xd8c
			 0x5087f635, /// 0xd90
			 0xf6cd0aa5, /// 0xd94
			 0xb98f5464, /// 0xd98
			 0x8526e1d3, /// 0xd9c
			 0x87f47062, /// 0xda0
			 0xb1506865, /// 0xda4
			 0x6b354810, /// 0xda8
			 0x12bbef54, /// 0xdac
			 0x5e78cff3, /// 0xdb0
			 0x909203ff, /// 0xdb4
			 0x4437f18c, /// 0xdb8
			 0x1ed7c265, /// 0xdbc
			 0x02034657, /// 0xdc0
			 0x6e0b5127, /// 0xdc4
			 0x27b04bc3, /// 0xdc8
			 0x60a27b03, /// 0xdcc
			 0xb1233abf, /// 0xdd0
			 0x097cf447, /// 0xdd4
			 0xe27ff7b3, /// 0xdd8
			 0xa60df25c, /// 0xddc
			 0xf7ded1d0, /// 0xde0
			 0xd3bc9ae3, /// 0xde4
			 0xcf347455, /// 0xde8
			 0xb51a01e7, /// 0xdec
			 0x0a039511, /// 0xdf0
			 0xbab98f06, /// 0xdf4
			 0x60e15382, /// 0xdf8
			 0x142c637e, /// 0xdfc
			 0x00524116, /// 0xe00
			 0xeaea775e, /// 0xe04
			 0x4bfa02a3, /// 0xe08
			 0xb0d5be6f, /// 0xe0c
			 0x915bb5cf, /// 0xe10
			 0xefbae3db, /// 0xe14
			 0x0b3a7a09, /// 0xe18
			 0x471b71a2, /// 0xe1c
			 0xdcd0d17e, /// 0xe20
			 0x68d54014, /// 0xe24
			 0xb9a650c4, /// 0xe28
			 0x2005d00c, /// 0xe2c
			 0x4a0080c6, /// 0xe30
			 0x324c56b9, /// 0xe34
			 0x056d765b, /// 0xe38
			 0x7a34c56e, /// 0xe3c
			 0x84dbb5d0, /// 0xe40
			 0xbc780be4, /// 0xe44
			 0x14a9f7f0, /// 0xe48
			 0x3b2cb3a0, /// 0xe4c
			 0xee09b73d, /// 0xe50
			 0xa9b3ce85, /// 0xe54
			 0x38edb3db, /// 0xe58
			 0x808551db, /// 0xe5c
			 0xcc5c8d01, /// 0xe60
			 0xad37da3a, /// 0xe64
			 0x0447ae50, /// 0xe68
			 0xa00d218a, /// 0xe6c
			 0x6a323e8a, /// 0xe70
			 0x7a00db65, /// 0xe74
			 0x31f86933, /// 0xe78
			 0xd034a378, /// 0xe7c
			 0x57087d4d, /// 0xe80
			 0xb90ac70e, /// 0xe84
			 0x2faf5329, /// 0xe88
			 0x4e3af225, /// 0xe8c
			 0x5ad0efe9, /// 0xe90
			 0x87f1b925, /// 0xe94
			 0x22b52c49, /// 0xe98
			 0x60244fbf, /// 0xe9c
			 0xf2ff1905, /// 0xea0
			 0x52766270, /// 0xea4
			 0x2e4d6cb6, /// 0xea8
			 0xdf3e29d4, /// 0xeac
			 0x05197c74, /// 0xeb0
			 0x5d921e34, /// 0xeb4
			 0x5c35fd83, /// 0xeb8
			 0xf9bc725b, /// 0xebc
			 0x795ca993, /// 0xec0
			 0xf6f46a4c, /// 0xec4
			 0x7640bab2, /// 0xec8
			 0x7cac7c0f, /// 0xecc
			 0x5ab72286, /// 0xed0
			 0x7d4b8838, /// 0xed4
			 0xdabd9c1a, /// 0xed8
			 0xde4ebc2a, /// 0xedc
			 0x5e934fb3, /// 0xee0
			 0xfa18739e, /// 0xee4
			 0xa399ed06, /// 0xee8
			 0x9ed2f602, /// 0xeec
			 0x3674fb3f, /// 0xef0
			 0x7aa7e8ed, /// 0xef4
			 0xec7e2ae6, /// 0xef8
			 0xcd022a59, /// 0xefc
			 0xf156058e, /// 0xf00
			 0xd67b1f35, /// 0xf04
			 0xb445d137, /// 0xf08
			 0x325c1ae0, /// 0xf0c
			 0x0edc16a0, /// 0xf10
			 0x8029d075, /// 0xf14
			 0x287aad63, /// 0xf18
			 0xf8ae27b7, /// 0xf1c
			 0x5054f4e9, /// 0xf20
			 0xc4661ba5, /// 0xf24
			 0xf67effb0, /// 0xf28
			 0x58b41804, /// 0xf2c
			 0x3a8b58d3, /// 0xf30
			 0x044f335f, /// 0xf34
			 0xccc8bab8, /// 0xf38
			 0xd5d035bc, /// 0xf3c
			 0xfd4d5685, /// 0xf40
			 0xbc0e4237, /// 0xf44
			 0x06578379, /// 0xf48
			 0xaf310124, /// 0xf4c
			 0x543833a4, /// 0xf50
			 0x375d4e00, /// 0xf54
			 0x300b10b2, /// 0xf58
			 0xc7186f9a, /// 0xf5c
			 0x031eafc4, /// 0xf60
			 0x4da2f2c2, /// 0xf64
			 0x29487f9a, /// 0xf68
			 0x500bfe0e, /// 0xf6c
			 0xcc16f59d, /// 0xf70
			 0xcda45206, /// 0xf74
			 0xeea247f3, /// 0xf78
			 0xeb23e933, /// 0xf7c
			 0x90521b5a, /// 0xf80
			 0x8624db70, /// 0xf84
			 0x25bad173, /// 0xf88
			 0xcb24ff60, /// 0xf8c
			 0x7c32cb0f, /// 0xf90
			 0xfac5f5fb, /// 0xf94
			 0x2bd2d832, /// 0xf98
			 0xcfed3d85, /// 0xf9c
			 0xcd2b6b3b, /// 0xfa0
			 0x18a5402c, /// 0xfa4
			 0xa4fc64e7, /// 0xfa8
			 0x4a1f487e, /// 0xfac
			 0x2731a1ba, /// 0xfb0
			 0xc17152e8, /// 0xfb4
			 0xaa0921c9, /// 0xfb8
			 0x22e98632, /// 0xfbc
			 0x1fcd7b2b, /// 0xfc0
			 0xee3b5c5a, /// 0xfc4
			 0x80dc4462, /// 0xfc8
			 0x31781e99, /// 0xfcc
			 0x3a47e76e, /// 0xfd0
			 0x28199e34, /// 0xfd4
			 0x5a02a8d4, /// 0xfd8
			 0x0b0a8282, /// 0xfdc
			 0x9647f81a, /// 0xfe0
			 0xa11ac7eb, /// 0xfe4
			 0x3b86edf5, /// 0xfe8
			 0x055a58d3, /// 0xfec
			 0xebd4b841, /// 0xff0
			 0x4d74f6de, /// 0xff4
			 0x64557a5e, /// 0xff8
			 0xc0a4bb7c /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00008
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00010
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00014
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0001c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00020
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00024
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00028
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00030
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0003c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00040
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00044
			 0xbf028f5c,                                                  // -0.51                                       /// 00048
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00050
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00054
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0005c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00064
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0006c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00074
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00078
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0007c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00084
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00088
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00090
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0xffc00001,                                                  // -signaling NaN                              /// 000a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c0
			 0x55555555,                                                  // 4 random values                             /// 000c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00100
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00104
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00108
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0010c
			 0x7f800000,                                                  // inf                                         /// 00110
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x3f028f5c,                                                  // 0.51                                        /// 00118
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0011c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x33333333,                                                  // 4 random values                             /// 00124
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00128
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00130
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00134
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00138
			 0xbf028f5c,                                                  // -0.51                                       /// 0013c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00140
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00148
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0014c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00150
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00158
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xff800000,                                                  // -inf                                        /// 00160
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00164
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0016c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00174
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x0e000007,                                                  // Trailing 1s:                                /// 00184
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00188
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0018c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00190
			 0x55555555,                                                  // 4 random values                             /// 00194
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00200
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00208
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0020c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00210
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00214
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00218
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00220
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00224
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00228
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0022c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x4b000000,                                                  // 8388608.0                                   /// 00238
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00240
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00244
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00248
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00250
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00258
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00260
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00264
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00268
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00274
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0028c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00298
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0029c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x55555555,                                                  // 4 random values                             /// 002d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00300
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00304
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00308
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00310
			 0xff800000,                                                  // -inf                                        /// 00314
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00318
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0031c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00320
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0032c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00330
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00334
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0033c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00344
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00348
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00350
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00358
			 0xbf028f5c,                                                  // -0.51                                       /// 0035c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x4b000000,                                                  // 8388608.0                                   /// 00368
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0036c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00370
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00374
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00378
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0037c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00380
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00384
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00394
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00398
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00400
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0040c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00410
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00418
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0041c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00420
			 0x33333333,                                                  // 4 random values                             /// 00424
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00428
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0042c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00430
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e000003,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00440
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00444
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00448
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0044c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00450
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00454
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xcb000000,                                                  // -8388608.0                                  /// 00460
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00464
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0046c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0c400000,                                                  // Leading 1s:                                 /// 00474
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c400000,                                                  // Leading 1s:                                 /// 0047c
			 0x7f800000,                                                  // inf                                         /// 00480
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00488
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00498
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0049c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xffc00001,                                                  // -signaling NaN                              /// 004c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0xff800000,                                                  // -inf                                        /// 004f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00500
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00504
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00508
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00514
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00520
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00524
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00538
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0053c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00540
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00550
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00560
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0056c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00580
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00584
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00588
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0058c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0059c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c8
			 0xcb000000,                                                  // -8388608.0                                  /// 005cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x33333333,                                                  // 4 random values                             /// 005e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00600
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00604
			 0x0c600000,                                                  // Leading 1s:                                 /// 00608
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0061c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00620
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00624
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00628
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0062c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00630
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00634
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00638
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0063c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x0c780000,                                                  // Leading 1s:                                 /// 0064c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00650
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00654
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00658
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0065c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00664
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0066c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00670
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0067c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00680
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00688
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0068c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00690
			 0x4b000000,                                                  // 8388608.0                                   /// 00694
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80000000,                                                  // -zero                                       /// 0069c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006bc
			 0x33333333,                                                  // 4 random values                             /// 006c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00704
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00708
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0070c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00710
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00718
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000000,                                                  // -zero                                       /// 00728
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000000,                                                  // -zero                                       /// 00734
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00740
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00744
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00748
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0074c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x0e000001,                                                  // Trailing 1s:                                /// 00754
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00760
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00768
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00770
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00774
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00780
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00788
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0078c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00790
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x4b000000,                                                  // 8388608.0                                   /// 00798
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007a4
			 0x7fc00001,                                                  // signaling NaN                               /// 007a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007b8
			 0x33333333,                                                  // 4 random values                             /// 007bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80000000,                                                  // -zero                                       /// 007d8
			 0x7f800000,                                                  // inf                                         /// 007dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x00011111,                                                  // 9.7958E-41                                  /// 00804
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00808
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0080c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00810
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00814
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0081c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00834
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00838
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00840
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00844
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00848
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0084c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00854
			 0xcb000000,                                                  // -8388608.0                                  /// 00858
			 0x0c780000,                                                  // Leading 1s:                                 /// 0085c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00864
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00870
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00874
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00878
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00880
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00884
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00888
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0088c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00890
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00894
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e000003,                                                  // Trailing 1s:                                /// 0089c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008fc
			 0x00000000,                                                  // zero                                        /// 00900
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00908
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0090c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00910
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0091c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00920
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00924
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00930
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00934
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00938
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0093c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000000,                                                  // zero                                        /// 00950
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0095c
			 0xff800000,                                                  // -inf                                        /// 00960
			 0x4b000000,                                                  // 8388608.0                                   /// 00964
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00968
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00974
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00978
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0097c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00980
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00984
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0098c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00994
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0099c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000000,                                                  // zero                                        /// 009bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a04
			 0xff800000,                                                  // -inf                                        /// 00a08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a28
			 0x3f028f5c,                                                  // 0.51                                        /// 00a2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a50
			 0x7f800000,                                                  // inf                                         /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00aa0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x33333333,                                                  // 4 random values                             /// 00aa8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ab4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ab8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00abc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ac0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ad8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ae4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ae8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00af8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xff800000,                                                  // -inf                                        /// 00b10
			 0x7fc00001,                                                  // signaling NaN                               /// 00b14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ba0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bfc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c2c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x3f028f5c,                                                  // 0.51                                        /// 00c74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x33333333,                                                  // 4 random values                             /// 00c98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ccc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xff800000,                                                  // -inf                                        /// 00d6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dbc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dcc
			 0x7f800000,                                                  // inf                                         /// 00dd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ddc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00de0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x00000000,                                                  // zero                                        /// 00df8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ea8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ebc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ecc
			 0x55555555,                                                  // 4 random values                             /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00edc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ee4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ee8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ef4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00efc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x33333333,                                                  // 4 random values                             /// 00f24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f54
			 0xcb000000,                                                  // -8388608.0                                  /// 00f58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80800001 // min norm + 1ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0c400000,                                                  // Leading 1s:                                 /// 00004
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00008
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00010
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00014
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00018
			 0x7fc00001,                                                  // signaling NaN                               /// 0001c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00024
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0002c
			 0x00000000,                                                  // zero                                        /// 00030
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00034
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0003c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00040
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00044
			 0x55555555,                                                  // 4 random values                             /// 00048
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00050
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x4b000000,                                                  // 8388608.0                                   /// 0005c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xff800000,                                                  // -inf                                        /// 00064
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00068
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0006c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00070
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00074
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00078
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0007c
			 0x7fc00001,                                                  // signaling NaN                               /// 00080
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00088
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0008c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00094
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ac
			 0xbf028f5c,                                                  // -0.51                                       /// 000b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000d4
			 0x55555555,                                                  // 4 random values                             /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00104
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00108
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00118
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0011c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00120
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00124
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00128
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0012c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00130
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00134
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00140
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00148
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00150
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00154
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00158
			 0x0e000003,                                                  // Trailing 1s:                                /// 0015c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00160
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00164
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0016c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00180
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00184
			 0x0e000007,                                                  // Trailing 1s:                                /// 00188
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xff800000,                                                  // -inf                                        /// 00190
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00198
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00208
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0020c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00214
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0c700000,                                                  // Leading 1s:                                 /// 0021c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00220
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00224
			 0x0e000007,                                                  // Trailing 1s:                                /// 00228
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0022c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00230
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00234
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00238
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0023c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e000003,                                                  // Trailing 1s:                                /// 00248
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0024c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00250
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00254
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00258
			 0x0e000001,                                                  // Trailing 1s:                                /// 0025c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00264
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00268
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0026c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00270
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00274
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0027c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00290
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00294
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c0
			 0xff800000,                                                  // -inf                                        /// 002c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c8
			 0x80000000,                                                  // -zero                                       /// 002cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00300
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00308
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0030c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000000,                                                  // -zero                                       /// 00314
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00318
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00320
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00324
			 0x3f028f5c,                                                  // 0.51                                        /// 00328
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00334
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0033c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00340
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00344
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0034c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00350
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0035c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00360
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00364
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00368
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00370
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0037c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00380
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00390
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00394
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00400
			 0x7f800000,                                                  // inf                                         /// 00404
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00408
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0040c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00410
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00414
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00418
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0041c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00424
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0042c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00430
			 0x7f800000,                                                  // inf                                         /// 00434
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0043c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00440
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00444
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e000003,                                                  // Trailing 1s:                                /// 0044c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00458
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0045c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00460
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00468
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0046c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00470
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00478
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00480
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00488
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0048c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00490
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00498
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c0
			 0xffc00001,                                                  // -signaling NaN                              /// 004c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d0
			 0xff800000,                                                  // -inf                                        /// 004d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00500
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00508
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0050c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00510
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00514
			 0x33333333,                                                  // 4 random values                             /// 00518
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00520
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00524
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00528
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0052c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00530
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0053c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00540
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00548
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0054c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00550
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00558
			 0xffc00001,                                                  // -signaling NaN                              /// 0055c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00560
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00564
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00568
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0056c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00574
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0057c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00584
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00588
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00590
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0059c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00608
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0060c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7f800000,                                                  // inf                                         /// 00614
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0xcb000000,                                                  // -8388608.0                                  /// 00624
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00628
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0062c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00630
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00634
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00638
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00640
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00644
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00648
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00650
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0xbf028f5c,                                                  // -0.51                                       /// 00668
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0066c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00670
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00000000,                                                  // zero                                        /// 0067c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0xff800000,                                                  // -inf                                        /// 00684
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00688
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0068c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00698
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00700
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00704
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00708
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00718
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0071c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00720
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00724
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00728
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0072c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00740
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00744
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00748
			 0xff800000,                                                  // -inf                                        /// 0074c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00750
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00754
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00764
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0076c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00774
			 0xffc00001,                                                  // -signaling NaN                              /// 00778
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0077c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00784
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00790
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00794
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00798
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0xcb000000,                                                  // -8388608.0                                  /// 007ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00800
			 0x0c780000,                                                  // Leading 1s:                                 /// 00804
			 0x0c700000,                                                  // Leading 1s:                                 /// 00808
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0080c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00810
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00814
			 0xffc00001,                                                  // -signaling NaN                              /// 00818
			 0x4b000000,                                                  // 8388608.0                                   /// 0081c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00000000,                                                  // zero                                        /// 00828
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0082c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00838
			 0x0c600000,                                                  // Leading 1s:                                 /// 0083c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00840
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00844
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00848
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00850
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00854
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0085c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00860
			 0x0e000003,                                                  // Trailing 1s:                                /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00870
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00874
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00878
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00880
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00888
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00894
			 0x0c780000,                                                  // Leading 1s:                                 /// 00898
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00908
			 0x00000000,                                                  // zero                                        /// 0090c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000000,                                                  // -zero                                       /// 00914
			 0x3f028f5c,                                                  // 0.51                                        /// 00918
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00920
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00924
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0092c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00930
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00934
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00938
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0093c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00944
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00948
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00960
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00964
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00968
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0097c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00980
			 0x0e000007,                                                  // Trailing 1s:                                /// 00984
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0098c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00990
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00994
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00998
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0099c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0xcb000000,                                                  // -8388608.0                                  /// 009e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ab4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00abc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ac0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ac8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00af0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00afc
			 0x00000000,                                                  // zero                                        /// 00b00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b8c
			 0x00000000,                                                  // zero                                        /// 00b90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ba8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c00
			 0x3f028f5c,                                                  // 0.51                                        /// 00c04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c50
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x33333333,                                                  // 4 random values                             /// 00c5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0xbf028f5c,                                                  // -0.51                                       /// 00c88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ca0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ccc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cdc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ce4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d18
			 0x4b000000,                                                  // 8388608.0                                   /// 00d1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d24
			 0xff800000,                                                  // -inf                                        /// 00d28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d58
			 0x7f800000,                                                  // inf                                         /// 00d5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d70
			 0xffc00001,                                                  // -signaling NaN                              /// 00d74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d9c
			 0x55555555,                                                  // 4 random values                             /// 00da0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00db0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ddc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00de8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00df4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00df8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dfc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x33333333,                                                  // 4 random values                             /// 00e90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ea4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00eb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80000000,                                                  // -zero                                       /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ed0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80000000,                                                  // -zero                                       /// 00eec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x4b000000,                                                  // 8388608.0                                   /// 00f08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x7f800000,                                                  // inf                                         /// 00f44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f50
			 0x4b000000,                                                  // 8388608.0                                   /// 00f54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fa0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fa4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fac
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fb4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fdc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fe8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ff0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff8
			 0xffc00000 // DefaultNan                                    // SP - ve numbers                             /// last
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
			 0x000004dc,
			 0x0000025c,
			 0x00000758,
			 0x000002e8,
			 0x00000734,
			 0x00000510,
			 0x000004f8,
			 0x00000080,

			 /// vpu register f2
			 0x41a00000,
			 0x41d00000,
			 0x41b80000,
			 0x40c00000,
			 0x41900000,
			 0x40800000,
			 0x41200000,
			 0x41c80000,

			 /// vpu register f3
			 0x41900000,
			 0x41c00000,
			 0x40a00000,
			 0x41a80000,
			 0x40400000,
			 0x41980000,
			 0x41c00000,
			 0x41800000,

			 /// vpu register f4
			 0x41c80000,
			 0x41980000,
			 0x41880000,
			 0x41d00000,
			 0x42000000,
			 0x41c80000,
			 0x40e00000,
			 0x41e00000,

			 /// vpu register f5
			 0x41700000,
			 0x41b80000,
			 0x41300000,
			 0x41900000,
			 0x41e00000,
			 0x41f00000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f6
			 0x41c80000,
			 0x41e80000,
			 0x41300000,
			 0x41000000,
			 0x41e80000,
			 0x41000000,
			 0x41d00000,
			 0x41600000,

			 /// vpu register f7
			 0x3f800000,
			 0x40a00000,
			 0x41300000,
			 0x41400000,
			 0x41d80000,
			 0x41900000,
			 0x41200000,
			 0x41a00000,

			 /// vpu register f8
			 0x40800000,
			 0x41c80000,
			 0x41b80000,
			 0x41000000,
			 0x41e80000,
			 0x41a80000,
			 0x41900000,
			 0x41500000,

			 /// vpu register f9
			 0x41400000,
			 0x41500000,
			 0x40c00000,
			 0x41b80000,
			 0x40800000,
			 0x41a00000,
			 0x41900000,
			 0x41880000,

			 /// vpu register f10
			 0x41e80000,
			 0x40000000,
			 0x41a00000,
			 0x41900000,
			 0x41d80000,
			 0x41980000,
			 0x40400000,
			 0x41d80000,

			 /// vpu register f11
			 0x41000000,
			 0x41f00000,
			 0x41c00000,
			 0x41700000,
			 0x41800000,
			 0x41c80000,
			 0x41880000,
			 0x41900000,

			 /// vpu register f12
			 0x41600000,
			 0x41b00000,
			 0x41200000,
			 0x41f80000,
			 0x41b80000,
			 0x41c80000,
			 0x40e00000,
			 0x41c00000,

			 /// vpu register f13
			 0x41800000,
			 0x41880000,
			 0x41880000,
			 0x41880000,
			 0x41d80000,
			 0x42000000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f14
			 0x41b00000,
			 0x41900000,
			 0x41c00000,
			 0x40c00000,
			 0x41880000,
			 0x41400000,
			 0x41b80000,
			 0x41980000,

			 /// vpu register f15
			 0x41300000,
			 0x41500000,
			 0x42000000,
			 0x41b80000,
			 0x41000000,
			 0x41d00000,
			 0x40e00000,
			 0x41c80000,

			 /// vpu register f16
			 0x41e80000,
			 0x41600000,
			 0x42000000,
			 0x41800000,
			 0x41400000,
			 0x41a80000,
			 0x41d00000,
			 0x3f800000,

			 /// vpu register f17
			 0x41b80000,
			 0x41400000,
			 0x41400000,
			 0x41e80000,
			 0x41d80000,
			 0x41b00000,
			 0x41c00000,
			 0x42000000,

			 /// vpu register f18
			 0x40c00000,
			 0x40c00000,
			 0x40000000,
			 0x42000000,
			 0x41d80000,
			 0x41980000,
			 0x40c00000,
			 0x41c80000,

			 /// vpu register f19
			 0x40400000,
			 0x40e00000,
			 0x41500000,
			 0x41f80000,
			 0x41700000,
			 0x41e00000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f20
			 0x41a80000,
			 0x40000000,
			 0x41200000,
			 0x41b80000,
			 0x40000000,
			 0x41c80000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f21
			 0x40400000,
			 0x40000000,
			 0x41000000,
			 0x41300000,
			 0x40a00000,
			 0x41800000,
			 0x41d80000,
			 0x41100000,

			 /// vpu register f22
			 0x41100000,
			 0x41400000,
			 0x42000000,
			 0x41a00000,
			 0x41f00000,
			 0x41a00000,
			 0x41d00000,
			 0x41000000,

			 /// vpu register f23
			 0x41300000,
			 0x41980000,
			 0x41b00000,
			 0x41600000,
			 0x41e00000,
			 0x41600000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f24
			 0x41d80000,
			 0x41e00000,
			 0x41600000,
			 0x40400000,
			 0x41400000,
			 0x41900000,
			 0x40a00000,
			 0x41f00000,

			 /// vpu register f25
			 0x41e00000,
			 0x40000000,
			 0x41700000,
			 0x41f80000,
			 0x41d80000,
			 0x41880000,
			 0x41b80000,
			 0x41a00000,

			 /// vpu register f26
			 0x41e00000,
			 0x41f00000,
			 0x41b80000,
			 0x41100000,
			 0x41400000,
			 0x40000000,
			 0x41a00000,
			 0x41c80000,

			 /// vpu register f27
			 0x41d80000,
			 0x41d00000,
			 0x41f80000,
			 0x41880000,
			 0x41900000,
			 0x3f800000,
			 0x41500000,
			 0x41880000,

			 /// vpu register f28
			 0x41900000,
			 0x41200000,
			 0x41400000,
			 0x40400000,
			 0x41c00000,
			 0x41c80000,
			 0x40e00000,
			 0x41880000,

			 /// vpu register f29
			 0x41b80000,
			 0x41400000,
			 0x41d00000,
			 0x41c00000,
			 0x41f80000,
			 0x41a80000,
			 0x40800000,
			 0x41f00000,

			 /// vpu register f30
			 0x41f80000,
			 0x41880000,
			 0x41b80000,
			 0x41c00000,
			 0x42000000,
			 0x41d80000,
			 0x41500000,
			 0x41d80000,

			 /// vpu register f31
			 0x40c00000,
			 0x41700000,
			 0x41f00000,
			 0x41f80000,
			 0x40400000,
			 0x41f00000,
			 0x41500000,
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
		"fcvt.un8.ps f21, f22\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f27, f14\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f8\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f12, f20\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f24, f19\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f5, f18\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f4, f18\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f24, f1\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f16, f4\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f18, f1\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f27, f27\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f8, f15\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f30, f11\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f17, f5\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f30, f21\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f10\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f2, f19\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f23, f19\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f6, f29\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f22, f28\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f22\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f29, f17\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f29, f12\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f7\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f25, f22\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f13, f5\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f20, f18\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f25, f26\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f6\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f25, f15\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f30, f18\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f8, f19\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f21, f28\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f24, f25\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f28\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f2, f14\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f3, f11\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f9, f16\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f22, f17\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f26, f10\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f30, f16\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f25, f19\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f16, f7\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f17\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f13, f18\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f30, f12\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f8, f21\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f22, f19\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f27, f14\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f25, f25\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f7, f23\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f21, f10\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f4\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f16, f28\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f27, f2\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f10, f10\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f8, f11\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f2, f3\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f16, f15\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f8\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f27, f19\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f2, f8\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f11, f17\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f2, f19\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f28, f10\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f5, f12\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f18, f19\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f8, f15\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f17, f18\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f17, f19\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f30, f20\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f10, f24\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f24, f7\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f29, f5\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f5, f0\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f25, f11\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f25\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f21, f16\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f19, f14\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f24, f22\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f22, f12\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f15, f21\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f26, f12\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f17, f3\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f10, f21\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f21, f1\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f15, f8\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f4, f26\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f17, f7\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f7, f14\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f22, f19\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f14, f14\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f20, f0\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f5, f7\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f24, f23\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f20, f10\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f3, f23\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f16, f15\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un8.ps f16, f10\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
