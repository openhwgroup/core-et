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
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00010
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00014
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0001c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00020
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00028
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00030
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00034
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0003c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00040
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00048
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0004c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00058
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0005c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00068
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0006c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00070
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0007c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00088
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0xbf028f5c,                                                  // -0.51                                       /// 00090
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00094
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0009c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00108
			 0x0c600000,                                                  // Leading 1s:                                 /// 0010c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00110
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00118
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0011c
			 0x4b000000,                                                  // 8388608.0                                   /// 00120
			 0x0c700000,                                                  // Leading 1s:                                 /// 00124
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00128
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00138
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00150
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x80000000,                                                  // -zero                                       /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00174
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00178
			 0x00000000,                                                  // zero                                        /// 0017c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00184
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0018c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00190
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00198
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00204
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0020c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00210
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x7fc00001,                                                  // signaling NaN                               /// 00218
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00220
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00228
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00234
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00238
			 0x80011111,                                                  // -9.7958E-41                                 /// 0023c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x3f028f5c,                                                  // 0.51                                        /// 00248
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00250
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00258
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00264
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00270
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00274
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00278
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00284
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00290
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00298
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0029c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00304
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00308
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00310
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00314
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00318
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0031c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00320
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0032c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00330
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00338
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00344
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00348
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0034c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00354
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00358
			 0x3f028f5c,                                                  // 0.51                                        /// 0035c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x7fc00001,                                                  // signaling NaN                               /// 00364
			 0x0c780000,                                                  // Leading 1s:                                 /// 00368
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0036c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00374
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0037c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00380
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e000007,                                                  // Trailing 1s:                                /// 00388
			 0x80000000,                                                  // -zero                                       /// 0038c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00390
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00394
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00398
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0039c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00400
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00408
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0040c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00414
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0041c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00424
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00428
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0042c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00430
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00434
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0043c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00440
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00444
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00448
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0044c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00450
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xbf028f5c,                                                  // -0.51                                       /// 00458
			 0x0e000007,                                                  // Trailing 1s:                                /// 0045c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00460
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00470
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00474
			 0x80000000,                                                  // -zero                                       /// 00478
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00484
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00488
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0048c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00494
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00498
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d0
			 0xbf028f5c,                                                  // -0.51                                       /// 004d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00500
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00504
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00508
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x00000000,                                                  // zero                                        /// 00510
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00514
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00518
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0051c
			 0xcb000000,                                                  // -8388608.0                                  /// 00520
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00524
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0052c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00534
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0053c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00540
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00550
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0055c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00564
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x7f800000,                                                  // inf                                         /// 00570
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0c400000,                                                  // Leading 1s:                                 /// 0057c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00584
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00588
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00594
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00598
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e0
			 0x33333333,                                                  // 4 random values                             /// 005e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00600
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00604
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00608
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00610
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00618
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00620
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00628
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00638
			 0x7fc00001,                                                  // signaling NaN                               /// 0063c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00640
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00644
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00648
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0064c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00654
			 0x0c400000,                                                  // Leading 1s:                                 /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00660
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00664
			 0xff800000,                                                  // -inf                                        /// 00668
			 0x7f800000,                                                  // inf                                         /// 0066c
			 0x7f800000,                                                  // inf                                         /// 00670
			 0x00011111,                                                  // 9.7958E-41                                  /// 00674
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0067c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00688
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00690
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0069c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006b4
			 0x4b000000,                                                  // 8388608.0                                   /// 006b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006c0
			 0xbf028f5c,                                                  // -0.51                                       /// 006c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00708
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00710
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00714
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00718
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00720
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00724
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00728
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0072c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00730
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00734
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00738
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0073c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00740
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00744
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00748
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0074c
			 0x7f800000,                                                  // inf                                         /// 00750
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0075c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00768
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00770
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e000001,                                                  // Trailing 1s:                                /// 00778
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0077c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00780
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00784
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x33333333,                                                  // 4 random values                             /// 0078c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00790
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00794
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00798
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0079c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007a8
			 0x33333333,                                                  // 4 random values                             /// 007ac
			 0x55555555,                                                  // 4 random values                             /// 007b0
			 0x3f028f5c,                                                  // 0.51                                        /// 007b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00800
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00804
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00810
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0081c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00820
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00824
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00828
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0082c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00830
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0083c
			 0xcb000000,                                                  // -8388608.0                                  /// 00840
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00844
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00848
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0084c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00854
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0085c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00868
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00870
			 0x7f800000,                                                  // inf                                         /// 00874
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00878
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0087c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00880
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00884
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x00000000,                                                  // zero                                        /// 0088c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00890
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00898
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0089c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c8
			 0x55555555,                                                  // 4 random values                             /// 008cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008f4
			 0x33333333,                                                  // 4 random values                             /// 008f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00904
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00908
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0090c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00910
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00914
			 0xcb000000,                                                  // -8388608.0                                  /// 00918
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0091c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00920
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00928
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0092c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7f800000,                                                  // inf                                         /// 00934
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00938
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0093c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0094c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00950
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00954
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00958
			 0x0c400000,                                                  // Leading 1s:                                 /// 0095c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00964
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x7f800000,                                                  // inf                                         /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00980
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0098c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00998
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0099c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a20
			 0x7f800000,                                                  // inf                                         /// 00a24
			 0xcb000000,                                                  // -8388608.0                                  /// 00a28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a50
			 0xffc00001,                                                  // -signaling NaN                              /// 00a54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7fc00001,                                                  // signaling NaN                               /// 00abc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00acc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00adc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ae4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ae8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00af4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00afc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000000,                                                  // zero                                        /// 00b14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b20
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000000,                                                  // -zero                                       /// 00b8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bd8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bdc
			 0xff800000,                                                  // -inf                                        /// 00be0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bf0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0x00000000,                                                  // zero                                        /// 00c14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c28
			 0xbf028f5c,                                                  // -0.51                                       /// 00c2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c30
			 0xff800000,                                                  // -inf                                        /// 00c34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0xcb000000,                                                  // -8388608.0                                  /// 00c44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ccc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ce0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000000,                                                  // -zero                                       /// 00d54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00da4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00db0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00db4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dbc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dcc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x33333333,                                                  // 4 random values                             /// 00de8
			 0x55555555,                                                  // 4 random values                             /// 00dec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00df4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x3f028f5c,                                                  // 0.51                                        /// 00e04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e34
			 0x7fc00001,                                                  // signaling NaN                               /// 00e38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0xffc00001,                                                  // -signaling NaN                              /// 00e5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e6c
			 0x00000000,                                                  // zero                                        /// 00e70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x3f028f5c,                                                  // 0.51                                        /// 00e84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e98
			 0x80000000,                                                  // -zero                                       /// 00e9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ec8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ecc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7f800000,                                                  // inf                                         /// 00ed4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ed8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00edc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00efc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f08
			 0x7f800000,                                                  // inf                                         /// 00f0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f14
			 0xcb000000,                                                  // -8388608.0                                  /// 00f18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa0
			 0x33333333,                                                  // 4 random values                             /// 00fa4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00fc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ff0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ff4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff8
			 0x00800000 // min norm                                      // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00000
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00010
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00018
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00020
			 0x0c400000,                                                  // Leading 1s:                                 /// 00024
			 0xffc00001,                                                  // -signaling NaN                              /// 00028
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0002c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00034
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0003c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00040
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00044
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00048
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0004c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00050
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00054
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00058
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0005c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00064
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00068
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0006c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00070
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00074
			 0x55555555,                                                  // 4 random values                             /// 00078
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0007c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00084
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0008c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00098
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0009c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x33333333,                                                  // 4 random values                             /// 000b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00104
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0010c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00118
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00120
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00128
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c780000,                                                  // Leading 1s:                                 /// 00134
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0013c
			 0x7fc00001,                                                  // signaling NaN                               /// 00140
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x33333333,                                                  // 4 random values                             /// 00150
			 0x7fc00001,                                                  // signaling NaN                               /// 00154
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00158
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0015c
			 0x7f800000,                                                  // inf                                         /// 00160
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00164
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00174
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00178
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0017c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00180
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c780000,                                                  // Leading 1s:                                 /// 00188
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0018c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00190
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00194
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00198
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001b0
			 0x7f800000,                                                  // inf                                         /// 001b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001bc
			 0x7f800000,                                                  // inf                                         /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x3f028f5c,                                                  // 0.51                                        /// 001c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00200
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00204
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00208
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0020c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00214
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00218
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0021c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00224
			 0x80011111,                                                  // -9.7958E-41                                 /// 00228
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00230
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00234
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00238
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00244
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00248
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0024c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00250
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00011111,                                                  // 9.7958E-41                                  /// 0025c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00260
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00264
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00274
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00278
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0027c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00280
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00288
			 0x0e000001,                                                  // Trailing 1s:                                /// 0028c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00290
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00294
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00298
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0029c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002bc
			 0x7f800000,                                                  // inf                                         /// 002c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002c8
			 0x4b000000,                                                  // 8388608.0                                   /// 002cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x3f028f5c,                                                  // 0.51                                        /// 002d8
			 0xcb000000,                                                  // -8388608.0                                  /// 002dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0xff800000,                                                  // -inf                                        /// 002e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00300
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00304
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x3f028f5c,                                                  // 0.51                                        /// 00310
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00314
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00318
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00320
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00324
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00328
			 0x0e000001,                                                  // Trailing 1s:                                /// 0032c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00330
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x0c600000,                                                  // Leading 1s:                                 /// 00338
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00344
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0034c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00350
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0035c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00360
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00364
			 0x4b000000,                                                  // 8388608.0                                   /// 00368
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0036c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00370
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00374
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00380
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00384
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00390
			 0x7fc00001,                                                  // signaling NaN                               /// 00394
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0039c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00404
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00408
			 0x80000000,                                                  // -zero                                       /// 0040c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00410
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00414
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00420
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00424
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00430
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00434
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0043c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00454
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0045c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00460
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0c600000,                                                  // Leading 1s:                                 /// 00468
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0046c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00470
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00474
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xff800000,                                                  // -inf                                        /// 0047c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00488
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0048c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00498
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0049c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004bc
			 0xcb000000,                                                  // -8388608.0                                  /// 004c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004c8
			 0xff800000,                                                  // -inf                                        /// 004cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xbf028f5c,                                                  // -0.51                                       /// 00504
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00514
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00518
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00524
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00528
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0052c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00530
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00534
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00538
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0053c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00011111,                                                  // 9.7958E-41                                  /// 00548
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0054c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00550
			 0x00011111,                                                  // 9.7958E-41                                  /// 00554
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00558
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00564
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00568
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0056c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00570
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00580
			 0x0e000007,                                                  // Trailing 1s:                                /// 00584
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00588
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0058c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00590
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005c8
			 0x3f028f5c,                                                  // 0.51                                        /// 005cc
			 0x7fc00001,                                                  // signaling NaN                               /// 005d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00600
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00608
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0060c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00610
			 0x7fc00001,                                                  // signaling NaN                               /// 00614
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x3f028f5c,                                                  // 0.51                                        /// 0061c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00620
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00624
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00628
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00634
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00638
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0063c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00664
			 0x00011111,                                                  // 9.7958E-41                                  /// 00668
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0066c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00670
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00674
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00678
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00684
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0068c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00690
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00694
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00698
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 006d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00700
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00704
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0070c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00710
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00714
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00720
			 0x55555555,                                                  // 4 random values                             /// 00724
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00728
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00730
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x7fc00001,                                                  // signaling NaN                               /// 00738
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0073c
			 0x33333333,                                                  // 4 random values                             /// 00740
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00748
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0074c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00760
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00764
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xffc00001,                                                  // -signaling NaN                              /// 00774
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00778
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00790
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00794
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c4
			 0x3f028f5c,                                                  // 0.51                                        /// 007c8
			 0x55555555,                                                  // 4 random values                             /// 007cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00800
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00804
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00808
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0080c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00810
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00818
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0081c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00824
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0082c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00830
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00834
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00840
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00844
			 0x0e000003,                                                  // Trailing 1s:                                /// 00848
			 0x80000000,                                                  // -zero                                       /// 0084c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0085c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00860
			 0x3f028f5c,                                                  // 0.51                                        /// 00864
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00868
			 0x0c600000,                                                  // Leading 1s:                                 /// 0086c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00878
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00884
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00888
			 0xffc00001,                                                  // -signaling NaN                              /// 0088c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e000007,                                                  // Trailing 1s:                                /// 00894
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00898
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x55555555,                                                  // 4 random values                             /// 008dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00900
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00910
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00914
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00920
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00924
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x80011111,                                                  // -9.7958E-41                                 /// 0092c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00938
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0093c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00940
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00948
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00958
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0095c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00964
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0096c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00974
			 0x3f028f5c,                                                  // 0.51                                        /// 00978
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0097c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00980
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0098c
			 0x4b000000,                                                  // 8388608.0                                   /// 00990
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00994
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0099c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x7fc00001,                                                  // signaling NaN                               /// 009cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a04
			 0x7f800000,                                                  // inf                                         /// 00a08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a28
			 0x7fc00001,                                                  // signaling NaN                               /// 00a2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00abc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xff800000,                                                  // -inf                                        /// 00afc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b10
			 0xffc00001,                                                  // -signaling NaN                              /// 00b14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b68
			 0x00000000,                                                  // zero                                        /// 00b6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x4b000000,                                                  // 8388608.0                                   /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00be0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00be8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c1c
			 0x7f800000,                                                  // inf                                         /// 00c20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c94
			 0x80000000,                                                  // -zero                                       /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ca4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cb8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ccc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cf4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d64
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d90
			 0x55555555,                                                  // 4 random values                             /// 00d94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00da0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0xcb000000,                                                  // -8388608.0                                  /// 00db8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dd8
			 0x80000000,                                                  // -zero                                       /// 00ddc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dfc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e40
			 0x55555555,                                                  // 4 random values                             /// 00e44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xbf028f5c,                                                  // -0.51                                       /// 00e98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ebc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed8
			 0x00000000,                                                  // zero                                        /// 00edc
			 0x7f800000,                                                  // inf                                         /// 00ee0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ee4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ee8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0x80000000,                                                  // -zero                                       /// 00f2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa4
			 0x33333333,                                                  // 4 random values                             /// 00fa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc4
			 0xff800000,                                                  // -inf                                        /// 00fc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fd0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fe4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fe8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0e00007f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xb18bd0ee, /// 0x0
			 0xa97624ac, /// 0x4
			 0x21fd3667, /// 0x8
			 0xad963ac0, /// 0xc
			 0x4ca5515d, /// 0x10
			 0x835b2ec9, /// 0x14
			 0xb6886d4e, /// 0x18
			 0x48aef897, /// 0x1c
			 0x17f241e0, /// 0x20
			 0x5be6c8f4, /// 0x24
			 0x11efe872, /// 0x28
			 0x07f6a949, /// 0x2c
			 0xda850fb2, /// 0x30
			 0x41ae8196, /// 0x34
			 0xcce545a6, /// 0x38
			 0xdd54ef5c, /// 0x3c
			 0x68fa36f7, /// 0x40
			 0xb4c30296, /// 0x44
			 0xd77229bc, /// 0x48
			 0xd404a613, /// 0x4c
			 0x73ef3243, /// 0x50
			 0xb093aef1, /// 0x54
			 0x156bfbd7, /// 0x58
			 0x210de4e1, /// 0x5c
			 0x5818e323, /// 0x60
			 0x15eb8df5, /// 0x64
			 0x69594f95, /// 0x68
			 0xb15769de, /// 0x6c
			 0xda9f012d, /// 0x70
			 0xa19f78d0, /// 0x74
			 0xd8fe6f13, /// 0x78
			 0x627cf8af, /// 0x7c
			 0x3be87752, /// 0x80
			 0xb34d7010, /// 0x84
			 0xb6343e6f, /// 0x88
			 0xfef735cf, /// 0x8c
			 0x3c53220c, /// 0x90
			 0x0d70669d, /// 0x94
			 0xf3a0c2be, /// 0x98
			 0xd2a24fb0, /// 0x9c
			 0x4d56a7bc, /// 0xa0
			 0x9b4f2e68, /// 0xa4
			 0x8d35dc25, /// 0xa8
			 0x9d8f44b2, /// 0xac
			 0x51d1c2ac, /// 0xb0
			 0x3c38a322, /// 0xb4
			 0x576459c9, /// 0xb8
			 0x43a0d027, /// 0xbc
			 0xd19ed465, /// 0xc0
			 0x13eaefaf, /// 0xc4
			 0xb701e919, /// 0xc8
			 0xbb453f34, /// 0xcc
			 0xd3e7f64d, /// 0xd0
			 0x9b4bd54d, /// 0xd4
			 0x9ac5de0c, /// 0xd8
			 0xa1c1b5dc, /// 0xdc
			 0x64f10cf5, /// 0xe0
			 0xb9302c14, /// 0xe4
			 0x66d0cccb, /// 0xe8
			 0x617f03e1, /// 0xec
			 0x6141f042, /// 0xf0
			 0xcf78d6e7, /// 0xf4
			 0x91c119fe, /// 0xf8
			 0x8e45235e, /// 0xfc
			 0x07f6bdb6, /// 0x100
			 0xb36a0462, /// 0x104
			 0xdf642115, /// 0x108
			 0x2ca58740, /// 0x10c
			 0x836968ba, /// 0x110
			 0x8f8ceb38, /// 0x114
			 0x343d17ba, /// 0x118
			 0x0e0b1b9a, /// 0x11c
			 0xdcc23efd, /// 0x120
			 0x75cbaade, /// 0x124
			 0x2ba40a19, /// 0x128
			 0x43c40218, /// 0x12c
			 0x4001ca00, /// 0x130
			 0x92c692f9, /// 0x134
			 0xe74c8985, /// 0x138
			 0x998e2a22, /// 0x13c
			 0x4dce55a3, /// 0x140
			 0x1b45dc85, /// 0x144
			 0x650bfe8b, /// 0x148
			 0x629dfb52, /// 0x14c
			 0x7a09efb8, /// 0x150
			 0xdc694051, /// 0x154
			 0xffe618c5, /// 0x158
			 0x5225f4e6, /// 0x15c
			 0x4a092c64, /// 0x160
			 0xa1e42cfe, /// 0x164
			 0xdefab1c8, /// 0x168
			 0xe000fe52, /// 0x16c
			 0x52c2f6a9, /// 0x170
			 0x911465dd, /// 0x174
			 0x836f5289, /// 0x178
			 0x96c89c27, /// 0x17c
			 0x9b6c438f, /// 0x180
			 0x738f4a83, /// 0x184
			 0xc6668a80, /// 0x188
			 0x85423a5c, /// 0x18c
			 0x58ee4733, /// 0x190
			 0x3c9c6f8a, /// 0x194
			 0x8d8b4bbf, /// 0x198
			 0xc6688a14, /// 0x19c
			 0xc977d2d7, /// 0x1a0
			 0x984d0da1, /// 0x1a4
			 0x3eaa045e, /// 0x1a8
			 0x0b282e67, /// 0x1ac
			 0x668956d2, /// 0x1b0
			 0x95e816d8, /// 0x1b4
			 0xc558b6f9, /// 0x1b8
			 0x248bb626, /// 0x1bc
			 0xd9626d39, /// 0x1c0
			 0x41e4a31b, /// 0x1c4
			 0x016c6191, /// 0x1c8
			 0xe63552f9, /// 0x1cc
			 0xdc945aa4, /// 0x1d0
			 0xeeb0ff8a, /// 0x1d4
			 0xeff62f14, /// 0x1d8
			 0x87a67ccc, /// 0x1dc
			 0x96620530, /// 0x1e0
			 0x6505ff7f, /// 0x1e4
			 0xfad23e16, /// 0x1e8
			 0xa6c8399b, /// 0x1ec
			 0xa0723b4c, /// 0x1f0
			 0xbaa09e80, /// 0x1f4
			 0x47b9994f, /// 0x1f8
			 0x41bd4d10, /// 0x1fc
			 0xdb292abd, /// 0x200
			 0xe37d9533, /// 0x204
			 0x59102e5a, /// 0x208
			 0xf9db7250, /// 0x20c
			 0x75d6cd62, /// 0x210
			 0xe988745b, /// 0x214
			 0xd83f5cf1, /// 0x218
			 0x6aa3927d, /// 0x21c
			 0x9fa6511e, /// 0x220
			 0xe570352b, /// 0x224
			 0xb15a5d74, /// 0x228
			 0x53efddc0, /// 0x22c
			 0xc9d70e1b, /// 0x230
			 0x6e70a077, /// 0x234
			 0x9c8a1ed9, /// 0x238
			 0x4ed690c0, /// 0x23c
			 0x8c02c214, /// 0x240
			 0xc085a8e0, /// 0x244
			 0xf1287dd2, /// 0x248
			 0x2ca53bd9, /// 0x24c
			 0xdf3292f1, /// 0x250
			 0x1b8adb27, /// 0x254
			 0xef702da9, /// 0x258
			 0x4db17e4b, /// 0x25c
			 0x51a0831a, /// 0x260
			 0xc76189f5, /// 0x264
			 0xb6f9b4f6, /// 0x268
			 0x422d94a6, /// 0x26c
			 0xdfcb7e12, /// 0x270
			 0x62c13b50, /// 0x274
			 0x49740529, /// 0x278
			 0x53ed2528, /// 0x27c
			 0xdd279df3, /// 0x280
			 0x16843265, /// 0x284
			 0xaa88b977, /// 0x288
			 0x4e4a482d, /// 0x28c
			 0xd60223ad, /// 0x290
			 0x32adda4b, /// 0x294
			 0xe1cea0d4, /// 0x298
			 0xb8c571b8, /// 0x29c
			 0xb6d0c017, /// 0x2a0
			 0xae9e794b, /// 0x2a4
			 0x63d5f9ce, /// 0x2a8
			 0x3f8d47a3, /// 0x2ac
			 0xb8326b6e, /// 0x2b0
			 0x71f69a44, /// 0x2b4
			 0xa1cfd1f3, /// 0x2b8
			 0xbbd0eb7b, /// 0x2bc
			 0xa36c6c19, /// 0x2c0
			 0x92dfc228, /// 0x2c4
			 0x1a69232e, /// 0x2c8
			 0xb4a3c5a0, /// 0x2cc
			 0x47ef2253, /// 0x2d0
			 0x041510f1, /// 0x2d4
			 0xb26af763, /// 0x2d8
			 0xb50e060b, /// 0x2dc
			 0x925ad817, /// 0x2e0
			 0x114decce, /// 0x2e4
			 0x378aca93, /// 0x2e8
			 0xece2505c, /// 0x2ec
			 0x1fc2e81d, /// 0x2f0
			 0x6c16be39, /// 0x2f4
			 0x3dc90590, /// 0x2f8
			 0x29d262c8, /// 0x2fc
			 0xdbae4ac7, /// 0x300
			 0x2eae6528, /// 0x304
			 0xd8aa6f11, /// 0x308
			 0xda8fa4c3, /// 0x30c
			 0x4665f447, /// 0x310
			 0xd14b0ec0, /// 0x314
			 0xc6e3cad7, /// 0x318
			 0x78f3ae06, /// 0x31c
			 0x9c23e489, /// 0x320
			 0xac86367b, /// 0x324
			 0xeaadfc2e, /// 0x328
			 0xc612c72a, /// 0x32c
			 0xabf6b3ba, /// 0x330
			 0x431267ab, /// 0x334
			 0xe178aa37, /// 0x338
			 0xdaa9f50d, /// 0x33c
			 0xebb8a876, /// 0x340
			 0x1a638dcc, /// 0x344
			 0xd352bc3c, /// 0x348
			 0x7a3754bf, /// 0x34c
			 0x2e9905a0, /// 0x350
			 0x25080840, /// 0x354
			 0xd65d769f, /// 0x358
			 0xadabc3f7, /// 0x35c
			 0x58434138, /// 0x360
			 0x2abdb179, /// 0x364
			 0xdc969778, /// 0x368
			 0xb28ffcdb, /// 0x36c
			 0x60f989ce, /// 0x370
			 0x0df3ae2f, /// 0x374
			 0x91ee4f16, /// 0x378
			 0x91a007ed, /// 0x37c
			 0x63ccaeeb, /// 0x380
			 0x2fb70dc9, /// 0x384
			 0xd8bad120, /// 0x388
			 0xfd3bdedd, /// 0x38c
			 0xf3703182, /// 0x390
			 0xd195da8b, /// 0x394
			 0xe3af7da8, /// 0x398
			 0xfdca68c8, /// 0x39c
			 0x5c136559, /// 0x3a0
			 0xb463c9bd, /// 0x3a4
			 0x3a17962f, /// 0x3a8
			 0x9a03a9b7, /// 0x3ac
			 0x25d564ac, /// 0x3b0
			 0x97da722d, /// 0x3b4
			 0xc6eae2ca, /// 0x3b8
			 0x92337633, /// 0x3bc
			 0xf4ec132b, /// 0x3c0
			 0x8c3e0599, /// 0x3c4
			 0x1d68356f, /// 0x3c8
			 0xcaf6fe1f, /// 0x3cc
			 0x1b29d5c0, /// 0x3d0
			 0x652a5f84, /// 0x3d4
			 0x9b71fe43, /// 0x3d8
			 0x3e63124a, /// 0x3dc
			 0x27ea2b04, /// 0x3e0
			 0x62506edb, /// 0x3e4
			 0x21e50593, /// 0x3e8
			 0x42cb2c85, /// 0x3ec
			 0x1251afed, /// 0x3f0
			 0xebac0f80, /// 0x3f4
			 0xadfa16e4, /// 0x3f8
			 0x375f2fd0, /// 0x3fc
			 0xed55233a, /// 0x400
			 0x98dccbbc, /// 0x404
			 0xf7e0cdad, /// 0x408
			 0x79702fd7, /// 0x40c
			 0x837c8750, /// 0x410
			 0xd28b53df, /// 0x414
			 0xd0e0fb7b, /// 0x418
			 0x3ebe8e59, /// 0x41c
			 0x36a38edb, /// 0x420
			 0xef4d5e2a, /// 0x424
			 0x3011a52b, /// 0x428
			 0xb6759f7c, /// 0x42c
			 0x5d2b7cc8, /// 0x430
			 0x88154f7e, /// 0x434
			 0xbaf2470e, /// 0x438
			 0x7ad16540, /// 0x43c
			 0xa1c83d6b, /// 0x440
			 0x8a3079f4, /// 0x444
			 0x60ff3150, /// 0x448
			 0x218bd50e, /// 0x44c
			 0x1de5eaa3, /// 0x450
			 0x704b3c0f, /// 0x454
			 0x63274a6a, /// 0x458
			 0xbfed84c1, /// 0x45c
			 0x4885371b, /// 0x460
			 0x74ece5fb, /// 0x464
			 0x1b34c5a4, /// 0x468
			 0x8e651fd6, /// 0x46c
			 0xe61ddc79, /// 0x470
			 0x6fcda8d5, /// 0x474
			 0xd95c2c41, /// 0x478
			 0x9240cb84, /// 0x47c
			 0x08cf6a61, /// 0x480
			 0x47331491, /// 0x484
			 0x409572be, /// 0x488
			 0x8964443d, /// 0x48c
			 0x6bcf71ea, /// 0x490
			 0xc5ef7b4b, /// 0x494
			 0x5f708df3, /// 0x498
			 0x24dbb738, /// 0x49c
			 0x61dfef3d, /// 0x4a0
			 0xb827c55c, /// 0x4a4
			 0x2f0076e5, /// 0x4a8
			 0xbfdcc3af, /// 0x4ac
			 0x306a5224, /// 0x4b0
			 0x6f88c99e, /// 0x4b4
			 0xbf8014c5, /// 0x4b8
			 0xe94f7af1, /// 0x4bc
			 0x9673e3e8, /// 0x4c0
			 0xf3c11eb5, /// 0x4c4
			 0x26926653, /// 0x4c8
			 0xc1a6a7a4, /// 0x4cc
			 0xbb166452, /// 0x4d0
			 0x6b796685, /// 0x4d4
			 0x7c083977, /// 0x4d8
			 0x768cb53a, /// 0x4dc
			 0xf39580ba, /// 0x4e0
			 0x4432b91b, /// 0x4e4
			 0x57433a4a, /// 0x4e8
			 0x6078fb92, /// 0x4ec
			 0x653fb1e4, /// 0x4f0
			 0x0033ef99, /// 0x4f4
			 0x13b0acb3, /// 0x4f8
			 0x7ae2321d, /// 0x4fc
			 0x0a910910, /// 0x500
			 0x0782d5bc, /// 0x504
			 0xf32049ed, /// 0x508
			 0xbebe932a, /// 0x50c
			 0xb0db4147, /// 0x510
			 0xf3b1f453, /// 0x514
			 0xbd8d2bc5, /// 0x518
			 0xcea48121, /// 0x51c
			 0xb96626b7, /// 0x520
			 0x4bd5bdc6, /// 0x524
			 0x5f685910, /// 0x528
			 0x207561c4, /// 0x52c
			 0x2cc2d043, /// 0x530
			 0x580d7dfd, /// 0x534
			 0x35174c15, /// 0x538
			 0x9f2e2b09, /// 0x53c
			 0xc44a53c6, /// 0x540
			 0xae82c859, /// 0x544
			 0xc1c719b1, /// 0x548
			 0xadb22d1d, /// 0x54c
			 0xe63c3c57, /// 0x550
			 0x181be909, /// 0x554
			 0x0ed479d5, /// 0x558
			 0xb57aec3d, /// 0x55c
			 0x77dacd28, /// 0x560
			 0x9d197c7f, /// 0x564
			 0x515dbc06, /// 0x568
			 0x805f8011, /// 0x56c
			 0x1bfacd5f, /// 0x570
			 0x20324208, /// 0x574
			 0xcd086a91, /// 0x578
			 0xe37cc701, /// 0x57c
			 0xb6c7eda5, /// 0x580
			 0x6b1ac298, /// 0x584
			 0xf65750e2, /// 0x588
			 0x6460d6b6, /// 0x58c
			 0x81d1ad5f, /// 0x590
			 0xe3628868, /// 0x594
			 0xd0a06e19, /// 0x598
			 0x8cbf7995, /// 0x59c
			 0xf72cf74e, /// 0x5a0
			 0xee4c7758, /// 0x5a4
			 0x0bec8a16, /// 0x5a8
			 0x223cad6a, /// 0x5ac
			 0x3e484a9b, /// 0x5b0
			 0x444601b4, /// 0x5b4
			 0x5d974bc1, /// 0x5b8
			 0x38a75ef7, /// 0x5bc
			 0xff72d55e, /// 0x5c0
			 0x072a77b9, /// 0x5c4
			 0x4e2f9b3d, /// 0x5c8
			 0x7337daed, /// 0x5cc
			 0xafe9a15c, /// 0x5d0
			 0x8a925c6f, /// 0x5d4
			 0x1bef95fb, /// 0x5d8
			 0xfabaf0a6, /// 0x5dc
			 0xb806bdda, /// 0x5e0
			 0x96bc15e6, /// 0x5e4
			 0x62d213c9, /// 0x5e8
			 0xe6a4c6df, /// 0x5ec
			 0x5a94349e, /// 0x5f0
			 0x6ab0c2ce, /// 0x5f4
			 0x0bdd3aa9, /// 0x5f8
			 0x14a1e846, /// 0x5fc
			 0x7f39d3fd, /// 0x600
			 0xe39bd593, /// 0x604
			 0x3b3ab42c, /// 0x608
			 0x0113b510, /// 0x60c
			 0x2c5f4052, /// 0x610
			 0xfd45488b, /// 0x614
			 0x6a71ee65, /// 0x618
			 0x886eb538, /// 0x61c
			 0xa360b9a2, /// 0x620
			 0x15257a9d, /// 0x624
			 0x34086f2a, /// 0x628
			 0x139c5a32, /// 0x62c
			 0x69953a10, /// 0x630
			 0x88966747, /// 0x634
			 0x14282d66, /// 0x638
			 0x94fcd35c, /// 0x63c
			 0xe98d1f7f, /// 0x640
			 0xc0fcdeb7, /// 0x644
			 0x0657eb0a, /// 0x648
			 0x90e48e21, /// 0x64c
			 0xec9a074d, /// 0x650
			 0x0257e59a, /// 0x654
			 0x41674a81, /// 0x658
			 0x2a453943, /// 0x65c
			 0xa2362528, /// 0x660
			 0x98a2004f, /// 0x664
			 0xf30588db, /// 0x668
			 0xcd7962a9, /// 0x66c
			 0x28eeb528, /// 0x670
			 0xfb075f91, /// 0x674
			 0x1485ec03, /// 0x678
			 0x95b03768, /// 0x67c
			 0x155bc89f, /// 0x680
			 0xed5482ea, /// 0x684
			 0x0bff13e7, /// 0x688
			 0x7dd69fa3, /// 0x68c
			 0x211521cc, /// 0x690
			 0xcd6e96f3, /// 0x694
			 0xd8725b02, /// 0x698
			 0x5f1e8fa7, /// 0x69c
			 0x407fdb64, /// 0x6a0
			 0x886bf855, /// 0x6a4
			 0xb1d7ab33, /// 0x6a8
			 0xffe70bb4, /// 0x6ac
			 0x50556f1e, /// 0x6b0
			 0x3a8e255d, /// 0x6b4
			 0x3505735a, /// 0x6b8
			 0x70d12403, /// 0x6bc
			 0x95a234d7, /// 0x6c0
			 0xf812f020, /// 0x6c4
			 0x1e26b9a0, /// 0x6c8
			 0x53b59d25, /// 0x6cc
			 0x764e98b7, /// 0x6d0
			 0xb04fd77d, /// 0x6d4
			 0xbee470f0, /// 0x6d8
			 0x64e2bea1, /// 0x6dc
			 0x5f5956ed, /// 0x6e0
			 0x4fa8c773, /// 0x6e4
			 0xdd8efaf6, /// 0x6e8
			 0x22d6e558, /// 0x6ec
			 0x3e5859a3, /// 0x6f0
			 0x56288d82, /// 0x6f4
			 0x843b3c17, /// 0x6f8
			 0xf6b02310, /// 0x6fc
			 0xed1f0604, /// 0x700
			 0x222bad08, /// 0x704
			 0x549daa1c, /// 0x708
			 0x2aec8367, /// 0x70c
			 0x55b8a05a, /// 0x710
			 0xcf76e5c8, /// 0x714
			 0xb0c6170d, /// 0x718
			 0x48b970df, /// 0x71c
			 0xda18ae96, /// 0x720
			 0x40ae4a76, /// 0x724
			 0x0628432a, /// 0x728
			 0xa83bb12e, /// 0x72c
			 0x9d9ad0fa, /// 0x730
			 0x179d571b, /// 0x734
			 0xd7a40f57, /// 0x738
			 0x33ecd388, /// 0x73c
			 0x29f9a920, /// 0x740
			 0x2a874fb6, /// 0x744
			 0x0770b97d, /// 0x748
			 0x846e52dd, /// 0x74c
			 0x11f7f77e, /// 0x750
			 0xfb22828d, /// 0x754
			 0x4d2f9436, /// 0x758
			 0xc771f4e3, /// 0x75c
			 0x7eb3725a, /// 0x760
			 0xdf3bf37d, /// 0x764
			 0x574e7bf3, /// 0x768
			 0x329fbb7c, /// 0x76c
			 0x6c1a3df9, /// 0x770
			 0x4f8e6836, /// 0x774
			 0xc0793468, /// 0x778
			 0x4b722de9, /// 0x77c
			 0x52c8e9ad, /// 0x780
			 0xa39b13f7, /// 0x784
			 0x673daabd, /// 0x788
			 0xb5ef46bc, /// 0x78c
			 0xda033ef3, /// 0x790
			 0x6c01fc10, /// 0x794
			 0x03c506e9, /// 0x798
			 0xa3be3c4a, /// 0x79c
			 0xbfadb426, /// 0x7a0
			 0xa1a668f0, /// 0x7a4
			 0x798e4183, /// 0x7a8
			 0x2f1789cd, /// 0x7ac
			 0x7809b613, /// 0x7b0
			 0x7e861151, /// 0x7b4
			 0x92a1476a, /// 0x7b8
			 0xda7629f7, /// 0x7bc
			 0x310c3046, /// 0x7c0
			 0xfb364e8d, /// 0x7c4
			 0xa50465eb, /// 0x7c8
			 0x12c248b2, /// 0x7cc
			 0x97a1815a, /// 0x7d0
			 0xe7cf328a, /// 0x7d4
			 0x57f53f35, /// 0x7d8
			 0x1cf2318f, /// 0x7dc
			 0x378aa8ed, /// 0x7e0
			 0x8d99241a, /// 0x7e4
			 0xd55905b8, /// 0x7e8
			 0x3d342b84, /// 0x7ec
			 0xbc772a07, /// 0x7f0
			 0xcb6c843a, /// 0x7f4
			 0x9f9a8714, /// 0x7f8
			 0x71ce4ea4, /// 0x7fc
			 0xd53b64ce, /// 0x800
			 0xa1ee4f94, /// 0x804
			 0x3c2b57e8, /// 0x808
			 0x813128a4, /// 0x80c
			 0xc5144173, /// 0x810
			 0x12c89572, /// 0x814
			 0x9d9d0d33, /// 0x818
			 0x582a702e, /// 0x81c
			 0xa7dd204e, /// 0x820
			 0x63efcb5f, /// 0x824
			 0x045c0b38, /// 0x828
			 0x0a10ae2b, /// 0x82c
			 0x672f436b, /// 0x830
			 0x477b491a, /// 0x834
			 0xa3881fed, /// 0x838
			 0x37f7e1b7, /// 0x83c
			 0xcf6d7b83, /// 0x840
			 0x35f1566a, /// 0x844
			 0xe2d78d31, /// 0x848
			 0x7d3f98c6, /// 0x84c
			 0x8b8f4809, /// 0x850
			 0x12435774, /// 0x854
			 0xdb2f0088, /// 0x858
			 0xd14e789b, /// 0x85c
			 0xd8d77fcf, /// 0x860
			 0x8653efec, /// 0x864
			 0x6a5dc7e8, /// 0x868
			 0xb91d9d0a, /// 0x86c
			 0x8f857c52, /// 0x870
			 0x824219fc, /// 0x874
			 0xb9823a35, /// 0x878
			 0xc7cbfbfb, /// 0x87c
			 0x9925631b, /// 0x880
			 0x3d30d8f6, /// 0x884
			 0x40d3d50e, /// 0x888
			 0xaa4a48ea, /// 0x88c
			 0xd3520d79, /// 0x890
			 0x13e59dae, /// 0x894
			 0xdf08be55, /// 0x898
			 0xbb896d3b, /// 0x89c
			 0x45a6dfde, /// 0x8a0
			 0x6cef97d6, /// 0x8a4
			 0x5bf1f242, /// 0x8a8
			 0xfdb0f122, /// 0x8ac
			 0x63c53686, /// 0x8b0
			 0x49a2ed56, /// 0x8b4
			 0xc523bc10, /// 0x8b8
			 0xf9318ba2, /// 0x8bc
			 0x18689548, /// 0x8c0
			 0x76fe87fa, /// 0x8c4
			 0x86c9bd9b, /// 0x8c8
			 0x55a1c07b, /// 0x8cc
			 0xd6749d19, /// 0x8d0
			 0xa41f7ad7, /// 0x8d4
			 0x5033943b, /// 0x8d8
			 0xbe42650f, /// 0x8dc
			 0xa668c514, /// 0x8e0
			 0x23da6006, /// 0x8e4
			 0x4b2c2bad, /// 0x8e8
			 0x074cd3e5, /// 0x8ec
			 0x32067324, /// 0x8f0
			 0xba3b6e6c, /// 0x8f4
			 0x661025b2, /// 0x8f8
			 0x30263073, /// 0x8fc
			 0x97e0df40, /// 0x900
			 0x35191090, /// 0x904
			 0x27803701, /// 0x908
			 0x7e44e7cc, /// 0x90c
			 0xb5b0ebda, /// 0x910
			 0x86ab5d28, /// 0x914
			 0xeb82617c, /// 0x918
			 0x3a1427ea, /// 0x91c
			 0x523c996d, /// 0x920
			 0xf09f5ba3, /// 0x924
			 0x2c333af9, /// 0x928
			 0x5de78249, /// 0x92c
			 0x6edd9dbe, /// 0x930
			 0xe642428f, /// 0x934
			 0xfc09cc9a, /// 0x938
			 0xe739c916, /// 0x93c
			 0x18761758, /// 0x940
			 0xa62f6f9e, /// 0x944
			 0x32408d5e, /// 0x948
			 0x930cbc27, /// 0x94c
			 0xbbea8295, /// 0x950
			 0x6dfd3678, /// 0x954
			 0x6bf2acdd, /// 0x958
			 0xe3712fd2, /// 0x95c
			 0xab8e14f2, /// 0x960
			 0xb4e4fe8e, /// 0x964
			 0x25c5a9a8, /// 0x968
			 0x1c424f29, /// 0x96c
			 0x63fe336b, /// 0x970
			 0x29cece18, /// 0x974
			 0x885666bc, /// 0x978
			 0xa4b0dc8f, /// 0x97c
			 0xe65dd7b4, /// 0x980
			 0x57aef9a5, /// 0x984
			 0x9c81e1bd, /// 0x988
			 0xd5892715, /// 0x98c
			 0xa9238bcc, /// 0x990
			 0x9bd16281, /// 0x994
			 0xd7b7126e, /// 0x998
			 0xb7f68b67, /// 0x99c
			 0xa0c1da76, /// 0x9a0
			 0x93a24fcd, /// 0x9a4
			 0x0b020e8e, /// 0x9a8
			 0xab53d288, /// 0x9ac
			 0xb5b6c352, /// 0x9b0
			 0x04e16fbb, /// 0x9b4
			 0x63fea03f, /// 0x9b8
			 0x6dfccef1, /// 0x9bc
			 0x1accfb6e, /// 0x9c0
			 0x8ada0978, /// 0x9c4
			 0x7b5964df, /// 0x9c8
			 0x95f08b2e, /// 0x9cc
			 0x935e7b0d, /// 0x9d0
			 0x2d9c226a, /// 0x9d4
			 0xb99184bb, /// 0x9d8
			 0x96c97524, /// 0x9dc
			 0x34f95e96, /// 0x9e0
			 0x54aeab66, /// 0x9e4
			 0xa4c33af8, /// 0x9e8
			 0x17c85171, /// 0x9ec
			 0xa95d520d, /// 0x9f0
			 0xbb9137de, /// 0x9f4
			 0x68a1b4fb, /// 0x9f8
			 0x4cacf4f7, /// 0x9fc
			 0xc0ebb3e3, /// 0xa00
			 0x8ecdd58d, /// 0xa04
			 0xe652e6a3, /// 0xa08
			 0xb69c2b1a, /// 0xa0c
			 0xab81ee67, /// 0xa10
			 0xd0b0b667, /// 0xa14
			 0x99bf40f6, /// 0xa18
			 0x4f59a1c6, /// 0xa1c
			 0x89131735, /// 0xa20
			 0x8b45edd8, /// 0xa24
			 0x1cd912e5, /// 0xa28
			 0x43bed2ca, /// 0xa2c
			 0x594fcc40, /// 0xa30
			 0x66c2e6e2, /// 0xa34
			 0x6d219381, /// 0xa38
			 0xef55f2cf, /// 0xa3c
			 0x067bd19e, /// 0xa40
			 0xf712e8e6, /// 0xa44
			 0x35d9e94f, /// 0xa48
			 0x1b0cc549, /// 0xa4c
			 0x8c5297e9, /// 0xa50
			 0x54b5a442, /// 0xa54
			 0x84f4b9db, /// 0xa58
			 0x99ca991a, /// 0xa5c
			 0x3ac62872, /// 0xa60
			 0x2e97ef25, /// 0xa64
			 0x9492d6a2, /// 0xa68
			 0xe3a90aab, /// 0xa6c
			 0x8035dffc, /// 0xa70
			 0xc8dede80, /// 0xa74
			 0xb7042782, /// 0xa78
			 0xaebd46d0, /// 0xa7c
			 0x57d20b53, /// 0xa80
			 0x6ef20936, /// 0xa84
			 0x5b449886, /// 0xa88
			 0x988838b6, /// 0xa8c
			 0x9363066d, /// 0xa90
			 0x47e4ad0d, /// 0xa94
			 0x71767ea0, /// 0xa98
			 0x84c2400b, /// 0xa9c
			 0x95f89931, /// 0xaa0
			 0xf965572e, /// 0xaa4
			 0x43f5f83c, /// 0xaa8
			 0x64ab98af, /// 0xaac
			 0xf6992adf, /// 0xab0
			 0x2806bb97, /// 0xab4
			 0x795c7a6e, /// 0xab8
			 0x8cb88c26, /// 0xabc
			 0xcf1a4360, /// 0xac0
			 0x04b3b9e3, /// 0xac4
			 0x46c2390b, /// 0xac8
			 0x927045ca, /// 0xacc
			 0x5c1666e3, /// 0xad0
			 0xffeae5fb, /// 0xad4
			 0x28d64348, /// 0xad8
			 0x751c9245, /// 0xadc
			 0x5f8454b0, /// 0xae0
			 0xda38fe20, /// 0xae4
			 0x755642ff, /// 0xae8
			 0x5315b98b, /// 0xaec
			 0x2a372b44, /// 0xaf0
			 0xc8f47d67, /// 0xaf4
			 0x7887b64b, /// 0xaf8
			 0xe133e342, /// 0xafc
			 0xc5efabb7, /// 0xb00
			 0x6abb7310, /// 0xb04
			 0xa88c212e, /// 0xb08
			 0x419f4499, /// 0xb0c
			 0x1a7091d7, /// 0xb10
			 0x06c095ce, /// 0xb14
			 0x0041e955, /// 0xb18
			 0x42e97ca1, /// 0xb1c
			 0x6de4a843, /// 0xb20
			 0x7e0687c4, /// 0xb24
			 0x80be6d39, /// 0xb28
			 0x0462165b, /// 0xb2c
			 0x2ac48004, /// 0xb30
			 0x7212a01c, /// 0xb34
			 0x8d372266, /// 0xb38
			 0x8f988872, /// 0xb3c
			 0x8f6758d4, /// 0xb40
			 0x8897c9e3, /// 0xb44
			 0x600a8161, /// 0xb48
			 0x1dbf0c3f, /// 0xb4c
			 0x9713b16b, /// 0xb50
			 0x6c05ff87, /// 0xb54
			 0x36328a96, /// 0xb58
			 0xbb4c6cf2, /// 0xb5c
			 0xb40f74bd, /// 0xb60
			 0xac57d365, /// 0xb64
			 0x7c51db2b, /// 0xb68
			 0xac42eef7, /// 0xb6c
			 0x4536cab6, /// 0xb70
			 0x3eedb8fa, /// 0xb74
			 0x3f54a3d8, /// 0xb78
			 0x22454e62, /// 0xb7c
			 0x79bf4a00, /// 0xb80
			 0x0637ef0c, /// 0xb84
			 0xdef68c1a, /// 0xb88
			 0x45ab6603, /// 0xb8c
			 0xabfcfcd5, /// 0xb90
			 0x5f6f5b05, /// 0xb94
			 0xe69dbb5f, /// 0xb98
			 0x20256391, /// 0xb9c
			 0xe14702c1, /// 0xba0
			 0x1180985e, /// 0xba4
			 0x2d7a522e, /// 0xba8
			 0xad696a40, /// 0xbac
			 0x6f1a24e9, /// 0xbb0
			 0xc7a1e247, /// 0xbb4
			 0xa279f055, /// 0xbb8
			 0x8e1c3db3, /// 0xbbc
			 0xd868622d, /// 0xbc0
			 0x9b7eebea, /// 0xbc4
			 0x4d48a5ca, /// 0xbc8
			 0x5f72a43a, /// 0xbcc
			 0x585a8447, /// 0xbd0
			 0x38a5e30d, /// 0xbd4
			 0x1eb68c83, /// 0xbd8
			 0x332a3330, /// 0xbdc
			 0xe4650b5e, /// 0xbe0
			 0x8d1650da, /// 0xbe4
			 0x74b830b2, /// 0xbe8
			 0x2bbab3e9, /// 0xbec
			 0x42b4184a, /// 0xbf0
			 0x3654d004, /// 0xbf4
			 0x088ea8be, /// 0xbf8
			 0x6ee0614e, /// 0xbfc
			 0xae931460, /// 0xc00
			 0x2b36f753, /// 0xc04
			 0xe04feb44, /// 0xc08
			 0xdc99f664, /// 0xc0c
			 0x431e9928, /// 0xc10
			 0x93343ca4, /// 0xc14
			 0xddce3c63, /// 0xc18
			 0xa725604b, /// 0xc1c
			 0x998ab584, /// 0xc20
			 0x675c4f38, /// 0xc24
			 0x29e02136, /// 0xc28
			 0x9ffa6894, /// 0xc2c
			 0x47d6fd02, /// 0xc30
			 0xf2fff863, /// 0xc34
			 0x010fa04d, /// 0xc38
			 0x92df3229, /// 0xc3c
			 0xaea5894a, /// 0xc40
			 0xed7e76a8, /// 0xc44
			 0xad03ca4c, /// 0xc48
			 0xa58c5f39, /// 0xc4c
			 0x1bef57a6, /// 0xc50
			 0xc1da50cc, /// 0xc54
			 0x462a918f, /// 0xc58
			 0x3ea43353, /// 0xc5c
			 0x741754d5, /// 0xc60
			 0x824b84df, /// 0xc64
			 0x2114344b, /// 0xc68
			 0x57f889a6, /// 0xc6c
			 0xd2b4caf2, /// 0xc70
			 0xa5b206bd, /// 0xc74
			 0x2dbb0fb5, /// 0xc78
			 0x131df3c6, /// 0xc7c
			 0xb65fc484, /// 0xc80
			 0x19ef70bd, /// 0xc84
			 0x2225bd33, /// 0xc88
			 0x59301dc1, /// 0xc8c
			 0x60812a80, /// 0xc90
			 0x39159674, /// 0xc94
			 0x45564656, /// 0xc98
			 0x5846d712, /// 0xc9c
			 0x8d7e5e18, /// 0xca0
			 0x0955c549, /// 0xca4
			 0x4847e73e, /// 0xca8
			 0x069fb242, /// 0xcac
			 0xcf1c63e2, /// 0xcb0
			 0x4c2792a5, /// 0xcb4
			 0xccc50421, /// 0xcb8
			 0x458a5dca, /// 0xcbc
			 0x0a0c44ad, /// 0xcc0
			 0x66b42f6f, /// 0xcc4
			 0x948d8e41, /// 0xcc8
			 0x64906ed7, /// 0xccc
			 0xdb1974ba, /// 0xcd0
			 0xfa934c1c, /// 0xcd4
			 0x1d401eb1, /// 0xcd8
			 0x3d0d2469, /// 0xcdc
			 0xd9b86712, /// 0xce0
			 0xfc4e7aa9, /// 0xce4
			 0x918f57ab, /// 0xce8
			 0x09715681, /// 0xcec
			 0xdaddc973, /// 0xcf0
			 0xe4a94697, /// 0xcf4
			 0xfaccc5f2, /// 0xcf8
			 0x9c97af76, /// 0xcfc
			 0xd28faf3b, /// 0xd00
			 0xe84a9ac9, /// 0xd04
			 0x155ee07e, /// 0xd08
			 0x355b814f, /// 0xd0c
			 0xd4e60b0b, /// 0xd10
			 0x36bc8579, /// 0xd14
			 0x3b2b5a72, /// 0xd18
			 0x34b1e79f, /// 0xd1c
			 0xa8022550, /// 0xd20
			 0xf48e02f5, /// 0xd24
			 0x661b6383, /// 0xd28
			 0x9e20d159, /// 0xd2c
			 0xd03f3bbd, /// 0xd30
			 0x62abb632, /// 0xd34
			 0xbc3a9650, /// 0xd38
			 0xfa7ac95b, /// 0xd3c
			 0x21d65390, /// 0xd40
			 0x204c001f, /// 0xd44
			 0x332ebba3, /// 0xd48
			 0x329b64d2, /// 0xd4c
			 0xb3abfd07, /// 0xd50
			 0x540d4577, /// 0xd54
			 0x980d704e, /// 0xd58
			 0x28e663a5, /// 0xd5c
			 0x44090a3f, /// 0xd60
			 0x0cca8a13, /// 0xd64
			 0xb10ae5a1, /// 0xd68
			 0xc50e3a6b, /// 0xd6c
			 0xe5ceaa61, /// 0xd70
			 0xd461f0fb, /// 0xd74
			 0x9f7cacf4, /// 0xd78
			 0x6b979449, /// 0xd7c
			 0x8620fd85, /// 0xd80
			 0x259fdc41, /// 0xd84
			 0xe8321c3d, /// 0xd88
			 0x2957c80e, /// 0xd8c
			 0x50d1fb8b, /// 0xd90
			 0x9b0a5f64, /// 0xd94
			 0xfde7bbaf, /// 0xd98
			 0x018c61fd, /// 0xd9c
			 0x884e62f3, /// 0xda0
			 0x5028e4aa, /// 0xda4
			 0x6054fa98, /// 0xda8
			 0x721f0c98, /// 0xdac
			 0x7a8c830e, /// 0xdb0
			 0x7f399a57, /// 0xdb4
			 0xf388fb4d, /// 0xdb8
			 0xd05ff444, /// 0xdbc
			 0x83a2fd44, /// 0xdc0
			 0xdd5c988f, /// 0xdc4
			 0x319e466c, /// 0xdc8
			 0xb283cd82, /// 0xdcc
			 0xb75a880e, /// 0xdd0
			 0x42142446, /// 0xdd4
			 0x6879432f, /// 0xdd8
			 0x174489a7, /// 0xddc
			 0x6910ab1d, /// 0xde0
			 0x1f474df5, /// 0xde4
			 0x03464d63, /// 0xde8
			 0xaf0aa58c, /// 0xdec
			 0x77ab31a0, /// 0xdf0
			 0x6a5a2eca, /// 0xdf4
			 0xd5f520af, /// 0xdf8
			 0xbfa64492, /// 0xdfc
			 0x89dead16, /// 0xe00
			 0x3a55c789, /// 0xe04
			 0xfb4dc868, /// 0xe08
			 0xc13cda2a, /// 0xe0c
			 0xeb95041b, /// 0xe10
			 0x347bb73a, /// 0xe14
			 0x3e21a465, /// 0xe18
			 0x9e50f27c, /// 0xe1c
			 0xe716dcad, /// 0xe20
			 0xcbeb4ad8, /// 0xe24
			 0x363beaad, /// 0xe28
			 0xee4e8b63, /// 0xe2c
			 0x3b9ec562, /// 0xe30
			 0x5fd77ea8, /// 0xe34
			 0x3df32e88, /// 0xe38
			 0xbfa7951e, /// 0xe3c
			 0x990374e6, /// 0xe40
			 0x6c437c69, /// 0xe44
			 0x46c83303, /// 0xe48
			 0x1ef03f59, /// 0xe4c
			 0x2e0ab3fb, /// 0xe50
			 0xd2a63660, /// 0xe54
			 0x9b9da632, /// 0xe58
			 0x18e56a3c, /// 0xe5c
			 0x52873533, /// 0xe60
			 0x7c997236, /// 0xe64
			 0x658cb2f7, /// 0xe68
			 0xb6c0c5f1, /// 0xe6c
			 0xde764f41, /// 0xe70
			 0xe3fe7006, /// 0xe74
			 0x69fcef8c, /// 0xe78
			 0x81d42b65, /// 0xe7c
			 0xe5eb5043, /// 0xe80
			 0x32fd118e, /// 0xe84
			 0x819399af, /// 0xe88
			 0xf36d6a0e, /// 0xe8c
			 0x7580386d, /// 0xe90
			 0x0a18e8d7, /// 0xe94
			 0x313394f0, /// 0xe98
			 0x58c559a4, /// 0xe9c
			 0xc51da4d6, /// 0xea0
			 0xdcdc61f9, /// 0xea4
			 0xab070601, /// 0xea8
			 0xd0a1b21f, /// 0xeac
			 0x2922cce3, /// 0xeb0
			 0x49fec52d, /// 0xeb4
			 0xb2a6ba6f, /// 0xeb8
			 0xb33bfaaa, /// 0xebc
			 0xbaad322b, /// 0xec0
			 0x1b9cd193, /// 0xec4
			 0xd3fd2ef6, /// 0xec8
			 0x4a7bab3f, /// 0xecc
			 0x11dadf32, /// 0xed0
			 0x89ca28b9, /// 0xed4
			 0x6b0b966f, /// 0xed8
			 0xa748b3b0, /// 0xedc
			 0xedc3971b, /// 0xee0
			 0x494f9a5c, /// 0xee4
			 0x8e844a36, /// 0xee8
			 0x11f44ee8, /// 0xeec
			 0x9a746d2c, /// 0xef0
			 0xc0dc8ca3, /// 0xef4
			 0xbe7034df, /// 0xef8
			 0x8fcd3463, /// 0xefc
			 0x4db8cca5, /// 0xf00
			 0x7855d91c, /// 0xf04
			 0xd32152b8, /// 0xf08
			 0x2faf896f, /// 0xf0c
			 0x61f0a5be, /// 0xf10
			 0xf370f03b, /// 0xf14
			 0x138936fd, /// 0xf18
			 0x576219df, /// 0xf1c
			 0x337e5f30, /// 0xf20
			 0xd39b1083, /// 0xf24
			 0xa42b8d01, /// 0xf28
			 0x07564e83, /// 0xf2c
			 0xc868129c, /// 0xf30
			 0x07d93d1b, /// 0xf34
			 0xaf5bb2dd, /// 0xf38
			 0x03ddd913, /// 0xf3c
			 0x72d1c63e, /// 0xf40
			 0xa77d1f43, /// 0xf44
			 0x8c4ca08e, /// 0xf48
			 0xcc626cd6, /// 0xf4c
			 0xd25e305c, /// 0xf50
			 0x908e05ff, /// 0xf54
			 0x9296f901, /// 0xf58
			 0xce11f869, /// 0xf5c
			 0xb107fe67, /// 0xf60
			 0x6677fdad, /// 0xf64
			 0xef6d23c2, /// 0xf68
			 0xbfd94f02, /// 0xf6c
			 0x4a08b093, /// 0xf70
			 0x39b3aa4b, /// 0xf74
			 0x138f6c79, /// 0xf78
			 0x21677c90, /// 0xf7c
			 0x533ccd93, /// 0xf80
			 0x1884ed46, /// 0xf84
			 0x04e5b937, /// 0xf88
			 0x36d2e5e4, /// 0xf8c
			 0x8b883131, /// 0xf90
			 0xfc267de8, /// 0xf94
			 0x1adfb114, /// 0xf98
			 0x7090538c, /// 0xf9c
			 0x7b2bdf29, /// 0xfa0
			 0x05b7d36b, /// 0xfa4
			 0x23c81066, /// 0xfa8
			 0x4d5d7349, /// 0xfac
			 0xd4ac677c, /// 0xfb0
			 0x8b9990c2, /// 0xfb4
			 0xd2b106ad, /// 0xfb8
			 0x0f8d73c3, /// 0xfbc
			 0x0df1da52, /// 0xfc0
			 0xff01f93b, /// 0xfc4
			 0xae3b6419, /// 0xfc8
			 0xeef7fc86, /// 0xfcc
			 0xd0274872, /// 0xfd0
			 0xcd7871f7, /// 0xfd4
			 0x6959ca08, /// 0xfd8
			 0x1fce888d, /// 0xfdc
			 0xe4e9a01a, /// 0xfe0
			 0xf865668c, /// 0xfe4
			 0xb06be67a, /// 0xfe8
			 0x79195acd, /// 0xfec
			 0xeffcd447, /// 0xff0
			 0x6fc88993, /// 0xff4
			 0x1c03221e, /// 0xff8
			 0x09f0e445 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00001,                                                  // signaling NaN                               /// 00000
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00004
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00008
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0000c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00010
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0001c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00024
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0002c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00030
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00038
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0003c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00048
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0004c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00054
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00058
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0005c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00060
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0xbf028f5c,                                                  // -0.51                                       /// 00068
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0006c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00070
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00078
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0xcb000000,                                                  // -8388608.0                                  /// 00080
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00084
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00088
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00094
			 0x0c780000,                                                  // Leading 1s:                                 /// 00098
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0009c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x33333333,                                                  // 4 random values                             /// 000a4
			 0x55555555,                                                  // 4 random values                             /// 000a8
			 0xcb000000,                                                  // -8388608.0                                  /// 000ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00100
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00104
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00108
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0010c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00130
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0c780000,                                                  // Leading 1s:                                 /// 00138
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00140
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00150
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00154
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00158
			 0x80011111,                                                  // -9.7958E-41                                 /// 0015c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00160
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00164
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00168
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0016c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00174
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00178
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xffc00001,                                                  // -signaling NaN                              /// 00180
			 0x55555555,                                                  // 4 random values                             /// 00184
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00188
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0018c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00190
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00194
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00198
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0019c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a4
			 0xbf028f5c,                                                  // -0.51                                       /// 001a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00200
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00204
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00208
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00210
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00214
			 0x0c400000,                                                  // Leading 1s:                                 /// 00218
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0021c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00230
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00234
			 0xff800000,                                                  // -inf                                        /// 00238
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0023c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00240
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00244
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00248
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0024c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00254
			 0x0e000001,                                                  // Trailing 1s:                                /// 00258
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0025c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00268
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0026c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00270
			 0xcb000000,                                                  // -8388608.0                                  /// 00274
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00278
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00280
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00288
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00294
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0029c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x33333333,                                                  // 4 random values                             /// 002a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002dc
			 0x7fc00001,                                                  // signaling NaN                               /// 002e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002fc
			 0x7f800000,                                                  // inf                                         /// 00300
			 0xcb000000,                                                  // -8388608.0                                  /// 00304
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00310
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0032c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00334
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00338
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0033c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00340
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00344
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00348
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0034c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00011111,                                                  // 9.7958E-41                                  /// 00354
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00358
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0035c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00360
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00364
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0036c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00370
			 0x4b000000,                                                  // 8388608.0                                   /// 00374
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00378
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00380
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e000007,                                                  // Trailing 1s:                                /// 0038c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00408
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0040c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0041c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00420
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00428
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0042c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00430
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00434
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0043c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00440
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00444
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00448
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0044c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00450
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00454
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00458
			 0x00011111,                                                  // 9.7958E-41                                  /// 0045c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00460
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00468
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0046c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00470
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00474
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00478
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0047c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00480
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00484
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00488
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0048c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00490
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00494
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0049c
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004c0
			 0x80000000,                                                  // -zero                                       /// 004c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0xff800000,                                                  // -inf                                        /// 004d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7fc00001,                                                  // signaling NaN                               /// 004e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00504
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00518
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00011111,                                                  // 9.7958E-41                                  /// 00524
			 0x0c780000,                                                  // Leading 1s:                                 /// 00528
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0052c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00534
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00538
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0053c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00540
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00548
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0054c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00554
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00558
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00560
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00564
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0056c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00570
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000000,                                                  // zero                                        /// 00584
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00588
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00594
			 0x0e000007,                                                  // Trailing 1s:                                /// 00598
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a0
			 0x7f800000,                                                  // inf                                         /// 005a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0xcb000000,                                                  // -8388608.0                                  /// 005b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xffc00001,                                                  // -signaling NaN                              /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d8
			 0x80000000,                                                  // -zero                                       /// 005dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00600
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00604
			 0x55555555,                                                  // 4 random values                             /// 00608
			 0x33333333,                                                  // 4 random values                             /// 0060c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00618
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00620
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00624
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00628
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00630
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00634
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00640
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00644
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00648
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x4b000000,                                                  // 8388608.0                                   /// 00654
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00660
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00664
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00668
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0066c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00670
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00674
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00678
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0067c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0068c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00690
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00694
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00698
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0069c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006a4
			 0x4b000000,                                                  // 8388608.0                                   /// 006a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x3f028f5c,                                                  // 0.51                                        /// 006b8
			 0x00000000,                                                  // zero                                        /// 006bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d0
			 0x80000000,                                                  // -zero                                       /// 006d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006ec
			 0x33333333,                                                  // 4 random values                             /// 006f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00700
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00708
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00710
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00714
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00718
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0072c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00730
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00734
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00738
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0073c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00748
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00754
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00758
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0075c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00770
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00774
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00778
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0077c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00784
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0078c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00790
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x7f800000,                                                  // inf                                         /// 00798
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007b8
			 0xff800000,                                                  // -inf                                        /// 007bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00804
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00808
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0080c
			 0x7fc00001,                                                  // signaling NaN                               /// 00810
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00814
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00818
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0081c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x3f028f5c,                                                  // 0.51                                        /// 00824
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0082c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00838
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0083c
			 0x00000000,                                                  // zero                                        /// 00840
			 0xcb000000,                                                  // -8388608.0                                  /// 00844
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00848
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0084c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00850
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00854
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00858
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0085c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00868
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0086c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c780000,                                                  // Leading 1s:                                 /// 00874
			 0x0c780000,                                                  // Leading 1s:                                 /// 00878
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0087c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00880
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00884
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00888
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00898
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0089c
			 0x0e000003,                                                  // Trailing 1s:                                /// 008a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00900
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0090c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00910
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00914
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0091c
			 0x4b000000,                                                  // 8388608.0                                   /// 00920
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00930
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00934
			 0x7f800000,                                                  // inf                                         /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0093c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00940
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00944
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0094c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00950
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00954
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00958
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0095c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00968
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0096c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00970
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00974
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00978
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00980
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0098c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00990
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00998
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0099c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b0
			 0xcb000000,                                                  // -8388608.0                                  /// 009b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0xcb000000,                                                  // -8388608.0                                  /// 009d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d8
			 0x00000000,                                                  // zero                                        /// 009dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x7f800000,                                                  // inf                                         /// 00a14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x33333333,                                                  // 4 random values                             /// 00a44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x55555555,                                                  // 4 random values                             /// 00ab4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ab8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ac0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00acc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00adc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ae8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00af0
			 0x00000000,                                                  // zero                                        /// 00af4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b08
			 0x80000000,                                                  // -zero                                       /// 00b0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b80
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b94
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b98
			 0x7f800000,                                                  // inf                                         /// 00b9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ba4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bcc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c2c
			 0x7f800000,                                                  // inf                                         /// 00c30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c68
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7fc00001,                                                  // signaling NaN                               /// 00c8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cf8
			 0xff800000,                                                  // -inf                                        /// 00cfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000000,                                                  // -zero                                       /// 00d54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00db8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00de4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00df8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x80000000,                                                  // -zero                                       /// 00e10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e24
			 0xff800000,                                                  // -inf                                        /// 00e28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e9c
			 0x55555555,                                                  // 4 random values                             /// 00ea0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00eb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ec8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ecc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ed0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ed8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00edc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00eec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00efc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f10
			 0x00000000,                                                  // zero                                        /// 00f14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80000000,                                                  // -zero                                       /// 00f5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0xffc00001,                                                  // -signaling NaN                              /// 00f74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f90
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff8
			 0x0e7fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x9d237bb1, /// 0x0
			 0xbc9d1312, /// 0x4
			 0xc18ae020, /// 0x8
			 0xce8201aa, /// 0xc
			 0x01a154c0, /// 0x10
			 0xb8eb980d, /// 0x14
			 0xdc4e3103, /// 0x18
			 0x4124bc1c, /// 0x1c
			 0xebf90f4b, /// 0x20
			 0x8c69e125, /// 0x24
			 0xd2c31c96, /// 0x28
			 0x190991c1, /// 0x2c
			 0x0e4badff, /// 0x30
			 0x16e31923, /// 0x34
			 0x5ef60d07, /// 0x38
			 0x1e56a67a, /// 0x3c
			 0x362a019a, /// 0x40
			 0x2781ee46, /// 0x44
			 0x9cb4b1cb, /// 0x48
			 0x9fa656a8, /// 0x4c
			 0xbbccae1a, /// 0x50
			 0xe5254a23, /// 0x54
			 0xed8cfe5f, /// 0x58
			 0xd5aac2e2, /// 0x5c
			 0xa73f988a, /// 0x60
			 0xe4b3828e, /// 0x64
			 0x12144b77, /// 0x68
			 0xe485f106, /// 0x6c
			 0xe0aa6a22, /// 0x70
			 0xf0084705, /// 0x74
			 0x8fd55fc6, /// 0x78
			 0xb8a4cf51, /// 0x7c
			 0xe5ad8498, /// 0x80
			 0xa048d856, /// 0x84
			 0xd245c860, /// 0x88
			 0x7659ffcf, /// 0x8c
			 0x08da8f8b, /// 0x90
			 0xd5c158ec, /// 0x94
			 0x56433c97, /// 0x98
			 0x27f4eed8, /// 0x9c
			 0xa0ba3d59, /// 0xa0
			 0x44d21186, /// 0xa4
			 0xee53a5e1, /// 0xa8
			 0xbd83e93a, /// 0xac
			 0x559877ac, /// 0xb0
			 0x6337e485, /// 0xb4
			 0xcdced5d6, /// 0xb8
			 0x5d7b9319, /// 0xbc
			 0xffe1a616, /// 0xc0
			 0x9dde4bdd, /// 0xc4
			 0x68577fcb, /// 0xc8
			 0x595f06ed, /// 0xcc
			 0x42fa9934, /// 0xd0
			 0x7b4faf5c, /// 0xd4
			 0xba25df43, /// 0xd8
			 0x8830628d, /// 0xdc
			 0xe269756a, /// 0xe0
			 0x9fea7fc5, /// 0xe4
			 0x1686e8c3, /// 0xe8
			 0x16221f8b, /// 0xec
			 0xc8b9b4fb, /// 0xf0
			 0x6b51086d, /// 0xf4
			 0x9bc38f0b, /// 0xf8
			 0x480797a1, /// 0xfc
			 0xbe7b130a, /// 0x100
			 0x2625372d, /// 0x104
			 0xdc1fa5fa, /// 0x108
			 0x7857c628, /// 0x10c
			 0x81a2404d, /// 0x110
			 0x097d2c60, /// 0x114
			 0x093d4fcc, /// 0x118
			 0x7c1cd671, /// 0x11c
			 0x2d1769e6, /// 0x120
			 0xd0565971, /// 0x124
			 0xc324a45b, /// 0x128
			 0x79f83b95, /// 0x12c
			 0x3457984a, /// 0x130
			 0xaa963c7f, /// 0x134
			 0x154a4d20, /// 0x138
			 0x732a26c7, /// 0x13c
			 0x76f74bf8, /// 0x140
			 0xf145f19e, /// 0x144
			 0x33c5235a, /// 0x148
			 0x7005efd6, /// 0x14c
			 0x497c27f6, /// 0x150
			 0xec5b5579, /// 0x154
			 0x938e621d, /// 0x158
			 0x2657ac30, /// 0x15c
			 0xdbd046e9, /// 0x160
			 0xb9bff357, /// 0x164
			 0x08ece67f, /// 0x168
			 0xc4a483b1, /// 0x16c
			 0x4daad6e3, /// 0x170
			 0x8c0e606f, /// 0x174
			 0xa360c3b5, /// 0x178
			 0x5c067159, /// 0x17c
			 0x863f572e, /// 0x180
			 0xa92a6603, /// 0x184
			 0x6345dc68, /// 0x188
			 0xc348bf36, /// 0x18c
			 0x971b4824, /// 0x190
			 0x5d23e160, /// 0x194
			 0x74c29ac9, /// 0x198
			 0x427cec49, /// 0x19c
			 0x2e55b8f0, /// 0x1a0
			 0x10db0b5b, /// 0x1a4
			 0x9d42c3d0, /// 0x1a8
			 0xb2f2b65c, /// 0x1ac
			 0x4306b524, /// 0x1b0
			 0x4d936963, /// 0x1b4
			 0xedf38fdc, /// 0x1b8
			 0x3c1fba94, /// 0x1bc
			 0x97b37890, /// 0x1c0
			 0x326b26bc, /// 0x1c4
			 0x7e398cc8, /// 0x1c8
			 0xc32efff8, /// 0x1cc
			 0xd906db21, /// 0x1d0
			 0x9aacf1ba, /// 0x1d4
			 0x525410c2, /// 0x1d8
			 0x822edf8d, /// 0x1dc
			 0xe50f7186, /// 0x1e0
			 0x1a7963af, /// 0x1e4
			 0xcfd8a8c1, /// 0x1e8
			 0xf791c0ee, /// 0x1ec
			 0xce9a2182, /// 0x1f0
			 0x6bb39403, /// 0x1f4
			 0xaaf22a33, /// 0x1f8
			 0x533a7977, /// 0x1fc
			 0x0ce7ea72, /// 0x200
			 0xb4bbea73, /// 0x204
			 0x8974faee, /// 0x208
			 0x84d446bd, /// 0x20c
			 0x07c13514, /// 0x210
			 0x5783579c, /// 0x214
			 0x720a5d36, /// 0x218
			 0xf35401be, /// 0x21c
			 0x0bbf5060, /// 0x220
			 0x7aa39ded, /// 0x224
			 0x68c05812, /// 0x228
			 0xafca4b4d, /// 0x22c
			 0x3e46fdb5, /// 0x230
			 0xf8b5fe22, /// 0x234
			 0xe3d7515a, /// 0x238
			 0x86b70c34, /// 0x23c
			 0x9a994dda, /// 0x240
			 0x6abd91a9, /// 0x244
			 0xb43c507e, /// 0x248
			 0xac8d5581, /// 0x24c
			 0x3d87d233, /// 0x250
			 0x17b92fd1, /// 0x254
			 0x2d5c8883, /// 0x258
			 0xee89ea7d, /// 0x25c
			 0x3656268f, /// 0x260
			 0x7353ffcd, /// 0x264
			 0xd3dd86b3, /// 0x268
			 0x774f699d, /// 0x26c
			 0xf0702be4, /// 0x270
			 0x64422bac, /// 0x274
			 0x09c896d6, /// 0x278
			 0xd8cf3aaa, /// 0x27c
			 0x46be70b1, /// 0x280
			 0x882ef973, /// 0x284
			 0x4c71ac91, /// 0x288
			 0xe7065e86, /// 0x28c
			 0x88115189, /// 0x290
			 0x475e1850, /// 0x294
			 0xf1a3f76a, /// 0x298
			 0xf4759801, /// 0x29c
			 0x2dc8aff4, /// 0x2a0
			 0xbd0fa5f9, /// 0x2a4
			 0xf57e1413, /// 0x2a8
			 0xecf3bdd8, /// 0x2ac
			 0xc624cb53, /// 0x2b0
			 0xef23f758, /// 0x2b4
			 0x81a854f5, /// 0x2b8
			 0xd44c20fb, /// 0x2bc
			 0xe6bb6205, /// 0x2c0
			 0x3e83b470, /// 0x2c4
			 0xfa3a23a4, /// 0x2c8
			 0xcadafd96, /// 0x2cc
			 0xcb4828e5, /// 0x2d0
			 0xad1d0d6d, /// 0x2d4
			 0x890566b3, /// 0x2d8
			 0x162094f1, /// 0x2dc
			 0x1123b7a6, /// 0x2e0
			 0x9b78d650, /// 0x2e4
			 0x48781fab, /// 0x2e8
			 0x3c75e303, /// 0x2ec
			 0x346c8754, /// 0x2f0
			 0x56cff58c, /// 0x2f4
			 0x235039df, /// 0x2f8
			 0x1ccc4ecb, /// 0x2fc
			 0x1139e54c, /// 0x300
			 0x387f826d, /// 0x304
			 0x8e4ac8e7, /// 0x308
			 0xbdebdd89, /// 0x30c
			 0x860d6cbe, /// 0x310
			 0x25bbb593, /// 0x314
			 0x4bec6d4e, /// 0x318
			 0x32810565, /// 0x31c
			 0x3130754f, /// 0x320
			 0x52177ae8, /// 0x324
			 0x30b3f606, /// 0x328
			 0xed9f444f, /// 0x32c
			 0x4714ee31, /// 0x330
			 0xbf49c41f, /// 0x334
			 0x92dbafcd, /// 0x338
			 0x784ef0de, /// 0x33c
			 0x5ddd33bd, /// 0x340
			 0xd71a8aef, /// 0x344
			 0x56aca045, /// 0x348
			 0x242c8a5d, /// 0x34c
			 0x812f2d79, /// 0x350
			 0x54cc8db3, /// 0x354
			 0x6237d490, /// 0x358
			 0x587bb007, /// 0x35c
			 0xb777f517, /// 0x360
			 0xe0961346, /// 0x364
			 0x2e81706f, /// 0x368
			 0xaacf3dbd, /// 0x36c
			 0x3d2d460e, /// 0x370
			 0x29999427, /// 0x374
			 0x83115bb9, /// 0x378
			 0x93a4e5c4, /// 0x37c
			 0x24a9c037, /// 0x380
			 0xcfa2abf3, /// 0x384
			 0x45c13289, /// 0x388
			 0x370a5892, /// 0x38c
			 0x3fb3d742, /// 0x390
			 0x1e2ff95a, /// 0x394
			 0x0923de1e, /// 0x398
			 0x1a02081a, /// 0x39c
			 0xca922a97, /// 0x3a0
			 0xfaedba32, /// 0x3a4
			 0x48b4a667, /// 0x3a8
			 0x7476b784, /// 0x3ac
			 0xec6bb735, /// 0x3b0
			 0x054ec623, /// 0x3b4
			 0x07efce01, /// 0x3b8
			 0xa1391315, /// 0x3bc
			 0x8367400a, /// 0x3c0
			 0x6d25d017, /// 0x3c4
			 0xc33c4132, /// 0x3c8
			 0xbf2d2f95, /// 0x3cc
			 0xc468551c, /// 0x3d0
			 0xe3dc2ae9, /// 0x3d4
			 0x5bec184d, /// 0x3d8
			 0x8067b3c5, /// 0x3dc
			 0x68ff2549, /// 0x3e0
			 0xd769d7ea, /// 0x3e4
			 0xcb78c3da, /// 0x3e8
			 0xc7334eca, /// 0x3ec
			 0x797e882e, /// 0x3f0
			 0x180a2d27, /// 0x3f4
			 0x5631b06b, /// 0x3f8
			 0xfd52b4e5, /// 0x3fc
			 0xb74bf84d, /// 0x400
			 0x6baa6f73, /// 0x404
			 0x8a1254f6, /// 0x408
			 0x046adff3, /// 0x40c
			 0x34d690fb, /// 0x410
			 0xbbcc378c, /// 0x414
			 0x97e21622, /// 0x418
			 0xb272d4f4, /// 0x41c
			 0xfdf820f9, /// 0x420
			 0x0b373fb9, /// 0x424
			 0x1a131eb1, /// 0x428
			 0x0af597c5, /// 0x42c
			 0x0e5ea629, /// 0x430
			 0xc146bda3, /// 0x434
			 0xe3bec404, /// 0x438
			 0x42cb0bfd, /// 0x43c
			 0x9c8f786f, /// 0x440
			 0x9e1479a9, /// 0x444
			 0xa8aa0646, /// 0x448
			 0xbf4fcac2, /// 0x44c
			 0xfa39bc99, /// 0x450
			 0x82bc37d1, /// 0x454
			 0xd74fa434, /// 0x458
			 0xbcd1d49c, /// 0x45c
			 0x12eb200c, /// 0x460
			 0x7ddb5679, /// 0x464
			 0x3409417e, /// 0x468
			 0xdea060e7, /// 0x46c
			 0x70c65da7, /// 0x470
			 0x16a10cda, /// 0x474
			 0x60f12bd2, /// 0x478
			 0xf5468431, /// 0x47c
			 0x2eec9219, /// 0x480
			 0x27ce22d3, /// 0x484
			 0xde495697, /// 0x488
			 0xb5011eb2, /// 0x48c
			 0xa4a71694, /// 0x490
			 0xe86367ca, /// 0x494
			 0x5f00adeb, /// 0x498
			 0x2afb6b0a, /// 0x49c
			 0xcc493610, /// 0x4a0
			 0x8328db50, /// 0x4a4
			 0xa62db581, /// 0x4a8
			 0x7d29de45, /// 0x4ac
			 0x4382a0dc, /// 0x4b0
			 0x02372ac5, /// 0x4b4
			 0x2be3b494, /// 0x4b8
			 0xa132dc0c, /// 0x4bc
			 0xc42c9494, /// 0x4c0
			 0xd867b881, /// 0x4c4
			 0x4a49c99e, /// 0x4c8
			 0x2f1c9b95, /// 0x4cc
			 0x06170409, /// 0x4d0
			 0x9e25dd31, /// 0x4d4
			 0x5b3b0453, /// 0x4d8
			 0xbbcec7dc, /// 0x4dc
			 0xa501534e, /// 0x4e0
			 0xe1497866, /// 0x4e4
			 0x46dfd18d, /// 0x4e8
			 0x9260643b, /// 0x4ec
			 0x57a0064b, /// 0x4f0
			 0xa296a801, /// 0x4f4
			 0x6520f5ba, /// 0x4f8
			 0xce3fc50d, /// 0x4fc
			 0x53e3c4d7, /// 0x500
			 0x2ffc4326, /// 0x504
			 0x1be556e3, /// 0x508
			 0xf689bf7d, /// 0x50c
			 0x5dd54242, /// 0x510
			 0x7dbdfd16, /// 0x514
			 0x043cf8df, /// 0x518
			 0xfe6bb3b3, /// 0x51c
			 0x2b676855, /// 0x520
			 0x496bb1ef, /// 0x524
			 0xd2e92e90, /// 0x528
			 0x4c662d0d, /// 0x52c
			 0x365fd3e4, /// 0x530
			 0x8afd6923, /// 0x534
			 0xf1038fa4, /// 0x538
			 0xc1c92ece, /// 0x53c
			 0xd40e3d1e, /// 0x540
			 0x737113b0, /// 0x544
			 0x028cfa0c, /// 0x548
			 0xa4081488, /// 0x54c
			 0x255c3a0a, /// 0x550
			 0xc6ea07aa, /// 0x554
			 0x6eb4c064, /// 0x558
			 0x662567b8, /// 0x55c
			 0x61aa6344, /// 0x560
			 0x0d9924c7, /// 0x564
			 0xfc6644db, /// 0x568
			 0x7882b92c, /// 0x56c
			 0x5e7a7a2e, /// 0x570
			 0x89759ace, /// 0x574
			 0x5dfe3b05, /// 0x578
			 0x5ecdcf31, /// 0x57c
			 0x89bac25d, /// 0x580
			 0xd1799ad3, /// 0x584
			 0xd53aaf6f, /// 0x588
			 0x06ae27d9, /// 0x58c
			 0x95f505df, /// 0x590
			 0x31e7ce10, /// 0x594
			 0x04bda0b1, /// 0x598
			 0x6a26111a, /// 0x59c
			 0x1ba806ca, /// 0x5a0
			 0x29385920, /// 0x5a4
			 0x3caf1c81, /// 0x5a8
			 0x6862bd74, /// 0x5ac
			 0x36178d02, /// 0x5b0
			 0xf872e1b5, /// 0x5b4
			 0x67b083e1, /// 0x5b8
			 0x315e6e28, /// 0x5bc
			 0x1205a439, /// 0x5c0
			 0x0e96daf7, /// 0x5c4
			 0x1f6505e2, /// 0x5c8
			 0x3bec87ca, /// 0x5cc
			 0x656fba8a, /// 0x5d0
			 0xd7793d31, /// 0x5d4
			 0x12038414, /// 0x5d8
			 0xf727a0c1, /// 0x5dc
			 0xb40f1830, /// 0x5e0
			 0x114d9bb1, /// 0x5e4
			 0xe3fee753, /// 0x5e8
			 0x8e10fa81, /// 0x5ec
			 0xeb7f597c, /// 0x5f0
			 0xceaa053f, /// 0x5f4
			 0x0eb6ba01, /// 0x5f8
			 0xde8feae2, /// 0x5fc
			 0xf4286299, /// 0x600
			 0x811059ec, /// 0x604
			 0x43bfbe36, /// 0x608
			 0xef987b75, /// 0x60c
			 0x8d5ce1a9, /// 0x610
			 0xeb9aed57, /// 0x614
			 0x2d730336, /// 0x618
			 0x3c86f2e6, /// 0x61c
			 0xca8bdb03, /// 0x620
			 0xfc918383, /// 0x624
			 0xf4f445fa, /// 0x628
			 0x0a553190, /// 0x62c
			 0xcf64c2b1, /// 0x630
			 0xc1aeb132, /// 0x634
			 0x9554bdf4, /// 0x638
			 0x7342dc88, /// 0x63c
			 0xc20d25c9, /// 0x640
			 0x0cac265d, /// 0x644
			 0xb2289a37, /// 0x648
			 0x4bfa5578, /// 0x64c
			 0x5c0c07d0, /// 0x650
			 0x79c863f2, /// 0x654
			 0xc81776a8, /// 0x658
			 0x0c25e9db, /// 0x65c
			 0x3795746f, /// 0x660
			 0x22c4ebf6, /// 0x664
			 0xde1a118d, /// 0x668
			 0x0626115f, /// 0x66c
			 0xdc3e47a4, /// 0x670
			 0x117dbfb9, /// 0x674
			 0x9874c169, /// 0x678
			 0x1a2eea90, /// 0x67c
			 0xee8e16fb, /// 0x680
			 0x08b22781, /// 0x684
			 0xcef90dc7, /// 0x688
			 0x166271f1, /// 0x68c
			 0x863f3d15, /// 0x690
			 0xbc80f735, /// 0x694
			 0xe1b235b8, /// 0x698
			 0xed51b5c2, /// 0x69c
			 0x8bfc5b3c, /// 0x6a0
			 0xdaa9338b, /// 0x6a4
			 0x87fdb802, /// 0x6a8
			 0x3dcadfed, /// 0x6ac
			 0xd088e139, /// 0x6b0
			 0xdcfdfdea, /// 0x6b4
			 0xee2b448e, /// 0x6b8
			 0x81690dc8, /// 0x6bc
			 0xa73dbdec, /// 0x6c0
			 0xc14546c2, /// 0x6c4
			 0x847ab2b6, /// 0x6c8
			 0x5b5c1f59, /// 0x6cc
			 0x2c778e7b, /// 0x6d0
			 0x1f17342f, /// 0x6d4
			 0xca83ead4, /// 0x6d8
			 0x63a923b2, /// 0x6dc
			 0x3b5ae739, /// 0x6e0
			 0x1703201b, /// 0x6e4
			 0x516a0d6d, /// 0x6e8
			 0x6eb79662, /// 0x6ec
			 0x64e1184a, /// 0x6f0
			 0xe097024d, /// 0x6f4
			 0x75eed9ce, /// 0x6f8
			 0xbaa41bd8, /// 0x6fc
			 0xae1aeb62, /// 0x700
			 0xc60c2c32, /// 0x704
			 0x89ae8ee7, /// 0x708
			 0x11256db7, /// 0x70c
			 0x1e075237, /// 0x710
			 0xbbc9c57e, /// 0x714
			 0x45e94c00, /// 0x718
			 0xc420d8a1, /// 0x71c
			 0x27c54f9e, /// 0x720
			 0xed0a1ecf, /// 0x724
			 0x72be2e17, /// 0x728
			 0x19393ecd, /// 0x72c
			 0x4dbd1c9a, /// 0x730
			 0x10994132, /// 0x734
			 0xcb9634ba, /// 0x738
			 0x78e85432, /// 0x73c
			 0x1e2a4d1d, /// 0x740
			 0xd6fbadc2, /// 0x744
			 0xc4941551, /// 0x748
			 0x894499dd, /// 0x74c
			 0xcec9a1fd, /// 0x750
			 0x82afe22b, /// 0x754
			 0x8f63c85e, /// 0x758
			 0x7e839c17, /// 0x75c
			 0x4e2cdd2c, /// 0x760
			 0x7ab5c732, /// 0x764
			 0x05346c54, /// 0x768
			 0xb4c9f627, /// 0x76c
			 0xbd80d550, /// 0x770
			 0x0be62c8d, /// 0x774
			 0x14ce699f, /// 0x778
			 0x18815821, /// 0x77c
			 0x972f7fe2, /// 0x780
			 0xd2d85f8a, /// 0x784
			 0x86832316, /// 0x788
			 0x5e596d01, /// 0x78c
			 0x7393a268, /// 0x790
			 0xf970b801, /// 0x794
			 0x99dda126, /// 0x798
			 0x910ac284, /// 0x79c
			 0x988e32d2, /// 0x7a0
			 0xbfd9d646, /// 0x7a4
			 0x8348a0cf, /// 0x7a8
			 0xa0490463, /// 0x7ac
			 0x12dee171, /// 0x7b0
			 0x412d3d87, /// 0x7b4
			 0x62c6b69a, /// 0x7b8
			 0xcc169e29, /// 0x7bc
			 0x3b8c10c3, /// 0x7c0
			 0xe3a44df9, /// 0x7c4
			 0xc449785b, /// 0x7c8
			 0x7d0367e2, /// 0x7cc
			 0xf774436d, /// 0x7d0
			 0x5a9fef91, /// 0x7d4
			 0x9bcf33b0, /// 0x7d8
			 0x74ef53da, /// 0x7dc
			 0x85661b2a, /// 0x7e0
			 0x1679b326, /// 0x7e4
			 0x6e52df34, /// 0x7e8
			 0x1bfdc100, /// 0x7ec
			 0x23190824, /// 0x7f0
			 0x983a25d7, /// 0x7f4
			 0x941e51ff, /// 0x7f8
			 0xd28b67dd, /// 0x7fc
			 0xd6277bac, /// 0x800
			 0xe65b7389, /// 0x804
			 0x35cbe5ca, /// 0x808
			 0xc1fd399f, /// 0x80c
			 0x3bd87cd3, /// 0x810
			 0xb274894a, /// 0x814
			 0xdac96e9c, /// 0x818
			 0x930a80ba, /// 0x81c
			 0xf1f57206, /// 0x820
			 0xd58fc63c, /// 0x824
			 0xbf93e9f0, /// 0x828
			 0xc1f6301f, /// 0x82c
			 0x88c35de1, /// 0x830
			 0xff570e55, /// 0x834
			 0xef70aaf4, /// 0x838
			 0x230ac93f, /// 0x83c
			 0x6f17c970, /// 0x840
			 0xb15f2acd, /// 0x844
			 0xcab40ffc, /// 0x848
			 0x54db88f2, /// 0x84c
			 0x400e6593, /// 0x850
			 0xfa618d1e, /// 0x854
			 0x62da09c9, /// 0x858
			 0xe9b53551, /// 0x85c
			 0xc4edae3d, /// 0x860
			 0x9a009891, /// 0x864
			 0x6646e9b1, /// 0x868
			 0x1ff48df0, /// 0x86c
			 0x3e32183b, /// 0x870
			 0x6817c870, /// 0x874
			 0x0ed3a770, /// 0x878
			 0x52efc708, /// 0x87c
			 0xb84c1890, /// 0x880
			 0x67cd7e0a, /// 0x884
			 0x6008d33a, /// 0x888
			 0x070e55b4, /// 0x88c
			 0xff7bbc00, /// 0x890
			 0x05c4f58d, /// 0x894
			 0x38eebc8f, /// 0x898
			 0xe4653845, /// 0x89c
			 0x6677895e, /// 0x8a0
			 0x7640f6ae, /// 0x8a4
			 0x04ecad8c, /// 0x8a8
			 0x93b7bc90, /// 0x8ac
			 0x993265b0, /// 0x8b0
			 0xdec79f59, /// 0x8b4
			 0xa52484f2, /// 0x8b8
			 0x113272ec, /// 0x8bc
			 0xcb6d2889, /// 0x8c0
			 0x0427e1ad, /// 0x8c4
			 0x50296a01, /// 0x8c8
			 0xabd26e34, /// 0x8cc
			 0x9b2e646b, /// 0x8d0
			 0x91631359, /// 0x8d4
			 0x9d0b15ea, /// 0x8d8
			 0xd19a715a, /// 0x8dc
			 0xc0f5bff6, /// 0x8e0
			 0x8413812e, /// 0x8e4
			 0xebea1a58, /// 0x8e8
			 0x94a2dcf0, /// 0x8ec
			 0x0d4cac21, /// 0x8f0
			 0xe6133949, /// 0x8f4
			 0xd6fd3f97, /// 0x8f8
			 0x13733f55, /// 0x8fc
			 0x34aa0c67, /// 0x900
			 0xba21ad84, /// 0x904
			 0xc0e74925, /// 0x908
			 0x47298271, /// 0x90c
			 0x4b59df14, /// 0x910
			 0xa95a2f77, /// 0x914
			 0x002cdd9a, /// 0x918
			 0x1a96fa18, /// 0x91c
			 0xc2755c50, /// 0x920
			 0xe692b682, /// 0x924
			 0x112e6f96, /// 0x928
			 0x409fabd0, /// 0x92c
			 0xee11046d, /// 0x930
			 0x1999a216, /// 0x934
			 0xf53a06e6, /// 0x938
			 0xd4ea1e5f, /// 0x93c
			 0xe2e24634, /// 0x940
			 0xe6ba05fb, /// 0x944
			 0xae793a66, /// 0x948
			 0xe6e7109c, /// 0x94c
			 0xaf621451, /// 0x950
			 0x630ad612, /// 0x954
			 0x56cfe5a2, /// 0x958
			 0xb597dd0c, /// 0x95c
			 0x8db83efd, /// 0x960
			 0xec81f563, /// 0x964
			 0x223fe21b, /// 0x968
			 0xebc26198, /// 0x96c
			 0x0552350a, /// 0x970
			 0x47933a7f, /// 0x974
			 0xf1930674, /// 0x978
			 0xbe2886eb, /// 0x97c
			 0x589934cf, /// 0x980
			 0x98d8d387, /// 0x984
			 0xa3dd1b81, /// 0x988
			 0xe609992f, /// 0x98c
			 0xd879925e, /// 0x990
			 0xac25ee88, /// 0x994
			 0x2d51fdec, /// 0x998
			 0x692d6c6b, /// 0x99c
			 0x56479703, /// 0x9a0
			 0x53d28784, /// 0x9a4
			 0x0a07888f, /// 0x9a8
			 0x6f8a57eb, /// 0x9ac
			 0xadf9b9a6, /// 0x9b0
			 0x30152f34, /// 0x9b4
			 0xbe365540, /// 0x9b8
			 0x518deaaa, /// 0x9bc
			 0x16170696, /// 0x9c0
			 0x57223468, /// 0x9c4
			 0x7ac5652c, /// 0x9c8
			 0x4c989673, /// 0x9cc
			 0xba497866, /// 0x9d0
			 0x0caf83cd, /// 0x9d4
			 0x0ce0f5c9, /// 0x9d8
			 0xf8a920ac, /// 0x9dc
			 0x447b73ba, /// 0x9e0
			 0x1ee9f23a, /// 0x9e4
			 0x8fa9b763, /// 0x9e8
			 0x4d4187a2, /// 0x9ec
			 0xcd989841, /// 0x9f0
			 0xc54d9147, /// 0x9f4
			 0x900de626, /// 0x9f8
			 0x384ce7b6, /// 0x9fc
			 0xd428f693, /// 0xa00
			 0x9843c36d, /// 0xa04
			 0xc7c2b4d7, /// 0xa08
			 0x940c2183, /// 0xa0c
			 0xd0177c35, /// 0xa10
			 0x8a34de43, /// 0xa14
			 0xa908b121, /// 0xa18
			 0xfc7a2b86, /// 0xa1c
			 0x51f6c165, /// 0xa20
			 0xca6b0cce, /// 0xa24
			 0xc15dce6b, /// 0xa28
			 0x242c7f2a, /// 0xa2c
			 0x14a7447b, /// 0xa30
			 0x781fbdaa, /// 0xa34
			 0xefb4bfeb, /// 0xa38
			 0x893b5cba, /// 0xa3c
			 0x7909de3b, /// 0xa40
			 0x1a76cd33, /// 0xa44
			 0x189ae676, /// 0xa48
			 0xd88c42c5, /// 0xa4c
			 0x42f39959, /// 0xa50
			 0x848797a0, /// 0xa54
			 0x7fe7ef6a, /// 0xa58
			 0x5a2a7fb1, /// 0xa5c
			 0x5df87b91, /// 0xa60
			 0x39226027, /// 0xa64
			 0x27427869, /// 0xa68
			 0x38332a09, /// 0xa6c
			 0xe8312c61, /// 0xa70
			 0xb7a17c57, /// 0xa74
			 0x129bf182, /// 0xa78
			 0x02affd2e, /// 0xa7c
			 0xb3d44b10, /// 0xa80
			 0xd68102c9, /// 0xa84
			 0xe597c39f, /// 0xa88
			 0x557fced2, /// 0xa8c
			 0x2503f94b, /// 0xa90
			 0x1a064a5c, /// 0xa94
			 0xbb3d75b7, /// 0xa98
			 0x9c91dddc, /// 0xa9c
			 0xdc2f7e90, /// 0xaa0
			 0x920f4f4b, /// 0xaa4
			 0x894979d2, /// 0xaa8
			 0x5a83ef17, /// 0xaac
			 0x55646cd5, /// 0xab0
			 0xafa24238, /// 0xab4
			 0xeac4528a, /// 0xab8
			 0x07cf08ff, /// 0xabc
			 0x2fcaa392, /// 0xac0
			 0xae646eda, /// 0xac4
			 0x3406be17, /// 0xac8
			 0x9fef5239, /// 0xacc
			 0x162debe9, /// 0xad0
			 0xf6184c2e, /// 0xad4
			 0xbe65fc58, /// 0xad8
			 0x7ba6d079, /// 0xadc
			 0x93982a43, /// 0xae0
			 0x65604c66, /// 0xae4
			 0x34bcb2a7, /// 0xae8
			 0x171968a2, /// 0xaec
			 0x3965ab90, /// 0xaf0
			 0x83df27ed, /// 0xaf4
			 0x8b345137, /// 0xaf8
			 0x56780b8f, /// 0xafc
			 0x9faf7d5d, /// 0xb00
			 0x53c6c993, /// 0xb04
			 0xf6acea77, /// 0xb08
			 0x083d62d4, /// 0xb0c
			 0x256a6b2d, /// 0xb10
			 0x8a00f95f, /// 0xb14
			 0x5ca46f69, /// 0xb18
			 0xcf53fba9, /// 0xb1c
			 0x0aeeee80, /// 0xb20
			 0xdee48e1e, /// 0xb24
			 0x91002e47, /// 0xb28
			 0x053dae7d, /// 0xb2c
			 0x9d264a6a, /// 0xb30
			 0x8037d409, /// 0xb34
			 0x0209706f, /// 0xb38
			 0xfe226b5f, /// 0xb3c
			 0x981dccef, /// 0xb40
			 0xfc0a7869, /// 0xb44
			 0x6905ba3d, /// 0xb48
			 0x744a0e05, /// 0xb4c
			 0xc7f868af, /// 0xb50
			 0x35d8fddb, /// 0xb54
			 0x9d6016e9, /// 0xb58
			 0x418e9d49, /// 0xb5c
			 0x27ec3b87, /// 0xb60
			 0xf43e40e1, /// 0xb64
			 0x4d032801, /// 0xb68
			 0xfb149460, /// 0xb6c
			 0xc07ba579, /// 0xb70
			 0x7f4eaac2, /// 0xb74
			 0x158fb867, /// 0xb78
			 0x1bb50b5b, /// 0xb7c
			 0xc10b9f77, /// 0xb80
			 0xdd076dd9, /// 0xb84
			 0x94414f65, /// 0xb88
			 0x2976cc4e, /// 0xb8c
			 0x976a6fdd, /// 0xb90
			 0x5b660057, /// 0xb94
			 0x1e95fefe, /// 0xb98
			 0xd83fc639, /// 0xb9c
			 0x592abdcf, /// 0xba0
			 0x2dcdd85d, /// 0xba4
			 0x138c64f6, /// 0xba8
			 0x981881c8, /// 0xbac
			 0xe27f4058, /// 0xbb0
			 0x8992b43f, /// 0xbb4
			 0x7373a8f8, /// 0xbb8
			 0xfdd6f4b5, /// 0xbbc
			 0xd2aac053, /// 0xbc0
			 0x8df7e134, /// 0xbc4
			 0x94278bb5, /// 0xbc8
			 0xd0e7ad9d, /// 0xbcc
			 0x967bd10f, /// 0xbd0
			 0x56df0d42, /// 0xbd4
			 0x71d9a9e5, /// 0xbd8
			 0xf767e9f8, /// 0xbdc
			 0x15c544b5, /// 0xbe0
			 0x6ece489f, /// 0xbe4
			 0x6df3cc93, /// 0xbe8
			 0x35260fed, /// 0xbec
			 0x351bba6a, /// 0xbf0
			 0x13ba4eb0, /// 0xbf4
			 0x402eeb45, /// 0xbf8
			 0x93b68cef, /// 0xbfc
			 0x1e8ff504, /// 0xc00
			 0xb93b7cd2, /// 0xc04
			 0xa84591d4, /// 0xc08
			 0xb9d689e4, /// 0xc0c
			 0xd5e5b732, /// 0xc10
			 0xd8f7cfad, /// 0xc14
			 0xc8a878c9, /// 0xc18
			 0x095e9f8e, /// 0xc1c
			 0xa2f6dd44, /// 0xc20
			 0x5e7e28ef, /// 0xc24
			 0xe7ea849b, /// 0xc28
			 0x4fd244c9, /// 0xc2c
			 0xedaa269d, /// 0xc30
			 0x18f56cc6, /// 0xc34
			 0x895c1cf9, /// 0xc38
			 0x710d60c6, /// 0xc3c
			 0x32c7ce7e, /// 0xc40
			 0x52d4ca42, /// 0xc44
			 0x0d102776, /// 0xc48
			 0xe6bf2ab6, /// 0xc4c
			 0x638cb577, /// 0xc50
			 0xdd2de483, /// 0xc54
			 0x419a86f5, /// 0xc58
			 0x54b2c1db, /// 0xc5c
			 0x133fd37a, /// 0xc60
			 0xc5d52aec, /// 0xc64
			 0x6dbeeb3e, /// 0xc68
			 0x81688849, /// 0xc6c
			 0x27c6489e, /// 0xc70
			 0xd2c896fc, /// 0xc74
			 0x32438a6a, /// 0xc78
			 0x0f482e1c, /// 0xc7c
			 0x29a46131, /// 0xc80
			 0x87e7b82a, /// 0xc84
			 0xdd0eb8d0, /// 0xc88
			 0xf19a8fec, /// 0xc8c
			 0x6bd9a606, /// 0xc90
			 0x25ccbd39, /// 0xc94
			 0x9b6b19b6, /// 0xc98
			 0x2cacad2e, /// 0xc9c
			 0x329defd8, /// 0xca0
			 0x615742c3, /// 0xca4
			 0x1f8194ca, /// 0xca8
			 0x977e1e06, /// 0xcac
			 0x2a9c47c5, /// 0xcb0
			 0x4611546c, /// 0xcb4
			 0x723305ba, /// 0xcb8
			 0xfe0376b4, /// 0xcbc
			 0xee6f51d3, /// 0xcc0
			 0x03bedcfb, /// 0xcc4
			 0xce433d6a, /// 0xcc8
			 0xc9ae7743, /// 0xccc
			 0xf387465a, /// 0xcd0
			 0x30c6887f, /// 0xcd4
			 0x1799ebb8, /// 0xcd8
			 0x31b2e4d7, /// 0xcdc
			 0xaf39896c, /// 0xce0
			 0x6894b72b, /// 0xce4
			 0xbdbbd845, /// 0xce8
			 0x0c1712b1, /// 0xcec
			 0x32394a1d, /// 0xcf0
			 0xb4bf98f7, /// 0xcf4
			 0x5fc22f29, /// 0xcf8
			 0xf9764c81, /// 0xcfc
			 0xde6784ef, /// 0xd00
			 0x11612181, /// 0xd04
			 0xf70d5642, /// 0xd08
			 0x815c544e, /// 0xd0c
			 0x32288d1d, /// 0xd10
			 0x0f639637, /// 0xd14
			 0xfdfdbdf9, /// 0xd18
			 0x6a09b0b3, /// 0xd1c
			 0x74c7ba67, /// 0xd20
			 0x24df5bcb, /// 0xd24
			 0xd855e77f, /// 0xd28
			 0x7bd84194, /// 0xd2c
			 0xb4479e07, /// 0xd30
			 0x7c6e5ef2, /// 0xd34
			 0xcb8451f1, /// 0xd38
			 0x43bc2305, /// 0xd3c
			 0xddb5173e, /// 0xd40
			 0x77720c72, /// 0xd44
			 0x7c9afede, /// 0xd48
			 0x44fd80e7, /// 0xd4c
			 0x4f57e796, /// 0xd50
			 0x3b68d638, /// 0xd54
			 0xf68a169c, /// 0xd58
			 0x8d02506c, /// 0xd5c
			 0xf59ed282, /// 0xd60
			 0xe56f8ec1, /// 0xd64
			 0xa9e8f881, /// 0xd68
			 0x916c0373, /// 0xd6c
			 0xab18fbc0, /// 0xd70
			 0x73c21317, /// 0xd74
			 0xcd1301f0, /// 0xd78
			 0x298f9767, /// 0xd7c
			 0xf5b3a5e2, /// 0xd80
			 0xd84ea711, /// 0xd84
			 0x5b4e653f, /// 0xd88
			 0x06d097b9, /// 0xd8c
			 0x05adc6cb, /// 0xd90
			 0xce5bcbda, /// 0xd94
			 0x46a3b7e1, /// 0xd98
			 0xaccc0fa3, /// 0xd9c
			 0x4d11cdc1, /// 0xda0
			 0x77a967b5, /// 0xda4
			 0xf4ee97ff, /// 0xda8
			 0xede8b9bc, /// 0xdac
			 0xd31b77a9, /// 0xdb0
			 0x79d58f47, /// 0xdb4
			 0xabb5f9b4, /// 0xdb8
			 0xd0b02f0f, /// 0xdbc
			 0x835f525a, /// 0xdc0
			 0x8e991f0d, /// 0xdc4
			 0xd8b52151, /// 0xdc8
			 0xcab4fd55, /// 0xdcc
			 0x93dfd55c, /// 0xdd0
			 0xa8344760, /// 0xdd4
			 0x0a1b1da0, /// 0xdd8
			 0x70e7bc78, /// 0xddc
			 0x2cff34c4, /// 0xde0
			 0x90f68ad1, /// 0xde4
			 0xe6797a2c, /// 0xde8
			 0x580bcb42, /// 0xdec
			 0x81696b38, /// 0xdf0
			 0x1330bfd8, /// 0xdf4
			 0xe33d302e, /// 0xdf8
			 0x26f84db1, /// 0xdfc
			 0xa059ab20, /// 0xe00
			 0x1a3da27f, /// 0xe04
			 0x0cc3cbb7, /// 0xe08
			 0x99d429ff, /// 0xe0c
			 0x6e6c7453, /// 0xe10
			 0xfca852bd, /// 0xe14
			 0xc02c76a8, /// 0xe18
			 0xd187c60f, /// 0xe1c
			 0x64801bfb, /// 0xe20
			 0x986121ac, /// 0xe24
			 0xa020bd17, /// 0xe28
			 0xceb377a8, /// 0xe2c
			 0x24eb3de0, /// 0xe30
			 0xee391159, /// 0xe34
			 0x6876ae9d, /// 0xe38
			 0x43f297ef, /// 0xe3c
			 0x354f1e6c, /// 0xe40
			 0x67e94cbd, /// 0xe44
			 0x9baa25b3, /// 0xe48
			 0x08010798, /// 0xe4c
			 0x4d28848a, /// 0xe50
			 0xce161c1e, /// 0xe54
			 0x7c6dfc19, /// 0xe58
			 0x1f372d12, /// 0xe5c
			 0x5cbb2ebe, /// 0xe60
			 0x24ac4265, /// 0xe64
			 0xdff25cdd, /// 0xe68
			 0xdb2e1e70, /// 0xe6c
			 0xf1449eeb, /// 0xe70
			 0x1fbf84b0, /// 0xe74
			 0x95eb93a0, /// 0xe78
			 0xbba80f12, /// 0xe7c
			 0x304152eb, /// 0xe80
			 0xb6b6fffa, /// 0xe84
			 0xa77458cd, /// 0xe88
			 0xe4cd9b1d, /// 0xe8c
			 0xfd93e8c7, /// 0xe90
			 0x126deac1, /// 0xe94
			 0x18694e97, /// 0xe98
			 0xa08dbb53, /// 0xe9c
			 0xe7b40c73, /// 0xea0
			 0x74f3b120, /// 0xea4
			 0x2a8c0076, /// 0xea8
			 0x02582eb6, /// 0xeac
			 0x3be98747, /// 0xeb0
			 0xecc2e8ae, /// 0xeb4
			 0x4accf7e6, /// 0xeb8
			 0x96f78b62, /// 0xebc
			 0xc6500644, /// 0xec0
			 0x0065c459, /// 0xec4
			 0x2deb3c6a, /// 0xec8
			 0x1a4b304f, /// 0xecc
			 0xdb2d3968, /// 0xed0
			 0x159ddc75, /// 0xed4
			 0x0c7505c4, /// 0xed8
			 0xac55607e, /// 0xedc
			 0x744b0cd4, /// 0xee0
			 0x31a506a2, /// 0xee4
			 0xbff59f3d, /// 0xee8
			 0x73c11a6a, /// 0xeec
			 0xe53a9f63, /// 0xef0
			 0xebeb51df, /// 0xef4
			 0xb19de5a5, /// 0xef8
			 0x48bd9df8, /// 0xefc
			 0xa3eda110, /// 0xf00
			 0x1451f36d, /// 0xf04
			 0x89f8686e, /// 0xf08
			 0xc7954329, /// 0xf0c
			 0x451e5366, /// 0xf10
			 0x934efb0b, /// 0xf14
			 0xb4f4c197, /// 0xf18
			 0x8c856c72, /// 0xf1c
			 0xf2db24fb, /// 0xf20
			 0xa6bb6083, /// 0xf24
			 0xcc1fea2d, /// 0xf28
			 0xa2aa9cf2, /// 0xf2c
			 0x23e8331e, /// 0xf30
			 0x465a28be, /// 0xf34
			 0xa89052f6, /// 0xf38
			 0xaf7009eb, /// 0xf3c
			 0xa0625344, /// 0xf40
			 0x057a6c32, /// 0xf44
			 0xb75f1260, /// 0xf48
			 0x53b18ede, /// 0xf4c
			 0x75dfaba5, /// 0xf50
			 0xbbc6ecda, /// 0xf54
			 0xf34c7ab1, /// 0xf58
			 0x51ea4a79, /// 0xf5c
			 0xb9491110, /// 0xf60
			 0xec460b50, /// 0xf64
			 0xa364ad48, /// 0xf68
			 0x883604a3, /// 0xf6c
			 0xf20a3565, /// 0xf70
			 0xf7ca9465, /// 0xf74
			 0xcc047219, /// 0xf78
			 0x5aa520ae, /// 0xf7c
			 0x8843e42b, /// 0xf80
			 0xc0255e75, /// 0xf84
			 0xa9d30eb3, /// 0xf88
			 0x8ea2277d, /// 0xf8c
			 0xd6bb7cf3, /// 0xf90
			 0x6202d268, /// 0xf94
			 0x243bd78f, /// 0xf98
			 0xc4636923, /// 0xf9c
			 0xc29aeb16, /// 0xfa0
			 0x9d2f916b, /// 0xfa4
			 0x62b31627, /// 0xfa8
			 0xfe68263a, /// 0xfac
			 0x4ad0b213, /// 0xfb0
			 0x5fb0c74f, /// 0xfb4
			 0x7191b084, /// 0xfb8
			 0x3197a0fa, /// 0xfbc
			 0xef42f1db, /// 0xfc0
			 0xfc50d98a, /// 0xfc4
			 0xfce12848, /// 0xfc8
			 0xe92f21e6, /// 0xfcc
			 0xfef914ff, /// 0xfd0
			 0x70d9a8c2, /// 0xfd4
			 0xb0ac6c09, /// 0xfd8
			 0xceda3716, /// 0xfdc
			 0xb29ae0ec, /// 0xfe0
			 0x21717ac9, /// 0xfe4
			 0x29f9c8c9, /// 0xfe8
			 0xa058e546, /// 0xfec
			 0xb52c423e, /// 0xff0
			 0x920c653e, /// 0xff4
			 0x708f3dbb, /// 0xff8
			 0x8301cf7a /// last
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
			 0x000000c0,
			 0x000005a8,
			 0x00000454,
			 0x000004bc,
			 0x00000670,
			 0x00000754,
			 0x0000066c,
			 0x00000378,

			 /// vpu register f2
			 0x41d80000,
			 0x41600000,
			 0x41c00000,
			 0x41a00000,
			 0x40400000,
			 0x41e00000,
			 0x41c80000,
			 0x41d80000,

			 /// vpu register f3
			 0x41a80000,
			 0x41d00000,
			 0x41800000,
			 0x41300000,
			 0x41100000,
			 0x41c80000,
			 0x40a00000,
			 0x41e80000,

			 /// vpu register f4
			 0x40800000,
			 0x41e00000,
			 0x41400000,
			 0x41000000,
			 0x41a80000,
			 0x40400000,
			 0x41b00000,
			 0x41600000,

			 /// vpu register f5
			 0x41800000,
			 0x41e00000,
			 0x40400000,
			 0x40c00000,
			 0x41400000,
			 0x41a80000,
			 0x41500000,
			 0x41880000,

			 /// vpu register f6
			 0x42000000,
			 0x41400000,
			 0x41f80000,
			 0x40a00000,
			 0x41100000,
			 0x41800000,
			 0x40c00000,
			 0x41b80000,

			 /// vpu register f7
			 0x40400000,
			 0x41600000,
			 0x40e00000,
			 0x41b80000,
			 0x41a80000,
			 0x40e00000,
			 0x41300000,
			 0x41b80000,

			 /// vpu register f8
			 0x41400000,
			 0x41a00000,
			 0x41f80000,
			 0x40400000,
			 0x41900000,
			 0x41600000,
			 0x41b00000,
			 0x42000000,

			 /// vpu register f9
			 0x41400000,
			 0x41300000,
			 0x41900000,
			 0x41000000,
			 0x41b80000,
			 0x41800000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f10
			 0x41500000,
			 0x41b80000,
			 0x41d00000,
			 0x40400000,
			 0x41c80000,
			 0x40800000,
			 0x41200000,
			 0x41e00000,

			 /// vpu register f11
			 0x40e00000,
			 0x41800000,
			 0x41100000,
			 0x41f80000,
			 0x41100000,
			 0x40000000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f12
			 0x40c00000,
			 0x41b80000,
			 0x3f800000,
			 0x40400000,
			 0x41c80000,
			 0x41c00000,
			 0x41c80000,
			 0x41600000,

			 /// vpu register f13
			 0x41300000,
			 0x41b80000,
			 0x41400000,
			 0x40000000,
			 0x41a00000,
			 0x41a80000,
			 0x41a00000,
			 0x41400000,

			 /// vpu register f14
			 0x41c80000,
			 0x41b80000,
			 0x41e80000,
			 0x41b80000,
			 0x41a00000,
			 0x41e80000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f15
			 0x41100000,
			 0x41d00000,
			 0x41c00000,
			 0x40000000,
			 0x41880000,
			 0x41d00000,
			 0x40000000,
			 0x41880000,

			 /// vpu register f16
			 0x41000000,
			 0x40800000,
			 0x3f800000,
			 0x41b00000,
			 0x41100000,
			 0x41000000,
			 0x41200000,
			 0x41d00000,

			 /// vpu register f17
			 0x41880000,
			 0x40400000,
			 0x41f00000,
			 0x41f00000,
			 0x40e00000,
			 0x40e00000,
			 0x3f800000,
			 0x41a80000,

			 /// vpu register f18
			 0x41400000,
			 0x41400000,
			 0x40a00000,
			 0x40e00000,
			 0x40a00000,
			 0x3f800000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f19
			 0x40400000,
			 0x41a00000,
			 0x41300000,
			 0x41b80000,
			 0x41c80000,
			 0x40000000,
			 0x41a00000,
			 0x41c80000,

			 /// vpu register f20
			 0x41980000,
			 0x42000000,
			 0x41800000,
			 0x41c00000,
			 0x41980000,
			 0x41800000,
			 0x41f00000,
			 0x41e00000,

			 /// vpu register f21
			 0x41b00000,
			 0x41c80000,
			 0x41600000,
			 0x3f800000,
			 0x3f800000,
			 0x41d00000,
			 0x41a00000,
			 0x40c00000,

			 /// vpu register f22
			 0x41980000,
			 0x41200000,
			 0x41d80000,
			 0x40000000,
			 0x40c00000,
			 0x41500000,
			 0x41d00000,
			 0x41000000,

			 /// vpu register f23
			 0x41e00000,
			 0x40a00000,
			 0x41980000,
			 0x3f800000,
			 0x3f800000,
			 0x41c00000,
			 0x41980000,
			 0x41e80000,

			 /// vpu register f24
			 0x41a00000,
			 0x3f800000,
			 0x3f800000,
			 0x41980000,
			 0x41c00000,
			 0x41600000,
			 0x40400000,
			 0x41f00000,

			 /// vpu register f25
			 0x41900000,
			 0x41000000,
			 0x41600000,
			 0x41d00000,
			 0x41600000,
			 0x41000000,
			 0x41f00000,
			 0x40000000,

			 /// vpu register f26
			 0x41700000,
			 0x41e80000,
			 0x41300000,
			 0x41c00000,
			 0x41e80000,
			 0x42000000,
			 0x41c80000,
			 0x41d00000,

			 /// vpu register f27
			 0x41f00000,
			 0x41e00000,
			 0x41a00000,
			 0x41b00000,
			 0x41800000,
			 0x41700000,
			 0x41c80000,
			 0x41900000,

			 /// vpu register f28
			 0x41f00000,
			 0x41000000,
			 0x3f800000,
			 0x41980000,
			 0x42000000,
			 0x41700000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f29
			 0x41800000,
			 0x41980000,
			 0x41900000,
			 0x40000000,
			 0x41700000,
			 0x41000000,
			 0x41900000,
			 0x41e80000,

			 /// vpu register f30
			 0x41000000,
			 0x41000000,
			 0x41a00000,
			 0x41200000,
			 0x41700000,
			 0x41e00000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f31
			 0x41c80000,
			 0x40a00000,
			 0x41880000,
			 0x41e80000,
			 0x41400000,
			 0x41b80000,
			 0x41900000,
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
		"fclass.ps f12, f6\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f24, f5\n"                                 ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f3, f30\n"                                 ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f21, f8\n"                                 ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f7, f19\n"                                 ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f16, f18\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f25\n"                                 ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f19, f26\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f27\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f13\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f16, f21\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f23, f9\n"                                 ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f4, f6\n"                                  ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f8, f20\n"                                 ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f15\n"                                 ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f19, f17\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f14, f20\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f14, f5\n"                                 ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f15, f19\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f7, f15\n"                                 ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f21\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f15, f16\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f4, f3\n"                                  ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f2, f5\n"                                  ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f19, f7\n"                                 ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f14, f16\n"                                ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f29, f29\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f14, f22\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f20, f22\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f18, f25\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f21, f4\n"                                 ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f18, f3\n"                                 ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f24, f20\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f12, f7\n"                                 ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f29, f11\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f4, f14\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f30, f13\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f14, f24\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f23, f29\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f4\n"                                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f13, f13\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f7\n"                                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f8, f20\n"                                 ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f26\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f2, f23\n"                                 ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f12, f26\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f15, f27\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f4, f21\n"                                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f30, f3\n"                                 ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f6, f1\n"                                  ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f28, f5\n"                                 ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f2, f8\n"                                  ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f7, f9\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f10\n"                                 ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f3, f20\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f20, f3\n"                                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f6, f10\n"                                 ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f13, f27\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f20, f1\n"                                 ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f17\n"                                 ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f5, f28\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f24, f18\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f16, f2\n"                                 ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f20, f7\n"                                 ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f4, f25\n"                                 ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f12, f19\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f12, f25\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f26, f0\n"                                 ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f19, f11\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f27, f19\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f24, f3\n"                                 ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f27, f3\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f15, f2\n"                                 ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f7, f13\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f21, f18\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f9, f0\n"                                  ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f5, f12\n"                                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f23, f29\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f29, f6\n"                                 ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f18, f2\n"                                 ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f2\n"                                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f22\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f21, f6\n"                                 ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f11, f15\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f13, f28\n"                                ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f4, f12\n"                                 ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f16, f5\n"                                 ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f20, f8\n"                                 ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f17, f18\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f24, f10\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f14, f0\n"                                 ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f3, f25\n"                                 ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f20, f5\n"                                 ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f30, f28\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f5\n"                                 ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f19, f24\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f10, f21\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f30, f7\n"                                 ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fclass.ps f3, f4\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
