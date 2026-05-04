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
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00000
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00004
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0000c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00010
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00014
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00018
			 0x7f800000,                                                  // inf                                         /// 0001c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00020
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00024
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00028
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0002c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00030
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00038
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00050
			 0xbf028f5c,                                                  // -0.51                                       /// 00054
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00058
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00064
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00070
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00074
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0007c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00080
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00084
			 0xbf028f5c,                                                  // -0.51                                       /// 00088
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00098
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0009c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0xbf028f5c,                                                  // -0.51                                       /// 000c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7f800000,                                                  // inf                                         /// 000e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00108
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0010c
			 0xbf028f5c,                                                  // -0.51                                       /// 00110
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00118
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00120
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00128
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0012c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00134
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00138
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00140
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0014c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00150
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00154
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00158
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0015c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e000003,                                                  // Trailing 1s:                                /// 00164
			 0x33333333,                                                  // 4 random values                             /// 00168
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0016c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00170
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0017c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00180
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00184
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0018c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00190
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0019c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c4
			 0xcb000000,                                                  // -8388608.0                                  /// 001c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f4
			 0xff800000,                                                  // -inf                                        /// 001f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00200
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00204
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00210
			 0x0c600000,                                                  // Leading 1s:                                 /// 00214
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00218
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0021c
			 0xff800000,                                                  // -inf                                        /// 00220
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00224
			 0xffc00001,                                                  // -signaling NaN                              /// 00228
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0022c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00234
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0023c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00240
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00244
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0024c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0e000007,                                                  // Trailing 1s:                                /// 00258
			 0x7fc00001,                                                  // signaling NaN                               /// 0025c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00268
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0026c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0xffc00001,                                                  // -signaling NaN                              /// 00274
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0e000003,                                                  // Trailing 1s:                                /// 0027c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00284
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0029c
			 0x55555555,                                                  // 4 random values                             /// 002a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c8
			 0x00000000,                                                  // zero                                        /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80000000,                                                  // -zero                                       /// 002d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7fc00001,                                                  // signaling NaN                               /// 002e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00304
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00308
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0030c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00320
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00324
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00330
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00334
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00338
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00340
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00344
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00348
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00350
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00354
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00358
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80000000,                                                  // -zero                                       /// 00360
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00364
			 0x00000000,                                                  // zero                                        /// 00368
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0036c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00370
			 0x7fc00001,                                                  // signaling NaN                               /// 00374
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0037c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00384
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0038c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00394
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0039c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00400
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00404
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00408
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00410
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00414
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00418
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0041c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00420
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00428
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0042c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00430
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00434
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00438
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00440
			 0x0c780000,                                                  // Leading 1s:                                 /// 00444
			 0x4b000000,                                                  // 8388608.0                                   /// 00448
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0044c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0c780000,                                                  // Leading 1s:                                 /// 00458
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0045c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0e000003,                                                  // Trailing 1s:                                /// 00468
			 0x0e000003,                                                  // Trailing 1s:                                /// 0046c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00470
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00478
			 0xff800000,                                                  // -inf                                        /// 0047c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00480
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00484
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00498
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0049c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xff800000,                                                  // -inf                                        /// 004f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00500
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00504
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00508
			 0x7fc00001,                                                  // signaling NaN                               /// 0050c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0051c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00520
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0052c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00530
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00534
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0xffc00001,                                                  // -signaling NaN                              /// 0053c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00540
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00544
			 0x0e000003,                                                  // Trailing 1s:                                /// 00548
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0054c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x0e000003,                                                  // Trailing 1s:                                /// 00564
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00574
			 0x0c700000,                                                  // Leading 1s:                                 /// 00578
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00580
			 0x0c780000,                                                  // Leading 1s:                                 /// 00584
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0058c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00598
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0059c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00600
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00604
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00610
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00614
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00628
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0062c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00630
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00634
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00638
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0063c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00640
			 0x80000000,                                                  // -zero                                       /// 00644
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0064c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x7f800000,                                                  // inf                                         /// 00654
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00658
			 0x0c400000,                                                  // Leading 1s:                                 /// 0065c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00660
			 0x80000000,                                                  // -zero                                       /// 00664
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0066c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0067c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00684
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00688
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0xbf028f5c,                                                  // -0.51                                       /// 006c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00704
			 0xcb000000,                                                  // -8388608.0                                  /// 00708
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0070c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00710
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0071c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00720
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00728
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0072c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00730
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00734
			 0x0e000003,                                                  // Trailing 1s:                                /// 00738
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0073c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00744
			 0xff800000,                                                  // -inf                                        /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00754
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00760
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00768
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0076c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00770
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00774
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00778
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0077c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00780
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x7fc00001,                                                  // signaling NaN                               /// 00788
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00790
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00798
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0079c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 007b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007e0
			 0x80000000,                                                  // -zero                                       /// 007e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00800
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00808
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0080c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00810
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00818
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00828
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00834
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00838
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0083c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00840
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00850
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00854
			 0x0c700000,                                                  // Leading 1s:                                 /// 00858
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0085c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00860
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x3f028f5c,                                                  // 0.51                                        /// 0086c
			 0x55555555,                                                  // 4 random values                             /// 00870
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00880
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00884
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00888
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0088c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00890
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d0
			 0x7f800000,                                                  // inf                                         /// 008d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00904
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00908
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0090c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00914
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0091c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00924
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00928
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0092c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00930
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00948
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0094c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00950
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00954
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00958
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0095c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00964
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00968
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00970
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00974
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00978
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00988
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00994
			 0x00011111,                                                  // 9.7958E-41                                  /// 00998
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e4
			 0x3f028f5c,                                                  // 0.51                                        /// 009e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0xffc00001,                                                  // -signaling NaN                              /// 009f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00000000,                                                  // zero                                        /// 00a14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x4b000000,                                                  // 8388608.0                                   /// 00a9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ac8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00acc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00afc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b44
			 0x7f800000,                                                  // inf                                         /// 00b48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b4c
			 0x80000000,                                                  // -zero                                       /// 00b50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b6c
			 0xff800000,                                                  // -inf                                        /// 00b70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ba4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bb0
			 0xff800000,                                                  // -inf                                        /// 00bb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bbc
			 0x55555555,                                                  // 4 random values                             /// 00bc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00be8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf4
			 0x7f800000,                                                  // inf                                         /// 00bf8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x33333333,                                                  // 4 random values                             /// 00c10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c4c
			 0x80000000,                                                  // -zero                                       /// 00c50
			 0xffc00001,                                                  // -signaling NaN                              /// 00c54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ca0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ccc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ce8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cf0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cfc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00000000,                                                  // zero                                        /// 00d4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d70
			 0x4b000000,                                                  // 8388608.0                                   /// 00d74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x00000000,                                                  // zero                                        /// 00d98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00db8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00de0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dec
			 0x7f800000,                                                  // inf                                         /// 00df0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x80000000,                                                  // -zero                                       /// 00dfc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e28
			 0xbf028f5c,                                                  // -0.51                                       /// 00e2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e48
			 0x55555555,                                                  // 4 random values                             /// 00e4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e5c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ea0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec0
			 0xff800000,                                                  // -inf                                        /// 00ec4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ec8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ecc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ed0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ee8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ef4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ef8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00efc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f18
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f28
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fa8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00fc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00fe0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fec
			 0xffc00001,                                                  // -signaling NaN                              /// 00ff0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ff4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff8
			 0xbf800001 // 1  + 1ulp                                     // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00000
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00004
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0c400000,                                                  // Leading 1s:                                 /// 0000c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00010
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00014
			 0x0e000007,                                                  // Trailing 1s:                                /// 00018
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0001c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00020
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00024
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0002c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00030
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00040
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00044
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00048
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x55555555,                                                  // 4 random values                             /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00058
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0005c
			 0xcb000000,                                                  // -8388608.0                                  /// 00060
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00064
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00068
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0006c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00070
			 0x0e000003,                                                  // Trailing 1s:                                /// 00074
			 0xffc00001,                                                  // -signaling NaN                              /// 00078
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0007c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00080
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00084
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0008c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00094
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00098
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00100
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x00000000,                                                  // zero                                        /// 0010c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00110
			 0x7f800000,                                                  // inf                                         /// 00114
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00118
			 0xffc00001,                                                  // -signaling NaN                              /// 0011c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00120
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00128
			 0x4b000000,                                                  // 8388608.0                                   /// 0012c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00130
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00138
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0013c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00140
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00144
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00148
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0014c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00150
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00154
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0e000007,                                                  // Trailing 1s:                                /// 0015c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00160
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00164
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x7fc00001,                                                  // signaling NaN                               /// 00174
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00178
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0017c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00180
			 0x0c700000,                                                  // Leading 1s:                                 /// 00184
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00188
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00198
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0019c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x55555555,                                                  // 4 random values                             /// 001ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80000000,                                                  // -zero                                       /// 001d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x7f800000,                                                  // inf                                         /// 00200
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00204
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00210
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00214
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00218
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0021c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00224
			 0x0c600000,                                                  // Leading 1s:                                 /// 00228
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0022c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00230
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00234
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00238
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0023c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00240
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00248
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0024c
			 0x7f800000,                                                  // inf                                         /// 00250
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00264
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00268
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0026c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00270
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00278
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0027c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0028c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00290
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x00011111,                                                  // 9.7958E-41                                  /// 0029c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a0
			 0x55555555,                                                  // 4 random values                             /// 002a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0xffc00001,                                                  // -signaling NaN                              /// 002e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 002e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00300
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00304
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00308
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0030c
			 0x55555555,                                                  // 4 random values                             /// 00310
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00324
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00328
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0032c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00330
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00344
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00348
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00350
			 0x0e000001,                                                  // Trailing 1s:                                /// 00354
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00364
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00368
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0036c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00374
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00378
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0037c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00380
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00384
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00388
			 0x0e000007,                                                  // Trailing 1s:                                /// 0038c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00390
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00398
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0039c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 003b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d0
			 0x4b000000,                                                  // 8388608.0                                   /// 003d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x0c400000,                                                  // Leading 1s:                                 /// 0040c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00410
			 0x0c780000,                                                  // Leading 1s:                                 /// 00414
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00418
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00428
			 0x7fc00001,                                                  // signaling NaN                               /// 0042c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00430
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00434
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00444
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00448
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0044c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00450
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00454
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00458
			 0x0e000007,                                                  // Trailing 1s:                                /// 0045c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00464
			 0x0c780000,                                                  // Leading 1s:                                 /// 00468
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00474
			 0xffc00001,                                                  // -signaling NaN                              /// 00478
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00480
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00488
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0048c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00490
			 0x55555555,                                                  // 4 random values                             /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0049c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a4
			 0x80000000,                                                  // -zero                                       /// 004a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x3f028f5c,                                                  // 0.51                                        /// 004bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004c8
			 0xffc00001,                                                  // -signaling NaN                              /// 004cc
			 0x3f028f5c,                                                  // 0.51                                        /// 004d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x33333333,                                                  // 4 random values                             /// 004e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00508
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00510
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00514
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0051c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00520
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00530
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00538
			 0x0c400000,                                                  // Leading 1s:                                 /// 0053c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00540
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00548
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0054c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00558
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00560
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00564
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00570
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00580
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xffc00001,                                                  // -signaling NaN                              /// 00588
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0058c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00590
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00594
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00598
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00604
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0060c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00610
			 0x4b000000,                                                  // 8388608.0                                   /// 00614
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00618
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x7fc00001,                                                  // signaling NaN                               /// 00620
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00628
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0063c
			 0x7f800000,                                                  // inf                                         /// 00640
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00644
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0064c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00650
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00660
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00664
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00668
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00678
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00680
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00684
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0068c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00690
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a8
			 0x33333333,                                                  // 4 random values                             /// 006ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00704
			 0xbf028f5c,                                                  // -0.51                                       /// 00708
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00710
			 0x80000000,                                                  // -zero                                       /// 00714
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00718
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00720
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00728
			 0x80000000,                                                  // -zero                                       /// 0072c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00730
			 0xffc00001,                                                  // -signaling NaN                              /// 00734
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00738
			 0x0c700000,                                                  // Leading 1s:                                 /// 0073c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00740
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00744
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0074c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00750
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00758
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0075c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00760
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00764
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00768
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00770
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x33333333,                                                  // 4 random values                             /// 0077c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00788
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0078c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00790
			 0x00011111,                                                  // 9.7958E-41                                  /// 00794
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00798
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0079c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x33333333,                                                  // 4 random values                             /// 007b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007cc
			 0x00000000,                                                  // zero                                        /// 007d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00800
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00804
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00808
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0080c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00810
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00814
			 0x00000000,                                                  // zero                                        /// 00818
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00824
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0082c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00838
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00844
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00850
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00858
			 0x0c400000,                                                  // Leading 1s:                                 /// 0085c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00860
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00868
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0086c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00870
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0087c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00884
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00890
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0089c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b8
			 0x00000000,                                                  // zero                                        /// 008bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008cc
			 0xff800000,                                                  // -inf                                        /// 008d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f4
			 0x00000000,                                                  // zero                                        /// 008f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00900
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00904
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00908
			 0x0e000007,                                                  // Trailing 1s:                                /// 0090c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00914
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0091c
			 0x4b000000,                                                  // 8388608.0                                   /// 00920
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00928
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0092c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00930
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00934
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00938
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0093c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00940
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00944
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00948
			 0x0e000007,                                                  // Trailing 1s:                                /// 0094c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00950
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00954
			 0x0e000007,                                                  // Trailing 1s:                                /// 00958
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0095c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00960
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00964
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00968
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00970
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x0c700000,                                                  // Leading 1s:                                 /// 00978
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0097c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00980
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00984
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00988
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0098c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00994
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00998
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0099c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x33333333,                                                  // 4 random values                             /// 009c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 009f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a4c
			 0x55555555,                                                  // 4 random values                             /// 00a50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a84
			 0x7fc00001,                                                  // signaling NaN                               /// 00a88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00aa4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00acc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ae4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80000000,                                                  // -zero                                       /// 00afc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b34
			 0x00000000,                                                  // zero                                        /// 00b38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x55555555,                                                  // 4 random values                             /// 00b60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ba4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ba8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bcc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bf0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ca0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ccc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ce0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d08
			 0xbf028f5c,                                                  // -0.51                                       /// 00d0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000000,                                                  // -zero                                       /// 00d54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d6c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d98
			 0x7fc00001,                                                  // signaling NaN                               /// 00d9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db4
			 0xbf028f5c,                                                  // -0.51                                       /// 00db8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dbc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dcc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000000,                                                  // zero                                        /// 00de0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00de4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df4
			 0x33333333,                                                  // 4 random values                             /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e40
			 0x33333333,                                                  // 4 random values                             /// 00e44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ea0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ebc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ee4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00efc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0xff800000,                                                  // -inf                                        /// 00f58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0xff800000,                                                  // -inf                                        /// 00f78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f7c
			 0x7f800000,                                                  // inf                                         /// 00f80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fa0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fe8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ff4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0e000001                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x00011111,                                                  // 9.7958E-41                                  /// 00004
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0000c
			 0xcb000000,                                                  // -8388608.0                                  /// 00010
			 0x0e000001,                                                  // Trailing 1s:                                /// 00014
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c700000,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00020
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00024
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0002c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00034
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00038
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00048
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0004c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00050
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00054
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00058
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0005c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0006c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00078
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00080
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00011111,                                                  // 9.7958E-41                                  /// 00088
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0008c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00094
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0009c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x33333333,                                                  // 4 random values                             /// 000c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00100
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00104
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00108
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0010c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00110
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0011c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00120
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00128
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0012c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00130
			 0x00000000,                                                  // zero                                        /// 00134
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00138
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00140
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00144
			 0x0c700000,                                                  // Leading 1s:                                 /// 00148
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0014c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00150
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00154
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00158
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0015c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00160
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00164
			 0x4b000000,                                                  // 8388608.0                                   /// 00168
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0016c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xcb000000,                                                  // -8388608.0                                  /// 00180
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00184
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00194
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00198
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00200
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00204
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00208
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0020c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00218
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0021c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00220
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00224
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00228
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00234
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00238
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00240
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00248
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0024c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00250
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00254
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00258
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00260
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00268
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0026c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00270
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00274
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00278
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0027c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00284
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00288
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xcb000000,                                                  // -8388608.0                                  /// 00290
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00298
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 002d0
			 0xff800000,                                                  // -inf                                        /// 002d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00300
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00304
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0030c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00318
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00320
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00324
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00328
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0032c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00330
			 0x0e000001,                                                  // Trailing 1s:                                /// 00334
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0033c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00344
			 0x0c700000,                                                  // Leading 1s:                                 /// 00348
			 0x7f800000,                                                  // inf                                         /// 0034c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00350
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00354
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e000007,                                                  // Trailing 1s:                                /// 0035c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00360
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00364
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00368
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0036c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00370
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x55555555,                                                  // 4 random values                             /// 00378
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0037c
			 0x80000000,                                                  // -zero                                       /// 00380
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00384
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x7f800000,                                                  // inf                                         /// 0038c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00390
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0039c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000000,                                                  // zero                                        /// 003ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003f4
			 0x4b000000,                                                  // 8388608.0                                   /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00400
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00404
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00408
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0040c
			 0x3f028f5c,                                                  // 0.51                                        /// 00410
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00414
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00418
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00420
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x0e000003,                                                  // Trailing 1s:                                /// 00428
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0042c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00434
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00438
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0043c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x0c700000,                                                  // Leading 1s:                                 /// 00448
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0044c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x33333333,                                                  // 4 random values                             /// 00454
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0045c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00464
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00468
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00470
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0047c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00480
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00484
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00488
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00490
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00494
			 0x0c700000,                                                  // Leading 1s:                                 /// 00498
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0049c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004dc
			 0xff800000,                                                  // -inf                                        /// 004e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e4
			 0x55555555,                                                  // 4 random values                             /// 004e8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000000,                                                  // zero                                        /// 004fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00500
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00504
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00508
			 0x4b000000,                                                  // 8388608.0                                   /// 0050c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00510
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00518
			 0x4b000000,                                                  // 8388608.0                                   /// 0051c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0052c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00530
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00534
			 0x0c780000,                                                  // Leading 1s:                                 /// 00538
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0xbf028f5c,                                                  // -0.51                                       /// 00544
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00548
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0054c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00550
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00554
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0055c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00560
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00564
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00568
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0056c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000000,                                                  // zero                                        /// 00574
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00578
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x7f800000,                                                  // inf                                         /// 0058c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00590
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00594
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00598
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0059c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x33333333,                                                  // 4 random values                             /// 005a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x4b000000,                                                  // 8388608.0                                   /// 005e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00600
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00624
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00628
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00638
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00640
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00650
			 0x80000000,                                                  // -zero                                       /// 00654
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00658
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0065c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00668
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00670
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00678
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00684
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00688
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00694
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00698
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0069c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006a4
			 0x4b000000,                                                  // 8388608.0                                   /// 006a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x33333333,                                                  // 4 random values                             /// 006f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00704
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00708
			 0x0c400000,                                                  // Leading 1s:                                 /// 0070c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00710
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00718
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0071c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00724
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00738
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0073c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00744
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00748
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0074c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0075c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00760
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00764
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00774
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0077c
			 0x80000000,                                                  // -zero                                       /// 00780
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00788
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0078c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00790
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00794
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007b8
			 0x4b000000,                                                  // 8388608.0                                   /// 007bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007dc
			 0x00000000,                                                  // zero                                        /// 007e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0e000003,                                                  // Trailing 1s:                                /// 00814
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0081c
			 0x7f800000,                                                  // inf                                         /// 00820
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x00011111,                                                  // 9.7958E-41                                  /// 00828
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0082c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00834
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0083c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00840
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00848
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00850
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00858
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0085c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00860
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00868
			 0x0c600000,                                                  // Leading 1s:                                 /// 0086c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00878
			 0x3f028f5c,                                                  // 0.51                                        /// 0087c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00880
			 0xcb000000,                                                  // -8388608.0                                  /// 00884
			 0xbf028f5c,                                                  // -0.51                                       /// 00888
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00890
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0089c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e4
			 0x7fc00001,                                                  // signaling NaN                               /// 008e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x33333333,                                                  // 4 random values                             /// 0090c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00920
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00924
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00928
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0092c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00930
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00934
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0093c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0c780000,                                                  // Leading 1s:                                 /// 00948
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x7fc00001,                                                  // signaling NaN                               /// 00950
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00954
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00958
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0095c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00968
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0096c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00970
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00974
			 0x3f028f5c,                                                  // 0.51                                        /// 00978
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0097c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00984
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00988
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0098c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00990
			 0xffc00001,                                                  // -signaling NaN                              /// 00994
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00998
			 0xffc00001,                                                  // -signaling NaN                              /// 0099c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x7fc00001,                                                  // signaling NaN                               /// 009b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c4
			 0x7f800000,                                                  // inf                                         /// 009c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009dc
			 0x33333333,                                                  // 4 random values                             /// 009e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80000000,                                                  // -zero                                       /// 00a54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aa8
			 0x80000000,                                                  // -zero                                       /// 00aac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ab0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00abc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ac8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00acc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ad4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00adc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ae0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00afc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b64
			 0x80000000,                                                  // -zero                                       /// 00b68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7f800000,                                                  // inf                                         /// 00b84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bf0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bf4
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ccc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ce8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cfc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f800000,                                                  // inf                                         /// 00d20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d24
			 0x33333333,                                                  // 4 random values                             /// 00d28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d2c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d38
			 0x7fc00001,                                                  // signaling NaN                               /// 00d3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d54
			 0x7fc00001,                                                  // signaling NaN                               /// 00d58
			 0xffc00001,                                                  // -signaling NaN                              /// 00d5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00da0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00da4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dbc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00df8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0xff800000,                                                  // -inf                                        /// 00e60
			 0x00000000,                                                  // zero                                        /// 00e64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x00000000,                                                  // zero                                        /// 00e8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ea4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ec8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00edc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ee0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ef0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ef4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00efc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f04
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0xcb000000,                                                  // -8388608.0                                  /// 00f84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fb0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fdc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fe8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ff0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ff4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x0e000007                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xf3d5f05c, /// 0x0
			 0xbc3387cf, /// 0x4
			 0xea511c87, /// 0x8
			 0x4a9ba0ce, /// 0xc
			 0xd47bc608, /// 0x10
			 0x0d017ca2, /// 0x14
			 0x63763d4b, /// 0x18
			 0x8549ddd5, /// 0x1c
			 0x5927b554, /// 0x20
			 0x28e78f75, /// 0x24
			 0x833f5a05, /// 0x28
			 0x55d5f2f7, /// 0x2c
			 0x5582c4bc, /// 0x30
			 0x87fa50d2, /// 0x34
			 0xf71ffe62, /// 0x38
			 0xe67a5fdd, /// 0x3c
			 0x2977be9a, /// 0x40
			 0x1885ea35, /// 0x44
			 0x62583fe9, /// 0x48
			 0x16f4c610, /// 0x4c
			 0x96850a90, /// 0x50
			 0xd87988fc, /// 0x54
			 0x5f138949, /// 0x58
			 0x6f3a64b9, /// 0x5c
			 0xfcf4a0b7, /// 0x60
			 0xe2b73793, /// 0x64
			 0x4eb43863, /// 0x68
			 0xb637a810, /// 0x6c
			 0x50803baf, /// 0x70
			 0x977f1854, /// 0x74
			 0x295bb2f9, /// 0x78
			 0x606433a4, /// 0x7c
			 0x2878622e, /// 0x80
			 0xf5121549, /// 0x84
			 0x6ffc26dd, /// 0x88
			 0x5388ce2d, /// 0x8c
			 0xc7bc9410, /// 0x90
			 0x3140ac5c, /// 0x94
			 0x1a1860b8, /// 0x98
			 0xd05a70cf, /// 0x9c
			 0x3d7e158d, /// 0xa0
			 0x28363c8d, /// 0xa4
			 0xac7bd1fe, /// 0xa8
			 0xfbeb845a, /// 0xac
			 0x0af8ee93, /// 0xb0
			 0xdf8a8936, /// 0xb4
			 0x44a65b84, /// 0xb8
			 0xa9c4221a, /// 0xbc
			 0xb7a62460, /// 0xc0
			 0x624b1f04, /// 0xc4
			 0x038928f3, /// 0xc8
			 0xbcde5147, /// 0xcc
			 0x55d52793, /// 0xd0
			 0xfb356e5e, /// 0xd4
			 0x19f47f8d, /// 0xd8
			 0xadb616cc, /// 0xdc
			 0x073434b3, /// 0xe0
			 0x47bfeeca, /// 0xe4
			 0xa9d6d7bf, /// 0xe8
			 0x98c22810, /// 0xec
			 0x114726ab, /// 0xf0
			 0x8caeea8b, /// 0xf4
			 0x46ff6c57, /// 0xf8
			 0x4c662d9c, /// 0xfc
			 0x23011475, /// 0x100
			 0xa4090d52, /// 0x104
			 0x9b391aba, /// 0x108
			 0xb8e40dec, /// 0x10c
			 0x936cd0e1, /// 0x110
			 0x15c07f86, /// 0x114
			 0xc564b5d7, /// 0x118
			 0xf4ed3a42, /// 0x11c
			 0xda0d87f7, /// 0x120
			 0x6462081a, /// 0x124
			 0xbc403549, /// 0x128
			 0x9214a2db, /// 0x12c
			 0x13dc63b2, /// 0x130
			 0x76c04f7c, /// 0x134
			 0xa0388622, /// 0x138
			 0xa4e99009, /// 0x13c
			 0xe5d87209, /// 0x140
			 0x0534dfef, /// 0x144
			 0x22ae03a2, /// 0x148
			 0xd67e8615, /// 0x14c
			 0xbd7cafe8, /// 0x150
			 0xb7631ff2, /// 0x154
			 0xe8b4c872, /// 0x158
			 0x1c5794a6, /// 0x15c
			 0x7d339de8, /// 0x160
			 0x7946f143, /// 0x164
			 0xafeb3d18, /// 0x168
			 0xc42bb5c1, /// 0x16c
			 0x31503e6a, /// 0x170
			 0x73d1e97e, /// 0x174
			 0x3c2be88a, /// 0x178
			 0x1f434f5b, /// 0x17c
			 0x39deb324, /// 0x180
			 0xa5f80fb4, /// 0x184
			 0x04d3dbc2, /// 0x188
			 0x59451c35, /// 0x18c
			 0x45aaac2d, /// 0x190
			 0x89f7c24a, /// 0x194
			 0xb9208b56, /// 0x198
			 0x28bb2afc, /// 0x19c
			 0x42fc520d, /// 0x1a0
			 0xd7a72c08, /// 0x1a4
			 0x311e1e38, /// 0x1a8
			 0xd537e40e, /// 0x1ac
			 0x28f3828d, /// 0x1b0
			 0x265e8716, /// 0x1b4
			 0x10d04575, /// 0x1b8
			 0x4fe836b5, /// 0x1bc
			 0x852f6d97, /// 0x1c0
			 0xe360d2cb, /// 0x1c4
			 0x79a3a05d, /// 0x1c8
			 0xf2e60899, /// 0x1cc
			 0x16faa42c, /// 0x1d0
			 0x0d86561e, /// 0x1d4
			 0x56d13a44, /// 0x1d8
			 0xef4ea203, /// 0x1dc
			 0xc6f47c2a, /// 0x1e0
			 0xc9a52fa2, /// 0x1e4
			 0x872253f7, /// 0x1e8
			 0x4ad654a0, /// 0x1ec
			 0xf10fcd50, /// 0x1f0
			 0x693c22c2, /// 0x1f4
			 0x7a9b54a3, /// 0x1f8
			 0xd1f16161, /// 0x1fc
			 0x4dd7a837, /// 0x200
			 0x626891f6, /// 0x204
			 0x67851c76, /// 0x208
			 0xa1aae3d4, /// 0x20c
			 0xab15209e, /// 0x210
			 0x33ec0969, /// 0x214
			 0x70a7a24d, /// 0x218
			 0x18ba5e2a, /// 0x21c
			 0x699a1d4a, /// 0x220
			 0x742e0716, /// 0x224
			 0x25f418f4, /// 0x228
			 0xb94f0f7a, /// 0x22c
			 0x9361c0d1, /// 0x230
			 0xf784c02e, /// 0x234
			 0x595f1499, /// 0x238
			 0x3c1fc412, /// 0x23c
			 0x89ac9a79, /// 0x240
			 0xdcdc3689, /// 0x244
			 0x76b7be90, /// 0x248
			 0x5fa1d05a, /// 0x24c
			 0x30a5dd16, /// 0x250
			 0xf02bb31f, /// 0x254
			 0x90cef332, /// 0x258
			 0x86322faf, /// 0x25c
			 0xc74c22e8, /// 0x260
			 0xdf4b1b29, /// 0x264
			 0x845aeda8, /// 0x268
			 0x0ce627ba, /// 0x26c
			 0xca71ed01, /// 0x270
			 0x85130c28, /// 0x274
			 0xe0b0ce7d, /// 0x278
			 0x135c08ca, /// 0x27c
			 0x133d6a3c, /// 0x280
			 0x26b7146b, /// 0x284
			 0xb2baa20d, /// 0x288
			 0xf2598dbd, /// 0x28c
			 0xca77f01a, /// 0x290
			 0x9ca6ec44, /// 0x294
			 0xd5c2bfd7, /// 0x298
			 0x0dede409, /// 0x29c
			 0x5446ef07, /// 0x2a0
			 0xdcbf4ec5, /// 0x2a4
			 0xeca99d04, /// 0x2a8
			 0xb3a7fbcb, /// 0x2ac
			 0x23ed84c7, /// 0x2b0
			 0x73ca61f6, /// 0x2b4
			 0x0d4e44dc, /// 0x2b8
			 0xc136bdf4, /// 0x2bc
			 0x850ceada, /// 0x2c0
			 0xbfe8b7fd, /// 0x2c4
			 0xd66d210d, /// 0x2c8
			 0x294b1310, /// 0x2cc
			 0x21cfc5d7, /// 0x2d0
			 0x40cfc18d, /// 0x2d4
			 0x4217b959, /// 0x2d8
			 0x491fefbd, /// 0x2dc
			 0x51feec47, /// 0x2e0
			 0x4361e62e, /// 0x2e4
			 0xc9d5f611, /// 0x2e8
			 0xe94a5d69, /// 0x2ec
			 0x6d45a64a, /// 0x2f0
			 0xc8010d5f, /// 0x2f4
			 0x307acce0, /// 0x2f8
			 0x83b9d5bf, /// 0x2fc
			 0xda4d26d7, /// 0x300
			 0x0c552f2f, /// 0x304
			 0xbb3a9ff5, /// 0x308
			 0x5ac7ceb4, /// 0x30c
			 0x65bead2b, /// 0x310
			 0x45f13afd, /// 0x314
			 0xd01d23a1, /// 0x318
			 0xa20872b3, /// 0x31c
			 0x4ffb53c3, /// 0x320
			 0x26c65767, /// 0x324
			 0x9619c648, /// 0x328
			 0x61986298, /// 0x32c
			 0x96b883a2, /// 0x330
			 0xb2b3f0d6, /// 0x334
			 0xf9a7b848, /// 0x338
			 0xe30ea0d2, /// 0x33c
			 0x8b613490, /// 0x340
			 0xcb33511c, /// 0x344
			 0xffd5ce18, /// 0x348
			 0xb0bba8f2, /// 0x34c
			 0xe005d03d, /// 0x350
			 0xace3a854, /// 0x354
			 0x1d6e7c58, /// 0x358
			 0x1e534f0f, /// 0x35c
			 0xc0727cfe, /// 0x360
			 0x1193cac8, /// 0x364
			 0x0bbbb5de, /// 0x368
			 0x247b25e0, /// 0x36c
			 0x2119284c, /// 0x370
			 0xb28711c9, /// 0x374
			 0x53e0a5a2, /// 0x378
			 0x34ca0252, /// 0x37c
			 0xe77c930f, /// 0x380
			 0x0556c1c1, /// 0x384
			 0x7b3591db, /// 0x388
			 0x03142d91, /// 0x38c
			 0x99381c93, /// 0x390
			 0xc56d60e4, /// 0x394
			 0x91689f3a, /// 0x398
			 0x40452839, /// 0x39c
			 0x9ce9eaa9, /// 0x3a0
			 0xe691e13c, /// 0x3a4
			 0x7299f63d, /// 0x3a8
			 0x607e5c4f, /// 0x3ac
			 0x42db0137, /// 0x3b0
			 0xd7d56122, /// 0x3b4
			 0x906f9239, /// 0x3b8
			 0xb45cfbd9, /// 0x3bc
			 0x300b8f82, /// 0x3c0
			 0x9117c832, /// 0x3c4
			 0x5fca388e, /// 0x3c8
			 0x5d94f01e, /// 0x3cc
			 0x1ea30e1c, /// 0x3d0
			 0xac95efc1, /// 0x3d4
			 0xf72a1401, /// 0x3d8
			 0x383809ac, /// 0x3dc
			 0xc71107eb, /// 0x3e0
			 0xcf7e8f55, /// 0x3e4
			 0xfbad40a5, /// 0x3e8
			 0xab2586b3, /// 0x3ec
			 0xf1a2b0b3, /// 0x3f0
			 0xe83e9d75, /// 0x3f4
			 0x608e7313, /// 0x3f8
			 0x2fb9013f, /// 0x3fc
			 0xaeb0fd2f, /// 0x400
			 0x5c42fb65, /// 0x404
			 0x58711f47, /// 0x408
			 0xbd95c871, /// 0x40c
			 0x7c7aca25, /// 0x410
			 0xe02f7e50, /// 0x414
			 0xb49fba68, /// 0x418
			 0x7b604faf, /// 0x41c
			 0x7c285e8e, /// 0x420
			 0xbf082d12, /// 0x424
			 0xd54dc055, /// 0x428
			 0x976574e0, /// 0x42c
			 0x59bb8f94, /// 0x430
			 0x0e99c3dd, /// 0x434
			 0x6f88630f, /// 0x438
			 0xef44e974, /// 0x43c
			 0x5c638375, /// 0x440
			 0xeb42c67d, /// 0x444
			 0xd445c8b5, /// 0x448
			 0x3ee90800, /// 0x44c
			 0xf8fef4ea, /// 0x450
			 0x656368ae, /// 0x454
			 0x62edb982, /// 0x458
			 0x9e3b47be, /// 0x45c
			 0x35b7aa21, /// 0x460
			 0x7ba8e7ea, /// 0x464
			 0x4eb03dcf, /// 0x468
			 0xead01e5d, /// 0x46c
			 0xc7b1de3f, /// 0x470
			 0x33aa79c8, /// 0x474
			 0xe349480d, /// 0x478
			 0x99dbfd4a, /// 0x47c
			 0x972b217a, /// 0x480
			 0x93a6260e, /// 0x484
			 0x25b4b6ec, /// 0x488
			 0xe0a393e3, /// 0x48c
			 0x2212c7e4, /// 0x490
			 0x45d66fc8, /// 0x494
			 0x0ceccc4a, /// 0x498
			 0xd745764a, /// 0x49c
			 0x7a6e112b, /// 0x4a0
			 0x6abae66a, /// 0x4a4
			 0xdd0ca86c, /// 0x4a8
			 0xa29eee51, /// 0x4ac
			 0x34fe84b4, /// 0x4b0
			 0xc0365864, /// 0x4b4
			 0xe225a665, /// 0x4b8
			 0xc670cf82, /// 0x4bc
			 0x4734889c, /// 0x4c0
			 0xebf22877, /// 0x4c4
			 0x731fa18e, /// 0x4c8
			 0xa5bb8232, /// 0x4cc
			 0xf3905e80, /// 0x4d0
			 0xd08af27c, /// 0x4d4
			 0x562e5c6f, /// 0x4d8
			 0x22820436, /// 0x4dc
			 0x47b9b126, /// 0x4e0
			 0x3701a736, /// 0x4e4
			 0x42366d62, /// 0x4e8
			 0x42828c2b, /// 0x4ec
			 0xc36c96a6, /// 0x4f0
			 0xe9721122, /// 0x4f4
			 0x3928e932, /// 0x4f8
			 0x3b760919, /// 0x4fc
			 0xc604a0a5, /// 0x500
			 0x8fa9de30, /// 0x504
			 0x6829f470, /// 0x508
			 0xb7d2daee, /// 0x50c
			 0x0cf04c45, /// 0x510
			 0xc3086a11, /// 0x514
			 0x87f2d8c4, /// 0x518
			 0x50ecfe20, /// 0x51c
			 0x9712ae61, /// 0x520
			 0xa855dba9, /// 0x524
			 0xaf52ff3e, /// 0x528
			 0xa415857a, /// 0x52c
			 0x6e4c2d92, /// 0x530
			 0xb27ac79a, /// 0x534
			 0xf55974ec, /// 0x538
			 0xe002c64b, /// 0x53c
			 0xf758e6a4, /// 0x540
			 0x2399ad85, /// 0x544
			 0x9b33dde8, /// 0x548
			 0x695b996f, /// 0x54c
			 0xc6d5374e, /// 0x550
			 0x70215b79, /// 0x554
			 0x9581b330, /// 0x558
			 0x2c924829, /// 0x55c
			 0xbe872e04, /// 0x560
			 0x2ead5727, /// 0x564
			 0x1c3518dd, /// 0x568
			 0xabd1941e, /// 0x56c
			 0xea86cbf3, /// 0x570
			 0xe2c77eb8, /// 0x574
			 0x99aa83b6, /// 0x578
			 0xfc67bfc4, /// 0x57c
			 0x683dcd47, /// 0x580
			 0xcc6a064e, /// 0x584
			 0xe5b4afe5, /// 0x588
			 0x106612e7, /// 0x58c
			 0xfd50077a, /// 0x590
			 0xa3ad5693, /// 0x594
			 0x183a56f0, /// 0x598
			 0x1da5662a, /// 0x59c
			 0x5a4313f6, /// 0x5a0
			 0xf9261b90, /// 0x5a4
			 0x92822a52, /// 0x5a8
			 0x1751a70a, /// 0x5ac
			 0xbd5416be, /// 0x5b0
			 0x290f9fcf, /// 0x5b4
			 0x5bf3c720, /// 0x5b8
			 0x2788ae6a, /// 0x5bc
			 0x8e845d25, /// 0x5c0
			 0xb714e4fc, /// 0x5c4
			 0xbb41a4a7, /// 0x5c8
			 0x587c7050, /// 0x5cc
			 0x2b6e6599, /// 0x5d0
			 0x4933a06b, /// 0x5d4
			 0x1edf0276, /// 0x5d8
			 0x5acacff5, /// 0x5dc
			 0xef1cfc08, /// 0x5e0
			 0x5ec55c59, /// 0x5e4
			 0xc2af356f, /// 0x5e8
			 0x1deff896, /// 0x5ec
			 0x6b334b16, /// 0x5f0
			 0x9921227b, /// 0x5f4
			 0x78017026, /// 0x5f8
			 0x114ad492, /// 0x5fc
			 0xfa0795e7, /// 0x600
			 0x15208e07, /// 0x604
			 0x1b6e7b87, /// 0x608
			 0xb02e3f6c, /// 0x60c
			 0xdcd17c5c, /// 0x610
			 0xad9683ab, /// 0x614
			 0xcd8a3d2f, /// 0x618
			 0x95a221a4, /// 0x61c
			 0x64683a75, /// 0x620
			 0xb11e1f00, /// 0x624
			 0xdfac9d66, /// 0x628
			 0x5dd960eb, /// 0x62c
			 0x5785f756, /// 0x630
			 0xdaca2ba9, /// 0x634
			 0xeb24e4c0, /// 0x638
			 0x1d143cba, /// 0x63c
			 0x32647b23, /// 0x640
			 0xdc701121, /// 0x644
			 0x4f372bd7, /// 0x648
			 0xdbeb6eb6, /// 0x64c
			 0x93dd73d0, /// 0x650
			 0x82bff6bc, /// 0x654
			 0x4037ac89, /// 0x658
			 0x504c8d92, /// 0x65c
			 0x3be41ce8, /// 0x660
			 0xf09dd92f, /// 0x664
			 0xaa935cd3, /// 0x668
			 0x0434506d, /// 0x66c
			 0xf5862162, /// 0x670
			 0x316283ba, /// 0x674
			 0xa4215bb5, /// 0x678
			 0x899b8b56, /// 0x67c
			 0x9eb51be6, /// 0x680
			 0x6b7a1141, /// 0x684
			 0xf9111f6b, /// 0x688
			 0xcb134ba5, /// 0x68c
			 0x613a0bd3, /// 0x690
			 0xbae61885, /// 0x694
			 0x76d7f09d, /// 0x698
			 0xe2979c73, /// 0x69c
			 0x8b73ce1b, /// 0x6a0
			 0xebbb0edf, /// 0x6a4
			 0x3f5b405a, /// 0x6a8
			 0x32a045b4, /// 0x6ac
			 0xa9f920ca, /// 0x6b0
			 0xba90780f, /// 0x6b4
			 0xf2a786e1, /// 0x6b8
			 0x6b66882c, /// 0x6bc
			 0x67b1da72, /// 0x6c0
			 0xc3c797a7, /// 0x6c4
			 0x7db6c93e, /// 0x6c8
			 0x5d7814f5, /// 0x6cc
			 0x8d030d8c, /// 0x6d0
			 0xbcab5515, /// 0x6d4
			 0x53368e89, /// 0x6d8
			 0x8a84e796, /// 0x6dc
			 0xb033ae90, /// 0x6e0
			 0x79371863, /// 0x6e4
			 0x208e622a, /// 0x6e8
			 0xfd80257c, /// 0x6ec
			 0xc518d93d, /// 0x6f0
			 0xe5601d1c, /// 0x6f4
			 0x9a767c7e, /// 0x6f8
			 0xb34067e6, /// 0x6fc
			 0x2eb10c4b, /// 0x700
			 0x5bc775b1, /// 0x704
			 0x367a1153, /// 0x708
			 0x66c1eb99, /// 0x70c
			 0x7e64e714, /// 0x710
			 0xd4d31d48, /// 0x714
			 0x6f92ab7d, /// 0x718
			 0x10c59f87, /// 0x71c
			 0x26e70714, /// 0x720
			 0x3f305649, /// 0x724
			 0x498800ef, /// 0x728
			 0xd7ef8d06, /// 0x72c
			 0x7273e730, /// 0x730
			 0xa686d3c3, /// 0x734
			 0x28e06363, /// 0x738
			 0x5ec97879, /// 0x73c
			 0xa77479be, /// 0x740
			 0x65cb27fc, /// 0x744
			 0x5ca2c962, /// 0x748
			 0x1761c63e, /// 0x74c
			 0x7ac551f9, /// 0x750
			 0xef9acedf, /// 0x754
			 0xb81cea6c, /// 0x758
			 0x9f0cdf65, /// 0x75c
			 0xe46f64a8, /// 0x760
			 0x79a097fd, /// 0x764
			 0x14eb8a05, /// 0x768
			 0xf8b54884, /// 0x76c
			 0x743d65ab, /// 0x770
			 0xa4d3214e, /// 0x774
			 0x1d391b5a, /// 0x778
			 0x59addfca, /// 0x77c
			 0xb7ae7a6d, /// 0x780
			 0x999312ea, /// 0x784
			 0x530ad5a6, /// 0x788
			 0x1327c216, /// 0x78c
			 0x0db32812, /// 0x790
			 0x350ee60c, /// 0x794
			 0x1c026ffd, /// 0x798
			 0xe9a487e4, /// 0x79c
			 0xdbedd5ae, /// 0x7a0
			 0x378372c2, /// 0x7a4
			 0xf090c689, /// 0x7a8
			 0xe62cab3d, /// 0x7ac
			 0x9863e261, /// 0x7b0
			 0xc3c4c0f1, /// 0x7b4
			 0x5ec83f86, /// 0x7b8
			 0xd1f014bd, /// 0x7bc
			 0xd9abb32b, /// 0x7c0
			 0x62a92e4a, /// 0x7c4
			 0xced6624d, /// 0x7c8
			 0xae29f7eb, /// 0x7cc
			 0x4bac4257, /// 0x7d0
			 0xab789b9b, /// 0x7d4
			 0x05f2b1e1, /// 0x7d8
			 0xccd250b6, /// 0x7dc
			 0x7b15da84, /// 0x7e0
			 0xdaed62e4, /// 0x7e4
			 0x8a5428ac, /// 0x7e8
			 0xafa7ab8d, /// 0x7ec
			 0x856c6eb1, /// 0x7f0
			 0x90cd2b55, /// 0x7f4
			 0x6eaafc58, /// 0x7f8
			 0xda747b03, /// 0x7fc
			 0x607652ba, /// 0x800
			 0xe42867d7, /// 0x804
			 0x31b69d33, /// 0x808
			 0xaa90666e, /// 0x80c
			 0x51df64b3, /// 0x810
			 0x473ed9cd, /// 0x814
			 0x4badcce5, /// 0x818
			 0x6d2d35d5, /// 0x81c
			 0xf5606373, /// 0x820
			 0xd960b842, /// 0x824
			 0x99411fb4, /// 0x828
			 0xf4661a58, /// 0x82c
			 0x2d0e034c, /// 0x830
			 0x4de1493a, /// 0x834
			 0x867fde20, /// 0x838
			 0x5a489ff3, /// 0x83c
			 0xa1b21458, /// 0x840
			 0x39147e04, /// 0x844
			 0x3e6cf0cd, /// 0x848
			 0x4de87431, /// 0x84c
			 0x9764742e, /// 0x850
			 0xfebb8b24, /// 0x854
			 0x49de0cf0, /// 0x858
			 0x38d0d9cc, /// 0x85c
			 0x20a9bd0f, /// 0x860
			 0x939d2787, /// 0x864
			 0x7b320acd, /// 0x868
			 0xf2af4d56, /// 0x86c
			 0xe2c8a752, /// 0x870
			 0xf95128dc, /// 0x874
			 0xbba52b87, /// 0x878
			 0xcd3fbc2b, /// 0x87c
			 0x7cd5f35d, /// 0x880
			 0x884fe24e, /// 0x884
			 0x8eda446c, /// 0x888
			 0x1edcd759, /// 0x88c
			 0xa06c177b, /// 0x890
			 0x81afdf85, /// 0x894
			 0xdf64d3dd, /// 0x898
			 0x0c355a8a, /// 0x89c
			 0xeecaff0c, /// 0x8a0
			 0x7792a821, /// 0x8a4
			 0xbdfc6773, /// 0x8a8
			 0x72562f16, /// 0x8ac
			 0xf429c740, /// 0x8b0
			 0x1f61a5bc, /// 0x8b4
			 0x4c191ac5, /// 0x8b8
			 0x04d38079, /// 0x8bc
			 0xa049c1ea, /// 0x8c0
			 0x76eb8830, /// 0x8c4
			 0x77fe36fa, /// 0x8c8
			 0x229430d8, /// 0x8cc
			 0xbdde8df0, /// 0x8d0
			 0x0ab47bcb, /// 0x8d4
			 0x6e4ed454, /// 0x8d8
			 0xdd655733, /// 0x8dc
			 0x3a168f7d, /// 0x8e0
			 0x56dc643b, /// 0x8e4
			 0x6a0de104, /// 0x8e8
			 0xc5c1b7f2, /// 0x8ec
			 0xb7e19950, /// 0x8f0
			 0x37437272, /// 0x8f4
			 0xf592e3b8, /// 0x8f8
			 0xc510d52f, /// 0x8fc
			 0x91f66e56, /// 0x900
			 0xf8ad4515, /// 0x904
			 0xe9e6e542, /// 0x908
			 0x19ac02ee, /// 0x90c
			 0x4a1ccf92, /// 0x910
			 0x0ab1701b, /// 0x914
			 0xc37296a8, /// 0x918
			 0x6fb25ec4, /// 0x91c
			 0x819eb709, /// 0x920
			 0x28ff3f71, /// 0x924
			 0x5e64b826, /// 0x928
			 0x3ea30ee1, /// 0x92c
			 0xbd7c948b, /// 0x930
			 0x2f765624, /// 0x934
			 0x695ddd6a, /// 0x938
			 0x038a66a4, /// 0x93c
			 0x91079a9b, /// 0x940
			 0x4f5c0af7, /// 0x944
			 0x81f1c94e, /// 0x948
			 0x19d95b5a, /// 0x94c
			 0xecd0bbd8, /// 0x950
			 0x137a56ae, /// 0x954
			 0x5103f4b4, /// 0x958
			 0x2e7835f3, /// 0x95c
			 0x6baceff7, /// 0x960
			 0x70210e12, /// 0x964
			 0x2be867ee, /// 0x968
			 0x4fa0cb58, /// 0x96c
			 0xf9cd2008, /// 0x970
			 0xf5faddc0, /// 0x974
			 0x63266486, /// 0x978
			 0x1c979394, /// 0x97c
			 0x5a4bf356, /// 0x980
			 0x3351f57d, /// 0x984
			 0x458efb48, /// 0x988
			 0x4f21d1a7, /// 0x98c
			 0xdb8d08e7, /// 0x990
			 0x8c4e6489, /// 0x994
			 0x1595b519, /// 0x998
			 0xe84f9f6a, /// 0x99c
			 0xde529d2d, /// 0x9a0
			 0x097e77b0, /// 0x9a4
			 0x5987d48a, /// 0x9a8
			 0x87ddb296, /// 0x9ac
			 0x1cc73847, /// 0x9b0
			 0x9dff4862, /// 0x9b4
			 0x06387b56, /// 0x9b8
			 0x6678c610, /// 0x9bc
			 0xe7411883, /// 0x9c0
			 0x870b327f, /// 0x9c4
			 0x22abff7b, /// 0x9c8
			 0x2deffda2, /// 0x9cc
			 0xb1267447, /// 0x9d0
			 0xc1730d0a, /// 0x9d4
			 0x05de136d, /// 0x9d8
			 0xfbc1b634, /// 0x9dc
			 0xac01920d, /// 0x9e0
			 0x133ddbd4, /// 0x9e4
			 0xaa1b95f5, /// 0x9e8
			 0x13d09405, /// 0x9ec
			 0xc41070fd, /// 0x9f0
			 0x5a0543c6, /// 0x9f4
			 0xe3b172d7, /// 0x9f8
			 0xff0e645f, /// 0x9fc
			 0xe4c019f8, /// 0xa00
			 0xa1e0a2c3, /// 0xa04
			 0xd41e71ce, /// 0xa08
			 0x0a775c69, /// 0xa0c
			 0x46aae05e, /// 0xa10
			 0xda6f51c2, /// 0xa14
			 0x723c82a6, /// 0xa18
			 0x63cceb15, /// 0xa1c
			 0x71e51da2, /// 0xa20
			 0x92328484, /// 0xa24
			 0x2ff76538, /// 0xa28
			 0xdbc384bc, /// 0xa2c
			 0x53c8fc53, /// 0xa30
			 0x7943fb10, /// 0xa34
			 0xa8fd3adc, /// 0xa38
			 0xce9d5058, /// 0xa3c
			 0x9e53ac21, /// 0xa40
			 0x2e9e1f7d, /// 0xa44
			 0x6e67e90c, /// 0xa48
			 0x84f08800, /// 0xa4c
			 0x72eecf4c, /// 0xa50
			 0xdde7260e, /// 0xa54
			 0xf099e45c, /// 0xa58
			 0xdbf75692, /// 0xa5c
			 0xf34958df, /// 0xa60
			 0x90357021, /// 0xa64
			 0x25cd3b4a, /// 0xa68
			 0x96c15bee, /// 0xa6c
			 0x469b71e4, /// 0xa70
			 0x06eb54df, /// 0xa74
			 0xed5f56b9, /// 0xa78
			 0xc841d0e1, /// 0xa7c
			 0x93ad7d48, /// 0xa80
			 0x13b83ab4, /// 0xa84
			 0xfabecbef, /// 0xa88
			 0x1d849710, /// 0xa8c
			 0xa717bde1, /// 0xa90
			 0x1487993d, /// 0xa94
			 0xd6313eb0, /// 0xa98
			 0x2bcc9b94, /// 0xa9c
			 0x09be3efa, /// 0xaa0
			 0xbc7188f5, /// 0xaa4
			 0x411f068f, /// 0xaa8
			 0x70c48c04, /// 0xaac
			 0x46b1c181, /// 0xab0
			 0xfb9a7386, /// 0xab4
			 0x4f259c22, /// 0xab8
			 0x4bd6dea2, /// 0xabc
			 0xb85dca72, /// 0xac0
			 0x97a3a76a, /// 0xac4
			 0x6a6d39b4, /// 0xac8
			 0xab81a51e, /// 0xacc
			 0x3ba30a97, /// 0xad0
			 0x501a31ce, /// 0xad4
			 0x1a09d381, /// 0xad8
			 0x70c37000, /// 0xadc
			 0xeb6f4de7, /// 0xae0
			 0x57ba8bc4, /// 0xae4
			 0x0a241ed0, /// 0xae8
			 0x638f3db6, /// 0xaec
			 0x98f39e7a, /// 0xaf0
			 0xea98ef3c, /// 0xaf4
			 0xc013c9c7, /// 0xaf8
			 0xde5ac7eb, /// 0xafc
			 0x94c10f58, /// 0xb00
			 0xf34d8fad, /// 0xb04
			 0x642844ee, /// 0xb08
			 0x2b1a33ba, /// 0xb0c
			 0xadb4df33, /// 0xb10
			 0x1b8ebebe, /// 0xb14
			 0x1c47c638, /// 0xb18
			 0x40569be3, /// 0xb1c
			 0x7ba40e30, /// 0xb20
			 0xadec7803, /// 0xb24
			 0x20572e52, /// 0xb28
			 0x3a1d6319, /// 0xb2c
			 0x1aeb26a4, /// 0xb30
			 0x9a1ae204, /// 0xb34
			 0xf9167815, /// 0xb38
			 0xcaf25b0d, /// 0xb3c
			 0x37ddee98, /// 0xb40
			 0xa13002bb, /// 0xb44
			 0x311e2637, /// 0xb48
			 0xdd2d3b8f, /// 0xb4c
			 0x01c823c7, /// 0xb50
			 0x9fef7fa0, /// 0xb54
			 0xbfa00b27, /// 0xb58
			 0x8707a0d3, /// 0xb5c
			 0x8f41552b, /// 0xb60
			 0x3418036e, /// 0xb64
			 0x8c297376, /// 0xb68
			 0x5d8ba3f4, /// 0xb6c
			 0x5390b9ab, /// 0xb70
			 0x71239ea9, /// 0xb74
			 0xd9acbe3d, /// 0xb78
			 0x9f8b0fb1, /// 0xb7c
			 0x3e4b81b9, /// 0xb80
			 0x978da823, /// 0xb84
			 0x252ff16c, /// 0xb88
			 0x844f0945, /// 0xb8c
			 0x77e23308, /// 0xb90
			 0xe277f378, /// 0xb94
			 0x7f3f00c0, /// 0xb98
			 0x14c8d4bb, /// 0xb9c
			 0xe31e3d91, /// 0xba0
			 0x3385ad4f, /// 0xba4
			 0x73c07b27, /// 0xba8
			 0x058d6713, /// 0xbac
			 0x23fac169, /// 0xbb0
			 0x59cda0ca, /// 0xbb4
			 0x6b33a1b9, /// 0xbb8
			 0xdb6f454c, /// 0xbbc
			 0xe04ff8ed, /// 0xbc0
			 0x3a66c1c4, /// 0xbc4
			 0x9f88c73b, /// 0xbc8
			 0x8b726eea, /// 0xbcc
			 0x33cd8294, /// 0xbd0
			 0xae9feac5, /// 0xbd4
			 0xa943ac1d, /// 0xbd8
			 0xcba93c36, /// 0xbdc
			 0x16b03098, /// 0xbe0
			 0x4af8c3e3, /// 0xbe4
			 0xe847058a, /// 0xbe8
			 0x9333fc1f, /// 0xbec
			 0x4108386e, /// 0xbf0
			 0x76f8450a, /// 0xbf4
			 0x964222dc, /// 0xbf8
			 0x7192b5a5, /// 0xbfc
			 0x2281df8b, /// 0xc00
			 0xf1165893, /// 0xc04
			 0x7d8a0f94, /// 0xc08
			 0x5e665dfa, /// 0xc0c
			 0xe672ca75, /// 0xc10
			 0x94e9c725, /// 0xc14
			 0xc118bb05, /// 0xc18
			 0xf375a9d6, /// 0xc1c
			 0x7a9a9779, /// 0xc20
			 0xeed27e32, /// 0xc24
			 0xe04897b6, /// 0xc28
			 0x31f22205, /// 0xc2c
			 0x1cb7fb3e, /// 0xc30
			 0xbd425e8b, /// 0xc34
			 0x0b2aa3b7, /// 0xc38
			 0x76f327e6, /// 0xc3c
			 0x45a8b4c7, /// 0xc40
			 0xa7e1c575, /// 0xc44
			 0x5f85dd4c, /// 0xc48
			 0x4b06f725, /// 0xc4c
			 0xf582f30a, /// 0xc50
			 0x780c03fb, /// 0xc54
			 0x372f5fa4, /// 0xc58
			 0x6a1f4694, /// 0xc5c
			 0x48a373dc, /// 0xc60
			 0xf25780ab, /// 0xc64
			 0x4d58f998, /// 0xc68
			 0xb79e8d8b, /// 0xc6c
			 0x0e087bc4, /// 0xc70
			 0x99b488ad, /// 0xc74
			 0x493cafe8, /// 0xc78
			 0xe7f372f9, /// 0xc7c
			 0xee5bffd3, /// 0xc80
			 0x626b89c5, /// 0xc84
			 0x8a76de7e, /// 0xc88
			 0xb1bee4af, /// 0xc8c
			 0x85aa98c7, /// 0xc90
			 0xd9c4d602, /// 0xc94
			 0x9827da68, /// 0xc98
			 0x95fc31c9, /// 0xc9c
			 0x2e374ba8, /// 0xca0
			 0x08864fc0, /// 0xca4
			 0x762ffa9c, /// 0xca8
			 0xbe84956d, /// 0xcac
			 0xa321feab, /// 0xcb0
			 0x56103d85, /// 0xcb4
			 0xecb6115c, /// 0xcb8
			 0x11dfb476, /// 0xcbc
			 0x9a445215, /// 0xcc0
			 0x75524abd, /// 0xcc4
			 0x365acea8, /// 0xcc8
			 0xeadc90a2, /// 0xccc
			 0x90615288, /// 0xcd0
			 0xc346f73d, /// 0xcd4
			 0x76e3843f, /// 0xcd8
			 0x5b8c69cb, /// 0xcdc
			 0x5feec01f, /// 0xce0
			 0x20ab3b67, /// 0xce4
			 0xa1add396, /// 0xce8
			 0x06267370, /// 0xcec
			 0x9a1767fe, /// 0xcf0
			 0x3bc5500f, /// 0xcf4
			 0x4d31c855, /// 0xcf8
			 0x6514de61, /// 0xcfc
			 0x85ef8bd0, /// 0xd00
			 0xe515450a, /// 0xd04
			 0x431d808b, /// 0xd08
			 0x91b9445b, /// 0xd0c
			 0x94c455f7, /// 0xd10
			 0xf3d41b46, /// 0xd14
			 0xfd5bdc1d, /// 0xd18
			 0x854604ba, /// 0xd1c
			 0x604bb758, /// 0xd20
			 0x56d53497, /// 0xd24
			 0xf04d4ae3, /// 0xd28
			 0x260d4f8f, /// 0xd2c
			 0xac4f0321, /// 0xd30
			 0xa8e64e78, /// 0xd34
			 0x360da40d, /// 0xd38
			 0xcad8b20f, /// 0xd3c
			 0x4f26dd74, /// 0xd40
			 0xfcddeb60, /// 0xd44
			 0x385c1bbf, /// 0xd48
			 0x1c064392, /// 0xd4c
			 0x3d6115db, /// 0xd50
			 0x094ccd49, /// 0xd54
			 0x499f83fa, /// 0xd58
			 0xa6f90fde, /// 0xd5c
			 0xc4bc23f1, /// 0xd60
			 0x22668397, /// 0xd64
			 0x9bf42e02, /// 0xd68
			 0xfdb07b2c, /// 0xd6c
			 0x7d68f82c, /// 0xd70
			 0x38058a7e, /// 0xd74
			 0x8f4166ac, /// 0xd78
			 0xe3ff07c0, /// 0xd7c
			 0x65067fea, /// 0xd80
			 0x43c038f7, /// 0xd84
			 0x211e9d61, /// 0xd88
			 0x1b6754e4, /// 0xd8c
			 0xbb9ca102, /// 0xd90
			 0x9e45273f, /// 0xd94
			 0xb63c8a57, /// 0xd98
			 0x3f7f1936, /// 0xd9c
			 0x6f57f22d, /// 0xda0
			 0xae4d6975, /// 0xda4
			 0x9554fd89, /// 0xda8
			 0xc63d9e1d, /// 0xdac
			 0x97917a85, /// 0xdb0
			 0x19ac49a0, /// 0xdb4
			 0x365e0742, /// 0xdb8
			 0x54a4b518, /// 0xdbc
			 0xed72baa6, /// 0xdc0
			 0x926a0f0e, /// 0xdc4
			 0x5a3e2452, /// 0xdc8
			 0x689b3f76, /// 0xdcc
			 0x7fabc11a, /// 0xdd0
			 0x481d2fad, /// 0xdd4
			 0xe556feda, /// 0xdd8
			 0x0a596b7d, /// 0xddc
			 0x8af9e0c0, /// 0xde0
			 0x9b3eadf5, /// 0xde4
			 0x5640db56, /// 0xde8
			 0xe7b51615, /// 0xdec
			 0x70e264ca, /// 0xdf0
			 0x1bd37ceb, /// 0xdf4
			 0xf1264484, /// 0xdf8
			 0x858e3fcd, /// 0xdfc
			 0x5c573cf4, /// 0xe00
			 0x9d2ffec1, /// 0xe04
			 0xf7bbf1d4, /// 0xe08
			 0xeb2076b1, /// 0xe0c
			 0xbf2c95ef, /// 0xe10
			 0xafdd5886, /// 0xe14
			 0xba6fc881, /// 0xe18
			 0xe77c93df, /// 0xe1c
			 0x18387b84, /// 0xe20
			 0x0fe7e412, /// 0xe24
			 0x2fe8858f, /// 0xe28
			 0xf460cc64, /// 0xe2c
			 0x242cc4be, /// 0xe30
			 0x7bde03af, /// 0xe34
			 0x86d3f54d, /// 0xe38
			 0xfcfdb46d, /// 0xe3c
			 0x6a5a2bee, /// 0xe40
			 0x02ac7ea0, /// 0xe44
			 0x54937dee, /// 0xe48
			 0x90cffd46, /// 0xe4c
			 0x6de2f240, /// 0xe50
			 0x6228e2b5, /// 0xe54
			 0x6d6cf49f, /// 0xe58
			 0x62f3fcbc, /// 0xe5c
			 0x2f866462, /// 0xe60
			 0xce72c66c, /// 0xe64
			 0xbac5cc04, /// 0xe68
			 0x439a623e, /// 0xe6c
			 0x6e54c265, /// 0xe70
			 0x73547583, /// 0xe74
			 0x5abf4e28, /// 0xe78
			 0x04701ed5, /// 0xe7c
			 0x5a961397, /// 0xe80
			 0xab340e60, /// 0xe84
			 0xeb92a129, /// 0xe88
			 0xf07e942a, /// 0xe8c
			 0x429a1ceb, /// 0xe90
			 0x792bfd55, /// 0xe94
			 0xbf84e5ba, /// 0xe98
			 0xaa60df09, /// 0xe9c
			 0x4303a4be, /// 0xea0
			 0xd0f70547, /// 0xea4
			 0xf7de89f0, /// 0xea8
			 0x88d501d2, /// 0xeac
			 0xde64158c, /// 0xeb0
			 0xfb82747f, /// 0xeb4
			 0x4b24bc29, /// 0xeb8
			 0xd064c9a0, /// 0xebc
			 0x742882c8, /// 0xec0
			 0xde6f1645, /// 0xec4
			 0x7d1453f5, /// 0xec8
			 0xa7019321, /// 0xecc
			 0x5ede1451, /// 0xed0
			 0xcf4089d5, /// 0xed4
			 0x80813faf, /// 0xed8
			 0x06450d77, /// 0xedc
			 0x6b9b4ffe, /// 0xee0
			 0x035dc9be, /// 0xee4
			 0x084ee727, /// 0xee8
			 0x6c6b3a50, /// 0xeec
			 0x5ac466a4, /// 0xef0
			 0x6227eb6a, /// 0xef4
			 0x3e63f206, /// 0xef8
			 0xabc062d5, /// 0xefc
			 0xd6476ca9, /// 0xf00
			 0x0290575f, /// 0xf04
			 0xb74ba501, /// 0xf08
			 0x19bc4a5f, /// 0xf0c
			 0x403b6eff, /// 0xf10
			 0x5f241f59, /// 0xf14
			 0xdf1ab68c, /// 0xf18
			 0xcc7aa8e3, /// 0xf1c
			 0x820cb6f3, /// 0xf20
			 0x3c4d7dd5, /// 0xf24
			 0x20b65e19, /// 0xf28
			 0x57a6e80f, /// 0xf2c
			 0x8bfb1877, /// 0xf30
			 0xbaec76bb, /// 0xf34
			 0x64694ecb, /// 0xf38
			 0x1012a6f4, /// 0xf3c
			 0x12d1897b, /// 0xf40
			 0x5fae51e6, /// 0xf44
			 0x082ee87a, /// 0xf48
			 0x7dbf555b, /// 0xf4c
			 0xed809167, /// 0xf50
			 0x6928173e, /// 0xf54
			 0x05e5bc52, /// 0xf58
			 0x90d1cc3f, /// 0xf5c
			 0xaae2725b, /// 0xf60
			 0x2ad1eb7b, /// 0xf64
			 0xb0db2cbe, /// 0xf68
			 0x11dd6536, /// 0xf6c
			 0x060a1e62, /// 0xf70
			 0xac6eddb8, /// 0xf74
			 0x618ab016, /// 0xf78
			 0x73f34ccc, /// 0xf7c
			 0xb746b1d0, /// 0xf80
			 0xbf8cd504, /// 0xf84
			 0xb672eee5, /// 0xf88
			 0x823849e2, /// 0xf8c
			 0x08c893a2, /// 0xf90
			 0xac70b4ce, /// 0xf94
			 0xaf2b2f58, /// 0xf98
			 0x4f118cc9, /// 0xf9c
			 0x6e292f2a, /// 0xfa0
			 0x352d2394, /// 0xfa4
			 0x8d2918db, /// 0xfa8
			 0xdce8a2b9, /// 0xfac
			 0xa881b6e1, /// 0xfb0
			 0x3a6c6c8b, /// 0xfb4
			 0x18a03af2, /// 0xfb8
			 0x7988267e, /// 0xfbc
			 0xbadc69fa, /// 0xfc0
			 0x87effbba, /// 0xfc4
			 0x5078309c, /// 0xfc8
			 0x04b851e3, /// 0xfcc
			 0xd4d9e834, /// 0xfd0
			 0x114cc77d, /// 0xfd4
			 0x8bda3202, /// 0xfd8
			 0x84311fcd, /// 0xfdc
			 0xcd913a59, /// 0xfe0
			 0x975222cf, /// 0xfe4
			 0x0acf1617, /// 0xfe8
			 0x1fa6dbe5, /// 0xfec
			 0xf72f8e2a, /// 0xff0
			 0xde8472b9, /// 0xff4
			 0xb180d50c, /// 0xff8
			 0x4fbf55c5 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00000
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00004
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000000,                                                  // zero                                        /// 0000c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00014
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00018
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00024
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00028
			 0x0c780000,                                                  // Leading 1s:                                 /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00038
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0003c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00040
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00044
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00048
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0004c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0005c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00064
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00068
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00070
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00074
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00088
			 0xbf028f5c,                                                  // -0.51                                       /// 0008c
			 0x3f028f5c,                                                  // 0.51                                        /// 00090
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00098
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0009c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a0
			 0x00000000,                                                  // zero                                        /// 000a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xcb000000,                                                  // -8388608.0                                  /// 000f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0xff800000,                                                  // -inf                                        /// 00100
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00110
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0c780000,                                                  // Leading 1s:                                 /// 00118
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0011c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00120
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00124
			 0x55555555,                                                  // 4 random values                             /// 00128
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0012c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00130
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00134
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00138
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0013c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00144
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0014c
			 0xcb000000,                                                  // -8388608.0                                  /// 00150
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00154
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00158
			 0x0c780000,                                                  // Leading 1s:                                 /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00160
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00164
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00168
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00170
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00178
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00180
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0018c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00190
			 0x80011111,                                                  // -9.7958E-41                                 /// 00194
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00198
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0019c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000000,                                                  // -zero                                       /// 001b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00000000,                                                  // zero                                        /// 001cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000000,                                                  // -zero                                       /// 001d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00210
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00224
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00228
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0022c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00230
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x4b000000,                                                  // 8388608.0                                   /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00240
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00244
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00250
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00254
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00258
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x7f800000,                                                  // inf                                         /// 00264
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00268
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0026c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00274
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0027c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00280
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00284
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0028c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00294
			 0x4b000000,                                                  // 8388608.0                                   /// 00298
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00304
			 0x3f028f5c,                                                  // 0.51                                        /// 00308
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0030c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00310
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00314
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00318
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00324
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00328
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0032c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00330
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00338
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0033c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00344
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00354
			 0xbf028f5c,                                                  // -0.51                                       /// 00358
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00360
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00364
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00368
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0037c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00380
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00388
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0038c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00398
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0039c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d8
			 0x7f800000,                                                  // inf                                         /// 003dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x3f028f5c,                                                  // 0.51                                        /// 003f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00400
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00404
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00408
			 0x7f800000,                                                  // inf                                         /// 0040c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00418
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0041c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00420
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00424
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00428
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00430
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00438
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0043c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0044c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00450
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00454
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00458
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00464
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0046c
			 0x7fc00001,                                                  // signaling NaN                               /// 00470
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00480
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0048c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00498
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0049c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00500
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00504
			 0x3f028f5c,                                                  // 0.51                                        /// 00508
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00000000,                                                  // zero                                        /// 00510
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00518
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0051c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00524
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x3f028f5c,                                                  // 0.51                                        /// 00538
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00544
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00548
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0054c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00550
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00554
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00558
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0055c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x80000000,                                                  // -zero                                       /// 00564
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00568
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00570
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00580
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00584
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00588
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00590
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00594
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00598
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00604
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00608
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0060c
			 0xbf028f5c,                                                  // -0.51                                       /// 00610
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00614
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00618
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x4b000000,                                                  // 8388608.0                                   /// 00620
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00624
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0062c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00634
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xcb000000,                                                  // -8388608.0                                  /// 0063c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00640
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00644
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00648
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x4b000000,                                                  // 8388608.0                                   /// 00650
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00654
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00658
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00660
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00664
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00674
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00678
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0067c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00684
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00688
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0068c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00694
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0069c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006e8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00700
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00708
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0070c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00710
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0071c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00720
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00730
			 0x4b000000,                                                  // 8388608.0                                   /// 00734
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0073c
			 0x80000000,                                                  // -zero                                       /// 00740
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00748
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00760
			 0x80000000,                                                  // -zero                                       /// 00764
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00768
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0e000003,                                                  // Trailing 1s:                                /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00778
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0078c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00794
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00798
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007e8
			 0x4b000000,                                                  // 8388608.0                                   /// 007ec
			 0x33333333,                                                  // 4 random values                             /// 007f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00800
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00808
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00810
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00814
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00818
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0081c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00824
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0082c
			 0x33333333,                                                  // 4 random values                             /// 00830
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00834
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00838
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0083c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00840
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00844
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00848
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00858
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00864
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00868
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0086c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00870
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00878
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00880
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00884
			 0x33333333,                                                  // 4 random values                             /// 00888
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x33333333,                                                  // 4 random values                             /// 00894
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0089c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d8
			 0x7f800000,                                                  // inf                                         /// 008dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xff800000,                                                  // -inf                                        /// 008f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00900
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00904
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00908
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00920
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00924
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00934
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00938
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00940
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00948
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0094c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x0e000007,                                                  // Trailing 1s:                                /// 00954
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00958
			 0x33333333,                                                  // 4 random values                             /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00968
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0096c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00970
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00978
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0097c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00980
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0e000007,                                                  // Trailing 1s:                                /// 00988
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0098c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00990
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00994
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009c0
			 0x55555555,                                                  // 4 random values                             /// 009c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0xff800000,                                                  // -inf                                        /// 009d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ab8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00acc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00adc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00af4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00af8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00afc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b8c
			 0x33333333,                                                  // 4 random values                             /// 00b90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0xff800000,                                                  // -inf                                        /// 00bb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bcc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be4
			 0x00000000,                                                  // zero                                        /// 00be8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bfc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c84
			 0x3f028f5c,                                                  // 0.51                                        /// 00c88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ce4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cf0
			 0x55555555,                                                  // 4 random values                             /// 00cf4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d84
			 0x55555555,                                                  // 4 random values                             /// 00d88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00db4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dc0
			 0x7f800000,                                                  // inf                                         /// 00dc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dd4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00df0
			 0xcb000000,                                                  // -8388608.0                                  /// 00df4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e34
			 0xbf028f5c,                                                  // -0.51                                       /// 00e38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e60
			 0xcb000000,                                                  // -8388608.0                                  /// 00e64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e84
			 0x7f800000,                                                  // inf                                         /// 00e88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ebc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00efc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000000,                                                  // -zero                                       /// 00f30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f64
			 0x7fc00001,                                                  // signaling NaN                               /// 00f68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f70
			 0x3f028f5c,                                                  // 0.51                                        /// 00f74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x7f800000,                                                  // inf                                         /// 00fc8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff8
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
			 0x00000120,
			 0x000005e0,
			 0x000005d8,
			 0x00000454,
			 0x00000400,
			 0x00000064,
			 0x00000724,
			 0x000003b8,

			 /// vpu register f2
			 0x3f800000,
			 0x41c00000,
			 0x41c80000,
			 0x41100000,
			 0x41d80000,
			 0x40a00000,
			 0x41a00000,
			 0x41800000,

			 /// vpu register f3
			 0x41f00000,
			 0x41c00000,
			 0x3f800000,
			 0x41300000,
			 0x41e00000,
			 0x41900000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f4
			 0x41b00000,
			 0x40400000,
			 0x40000000,
			 0x40800000,
			 0x41d80000,
			 0x41800000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f5
			 0x40000000,
			 0x40c00000,
			 0x41f80000,
			 0x41c80000,
			 0x41200000,
			 0x3f800000,
			 0x41500000,
			 0x41000000,

			 /// vpu register f6
			 0x41700000,
			 0x41f80000,
			 0x41880000,
			 0x40800000,
			 0x41a80000,
			 0x41500000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f7
			 0x41b00000,
			 0x40800000,
			 0x41c00000,
			 0x41c00000,
			 0x41600000,
			 0x3f800000,
			 0x41b80000,
			 0x41c00000,

			 /// vpu register f8
			 0x40e00000,
			 0x41000000,
			 0x41880000,
			 0x40e00000,
			 0x40e00000,
			 0x41700000,
			 0x41000000,
			 0x41200000,

			 /// vpu register f9
			 0x41e80000,
			 0x41300000,
			 0x40a00000,
			 0x41f00000,
			 0x41980000,
			 0x41a00000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f10
			 0x41100000,
			 0x40a00000,
			 0x41c80000,
			 0x41c80000,
			 0x41200000,
			 0x41200000,
			 0x41400000,
			 0x41100000,

			 /// vpu register f11
			 0x3f800000,
			 0x41f80000,
			 0x41700000,
			 0x41a00000,
			 0x41200000,
			 0x40400000,
			 0x41500000,
			 0x41d80000,

			 /// vpu register f12
			 0x41880000,
			 0x41200000,
			 0x41b00000,
			 0x41600000,
			 0x41600000,
			 0x42000000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f13
			 0x41d00000,
			 0x40c00000,
			 0x41600000,
			 0x41300000,
			 0x40c00000,
			 0x41000000,
			 0x42000000,
			 0x41e00000,

			 /// vpu register f14
			 0x3f800000,
			 0x41500000,
			 0x41000000,
			 0x41d00000,
			 0x40e00000,
			 0x40800000,
			 0x42000000,
			 0x41200000,

			 /// vpu register f15
			 0x41700000,
			 0x41e00000,
			 0x41200000,
			 0x41200000,
			 0x41f00000,
			 0x41300000,
			 0x41980000,
			 0x41900000,

			 /// vpu register f16
			 0x41600000,
			 0x3f800000,
			 0x42000000,
			 0x41d80000,
			 0x41800000,
			 0x41700000,
			 0x41d80000,
			 0x41c00000,

			 /// vpu register f17
			 0x41800000,
			 0x41100000,
			 0x41c80000,
			 0x42000000,
			 0x41e80000,
			 0x41a80000,
			 0x41800000,
			 0x41a80000,

			 /// vpu register f18
			 0x41f80000,
			 0x42000000,
			 0x41f80000,
			 0x41200000,
			 0x41a80000,
			 0x40800000,
			 0x40800000,
			 0x41b00000,

			 /// vpu register f19
			 0x41300000,
			 0x40c00000,
			 0x41a80000,
			 0x3f800000,
			 0x41900000,
			 0x41400000,
			 0x41c80000,
			 0x41d80000,

			 /// vpu register f20
			 0x41e80000,
			 0x41e00000,
			 0x40400000,
			 0x42000000,
			 0x40e00000,
			 0x40000000,
			 0x41600000,
			 0x41c00000,

			 /// vpu register f21
			 0x41e80000,
			 0x41600000,
			 0x41300000,
			 0x40000000,
			 0x41b00000,
			 0x41300000,
			 0x40c00000,
			 0x41600000,

			 /// vpu register f22
			 0x41800000,
			 0x41880000,
			 0x41e80000,
			 0x41a00000,
			 0x41d00000,
			 0x41b80000,
			 0x40c00000,
			 0x41f80000,

			 /// vpu register f23
			 0x41000000,
			 0x40400000,
			 0x41b00000,
			 0x41000000,
			 0x41c80000,
			 0x41100000,
			 0x41a80000,
			 0x41980000,

			 /// vpu register f24
			 0x41980000,
			 0x41b80000,
			 0x41a80000,
			 0x41a00000,
			 0x41f80000,
			 0x41200000,
			 0x41d80000,
			 0x41e00000,

			 /// vpu register f25
			 0x41880000,
			 0x40400000,
			 0x41880000,
			 0x41f00000,
			 0x41980000,
			 0x41c80000,
			 0x41f80000,
			 0x41100000,

			 /// vpu register f26
			 0x41b80000,
			 0x40400000,
			 0x40a00000,
			 0x41600000,
			 0x40800000,
			 0x41700000,
			 0x41c00000,
			 0x41a00000,

			 /// vpu register f27
			 0x40800000,
			 0x40a00000,
			 0x40000000,
			 0x41900000,
			 0x41b00000,
			 0x41980000,
			 0x41d80000,
			 0x41f80000,

			 /// vpu register f28
			 0x41200000,
			 0x41100000,
			 0x41800000,
			 0x41900000,
			 0x41b00000,
			 0x41600000,
			 0x41c00000,
			 0x41d00000,

			 /// vpu register f29
			 0x41980000,
			 0x41900000,
			 0x41c80000,
			 0x3f800000,
			 0x40a00000,
			 0x40800000,
			 0x41600000,
			 0x41500000,

			 /// vpu register f30
			 0x41400000,
			 0x41f80000,
			 0x40000000,
			 0x40e00000,
			 0x41800000,
			 0x42000000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f31
			 0x41c00000,
			 0x41c80000,
			 0x41980000,
			 0x40e00000,
			 0x3f800000,
			 0x41a00000,
			 0x41c00000,
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
		"fcvt.f10.ps f13, f12\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f5, f16\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f29, f1\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f26, f0\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f4, f19\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f28, f22\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f12\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f22, f27\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f15, f9\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f17, f10\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f5, f25\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f11, f0\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f12, f9\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f22, f6\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f11, f23\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f10, f2\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f9, f9\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f24, f5\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f10, f0\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f16, f20\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f13, f27\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f17, f26\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f6, f10\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f12, f18\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f7, f13\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f3, f29\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f15, f15\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f30, f30\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f8, f18\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f22, f28\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f9, f17\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f25, f15\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f25, f12\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f3, f29\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f25, f28\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f14, f22\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f30, f16\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f18, f10\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f7, f8\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f10, f6\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f11\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f16, f19\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f16, f15\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f24, f4\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f5, f30\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f9, f29\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f26, f19\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f29, f4\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f15, f6\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f8, f30\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f22\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f15\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f18, f23\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f12, f26\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f24, f20\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f7, f5\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f19, f13\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f12, f10\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f26, f0\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f10, f19\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f30, f24\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f9, f21\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f12, f30\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f22\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f19, f3\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f8\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f8, f13\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f16, f24\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f26, f12\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f19, f4\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f11, f3\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f23, f16\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f28, f14\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f15, f15\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f11, f17\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f2, f14\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f29, f9\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f13, f30\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f25, f6\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f28, f18\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f21, f14\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f9, f9\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f28, f22\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f24, f30\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f6, f23\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f5, f7\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f26, f20\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f5, f3\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f26, f29\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f28, f21\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f19, f1\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f27, f20\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f14, f12\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f12, f21\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f25, f10\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f4, f7\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f15, f6\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f18, f20\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f10.ps f10, f21\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
