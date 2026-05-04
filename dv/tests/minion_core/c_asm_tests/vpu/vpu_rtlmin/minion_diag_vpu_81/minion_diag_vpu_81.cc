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
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00000
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00004
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x80011111,                                                  // -9.7958E-41                                 /// 0000c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00014
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00018
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00024
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00028
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0002c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00034
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00038
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0004c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00054
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0005c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00060
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00068
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00070
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00074
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00078
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0007c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00084
			 0xbf028f5c,                                                  // -0.51                                       /// 00088
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0008c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00090
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00094
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00098
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0009c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000a8
			 0x3f028f5c,                                                  // 0.51                                        /// 000ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000e4
			 0x7f800000,                                                  // inf                                         /// 000e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00100
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00108
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00110
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00118
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00124
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0012c
			 0x3f028f5c,                                                  // 0.51                                        /// 00130
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00134
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00138
			 0x4b000000,                                                  // 8388608.0                                   /// 0013c
			 0xcb000000,                                                  // -8388608.0                                  /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00144
			 0x0e000007,                                                  // Trailing 1s:                                /// 00148
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0014c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00150
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00154
			 0x33333333,                                                  // 4 random values                             /// 00158
			 0x0c600000,                                                  // Leading 1s:                                 /// 0015c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00160
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00164
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00168
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0016c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00170
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00180
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00188
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00190
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0019c
			 0x33333333,                                                  // 4 random values                             /// 001a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x4b000000,                                                  // 8388608.0                                   /// 001cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00204
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00208
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x7f800000,                                                  // inf                                         /// 00214
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00218
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00220
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0022c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00234
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00238
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0xff800000,                                                  // -inf                                        /// 00240
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00244
			 0xffc00001,                                                  // -signaling NaN                              /// 00248
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0024c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00250
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00254
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00258
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0025c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0026c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00270
			 0x7fc00001,                                                  // signaling NaN                               /// 00274
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00278
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00280
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00288
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0028c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00290
			 0xff800000,                                                  // -inf                                        /// 00294
			 0x7fc00001,                                                  // signaling NaN                               /// 00298
			 0x0c780000,                                                  // Leading 1s:                                 /// 0029c
			 0x4b000000,                                                  // 8388608.0                                   /// 002a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7f800000,                                                  // inf                                         /// 002b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0xcb000000,                                                  // -8388608.0                                  /// 002c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d8
			 0x00000000,                                                  // zero                                        /// 002dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00300
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00308
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0030c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00310
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00314
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0031c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00320
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0032c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00330
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00334
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00338
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00340
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00344
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00348
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00350
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00354
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00358
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0035c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00360
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00364
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00368
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0036c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00370
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00374
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0037c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00380
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00388
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x7fc00001,                                                  // signaling NaN                               /// 00390
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00394
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00398
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0039c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003c0
			 0x7fc00001,                                                  // signaling NaN                               /// 003c4
			 0x33333333,                                                  // 4 random values                             /// 003c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003fc
			 0x55555555,                                                  // 4 random values                             /// 00400
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00404
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00408
			 0x00000000,                                                  // zero                                        /// 0040c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00410
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00414
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0041c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00420
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00428
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0042c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00430
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00434
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00440
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00444
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00448
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0044c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00450
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00454
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00464
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00468
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0046c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00470
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00478
			 0x0c600000,                                                  // Leading 1s:                                 /// 0047c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00480
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00484
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0048c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00490
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0049c
			 0x00011111,                                                  // 9.7958E-41                                  /// 004a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x33333333,                                                  // 4 random values                             /// 004c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004cc
			 0xff800000,                                                  // -inf                                        /// 004d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00500
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00508
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00510
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0051c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00524
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00534
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00538
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00540
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00548
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0054c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00558
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00560
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0056c
			 0x80000000,                                                  // -zero                                       /// 00570
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00574
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0057c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00580
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0058c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00590
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00594
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x00011111,                                                  // 9.7958E-41                                  /// 005a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005b8
			 0xbf028f5c,                                                  // -0.51                                       /// 005bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0c600000,                                                  // Leading 1s:                                 /// 00604
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00608
			 0x0c700000,                                                  // Leading 1s:                                 /// 0060c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00614
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00618
			 0x55555555,                                                  // 4 random values                             /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00624
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0062c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00634
			 0xffc00001,                                                  // -signaling NaN                              /// 00638
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00644
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0064c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00654
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00660
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00664
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00668
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0066c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00670
			 0x0c700000,                                                  // Leading 1s:                                 /// 00674
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0067c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0069c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a4
			 0x4b000000,                                                  // 8388608.0                                   /// 006a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00704
			 0x7f800000,                                                  // inf                                         /// 00708
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0070c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00710
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00718
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0071c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0c780000,                                                  // Leading 1s:                                 /// 00724
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0072c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x33333333,                                                  // 4 random values                             /// 00734
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00738
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0074c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00750
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00758
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0075c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00760
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00764
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x00000000,                                                  // zero                                        /// 0076c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00770
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00774
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00780
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00784
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00790
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00794
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00798
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0079c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00800
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00804
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00810
			 0x4b000000,                                                  // 8388608.0                                   /// 00814
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00818
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0081c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00820
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00824
			 0x3f028f5c,                                                  // 0.51                                        /// 00828
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00830
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00840
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00844
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0084c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0085c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00860
			 0x7fc00001,                                                  // signaling NaN                               /// 00864
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0086c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x80000000,                                                  // -zero                                       /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00888
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0088c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00890
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00894
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0089c
			 0x00011111,                                                  // 9.7958E-41                                  /// 008a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008ac
			 0x7f800000,                                                  // inf                                         /// 008b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00900
			 0x4b000000,                                                  // 8388608.0                                   /// 00904
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00908
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0090c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00910
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00918
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0091c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00920
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00924
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00928
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0092c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00934
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00944
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00948
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00950
			 0x0c700000,                                                  // Leading 1s:                                 /// 00954
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00958
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0095c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00970
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00974
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00978
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00980
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00984
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00988
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0098c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00990
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xffc00001,                                                  // -signaling NaN                              /// 0099c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009cc
			 0x4b000000,                                                  // 8388608.0                                   /// 009d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xcb000000,                                                  // -8388608.0                                  /// 00a34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00abc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ac0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ac8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ad8
			 0x7f800000,                                                  // inf                                         /// 00adc
			 0x00000000,                                                  // zero                                        /// 00ae0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00af8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b18
			 0xff800000,                                                  // -inf                                        /// 00b1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b30
			 0x33333333,                                                  // 4 random values                             /// 00b34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b54
			 0x4b000000,                                                  // 8388608.0                                   /// 00b58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b88
			 0xcb000000,                                                  // -8388608.0                                  /// 00b8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bac
			 0xbf028f5c,                                                  // -0.51                                       /// 00bb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00be0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00be8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bf0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bf4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bf8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bfc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x55555555,                                                  // 4 random values                             /// 00c10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ca4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ccc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ce0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ce4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cec
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d40
			 0x7fc00001,                                                  // signaling NaN                               /// 00d44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x4b000000,                                                  // 8388608.0                                   /// 00d68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d7c
			 0x33333333,                                                  // 4 random values                             /// 00d80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da0
			 0x00000000,                                                  // zero                                        /// 00da4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00da8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ddc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dfc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x7f800000,                                                  // inf                                         /// 00e10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e64
			 0xbf028f5c,                                                  // -0.51                                       /// 00e68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x4b000000,                                                  // 8388608.0                                   /// 00e98
			 0xcb000000,                                                  // -8388608.0                                  /// 00e9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ea0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eb8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ebc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ec0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ecc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ed4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00edc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00efc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f10
			 0x33333333,                                                  // 4 random values                             /// 00f14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f50
			 0x80000000,                                                  // -zero                                       /// 00f54
			 0xffc00001,                                                  // -signaling NaN                              /// 00f58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x55555555,                                                  // 4 random values                             /// 00fa0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd0
			 0x55555555,                                                  // 4 random values                             /// 00fd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x0c7fffc0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x4ad9a709, /// 0x0
			 0xd67ed44f, /// 0x4
			 0x23c58953, /// 0x8
			 0xd2561058, /// 0xc
			 0x9085eeb2, /// 0x10
			 0x13494a4b, /// 0x14
			 0x2efa106e, /// 0x18
			 0x8bf2e8b4, /// 0x1c
			 0x7d2932eb, /// 0x20
			 0x87bae504, /// 0x24
			 0x46fd6b35, /// 0x28
			 0x2c65f56a, /// 0x2c
			 0xf2e0e8a8, /// 0x30
			 0x9aa91b7c, /// 0x34
			 0x1858bbf2, /// 0x38
			 0xfc271bfe, /// 0x3c
			 0x38dda8e9, /// 0x40
			 0xa28a3902, /// 0x44
			 0xf3eed182, /// 0x48
			 0x07511a30, /// 0x4c
			 0xc005a21b, /// 0x50
			 0x37858997, /// 0x54
			 0x4aebdb47, /// 0x58
			 0x07e3a490, /// 0x5c
			 0x8ae1f293, /// 0x60
			 0xd6b269a7, /// 0x64
			 0x10528f63, /// 0x68
			 0x9fe86ae5, /// 0x6c
			 0xd1360d84, /// 0x70
			 0x2bcd8de2, /// 0x74
			 0xabc93528, /// 0x78
			 0x994b03c3, /// 0x7c
			 0x87e04e2f, /// 0x80
			 0x9276395a, /// 0x84
			 0xe698bb47, /// 0x88
			 0x11776d26, /// 0x8c
			 0x5692e0b0, /// 0x90
			 0x8e8e651a, /// 0x94
			 0xfe349eac, /// 0x98
			 0x0f97e8c2, /// 0x9c
			 0xa58dfd14, /// 0xa0
			 0xc8d2bdb5, /// 0xa4
			 0xf261305e, /// 0xa8
			 0xa6781ea1, /// 0xac
			 0xc28ed51d, /// 0xb0
			 0xe69234a3, /// 0xb4
			 0x8b7ef7b4, /// 0xb8
			 0x4af79ab9, /// 0xbc
			 0x98c96e9f, /// 0xc0
			 0xb6fb3b1a, /// 0xc4
			 0x5dc54bc5, /// 0xc8
			 0x6b13ad3c, /// 0xcc
			 0x5fd15cc7, /// 0xd0
			 0x4daa0a0a, /// 0xd4
			 0x52bbb78b, /// 0xd8
			 0xd83d578c, /// 0xdc
			 0x15448e51, /// 0xe0
			 0x19c72a1b, /// 0xe4
			 0x7ff3f88f, /// 0xe8
			 0x08cc8aa3, /// 0xec
			 0xb5f6d77d, /// 0xf0
			 0xbb755619, /// 0xf4
			 0x3b1549fc, /// 0xf8
			 0x89e5f8cd, /// 0xfc
			 0x62293a6a, /// 0x100
			 0x907860f2, /// 0x104
			 0x39cefb4c, /// 0x108
			 0xae915122, /// 0x10c
			 0x6131e900, /// 0x110
			 0x01442efc, /// 0x114
			 0xdfbfe76a, /// 0x118
			 0x448fa2a5, /// 0x11c
			 0xe5112101, /// 0x120
			 0x0e042e0c, /// 0x124
			 0xde13975e, /// 0x128
			 0x9689d0f2, /// 0x12c
			 0xa3204341, /// 0x130
			 0x6ba2e5f8, /// 0x134
			 0xf234a14c, /// 0x138
			 0x8ada7c70, /// 0x13c
			 0xe94a8a97, /// 0x140
			 0x2289ab8e, /// 0x144
			 0x3c1d17c2, /// 0x148
			 0x4045fbd6, /// 0x14c
			 0xa432b3d4, /// 0x150
			 0x43ab6f24, /// 0x154
			 0x8b0bb2f8, /// 0x158
			 0x263b406b, /// 0x15c
			 0x59bc5c26, /// 0x160
			 0xcfe3e15f, /// 0x164
			 0xe391cb40, /// 0x168
			 0x360b9a73, /// 0x16c
			 0x657e8d39, /// 0x170
			 0xfeaf3223, /// 0x174
			 0x1d9d95f5, /// 0x178
			 0x87fde81c, /// 0x17c
			 0x172fca2c, /// 0x180
			 0xf91a658f, /// 0x184
			 0xee4fbd21, /// 0x188
			 0x6bc539e6, /// 0x18c
			 0xade6090e, /// 0x190
			 0x90910e83, /// 0x194
			 0x1991cbda, /// 0x198
			 0x2b41ea93, /// 0x19c
			 0x754f9c4b, /// 0x1a0
			 0x34f8b7f9, /// 0x1a4
			 0x5fefdfc8, /// 0x1a8
			 0x32cb7908, /// 0x1ac
			 0x4ad18794, /// 0x1b0
			 0xc587453d, /// 0x1b4
			 0x9a70050b, /// 0x1b8
			 0x1592a30f, /// 0x1bc
			 0x4f03b4a5, /// 0x1c0
			 0x5ea8e32e, /// 0x1c4
			 0xb109cf56, /// 0x1c8
			 0x55a1702f, /// 0x1cc
			 0x1e68ee0a, /// 0x1d0
			 0xdd0adabc, /// 0x1d4
			 0x184e6ebb, /// 0x1d8
			 0xcf2f75dd, /// 0x1dc
			 0x68550032, /// 0x1e0
			 0xc421fddc, /// 0x1e4
			 0xf6a6bc62, /// 0x1e8
			 0x955725f2, /// 0x1ec
			 0xc5ecd8a3, /// 0x1f0
			 0xa9cff5d3, /// 0x1f4
			 0xe594c6af, /// 0x1f8
			 0xd9849da1, /// 0x1fc
			 0x1420864e, /// 0x200
			 0xf7896a3e, /// 0x204
			 0x4819d6f4, /// 0x208
			 0xfe28b661, /// 0x20c
			 0x21829fca, /// 0x210
			 0x4fb623db, /// 0x214
			 0x49fec605, /// 0x218
			 0x1838182f, /// 0x21c
			 0x7918119a, /// 0x220
			 0x2ee0d296, /// 0x224
			 0xe1f5b9cc, /// 0x228
			 0x4402e5e1, /// 0x22c
			 0x1878293c, /// 0x230
			 0xa6827e5e, /// 0x234
			 0x42f10b29, /// 0x238
			 0x51f61f54, /// 0x23c
			 0x4135f30a, /// 0x240
			 0x8a587664, /// 0x244
			 0x8139fed3, /// 0x248
			 0x50c5cc4c, /// 0x24c
			 0x6e5cb9ad, /// 0x250
			 0xfe4d9c1c, /// 0x254
			 0xb6a33219, /// 0x258
			 0x80f676b9, /// 0x25c
			 0xcc3678ba, /// 0x260
			 0x61bd9a76, /// 0x264
			 0x7a29de49, /// 0x268
			 0xa730586f, /// 0x26c
			 0xf2c598c5, /// 0x270
			 0xd19229ce, /// 0x274
			 0xa964b031, /// 0x278
			 0xcd4d0df3, /// 0x27c
			 0x65ed9ff2, /// 0x280
			 0xdfb92ad1, /// 0x284
			 0x6de44c74, /// 0x288
			 0xf7fc7f67, /// 0x28c
			 0x82ed6cc6, /// 0x290
			 0x85d1887d, /// 0x294
			 0x8400558e, /// 0x298
			 0x1d9ee733, /// 0x29c
			 0x61ac2833, /// 0x2a0
			 0x822028fa, /// 0x2a4
			 0x0d333cee, /// 0x2a8
			 0xc8f931fa, /// 0x2ac
			 0x0014a8e1, /// 0x2b0
			 0x949e9e39, /// 0x2b4
			 0x18a6871f, /// 0x2b8
			 0xf281ec87, /// 0x2bc
			 0x6dc3a613, /// 0x2c0
			 0xc4f0ddd3, /// 0x2c4
			 0x10bd47ac, /// 0x2c8
			 0xb7eb4ff3, /// 0x2cc
			 0x6a186dee, /// 0x2d0
			 0xe88ef23c, /// 0x2d4
			 0xa0b301bc, /// 0x2d8
			 0x9e07af2b, /// 0x2dc
			 0x00818a8d, /// 0x2e0
			 0x07b55d59, /// 0x2e4
			 0x40717d34, /// 0x2e8
			 0x9e6e7ef5, /// 0x2ec
			 0x1f293440, /// 0x2f0
			 0x2a8ee021, /// 0x2f4
			 0xdcabcfb0, /// 0x2f8
			 0x595bf227, /// 0x2fc
			 0x0225298d, /// 0x300
			 0x636b8e4b, /// 0x304
			 0x787bff64, /// 0x308
			 0x31b319bd, /// 0x30c
			 0xd963733f, /// 0x310
			 0x583318e5, /// 0x314
			 0x3f29bd9c, /// 0x318
			 0xa3c14c9a, /// 0x31c
			 0x0b5605bb, /// 0x320
			 0x7e738152, /// 0x324
			 0xea99a08c, /// 0x328
			 0x8539f596, /// 0x32c
			 0x79b61f38, /// 0x330
			 0x0a52dadf, /// 0x334
			 0xe9619208, /// 0x338
			 0x05c8e0ad, /// 0x33c
			 0x9a81e3d2, /// 0x340
			 0x4818d327, /// 0x344
			 0x6c1616e3, /// 0x348
			 0xc31ea18d, /// 0x34c
			 0x46223a0f, /// 0x350
			 0x323b1bea, /// 0x354
			 0x758f09ff, /// 0x358
			 0xe972c850, /// 0x35c
			 0x1f7d9c69, /// 0x360
			 0x9deae03a, /// 0x364
			 0xbe9db151, /// 0x368
			 0xdcff2e6e, /// 0x36c
			 0xcd09b412, /// 0x370
			 0xd6b96f3a, /// 0x374
			 0xc20742af, /// 0x378
			 0x29b056bb, /// 0x37c
			 0x8b85cc55, /// 0x380
			 0x11560050, /// 0x384
			 0xe7edb702, /// 0x388
			 0x0c0d1998, /// 0x38c
			 0xb9d9b0f7, /// 0x390
			 0x124ee011, /// 0x394
			 0x29d03918, /// 0x398
			 0xa0081807, /// 0x39c
			 0xa13f2cf8, /// 0x3a0
			 0x64099cf7, /// 0x3a4
			 0xde9c3860, /// 0x3a8
			 0xb63a5f71, /// 0x3ac
			 0x3c4425d2, /// 0x3b0
			 0xdc4152c0, /// 0x3b4
			 0x7024c9d1, /// 0x3b8
			 0x12150d8f, /// 0x3bc
			 0xea9858c9, /// 0x3c0
			 0x1a36dad9, /// 0x3c4
			 0x2848080a, /// 0x3c8
			 0xde18bc62, /// 0x3cc
			 0x1fb55edc, /// 0x3d0
			 0x86bb2d3b, /// 0x3d4
			 0x3f9686f0, /// 0x3d8
			 0x7ad6fe12, /// 0x3dc
			 0x4256ab67, /// 0x3e0
			 0x5a24b04d, /// 0x3e4
			 0xde174311, /// 0x3e8
			 0x9498cd55, /// 0x3ec
			 0x9d115f56, /// 0x3f0
			 0x1f84ac2a, /// 0x3f4
			 0xdd1544a7, /// 0x3f8
			 0x6ed92032, /// 0x3fc
			 0xa63f20da, /// 0x400
			 0x56de4d6b, /// 0x404
			 0xec9b9ebd, /// 0x408
			 0xa65143eb, /// 0x40c
			 0xe280e934, /// 0x410
			 0x01115609, /// 0x414
			 0x001257bf, /// 0x418
			 0xae028b11, /// 0x41c
			 0xf79bdea3, /// 0x420
			 0xb598155a, /// 0x424
			 0x0770f25a, /// 0x428
			 0x0e78c00b, /// 0x42c
			 0xa9d40ede, /// 0x430
			 0x2b6055e4, /// 0x434
			 0x61824564, /// 0x438
			 0x9050888d, /// 0x43c
			 0xb2961977, /// 0x440
			 0x5b4b4a5d, /// 0x444
			 0x9e5573e5, /// 0x448
			 0x1e0d1c03, /// 0x44c
			 0x6a397ee8, /// 0x450
			 0x4b0d38bb, /// 0x454
			 0x4a46a5a3, /// 0x458
			 0xc06e50c2, /// 0x45c
			 0x41b10d29, /// 0x460
			 0x273e4c94, /// 0x464
			 0x0e135df0, /// 0x468
			 0xaed32e82, /// 0x46c
			 0x63602fa1, /// 0x470
			 0xc56e70b0, /// 0x474
			 0x9c05c600, /// 0x478
			 0x8773be00, /// 0x47c
			 0x7ef280f0, /// 0x480
			 0xb76da785, /// 0x484
			 0x80a0bc20, /// 0x488
			 0x520828b4, /// 0x48c
			 0xd3cc3a51, /// 0x490
			 0x9b863d48, /// 0x494
			 0x98c08e24, /// 0x498
			 0xea016a46, /// 0x49c
			 0xfcc5b544, /// 0x4a0
			 0xc9b42c1a, /// 0x4a4
			 0x3373e9af, /// 0x4a8
			 0x4942c7af, /// 0x4ac
			 0x75389136, /// 0x4b0
			 0xa7eebb7a, /// 0x4b4
			 0x241bbedd, /// 0x4b8
			 0xa01cbdd8, /// 0x4bc
			 0x2248f633, /// 0x4c0
			 0x2b4c4973, /// 0x4c4
			 0x9f0c09c4, /// 0x4c8
			 0x9736b2ed, /// 0x4cc
			 0xdc264b04, /// 0x4d0
			 0x64175a9a, /// 0x4d4
			 0xea388400, /// 0x4d8
			 0xba7ae63e, /// 0x4dc
			 0xfb35caff, /// 0x4e0
			 0x6e770cc0, /// 0x4e4
			 0x8088b053, /// 0x4e8
			 0x9cff644f, /// 0x4ec
			 0x065e71a8, /// 0x4f0
			 0xb0a3299e, /// 0x4f4
			 0x2dc8b429, /// 0x4f8
			 0xa3653750, /// 0x4fc
			 0x59f0e65e, /// 0x500
			 0x22bf71ee, /// 0x504
			 0x1cb097ab, /// 0x508
			 0xd7d3b0c9, /// 0x50c
			 0x47ef506f, /// 0x510
			 0xa1a46022, /// 0x514
			 0x240a7567, /// 0x518
			 0xa21d3b5c, /// 0x51c
			 0x790e9b64, /// 0x520
			 0x96cb3864, /// 0x524
			 0xf5a5ce9c, /// 0x528
			 0x6005bb5d, /// 0x52c
			 0x89d9a31a, /// 0x530
			 0x73e27f49, /// 0x534
			 0xd5bd3205, /// 0x538
			 0xccaba4b4, /// 0x53c
			 0x8a6d91b0, /// 0x540
			 0x576c0c3d, /// 0x544
			 0xf5353929, /// 0x548
			 0x490fd575, /// 0x54c
			 0x4f00af3b, /// 0x550
			 0x9dbe662d, /// 0x554
			 0x8a089b3b, /// 0x558
			 0xbb599726, /// 0x55c
			 0x3daf664c, /// 0x560
			 0xdecfa167, /// 0x564
			 0x54dca9a4, /// 0x568
			 0x8cceb7a3, /// 0x56c
			 0x210e9285, /// 0x570
			 0x622bdb20, /// 0x574
			 0xbf4a503a, /// 0x578
			 0xf88169fc, /// 0x57c
			 0x09f42239, /// 0x580
			 0x2d91387a, /// 0x584
			 0xa88eb4de, /// 0x588
			 0x87faea2e, /// 0x58c
			 0x3187310e, /// 0x590
			 0xb1f6790b, /// 0x594
			 0x4562b23e, /// 0x598
			 0xf1526c7d, /// 0x59c
			 0x4ddbd820, /// 0x5a0
			 0x237273b9, /// 0x5a4
			 0x2b28fb2a, /// 0x5a8
			 0x38a6ab6c, /// 0x5ac
			 0xa5ee8a40, /// 0x5b0
			 0xaff151a6, /// 0x5b4
			 0x5f7ddf33, /// 0x5b8
			 0x55e7a627, /// 0x5bc
			 0x307140e3, /// 0x5c0
			 0x0b5d6693, /// 0x5c4
			 0x943a8e3a, /// 0x5c8
			 0xc71f1a41, /// 0x5cc
			 0xf02e3b6c, /// 0x5d0
			 0x33640b7e, /// 0x5d4
			 0x01dab91e, /// 0x5d8
			 0x813ad30e, /// 0x5dc
			 0x4c32dc14, /// 0x5e0
			 0x1a22623f, /// 0x5e4
			 0xf9395695, /// 0x5e8
			 0x2318af7f, /// 0x5ec
			 0xd5cd8e2d, /// 0x5f0
			 0x3e0e1730, /// 0x5f4
			 0xd18b9d90, /// 0x5f8
			 0xa70bc7e7, /// 0x5fc
			 0x9ceecd49, /// 0x600
			 0xd97adad8, /// 0x604
			 0xdf2af4ee, /// 0x608
			 0x41ae33b6, /// 0x60c
			 0x08788584, /// 0x610
			 0x36ab6886, /// 0x614
			 0x402c08ec, /// 0x618
			 0x5a93eaea, /// 0x61c
			 0xc9f0e5a0, /// 0x620
			 0x62b8cdbf, /// 0x624
			 0x9abc7116, /// 0x628
			 0x2a60e043, /// 0x62c
			 0x5ff56d9d, /// 0x630
			 0x9b60a2f7, /// 0x634
			 0x9d118905, /// 0x638
			 0xacae3cfd, /// 0x63c
			 0x4771d5e9, /// 0x640
			 0xefd40cb7, /// 0x644
			 0xe09fb30c, /// 0x648
			 0xfb631046, /// 0x64c
			 0x3f8f1574, /// 0x650
			 0xd55cf719, /// 0x654
			 0xf1d7bef6, /// 0x658
			 0x3bbb4b66, /// 0x65c
			 0xa2415efc, /// 0x660
			 0xc6ea9c32, /// 0x664
			 0x65de82c0, /// 0x668
			 0x240e31f3, /// 0x66c
			 0xbca0128d, /// 0x670
			 0xd7a2bdc8, /// 0x674
			 0x7a638246, /// 0x678
			 0x0ea7d420, /// 0x67c
			 0xbe3014c2, /// 0x680
			 0xa8988ea1, /// 0x684
			 0x57ce5e70, /// 0x688
			 0x824dc397, /// 0x68c
			 0x237dac2e, /// 0x690
			 0x00b0cb33, /// 0x694
			 0xdacb096f, /// 0x698
			 0xef5c545f, /// 0x69c
			 0xa18a29ad, /// 0x6a0
			 0x121f456c, /// 0x6a4
			 0x1131de07, /// 0x6a8
			 0xd6a60791, /// 0x6ac
			 0x77fc5fbf, /// 0x6b0
			 0x80712a9c, /// 0x6b4
			 0x79848f2a, /// 0x6b8
			 0x9008ec78, /// 0x6bc
			 0x717a4c25, /// 0x6c0
			 0x56071530, /// 0x6c4
			 0xa2ca69fd, /// 0x6c8
			 0x90c64a6c, /// 0x6cc
			 0x681a416d, /// 0x6d0
			 0xbcac645b, /// 0x6d4
			 0x443b041d, /// 0x6d8
			 0xea2c45aa, /// 0x6dc
			 0x047d7aab, /// 0x6e0
			 0x8f7fd5bb, /// 0x6e4
			 0xc26f469a, /// 0x6e8
			 0x4ebae72c, /// 0x6ec
			 0xe0905222, /// 0x6f0
			 0x7884b74c, /// 0x6f4
			 0xc6106699, /// 0x6f8
			 0xfc086ba0, /// 0x6fc
			 0x4969d03d, /// 0x700
			 0x2564a7dd, /// 0x704
			 0x7491d5a0, /// 0x708
			 0x6c1eea65, /// 0x70c
			 0xe80f478f, /// 0x710
			 0x83418400, /// 0x714
			 0xb87a76c9, /// 0x718
			 0xaf60db31, /// 0x71c
			 0xc5cf185c, /// 0x720
			 0x335af058, /// 0x724
			 0xb05ce187, /// 0x728
			 0xde751b1b, /// 0x72c
			 0x35f2e85b, /// 0x730
			 0x2d63c64b, /// 0x734
			 0x250485a7, /// 0x738
			 0xd5f0a7e9, /// 0x73c
			 0xb9640655, /// 0x740
			 0x949211bf, /// 0x744
			 0xf6a75df4, /// 0x748
			 0x47b596e8, /// 0x74c
			 0x6cedff75, /// 0x750
			 0xad193d5e, /// 0x754
			 0x6c6d0a48, /// 0x758
			 0xb62d4d73, /// 0x75c
			 0xac0bc57f, /// 0x760
			 0x53828ff1, /// 0x764
			 0x9461f4a6, /// 0x768
			 0x8fa9c85b, /// 0x76c
			 0x55ee6749, /// 0x770
			 0xcf67fcb2, /// 0x774
			 0x91358afa, /// 0x778
			 0xe8734334, /// 0x77c
			 0x86695ab3, /// 0x780
			 0x4d787550, /// 0x784
			 0xf5a365bb, /// 0x788
			 0x88945d53, /// 0x78c
			 0xfcc4dd88, /// 0x790
			 0x7a1efc5f, /// 0x794
			 0x6e547dd5, /// 0x798
			 0x1835897f, /// 0x79c
			 0xacf24705, /// 0x7a0
			 0xa882e1de, /// 0x7a4
			 0x2f1e3b87, /// 0x7a8
			 0x54181653, /// 0x7ac
			 0xb059d3b1, /// 0x7b0
			 0xac52568d, /// 0x7b4
			 0x46bdc7d3, /// 0x7b8
			 0x89d6ad21, /// 0x7bc
			 0x80ae5087, /// 0x7c0
			 0x81a7bdfc, /// 0x7c4
			 0xea44247d, /// 0x7c8
			 0xd7bdc139, /// 0x7cc
			 0x901d8fef, /// 0x7d0
			 0xadc89874, /// 0x7d4
			 0x80ac9553, /// 0x7d8
			 0x11a60d08, /// 0x7dc
			 0x13403116, /// 0x7e0
			 0x0f364c21, /// 0x7e4
			 0x90367448, /// 0x7e8
			 0xaafaad3d, /// 0x7ec
			 0xdd3c3f1d, /// 0x7f0
			 0x909d517a, /// 0x7f4
			 0x687562c6, /// 0x7f8
			 0xc2ed529f, /// 0x7fc
			 0x822a6fd1, /// 0x800
			 0x4c560e23, /// 0x804
			 0x0c635539, /// 0x808
			 0x020b2d8d, /// 0x80c
			 0xc36f7309, /// 0x810
			 0xcbb112d7, /// 0x814
			 0xad1253d8, /// 0x818
			 0xdfda3aad, /// 0x81c
			 0xa9c4da8a, /// 0x820
			 0x667cf62d, /// 0x824
			 0x4c11a26a, /// 0x828
			 0xda9c3c1f, /// 0x82c
			 0x72cb33b6, /// 0x830
			 0x7c70fbd8, /// 0x834
			 0x3e65b9ef, /// 0x838
			 0x40de5f8f, /// 0x83c
			 0x534ca499, /// 0x840
			 0x29e9f540, /// 0x844
			 0x9e2b2627, /// 0x848
			 0x295fe6e0, /// 0x84c
			 0x7a619049, /// 0x850
			 0xd0ec14be, /// 0x854
			 0xe25b44f5, /// 0x858
			 0xfbd37bcf, /// 0x85c
			 0x6409396d, /// 0x860
			 0x00ff6942, /// 0x864
			 0x54304226, /// 0x868
			 0x113fecf2, /// 0x86c
			 0xa7c6f11a, /// 0x870
			 0x0cbfbf8f, /// 0x874
			 0x7794b49d, /// 0x878
			 0x582107a7, /// 0x87c
			 0x1bb6cb9e, /// 0x880
			 0x4894f466, /// 0x884
			 0x21853a97, /// 0x888
			 0x3bd1979f, /// 0x88c
			 0xc7678042, /// 0x890
			 0x7d88f5f5, /// 0x894
			 0xa7ac1221, /// 0x898
			 0x8c4c8228, /// 0x89c
			 0x992800c5, /// 0x8a0
			 0xfe70b46f, /// 0x8a4
			 0x166bc7d1, /// 0x8a8
			 0xa65b7d54, /// 0x8ac
			 0xd1fa72c8, /// 0x8b0
			 0x5a482f61, /// 0x8b4
			 0x5f34a9dc, /// 0x8b8
			 0x4326eeef, /// 0x8bc
			 0xad8b68b0, /// 0x8c0
			 0xdd90bbb1, /// 0x8c4
			 0xdaa6b616, /// 0x8c8
			 0x4bb5cdf2, /// 0x8cc
			 0xc989fd30, /// 0x8d0
			 0x822bb811, /// 0x8d4
			 0xea8dc6e2, /// 0x8d8
			 0x21b4d9e8, /// 0x8dc
			 0xd843508a, /// 0x8e0
			 0xc8e9e9c5, /// 0x8e4
			 0xcb179700, /// 0x8e8
			 0x5ada4b41, /// 0x8ec
			 0x589b8233, /// 0x8f0
			 0xf268b298, /// 0x8f4
			 0x18c6f553, /// 0x8f8
			 0x4e5f69a6, /// 0x8fc
			 0x42802764, /// 0x900
			 0x01c325be, /// 0x904
			 0x2c1fdcbf, /// 0x908
			 0xe8a5e765, /// 0x90c
			 0xcd6cd17f, /// 0x910
			 0xbbcbb2e6, /// 0x914
			 0x874c1eaa, /// 0x918
			 0x4cf2c0ca, /// 0x91c
			 0xba2b9f19, /// 0x920
			 0x1350c445, /// 0x924
			 0x694d6eb1, /// 0x928
			 0xea09a6f3, /// 0x92c
			 0xefd6498c, /// 0x930
			 0x7cd7326e, /// 0x934
			 0x7f9c0800, /// 0x938
			 0x50b8764f, /// 0x93c
			 0xed5d0929, /// 0x940
			 0xf3e6ad50, /// 0x944
			 0x3752f9ad, /// 0x948
			 0xb9ca05e6, /// 0x94c
			 0x63ed8de0, /// 0x950
			 0x3fcbbeee, /// 0x954
			 0x2d69727d, /// 0x958
			 0x73f45efc, /// 0x95c
			 0xe91372c9, /// 0x960
			 0x6e41b8ec, /// 0x964
			 0xc0b19bdf, /// 0x968
			 0xc499fb81, /// 0x96c
			 0x8a73d767, /// 0x970
			 0x714b9851, /// 0x974
			 0x9da847ac, /// 0x978
			 0xc87ce792, /// 0x97c
			 0x2ad08276, /// 0x980
			 0x17571be6, /// 0x984
			 0x3eb207df, /// 0x988
			 0x94ac5510, /// 0x98c
			 0x956f04fa, /// 0x990
			 0x22835b70, /// 0x994
			 0xfed50052, /// 0x998
			 0xf019d520, /// 0x99c
			 0xd6b01a03, /// 0x9a0
			 0x70ce1ae3, /// 0x9a4
			 0xfe27ee38, /// 0x9a8
			 0xbea90fc5, /// 0x9ac
			 0x1ace18dc, /// 0x9b0
			 0x82e2c7f2, /// 0x9b4
			 0xb53f58f8, /// 0x9b8
			 0x38b7bbba, /// 0x9bc
			 0x3384e1aa, /// 0x9c0
			 0x1ead9b91, /// 0x9c4
			 0xd69e74c6, /// 0x9c8
			 0x5a32f484, /// 0x9cc
			 0xd40821dd, /// 0x9d0
			 0xf0d2122f, /// 0x9d4
			 0x1e1f7395, /// 0x9d8
			 0x58b344eb, /// 0x9dc
			 0x86c72a4e, /// 0x9e0
			 0x1d829313, /// 0x9e4
			 0x96ca32d0, /// 0x9e8
			 0x47defa2a, /// 0x9ec
			 0x35e87612, /// 0x9f0
			 0x3c7527be, /// 0x9f4
			 0xc0cf3dd4, /// 0x9f8
			 0x35c49965, /// 0x9fc
			 0xbd174afc, /// 0xa00
			 0x93d67f61, /// 0xa04
			 0xd118b54d, /// 0xa08
			 0x9daa90d2, /// 0xa0c
			 0x3904a47c, /// 0xa10
			 0xc85c93da, /// 0xa14
			 0x48600b2e, /// 0xa18
			 0x3b0da76d, /// 0xa1c
			 0x0495f2f0, /// 0xa20
			 0xfc90d9e3, /// 0xa24
			 0x8f280ec7, /// 0xa28
			 0x58d348b8, /// 0xa2c
			 0x680819f1, /// 0xa30
			 0xabb53633, /// 0xa34
			 0x184bb624, /// 0xa38
			 0x6083715e, /// 0xa3c
			 0x34e44d10, /// 0xa40
			 0x8bc523a6, /// 0xa44
			 0xc6419dac, /// 0xa48
			 0x7dc7daa3, /// 0xa4c
			 0xb92473ba, /// 0xa50
			 0x0307672d, /// 0xa54
			 0xb9edddb8, /// 0xa58
			 0x7e68a555, /// 0xa5c
			 0xb79185b9, /// 0xa60
			 0xa2042704, /// 0xa64
			 0x0a52cee6, /// 0xa68
			 0xc92241e0, /// 0xa6c
			 0x44bec56d, /// 0xa70
			 0xba426942, /// 0xa74
			 0xb75209b0, /// 0xa78
			 0xe0a7672e, /// 0xa7c
			 0x4d30112e, /// 0xa80
			 0x884bd09f, /// 0xa84
			 0x0add1ed3, /// 0xa88
			 0x304c5b2f, /// 0xa8c
			 0xbda93f4b, /// 0xa90
			 0x87dbb2c7, /// 0xa94
			 0x1ec8ba5a, /// 0xa98
			 0x4418a75a, /// 0xa9c
			 0xf98f9315, /// 0xaa0
			 0x5a35e793, /// 0xaa4
			 0x409a1095, /// 0xaa8
			 0x1d1a5a97, /// 0xaac
			 0x2fd5aa19, /// 0xab0
			 0x24cc52c1, /// 0xab4
			 0x1eb35088, /// 0xab8
			 0x613ed039, /// 0xabc
			 0x14bbd5ac, /// 0xac0
			 0x0f5ff388, /// 0xac4
			 0xd91902c4, /// 0xac8
			 0xabd8e272, /// 0xacc
			 0x580c1768, /// 0xad0
			 0xe8737086, /// 0xad4
			 0xc4567488, /// 0xad8
			 0x6d8e8d5b, /// 0xadc
			 0x147f6939, /// 0xae0
			 0x5bc7deea, /// 0xae4
			 0x9e6ace3e, /// 0xae8
			 0x465045e2, /// 0xaec
			 0x82f878b9, /// 0xaf0
			 0xa9dc6a14, /// 0xaf4
			 0x6591087c, /// 0xaf8
			 0xe0b23210, /// 0xafc
			 0xbc97ff0e, /// 0xb00
			 0x29193d63, /// 0xb04
			 0xad873536, /// 0xb08
			 0x3983792d, /// 0xb0c
			 0x2a731a14, /// 0xb10
			 0xb679ce84, /// 0xb14
			 0xa7fa0bfe, /// 0xb18
			 0xda298aad, /// 0xb1c
			 0xfb6fa50b, /// 0xb20
			 0xd205c737, /// 0xb24
			 0x66c6d49d, /// 0xb28
			 0x45038330, /// 0xb2c
			 0x6c6d1a2e, /// 0xb30
			 0xde269ace, /// 0xb34
			 0x1f55f52a, /// 0xb38
			 0x17c5dfee, /// 0xb3c
			 0xb41676d2, /// 0xb40
			 0x05460f5a, /// 0xb44
			 0xf5e647bc, /// 0xb48
			 0xe96a8f87, /// 0xb4c
			 0x4b27b766, /// 0xb50
			 0x022ea55b, /// 0xb54
			 0x3f581e96, /// 0xb58
			 0x8284a209, /// 0xb5c
			 0xb6294933, /// 0xb60
			 0x1f77b61c, /// 0xb64
			 0x66efeb21, /// 0xb68
			 0x366e66e8, /// 0xb6c
			 0xaa16d2a9, /// 0xb70
			 0x32a1f2e6, /// 0xb74
			 0xac0fa972, /// 0xb78
			 0x75e8b61c, /// 0xb7c
			 0xebeb599b, /// 0xb80
			 0x985313ab, /// 0xb84
			 0x8b32f0b5, /// 0xb88
			 0x43717459, /// 0xb8c
			 0x4fb0b603, /// 0xb90
			 0x785a4561, /// 0xb94
			 0x7483b18a, /// 0xb98
			 0x6d09da12, /// 0xb9c
			 0xb339d6f2, /// 0xba0
			 0x6e46e2d5, /// 0xba4
			 0x4d450575, /// 0xba8
			 0x35b86d05, /// 0xbac
			 0x92ca8b39, /// 0xbb0
			 0xecb065d1, /// 0xbb4
			 0x22819884, /// 0xbb8
			 0xcb93324a, /// 0xbbc
			 0xa70a8a32, /// 0xbc0
			 0x95847945, /// 0xbc4
			 0xb86d010f, /// 0xbc8
			 0x4bd7d8e9, /// 0xbcc
			 0x5e4883d4, /// 0xbd0
			 0xd3574cf2, /// 0xbd4
			 0x25686fe5, /// 0xbd8
			 0x61cd0cfd, /// 0xbdc
			 0xafe4fc07, /// 0xbe0
			 0xa1e74c3e, /// 0xbe4
			 0x17ce40b0, /// 0xbe8
			 0xfb285ada, /// 0xbec
			 0xb8d52149, /// 0xbf0
			 0xe139b1ae, /// 0xbf4
			 0xfa02b5d5, /// 0xbf8
			 0xcfa9b813, /// 0xbfc
			 0x79e6b2a9, /// 0xc00
			 0xd7d69640, /// 0xc04
			 0x8a998096, /// 0xc08
			 0x21b80ab4, /// 0xc0c
			 0x39a3969f, /// 0xc10
			 0xc2cefdf3, /// 0xc14
			 0x7a4d3cf4, /// 0xc18
			 0x890614dc, /// 0xc1c
			 0xa9d770ae, /// 0xc20
			 0x43ba670d, /// 0xc24
			 0x8cd75a47, /// 0xc28
			 0x33ef3e85, /// 0xc2c
			 0xe4475713, /// 0xc30
			 0x6a674913, /// 0xc34
			 0xd2cab009, /// 0xc38
			 0xfa5cf614, /// 0xc3c
			 0x210556b0, /// 0xc40
			 0x7bd4a599, /// 0xc44
			 0x2c3dbbd9, /// 0xc48
			 0xfbd7bdc2, /// 0xc4c
			 0xe3f346ce, /// 0xc50
			 0x303999be, /// 0xc54
			 0x78cf4a8e, /// 0xc58
			 0x67db5e0a, /// 0xc5c
			 0x0ba3c8c8, /// 0xc60
			 0x405ce2b6, /// 0xc64
			 0xe0b94e66, /// 0xc68
			 0x451a71fb, /// 0xc6c
			 0xe9827126, /// 0xc70
			 0x910b76ce, /// 0xc74
			 0x0d58580e, /// 0xc78
			 0xa9a62b3b, /// 0xc7c
			 0x54089d62, /// 0xc80
			 0xa7d6b9e2, /// 0xc84
			 0x981eb402, /// 0xc88
			 0x2dab66af, /// 0xc8c
			 0x2099d83f, /// 0xc90
			 0x69e07614, /// 0xc94
			 0xb2b7fd37, /// 0xc98
			 0xe6f6d39c, /// 0xc9c
			 0xf728fb6e, /// 0xca0
			 0xfb8049d0, /// 0xca4
			 0xde2b8341, /// 0xca8
			 0x8edad48b, /// 0xcac
			 0xeeadf872, /// 0xcb0
			 0xe7b9f301, /// 0xcb4
			 0x388373e3, /// 0xcb8
			 0xb4daa4b3, /// 0xcbc
			 0x9073f95b, /// 0xcc0
			 0xe1d09b85, /// 0xcc4
			 0x430122fc, /// 0xcc8
			 0x92a0ac3a, /// 0xccc
			 0xa351673b, /// 0xcd0
			 0x53be6874, /// 0xcd4
			 0xe7822445, /// 0xcd8
			 0x7d4b2e14, /// 0xcdc
			 0x88b72507, /// 0xce0
			 0xc2493529, /// 0xce4
			 0x5531ab64, /// 0xce8
			 0xbfe201de, /// 0xcec
			 0xa92bedb6, /// 0xcf0
			 0xd016697d, /// 0xcf4
			 0x2b641c47, /// 0xcf8
			 0x266cbc45, /// 0xcfc
			 0xdd6997ac, /// 0xd00
			 0xd2cbb7ab, /// 0xd04
			 0xdc7dfeff, /// 0xd08
			 0x7ddb88a4, /// 0xd0c
			 0xf2331e57, /// 0xd10
			 0xe9b7f6e4, /// 0xd14
			 0x828c94dc, /// 0xd18
			 0x8e4f5a81, /// 0xd1c
			 0xbfe4662f, /// 0xd20
			 0xe9bbc8f1, /// 0xd24
			 0x27759b8c, /// 0xd28
			 0xd97e97d8, /// 0xd2c
			 0x2236f4d6, /// 0xd30
			 0xb06ddc32, /// 0xd34
			 0xf1607a04, /// 0xd38
			 0xb3840814, /// 0xd3c
			 0x981912c5, /// 0xd40
			 0x0080f217, /// 0xd44
			 0x8d8831c8, /// 0xd48
			 0xac53d9d1, /// 0xd4c
			 0x16d43ceb, /// 0xd50
			 0x30dd4c62, /// 0xd54
			 0x488c75e5, /// 0xd58
			 0x556b9a8a, /// 0xd5c
			 0xa47a3726, /// 0xd60
			 0xd6c38481, /// 0xd64
			 0xee826c4a, /// 0xd68
			 0xd6e6a4a0, /// 0xd6c
			 0xf1da07ef, /// 0xd70
			 0x32e7a92e, /// 0xd74
			 0xcd82dbf5, /// 0xd78
			 0x8eebecb2, /// 0xd7c
			 0x4a29bea1, /// 0xd80
			 0x4d957cf7, /// 0xd84
			 0x8e1c8937, /// 0xd88
			 0x7f75e368, /// 0xd8c
			 0x0df4b2e8, /// 0xd90
			 0xb8e5dc0b, /// 0xd94
			 0x136550b5, /// 0xd98
			 0xeae81903, /// 0xd9c
			 0x5ec081ee, /// 0xda0
			 0x28be94ac, /// 0xda4
			 0x1f687c75, /// 0xda8
			 0xa69806e6, /// 0xdac
			 0xfc4d9be3, /// 0xdb0
			 0x48daed91, /// 0xdb4
			 0x7a568248, /// 0xdb8
			 0xbee41f2e, /// 0xdbc
			 0xdcbb5f54, /// 0xdc0
			 0x2d42c693, /// 0xdc4
			 0x793968d7, /// 0xdc8
			 0x1762a043, /// 0xdcc
			 0xcf3823ba, /// 0xdd0
			 0x021c3238, /// 0xdd4
			 0xc73e7147, /// 0xdd8
			 0x01e0e417, /// 0xddc
			 0xc3779a55, /// 0xde0
			 0x97c32913, /// 0xde4
			 0xcf785d62, /// 0xde8
			 0x0d96ae6c, /// 0xdec
			 0xabed5f86, /// 0xdf0
			 0xbeb6ea12, /// 0xdf4
			 0xec472312, /// 0xdf8
			 0xcf4c9f01, /// 0xdfc
			 0x8ad03e00, /// 0xe00
			 0x4c19fb2e, /// 0xe04
			 0xc3a2189f, /// 0xe08
			 0xa3298436, /// 0xe0c
			 0xf5dce97e, /// 0xe10
			 0xc9636e92, /// 0xe14
			 0xc45b3fe1, /// 0xe18
			 0x58da96c3, /// 0xe1c
			 0x850394e3, /// 0xe20
			 0x6593e97d, /// 0xe24
			 0x1fda5174, /// 0xe28
			 0x92eff338, /// 0xe2c
			 0xddf72613, /// 0xe30
			 0xac2d88f8, /// 0xe34
			 0xba5a450c, /// 0xe38
			 0xf2ecf62b, /// 0xe3c
			 0x24c59da2, /// 0xe40
			 0xdd04825b, /// 0xe44
			 0x5be4d3ae, /// 0xe48
			 0x3b06d9e1, /// 0xe4c
			 0x12ea4fac, /// 0xe50
			 0xc3976391, /// 0xe54
			 0xc62dd430, /// 0xe58
			 0x5b0a6d30, /// 0xe5c
			 0xe79e519a, /// 0xe60
			 0x2d580503, /// 0xe64
			 0x069cbe27, /// 0xe68
			 0x3c95bc04, /// 0xe6c
			 0xd4cde8e1, /// 0xe70
			 0x1f9568d0, /// 0xe74
			 0x83f8f03a, /// 0xe78
			 0x7779ae49, /// 0xe7c
			 0x4ae56569, /// 0xe80
			 0xccb43a78, /// 0xe84
			 0xffdb63e9, /// 0xe88
			 0xffb39d23, /// 0xe8c
			 0xc3bb71bd, /// 0xe90
			 0x3b1a6e3c, /// 0xe94
			 0xf50d70b7, /// 0xe98
			 0x5cee7c38, /// 0xe9c
			 0x167a1a2f, /// 0xea0
			 0xcfb1baac, /// 0xea4
			 0x7a9e44d4, /// 0xea8
			 0x1748a38d, /// 0xeac
			 0x8872095c, /// 0xeb0
			 0x867d581b, /// 0xeb4
			 0xb75cda59, /// 0xeb8
			 0x1e1814f3, /// 0xebc
			 0x2d358fc8, /// 0xec0
			 0x17c182bd, /// 0xec4
			 0xb13da16f, /// 0xec8
			 0x4f726ba7, /// 0xecc
			 0x5f2ce279, /// 0xed0
			 0x43dd401a, /// 0xed4
			 0x4254ff85, /// 0xed8
			 0x7e65e910, /// 0xedc
			 0x23bb7eb6, /// 0xee0
			 0x25666891, /// 0xee4
			 0xd42365eb, /// 0xee8
			 0x873fdbd2, /// 0xeec
			 0xb96fc02f, /// 0xef0
			 0xd437eecf, /// 0xef4
			 0xaeda9acc, /// 0xef8
			 0xc873ca7a, /// 0xefc
			 0xac6633e2, /// 0xf00
			 0x17c975d0, /// 0xf04
			 0x025bc9de, /// 0xf08
			 0x7acdd480, /// 0xf0c
			 0x23576e6d, /// 0xf10
			 0x9890a096, /// 0xf14
			 0x71a6b3ec, /// 0xf18
			 0x4bc95dac, /// 0xf1c
			 0xba68e427, /// 0xf20
			 0xd20d8841, /// 0xf24
			 0x3d219cdf, /// 0xf28
			 0xa9f3460d, /// 0xf2c
			 0xc910fb9f, /// 0xf30
			 0x9144fa7e, /// 0xf34
			 0x2f789e81, /// 0xf38
			 0xa500a584, /// 0xf3c
			 0x71e8df2e, /// 0xf40
			 0xc7e3352d, /// 0xf44
			 0x7331fa78, /// 0xf48
			 0xeb614d3c, /// 0xf4c
			 0xc4ae6adc, /// 0xf50
			 0x9d4d55c5, /// 0xf54
			 0x88c50a54, /// 0xf58
			 0x6635270c, /// 0xf5c
			 0x2ab4a2e6, /// 0xf60
			 0x70defba2, /// 0xf64
			 0x7e60bc18, /// 0xf68
			 0xe9ca5b4d, /// 0xf6c
			 0xc9b17098, /// 0xf70
			 0xb722a20c, /// 0xf74
			 0x9220da67, /// 0xf78
			 0xc16add45, /// 0xf7c
			 0x3696e23b, /// 0xf80
			 0xbc517958, /// 0xf84
			 0x52e59685, /// 0xf88
			 0xca2f4471, /// 0xf8c
			 0xf846ebf1, /// 0xf90
			 0x11cd56e6, /// 0xf94
			 0x880d1ac2, /// 0xf98
			 0x7d4bb9bf, /// 0xf9c
			 0xf51f142a, /// 0xfa0
			 0x81804fa8, /// 0xfa4
			 0x3ad2a6f4, /// 0xfa8
			 0x7f28a06d, /// 0xfac
			 0xb09710e6, /// 0xfb0
			 0xbf47b035, /// 0xfb4
			 0x17d06f48, /// 0xfb8
			 0x2063bf82, /// 0xfbc
			 0x92573144, /// 0xfc0
			 0x998c4a13, /// 0xfc4
			 0x7a0ae9dd, /// 0xfc8
			 0xdb6c9b7a, /// 0xfcc
			 0x836e22bc, /// 0xfd0
			 0x927bf8c4, /// 0xfd4
			 0xcbce3799, /// 0xfd8
			 0x9cd456f4, /// 0xfdc
			 0x77fac954, /// 0xfe0
			 0xb893cf03, /// 0xfe4
			 0x91d5d3a7, /// 0xfe8
			 0xe77d13ab, /// 0xfec
			 0x91568370, /// 0xff0
			 0x36442bea, /// 0xff4
			 0xfb32c58d, /// 0xff8
			 0xf699f5da /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x7f800000,                                                  // inf                                         /// 00008
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00010
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00018
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00024
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00028
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x7f800000,                                                  // inf                                         /// 00034
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0003c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00040
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00044
			 0x0c700000,                                                  // Leading 1s:                                 /// 00048
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0004c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00050
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00054
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00058
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0005c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00060
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0006c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00070
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00074
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00078
			 0xbf028f5c,                                                  // -0.51                                       /// 0007c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00080
			 0x0e000001,                                                  // Trailing 1s:                                /// 00084
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00088
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0008c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00090
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0009c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x55555555,                                                  // 4 random values                             /// 000a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0xbf028f5c,                                                  // -0.51                                       /// 000c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00100
			 0x55555555,                                                  // 4 random values                             /// 00104
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00108
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0010c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00114
			 0xbf028f5c,                                                  // -0.51                                       /// 00118
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00124
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00128
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0012c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00138
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00144
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00148
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0014c
			 0x55555555,                                                  // 4 random values                             /// 00150
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00158
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0015c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00164
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00174
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00178
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00188
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00190
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c700000,                                                  // Leading 1s:                                 /// 00198
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a0
			 0x7f800000,                                                  // inf                                         /// 001a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001b0
			 0x33333333,                                                  // 4 random values                             /// 001b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001e0
			 0x55555555,                                                  // 4 random values                             /// 001e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00204
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00208
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0xbf028f5c,                                                  // -0.51                                       /// 00218
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00220
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00228
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0022c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0023c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00240
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00248
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00250
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000000,                                                  // -zero                                       /// 00258
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0025c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x7f800000,                                                  // inf                                         /// 00268
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0026c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00278
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0027c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00280
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00288
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0028c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00294
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00298
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 002b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x7fc00001,                                                  // signaling NaN                               /// 002b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002d0
			 0xbf028f5c,                                                  // -0.51                                       /// 002d4
			 0x33333333,                                                  // 4 random values                             /// 002d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00300
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00304
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00310
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00314
			 0x00000000,                                                  // zero                                        /// 00318
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0031c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00320
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00328
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x80000000,                                                  // -zero                                       /// 00330
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00334
			 0x0c700000,                                                  // Leading 1s:                                 /// 00338
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0033c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00344
			 0x7f800000,                                                  // inf                                         /// 00348
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00350
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00354
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00358
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0035c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00364
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0036c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x55555555,                                                  // 4 random values                             /// 00374
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0xbf028f5c,                                                  // -0.51                                       /// 0037c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00384
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00388
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00390
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00394
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00398
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0039c
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003c8
			 0xbf028f5c,                                                  // -0.51                                       /// 003cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xffc00001,                                                  // -signaling NaN                              /// 003d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00400
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00404
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00408
			 0x80011111,                                                  // -9.7958E-41                                 /// 0040c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00410
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00420
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00424
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0042c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00430
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00434
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00438
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0043c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00444
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00448
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00450
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0045c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0046c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00470
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x55555555,                                                  // 4 random values                             /// 00484
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00488
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0048c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00490
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00494
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004bc
			 0x33333333,                                                  // 4 random values                             /// 004c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004d0
			 0x00000000,                                                  // zero                                        /// 004d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00500
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00504
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00508
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0050c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0xbf028f5c,                                                  // -0.51                                       /// 00514
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00520
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00524
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0052c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00530
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00534
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00538
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0053c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00544
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00548
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0054c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00550
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00554
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00558
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0055c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00560
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00564
			 0x3f028f5c,                                                  // 0.51                                        /// 00568
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0056c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00570
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00578
			 0xbf028f5c,                                                  // -0.51                                       /// 0057c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00580
			 0x00000000,                                                  // zero                                        /// 00584
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0058c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00590
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00598
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0059c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x55555555,                                                  // 4 random values                             /// 005f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00610
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0061c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00620
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00624
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00628
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00638
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0063c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00640
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00648
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00650
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00658
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0065c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00664
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00674
			 0x0c700000,                                                  // Leading 1s:                                 /// 00678
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0067c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00680
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00688
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0068c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00690
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00698
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00700
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00704
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xffc00001,                                                  // -signaling NaN                              /// 0070c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0071c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00720
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00724
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00728
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0072c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0073c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00740
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00764
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0076c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00770
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00774
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00778
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0077c
			 0x55555555,                                                  // 4 random values                             /// 00780
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00784
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00788
			 0x0c400000,                                                  // Leading 1s:                                 /// 0078c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00794
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00798
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0079c
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00804
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00808
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0080c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00810
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0081c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00824
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00828
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0082c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00830
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00834
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00838
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0083c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0084c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f800000,                                                  // inf                                         /// 00858
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00864
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00868
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0086c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00878
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0087c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00880
			 0x7fc00001,                                                  // signaling NaN                               /// 00884
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00888
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00898
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0089c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xbf028f5c,                                                  // -0.51                                       /// 008ac
			 0x7fc00001,                                                  // signaling NaN                               /// 008b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00900
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00904
			 0xbf028f5c,                                                  // -0.51                                       /// 00908
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00910
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00914
			 0x7fc00001,                                                  // signaling NaN                               /// 00918
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0091c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00920
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00924
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00928
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0092c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00930
			 0xcb000000,                                                  // -8388608.0                                  /// 00934
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00938
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00940
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00944
			 0x0e000003,                                                  // Trailing 1s:                                /// 00948
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00950
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00960
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00964
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0096c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00970
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00978
			 0x0c780000,                                                  // Leading 1s:                                 /// 0097c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00980
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0098c
			 0x33333333,                                                  // 4 random values                             /// 00990
			 0xffc00001,                                                  // -signaling NaN                              /// 00994
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a8
			 0x33333333,                                                  // 4 random values                             /// 009ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009bc
			 0x00000000,                                                  // zero                                        /// 009c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 009d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a10
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a40
			 0x55555555,                                                  // 4 random values                             /// 00a44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a84
			 0x7f800000,                                                  // inf                                         /// 00a88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aa8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ab4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00abc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ac0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00adc
			 0x80000000,                                                  // -zero                                       /// 00ae0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x55555555,                                                  // 4 random values                             /// 00af8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ba4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bbc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bdc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be4
			 0x3f028f5c,                                                  // 0.51                                        /// 00be8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bf0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000000,                                                  // -zero                                       /// 00c2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c48
			 0x7fc00001,                                                  // signaling NaN                               /// 00c4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c6c
			 0x33333333,                                                  // 4 random values                             /// 00c70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cb8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ce4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cf0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00000000,                                                  // zero                                        /// 00cf8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d1c
			 0x00000000,                                                  // zero                                        /// 00d20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d24
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x7f800000,                                                  // inf                                         /// 00d30
			 0xbf028f5c,                                                  // -0.51                                       /// 00d34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d88
			 0x80000000,                                                  // -zero                                       /// 00d8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d94
			 0xcb000000,                                                  // -8388608.0                                  /// 00d98
			 0x4b000000,                                                  // 8388608.0                                   /// 00d9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xbf028f5c,                                                  // -0.51                                       /// 00db0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00db4
			 0x4b000000,                                                  // 8388608.0                                   /// 00db8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ddc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00df4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00df8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e10
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e3c
			 0x00000000,                                                  // zero                                        /// 00e40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e50
			 0x4b000000,                                                  // 8388608.0                                   /// 00e54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e70
			 0xffc00001,                                                  // -signaling NaN                              /// 00e74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e78
			 0xff800000,                                                  // -inf                                        /// 00e7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ea8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eac
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ec4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ed8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00edc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ee8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00efc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f0c
			 0x7f800000,                                                  // inf                                         /// 00f10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f2c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0x3f028f5c,                                                  // 0.51                                        /// 00f7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f80
			 0x4b000000,                                                  // 8388608.0                                   /// 00f84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f98
			 0xff800000,                                                  // -inf                                        /// 00f9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80000000,                                                  // -zero                                       /// 00fbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fe8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ff0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ff4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7ff800                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0c600000,                                                  // Leading 1s:                                 /// 00004
			 0x3f028f5c,                                                  // 0.51                                        /// 00008
			 0xffc00001,                                                  // -signaling NaN                              /// 0000c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00010
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0001c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00020
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x00000000,                                                  // zero                                        /// 00030
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00038
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0003c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00054
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00058
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0005c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00060
			 0x0c400000,                                                  // Leading 1s:                                 /// 00064
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0006c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00070
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00078
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0007c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00084
			 0xcb000000,                                                  // -8388608.0                                  /// 00088
			 0x0c780000,                                                  // Leading 1s:                                 /// 0008c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00090
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00094
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x7fc00001,                                                  // signaling NaN                               /// 0009c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x7f800000,                                                  // inf                                         /// 000a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x0c400000,                                                  // Leading 1s:                                 /// 00104
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00108
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0010c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00114
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00118
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0011c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00120
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00124
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00130
			 0x4b000000,                                                  // 8388608.0                                   /// 00134
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00140
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00148
			 0x0c400000,                                                  // Leading 1s:                                 /// 0014c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x4b000000,                                                  // 8388608.0                                   /// 00154
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00158
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00160
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00164
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00168
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00170
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00174
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00184
			 0x00011111,                                                  // 9.7958E-41                                  /// 00188
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00190
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00194
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00198
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0019c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00204
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00208
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0020c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00214
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0021c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00228
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0022c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x33333333,                                                  // 4 random values                             /// 00240
			 0x00000000,                                                  // zero                                        /// 00244
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00248
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0024c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00250
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00258
			 0x3f028f5c,                                                  // 0.51                                        /// 0025c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00260
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00264
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00274
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0027c
			 0x33333333,                                                  // 4 random values                             /// 00280
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00290
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00294
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0029c
			 0x0c780000,                                                  // Leading 1s:                                 /// 002a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x3f028f5c,                                                  // 0.51                                        /// 002d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00300
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00304
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00308
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0030c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00310
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7f800000,                                                  // inf                                         /// 00320
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00328
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0032c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00330
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00338
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0033c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00340
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00344
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00350
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00358
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0035c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e000001,                                                  // Trailing 1s:                                /// 00364
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00368
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0036c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00370
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xffc00001,                                                  // -signaling NaN                              /// 0037c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00380
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00384
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x3f028f5c,                                                  // 0.51                                        /// 0038c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00394
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0039c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c4
			 0xbf028f5c,                                                  // -0.51                                       /// 003c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x55555555,                                                  // 4 random values                             /// 003d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d8
			 0xffc00001,                                                  // -signaling NaN                              /// 003dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0xbf028f5c,                                                  // -0.51                                       /// 003f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00404
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0040c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00414
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00418
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00420
			 0x0c780000,                                                  // Leading 1s:                                 /// 00424
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00430
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00434
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00438
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00444
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00448
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00450
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0045c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00468
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0046c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x33333333,                                                  // 4 random values                             /// 00478
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00484
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00488
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0048c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00498
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0049c
			 0x33333333,                                                  // 4 random values                             /// 004a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0050c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00510
			 0xcb000000,                                                  // -8388608.0                                  /// 00514
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00518
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0051c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x55555555,                                                  // 4 random values                             /// 00528
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00530
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00534
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00538
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0053c
			 0x7f800000,                                                  // inf                                         /// 00540
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00544
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00560
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0056c
			 0x55555555,                                                  // 4 random values                             /// 00570
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00574
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0057c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00580
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00584
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00588
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0058c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00590
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00594
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0059c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x55555555,                                                  // 4 random values                             /// 005b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00600
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0060c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00610
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00614
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00618
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0061c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00620
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00624
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00628
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00638
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0063c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0064c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00650
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00654
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00658
			 0xbf028f5c,                                                  // -0.51                                       /// 0065c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0xffc00001,                                                  // -signaling NaN                              /// 00668
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0066c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00670
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00674
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00678
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0067c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00680
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00684
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00688
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00690
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00694
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00698
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xcb000000,                                                  // -8388608.0                                  /// 006a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e4
			 0xcb000000,                                                  // -8388608.0                                  /// 006e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00708
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0070c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00710
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00718
			 0xbf028f5c,                                                  // -0.51                                       /// 0071c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00000000,                                                  // zero                                        /// 00724
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00728
			 0x0c400000,                                                  // Leading 1s:                                 /// 0072c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00734
			 0x0c700000,                                                  // Leading 1s:                                 /// 00738
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00748
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00750
			 0x55555555,                                                  // 4 random values                             /// 00754
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00758
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00760
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0e000003,                                                  // Trailing 1s:                                /// 0076c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00774
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0077c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xcb000000,                                                  // -8388608.0                                  /// 00784
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00011111,                                                  // 9.7958E-41                                  /// 0078c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00790
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00798
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x33333333,                                                  // 4 random values                             /// 007a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x55555555,                                                  // 4 random values                             /// 007d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x7fc00001,                                                  // signaling NaN                               /// 007f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007fc
			 0x00000000,                                                  // zero                                        /// 00800
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00804
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00810
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00814
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00818
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0081c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x4b000000,                                                  // 8388608.0                                   /// 0082c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00830
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00834
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0083c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00840
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00848
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x7f800000,                                                  // inf                                         /// 00850
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00854
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00858
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00860
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x00000000,                                                  // zero                                        /// 0086c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00870
			 0x33333333,                                                  // 4 random values                             /// 00874
			 0x0e000001,                                                  // Trailing 1s:                                /// 00878
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0c780000,                                                  // Leading 1s:                                 /// 00884
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00888
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0088c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00894
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00898
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0089c
			 0x33333333,                                                  // 4 random values                             /// 008a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0xff800000,                                                  // -inf                                        /// 008b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008b4
			 0x33333333,                                                  // 4 random values                             /// 008b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008bc
			 0x7f800000,                                                  // inf                                         /// 008c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008dc
			 0xff800000,                                                  // -inf                                        /// 008e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008f0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00900
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0090c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00910
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00914
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00918
			 0x00011111,                                                  // 9.7958E-41                                  /// 0091c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00920
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00924
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00928
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0092c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00930
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0093c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00940
			 0x80011111,                                                  // -9.7958E-41                                 /// 00944
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00954
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00960
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00964
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0096c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0097c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00984
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0098c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00990
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00994
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00998
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0099c
			 0xffc00001,                                                  // -signaling NaN                              /// 009a0
			 0xcb000000,                                                  // -8388608.0                                  /// 009a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a20
			 0xffc00001,                                                  // -signaling NaN                              /// 00a24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00000000,                                                  // zero                                        /// 00a30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aa4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ab0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00abc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ac4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ad8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00adc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae0
			 0x7f800000,                                                  // inf                                         /// 00ae4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00af4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00af8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x80000000,                                                  // -zero                                       /// 00b08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ba0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00be4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bf0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bf4
			 0x80000000,                                                  // -zero                                       /// 00bf8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xff800000,                                                  // -inf                                        /// 00c04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c34
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c84
			 0xffc00001,                                                  // -signaling NaN                              /// 00c88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c90
			 0x55555555,                                                  // 4 random values                             /// 00c94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x7f800000,                                                  // inf                                         /// 00cac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x55555555,                                                  // 4 random values                             /// 00ccc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cdc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ce0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d40
			 0x33333333,                                                  // 4 random values                             /// 00d44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0xff800000,                                                  // -inf                                        /// 00d70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d78
			 0xcb000000,                                                  // -8388608.0                                  /// 00d7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d94
			 0x3f028f5c,                                                  // 0.51                                        /// 00d98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00db0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00df0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df8
			 0xff800000,                                                  // -inf                                        /// 00dfc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0xbf028f5c,                                                  // -0.51                                       /// 00e58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x4b000000,                                                  // 8388608.0                                   /// 00e64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ea4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eb0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ebc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ec0
			 0x55555555,                                                  // 4 random values                             /// 00ec4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ecc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ed4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ee4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ef4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f20
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f24
			 0xcb000000,                                                  // -8388608.0                                  /// 00f28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x33333333,                                                  // 4 random values                             /// 00f88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f90
			 0x7fc00001,                                                  // signaling NaN                               /// 00f94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fe8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff8
			 0x80000010                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00000
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00004
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00008
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0000c
			 0x7fc00001,                                                  // signaling NaN                               /// 00010
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00014
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00018
			 0x55555555,                                                  // 4 random values                             /// 0001c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00028
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00030
			 0x33333333,                                                  // 4 random values                             /// 00034
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00038
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0003c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00044
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80000000,                                                  // -zero                                       /// 0004c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00050
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00054
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00058
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0005c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00060
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00064
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00068
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0006c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0c600000,                                                  // Leading 1s:                                 /// 00078
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0007c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c400000,                                                  // Leading 1s:                                 /// 00084
			 0x3f028f5c,                                                  // 0.51                                        /// 00088
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00094
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00098
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0009c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000b8
			 0xbf028f5c,                                                  // -0.51                                       /// 000bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80000000,                                                  // -zero                                       /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00100
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00104
			 0xcb000000,                                                  // -8388608.0                                  /// 00108
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00110
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00124
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0012c
			 0x3f028f5c,                                                  // 0.51                                        /// 00130
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00134
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00138
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00148
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00150
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00154
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0015c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00160
			 0x0e000001,                                                  // Trailing 1s:                                /// 00164
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00174
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00178
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00184
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00190
			 0x0e000003,                                                  // Trailing 1s:                                /// 00194
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00198
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0019c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0xff800000,                                                  // -inf                                        /// 001b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00200
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0020c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0c400000,                                                  // Leading 1s:                                 /// 0021c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00220
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00224
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00230
			 0x80011111,                                                  // -9.7958E-41                                 /// 00234
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00238
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0023c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00248
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00250
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00254
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00258
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0025c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00264
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0026c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00270
			 0x0c780000,                                                  // Leading 1s:                                 /// 00274
			 0x0e000007,                                                  // Trailing 1s:                                /// 00278
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00284
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00290
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00294
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00298
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xbf028f5c,                                                  // -0.51                                       /// 002a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002a8
			 0x7fc00001,                                                  // signaling NaN                               /// 002ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x7f800000,                                                  // inf                                         /// 002e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00300
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00304
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x0c400000,                                                  // Leading 1s:                                 /// 0030c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0031c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00324
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00328
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0032c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00334
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00338
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00340
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00344
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00348
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0034c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00354
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0035c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00364
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000000,                                                  // -zero                                       /// 0036c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00370
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00374
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00378
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0037c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00380
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00384
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00388
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0038c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00390
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00398
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003c4
			 0xbf028f5c,                                                  // -0.51                                       /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003ec
			 0x33333333,                                                  // 4 random values                             /// 003f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00400
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00408
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0040c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00414
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00418
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0041c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00420
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00424
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0042c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000000,                                                  // zero                                        /// 00434
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00438
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0043c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x3f028f5c,                                                  // 0.51                                        /// 00448
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0044c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00450
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x3f028f5c,                                                  // 0.51                                        /// 00458
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00460
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00464
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00468
			 0x0c780000,                                                  // Leading 1s:                                 /// 0046c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00470
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00474
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00478
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0047c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00480
			 0x7f800000,                                                  // inf                                         /// 00484
			 0x0e000001,                                                  // Trailing 1s:                                /// 00488
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00490
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00498
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004f4
			 0xff800000,                                                  // -inf                                        /// 004f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00500
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00508
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0051c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00524
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00528
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0052c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00530
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00534
			 0xffc00001,                                                  // -signaling NaN                              /// 00538
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00540
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00544
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00548
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0054c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00550
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0x80011111,                                                  // -9.7958E-41                                 /// 0055c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00564
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00568
			 0x0c700000,                                                  // Leading 1s:                                 /// 0056c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00570
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00578
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0057c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00580
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x7f800000,                                                  // inf                                         /// 00588
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00590
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0059c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005c0
			 0x7fc00001,                                                  // signaling NaN                               /// 005c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x80000000,                                                  // -zero                                       /// 005f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00604
			 0x7f800000,                                                  // inf                                         /// 00608
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00610
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x4b000000,                                                  // 8388608.0                                   /// 00628
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00630
			 0x0c400000,                                                  // Leading 1s:                                 /// 00634
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00638
			 0x55555555,                                                  // 4 random values                             /// 0063c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00644
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0064c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00658
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00664
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0xbf028f5c,                                                  // -0.51                                       /// 00670
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0c600000,                                                  // Leading 1s:                                 /// 0067c
			 0xbf028f5c,                                                  // -0.51                                       /// 00680
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00684
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00698
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x7f800000,                                                  // inf                                         /// 006e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00000000,                                                  // zero                                        /// 006f8
			 0x3f028f5c,                                                  // 0.51                                        /// 006fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00700
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0070c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00710
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00714
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00718
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0071c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00728
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00730
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0073c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00740
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00754
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00760
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00764
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0076c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00774
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00778
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00784
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00788
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00790
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00794
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0079c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007ac
			 0xff800000,                                                  // -inf                                        /// 007b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00808
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00810
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00814
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00818
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00820
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00824
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00830
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0083c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00840
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00848
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00850
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00854
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00858
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0085c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00868
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0086c
			 0xcb000000,                                                  // -8388608.0                                  /// 00870
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00874
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0087c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00884
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x4b000000,                                                  // 8388608.0                                   /// 0088c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00890
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00894
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00898
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0089c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x00000000,                                                  // zero                                        /// 008b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00904
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00908
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0090c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00914
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00920
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00924
			 0xffc00001,                                                  // -signaling NaN                              /// 00928
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00944
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00948
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0094c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00950
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00954
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00958
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00960
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00964
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00968
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0096c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00970
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00974
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xcb000000,                                                  // -8388608.0                                  /// 0097c
			 0x3f028f5c,                                                  // 0.51                                        /// 00980
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00984
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00988
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0098c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00998
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0099c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e4
			 0x3f028f5c,                                                  // 0.51                                        /// 009e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009ec
			 0xff800000,                                                  // -inf                                        /// 009f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a80
			 0x3f028f5c,                                                  // 0.51                                        /// 00a84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a94
			 0x3f028f5c,                                                  // 0.51                                        /// 00a98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aa4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ab8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00abc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ac4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ad0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af0
			 0x7f800000,                                                  // inf                                         /// 00af4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b14
			 0x3f028f5c,                                                  // 0.51                                        /// 00b18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xbf028f5c,                                                  // -0.51                                       /// 00b78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ba4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bfc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c4c
			 0x00000000,                                                  // zero                                        /// 00c50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c7c
			 0x7f800000,                                                  // inf                                         /// 00c80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ca8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cc4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ce4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ce8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cfc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xff800000,                                                  // -inf                                        /// 00d34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d60
			 0x7fc00001,                                                  // signaling NaN                               /// 00d64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d94
			 0xcb000000,                                                  // -8388608.0                                  /// 00d98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80000000,                                                  // -zero                                       /// 00da4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00da8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dfc
			 0x33333333,                                                  // 4 random values                             /// 00e00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x7f800000,                                                  // inf                                         /// 00e0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e7c
			 0x33333333,                                                  // 4 random values                             /// 00e80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ea0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ea8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00eb0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ebc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ec0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ecc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ed8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00edc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ee8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ef4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f44
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0xbf028f5c,                                                  // -0.51                                       /// 00f64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f88
			 0xffc00001,                                                  // -signaling NaN                              /// 00f8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x55555555,                                                  // 4 random values                             /// 00fa4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fa8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff4
			 0x00000000,                                                  // zero                                        /// 00ff8
			 0x00000000 // 0                                             // SP +ve numbers                              /// last
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
			 0x00000488,
			 0x000006a0,
			 0x0000029c,
			 0x000007c0,
			 0x0000016c,
			 0x00000698,
			 0x000007a0,
			 0x000006ac,

			 /// vpu register f2
			 0x40e00000,
			 0x41c80000,
			 0x41e80000,
			 0x41e80000,
			 0x41500000,
			 0x42000000,
			 0x41b00000,
			 0x3f800000,

			 /// vpu register f3
			 0x41d00000,
			 0x41d80000,
			 0x40800000,
			 0x40c00000,
			 0x41d00000,
			 0x41980000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f4
			 0x41100000,
			 0x41d80000,
			 0x41980000,
			 0x41b00000,
			 0x41300000,
			 0x41a00000,
			 0x41d80000,
			 0x41a00000,

			 /// vpu register f5
			 0x40400000,
			 0x42000000,
			 0x40a00000,
			 0x41c00000,
			 0x41300000,
			 0x41100000,
			 0x41d80000,
			 0x41200000,

			 /// vpu register f6
			 0x41b80000,
			 0x41600000,
			 0x41400000,
			 0x41c80000,
			 0x41600000,
			 0x41800000,
			 0x41f00000,
			 0x41a00000,

			 /// vpu register f7
			 0x41700000,
			 0x40400000,
			 0x41c00000,
			 0x41800000,
			 0x41b80000,
			 0x41100000,
			 0x41a00000,
			 0x41a00000,

			 /// vpu register f8
			 0x41b00000,
			 0x41c80000,
			 0x41980000,
			 0x40c00000,
			 0x41300000,
			 0x41200000,
			 0x41700000,
			 0x41100000,

			 /// vpu register f9
			 0x41900000,
			 0x3f800000,
			 0x41600000,
			 0x41a80000,
			 0x41d80000,
			 0x41c80000,
			 0x41c00000,
			 0x40400000,

			 /// vpu register f10
			 0x41f80000,
			 0x41f00000,
			 0x41000000,
			 0x41800000,
			 0x41400000,
			 0x40400000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f11
			 0x40a00000,
			 0x41300000,
			 0x41d00000,
			 0x41900000,
			 0x41b80000,
			 0x41e80000,
			 0x40a00000,
			 0x41c80000,

			 /// vpu register f12
			 0x42000000,
			 0x41f00000,
			 0x40800000,
			 0x41f80000,
			 0x41880000,
			 0x41100000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f13
			 0x41e80000,
			 0x41b00000,
			 0x41e00000,
			 0x41e00000,
			 0x41a80000,
			 0x41100000,
			 0x40a00000,
			 0x3f800000,

			 /// vpu register f14
			 0x41d80000,
			 0x41d80000,
			 0x41700000,
			 0x41200000,
			 0x41e00000,
			 0x41d00000,
			 0x41980000,
			 0x40800000,

			 /// vpu register f15
			 0x41980000,
			 0x41000000,
			 0x41e00000,
			 0x41400000,
			 0x3f800000,
			 0x41f00000,
			 0x40800000,
			 0x41500000,

			 /// vpu register f16
			 0x41600000,
			 0x41000000,
			 0x40800000,
			 0x41a80000,
			 0x41200000,
			 0x40800000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f17
			 0x41500000,
			 0x41100000,
			 0x41f80000,
			 0x41e80000,
			 0x41a00000,
			 0x41c00000,
			 0x40a00000,
			 0x40e00000,

			 /// vpu register f18
			 0x41d00000,
			 0x41500000,
			 0x40800000,
			 0x41600000,
			 0x41100000,
			 0x41800000,
			 0x41d80000,
			 0x41e00000,

			 /// vpu register f19
			 0x41c80000,
			 0x40000000,
			 0x41a80000,
			 0x41900000,
			 0x40400000,
			 0x40a00000,
			 0x41b80000,
			 0x41100000,

			 /// vpu register f20
			 0x41a00000,
			 0x42000000,
			 0x41880000,
			 0x41c80000,
			 0x41e00000,
			 0x41a00000,
			 0x41c80000,
			 0x41500000,

			 /// vpu register f21
			 0x41e80000,
			 0x40c00000,
			 0x41e80000,
			 0x3f800000,
			 0x41d80000,
			 0x41900000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f22
			 0x40000000,
			 0x41800000,
			 0x41800000,
			 0x41a80000,
			 0x41880000,
			 0x40400000,
			 0x41f80000,
			 0x41a00000,

			 /// vpu register f23
			 0x41700000,
			 0x41600000,
			 0x42000000,
			 0x41880000,
			 0x41b00000,
			 0x41c00000,
			 0x41980000,
			 0x41d80000,

			 /// vpu register f24
			 0x41c80000,
			 0x41800000,
			 0x41200000,
			 0x41a00000,
			 0x41600000,
			 0x41b80000,
			 0x41000000,
			 0x41e80000,

			 /// vpu register f25
			 0x40800000,
			 0x41100000,
			 0x40e00000,
			 0x41400000,
			 0x41f00000,
			 0x41b00000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f26
			 0x42000000,
			 0x41500000,
			 0x41800000,
			 0x41900000,
			 0x41300000,
			 0x41f00000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f27
			 0x40800000,
			 0x41a00000,
			 0x41800000,
			 0x40400000,
			 0x41e80000,
			 0x41980000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f28
			 0x40e00000,
			 0x41d00000,
			 0x41880000,
			 0x41000000,
			 0x41700000,
			 0x40000000,
			 0x41a80000,
			 0x42000000,

			 /// vpu register f29
			 0x40a00000,
			 0x41100000,
			 0x41e00000,
			 0x41600000,
			 0x40a00000,
			 0x41b80000,
			 0x41a00000,
			 0x40400000,

			 /// vpu register f30
			 0x40a00000,
			 0x41800000,
			 0x41c80000,
			 0x40000000,
			 0x41a80000,
			 0x41b00000,
			 0x41200000,
			 0x40800000,

			 /// vpu register f31
			 0x41f80000,
			 0x40a00000,
			 0x41d00000,
			 0x40e00000,
			 0x41b00000,
			 0x3f800000,
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
		"fcvt.ps.un2 f14, f7\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f2, f17\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f4, f24\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f17, f7\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f14, f0\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f7\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f3, f27\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f25, f1\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f4, f16\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f19, f0\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f27, f29\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f30, f0\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f14, f22\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f24\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f19\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f22, f28\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f12, f18\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f25, f25\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f21, f6\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f6, f27\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f27, f6\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f16, f15\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f16, f28\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f9, f26\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f13\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f28, f27\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f8, f14\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f6, f23\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f12, f9\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f12, f14\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f7\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f4, f16\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f4, f4\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f14, f27\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f23\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f19, f12\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f18, f24\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f13, f4\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f10, f26\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f30, f4\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f19\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f4\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f5\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f23\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f14\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f3, f9\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f12, f6\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f28, f24\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f9, f30\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f14, f27\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f28\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f7\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f19, f7\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f5, f30\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f19, f2\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f28\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f11, f3\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f13, f27\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f4\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f9, f11\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f14, f27\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f28, f29\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f0\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f20\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f18, f3\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f18, f13\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f11\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f8, f17\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f29, f20\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f5, f2\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f5, f10\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f6\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f21, f29\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f19\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f9, f25\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f2, f25\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f22, f29\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f26\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f5\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f17\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f20, f21\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f25, f18\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f22, f12\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f5, f8\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f24\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f23, f6\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f14, f19\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f7, f10\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f5, f4\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f25\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f27\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f26, f6\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f9, f20\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f4, f15\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f15, f30\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f30, f29\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f5, f19\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f24, f16\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un2 f12, f14\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
