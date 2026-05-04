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
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x7fc00001,                                                  // signaling NaN                               /// 00004
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00008
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00010
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00018
			 0xffc00001,                                                  // -signaling NaN                              /// 0001c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00024
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00028
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00030
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c400000,                                                  // Leading 1s:                                 /// 00038
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00040
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00044
			 0x0c600000,                                                  // Leading 1s:                                 /// 00048
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0004c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00050
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0005c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00068
			 0x3f028f5c,                                                  // 0.51                                        /// 0006c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00070
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00074
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00078
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0007c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00080
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00088
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0008c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00090
			 0xffc00001,                                                  // -signaling NaN                              /// 00094
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00100
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00108
			 0x0e000007,                                                  // Trailing 1s:                                /// 0010c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00110
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00114
			 0x55555555,                                                  // 4 random values                             /// 00118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00124
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00130
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00138
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00140
			 0x0e000007,                                                  // Trailing 1s:                                /// 00144
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00160
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00164
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00168
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00174
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00178
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0017c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00180
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00184
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00188
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00198
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80000000,                                                  // -zero                                       /// 001b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001c4
			 0x7f800000,                                                  // inf                                         /// 001c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d8
			 0xbf028f5c,                                                  // -0.51                                       /// 001dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00200
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0020c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00210
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xcb000000,                                                  // -8388608.0                                  /// 00218
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0021c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00224
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00230
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00234
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00244
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0xcb000000,                                                  // -8388608.0                                  /// 00250
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00254
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00260
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00268
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00270
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00280
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00284
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00288
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00290
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00294
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0029c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x7f800000,                                                  // inf                                         /// 002b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002c0
			 0x33333333,                                                  // 4 random values                             /// 002c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00304
			 0x4b000000,                                                  // 8388608.0                                   /// 00308
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0030c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00310
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00314
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00318
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00320
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00324
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0032c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00330
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00334
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00338
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00348
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0034c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00000000,                                                  // zero                                        /// 00358
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00360
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x55555555,                                                  // 4 random values                             /// 0036c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00378
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0037c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00380
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00384
			 0x0e000003,                                                  // Trailing 1s:                                /// 00388
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0038c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x7fc00001,                                                  // signaling NaN                               /// 00394
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00398
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e0
			 0x3f028f5c,                                                  // 0.51                                        /// 003e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00400
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00408
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0040c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00410
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00418
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0041c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00420
			 0x0e000007,                                                  // Trailing 1s:                                /// 00424
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0042c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00430
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00434
			 0x33333333,                                                  // 4 random values                             /// 00438
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0043c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00444
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00450
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00458
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00464
			 0x4b000000,                                                  // 8388608.0                                   /// 00468
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0046c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00470
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00474
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00478
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0047c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00480
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00488
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00490
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00498
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0049c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b4
			 0x33333333,                                                  // 4 random values                             /// 004b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004f8
			 0x33333333,                                                  // 4 random values                             /// 004fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00500
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00504
			 0xcb000000,                                                  // -8388608.0                                  /// 00508
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0050c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00510
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00514
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00518
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0051c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00520
			 0x0c600000,                                                  // Leading 1s:                                 /// 00524
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0052c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00530
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00534
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00538
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0053c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00540
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00544
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00548
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00550
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00554
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00558
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0c700000,                                                  // Leading 1s:                                 /// 00568
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0056c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0xffc00001,                                                  // -signaling NaN                              /// 00574
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00578
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00580
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00584
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0058c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00590
			 0x0e000007,                                                  // Trailing 1s:                                /// 00594
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00598
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0059c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005c0
			 0x00000000,                                                  // zero                                        /// 005c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00600
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00608
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00610
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00614
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00620
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0062c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00630
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00638
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0063c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00640
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0064c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00650
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00654
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00658
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x7f800000,                                                  // inf                                         /// 00664
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80011111,                                                  // -9.7958E-41                                 /// 0066c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00670
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00674
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00688
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00694
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00698
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0069c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x33333333,                                                  // 4 random values                             /// 006ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x7fc00001,                                                  // signaling NaN                               /// 006f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00700
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00704
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00710
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00714
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00718
			 0x55555555,                                                  // 4 random values                             /// 0071c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00724
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00728
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0072c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00734
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00740
			 0x0e000003,                                                  // Trailing 1s:                                /// 00744
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00748
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0074c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0075c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00760
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00764
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00770
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00774
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00778
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00780
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00784
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0078c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00790
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00798
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 007b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x33333333,                                                  // 4 random values                             /// 007c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00800
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00808
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0080c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00810
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0081c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00820
			 0x80011111,                                                  // -9.7958E-41                                 /// 00824
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00838
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00848
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00858
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0085c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00868
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00874
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00878
			 0xcb000000,                                                  // -8388608.0                                  /// 0087c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00880
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0088c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00890
			 0x80000000,                                                  // -zero                                       /// 00894
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00898
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008bc
			 0x7f800000,                                                  // inf                                         /// 008c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f4
			 0xcb000000,                                                  // -8388608.0                                  /// 008f8
			 0x7f800000,                                                  // inf                                         /// 008fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00900
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00904
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00908
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00910
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00914
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00920
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00924
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00930
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00934
			 0xbf028f5c,                                                  // -0.51                                       /// 00938
			 0x7fc00001,                                                  // signaling NaN                               /// 0093c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00940
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00944
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0094c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00950
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00954
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00958
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0095c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00964
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00968
			 0xbf028f5c,                                                  // -0.51                                       /// 0096c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00970
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00978
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0097c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00980
			 0x3f028f5c,                                                  // 0.51                                        /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00988
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0098c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0099c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d8
			 0x4b000000,                                                  // 8388608.0                                   /// 009dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a34
			 0x00000000,                                                  // zero                                        /// 00a38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a84
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a8c
			 0x33333333,                                                  // 4 random values                             /// 00a90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00abc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00acc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00adc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00af0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00af8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00afc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b00
			 0xffc00001,                                                  // -signaling NaN                              /// 00b04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b08
			 0x3f028f5c,                                                  // 0.51                                        /// 00b0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b58
			 0x7fc00001,                                                  // signaling NaN                               /// 00b5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b64
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b74
			 0xbf028f5c,                                                  // -0.51                                       /// 00b78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x7f800000,                                                  // inf                                         /// 00b88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x7fc00001,                                                  // signaling NaN                               /// 00b9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x55555555,                                                  // 4 random values                             /// 00ba8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bb4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00be8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c68
			 0x3f028f5c,                                                  // 0.51                                        /// 00c6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ca8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cac
			 0x7f800000,                                                  // inf                                         /// 00cb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cd0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cdc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ce4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cfc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d20
			 0x33333333,                                                  // 4 random values                             /// 00d24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f800000,                                                  // inf                                         /// 00d70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x00000000,                                                  // zero                                        /// 00d78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x80000000,                                                  // -zero                                       /// 00d8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d98
			 0x00000000,                                                  // zero                                        /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00da4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00da8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00db0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db4
			 0x4b000000,                                                  // 8388608.0                                   /// 00db8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dcc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00de0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00df8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e14
			 0xcb000000,                                                  // -8388608.0                                  /// 00e18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00000000,                                                  // zero                                        /// 00e2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7fc00001,                                                  // signaling NaN                               /// 00e58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ebc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ec0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ee8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ef8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00efc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f04
			 0xffc00001,                                                  // -signaling NaN                              /// 00f08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f64
			 0x55555555,                                                  // 4 random values                             /// 00f68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fcc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fd0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fe8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ff0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00000100                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xa9197ea1, /// 0x0
			 0x10fdc676, /// 0x4
			 0x9d4dabcd, /// 0x8
			 0x3a0cc197, /// 0xc
			 0x98ae7a1a, /// 0x10
			 0x4aeb944a, /// 0x14
			 0x66795db2, /// 0x18
			 0x2723787d, /// 0x1c
			 0xfad8ca2f, /// 0x20
			 0xcf9dc803, /// 0x24
			 0xa0b8db4d, /// 0x28
			 0xd0b47fa6, /// 0x2c
			 0xee63be58, /// 0x30
			 0xda475647, /// 0x34
			 0x68d2ffb4, /// 0x38
			 0x0a9c6772, /// 0x3c
			 0xd72fa9df, /// 0x40
			 0x0446f2d2, /// 0x44
			 0x6be2f2ce, /// 0x48
			 0xd3848f9b, /// 0x4c
			 0xe1e5bd2a, /// 0x50
			 0xe8f7c287, /// 0x54
			 0x1625cf94, /// 0x58
			 0x62ab8548, /// 0x5c
			 0x671eb5ce, /// 0x60
			 0xfcfe9b93, /// 0x64
			 0x7a4a9f29, /// 0x68
			 0x5c99bcc4, /// 0x6c
			 0x3a04f65d, /// 0x70
			 0x9e8936da, /// 0x74
			 0x530380c4, /// 0x78
			 0x708319ac, /// 0x7c
			 0x8d9ed527, /// 0x80
			 0x1ee5b169, /// 0x84
			 0xa5a61012, /// 0x88
			 0x96abc8f1, /// 0x8c
			 0xb4cf1135, /// 0x90
			 0x8b5491cf, /// 0x94
			 0x2451a610, /// 0x98
			 0x5af03079, /// 0x9c
			 0xa49e1121, /// 0xa0
			 0x8b895aaf, /// 0xa4
			 0x705b2e70, /// 0xa8
			 0x78f5df37, /// 0xac
			 0xcf9ee364, /// 0xb0
			 0x5b526fec, /// 0xb4
			 0xbce0f166, /// 0xb8
			 0x54e26702, /// 0xbc
			 0x0a51b981, /// 0xc0
			 0x5ce04e79, /// 0xc4
			 0xff59d97e, /// 0xc8
			 0x00944d66, /// 0xcc
			 0x085dff06, /// 0xd0
			 0x2884feac, /// 0xd4
			 0xb5829c08, /// 0xd8
			 0x5022c776, /// 0xdc
			 0x73c3aadb, /// 0xe0
			 0x971ceede, /// 0xe4
			 0x386d870b, /// 0xe8
			 0x32d1a90c, /// 0xec
			 0xd9d856c6, /// 0xf0
			 0x9f4f398d, /// 0xf4
			 0x093895c8, /// 0xf8
			 0xebbe7d2e, /// 0xfc
			 0x15f4ef13, /// 0x100
			 0x5df792ab, /// 0x104
			 0xa4a3e0e9, /// 0x108
			 0xf7443398, /// 0x10c
			 0x130a1a8a, /// 0x110
			 0x1c8d6516, /// 0x114
			 0x99419014, /// 0x118
			 0x85cf9b0a, /// 0x11c
			 0x56ab023f, /// 0x120
			 0x124e31c2, /// 0x124
			 0x15fbe61d, /// 0x128
			 0x5b120f5b, /// 0x12c
			 0xb49ad2b1, /// 0x130
			 0xaa9d333e, /// 0x134
			 0x165520b3, /// 0x138
			 0x4f69c941, /// 0x13c
			 0x55cc8b9a, /// 0x140
			 0x1362ac9f, /// 0x144
			 0x0ef4e85e, /// 0x148
			 0x7d08513b, /// 0x14c
			 0xbea8670b, /// 0x150
			 0x617f15fc, /// 0x154
			 0x169d1363, /// 0x158
			 0xda9f8f22, /// 0x15c
			 0x1f824139, /// 0x160
			 0x38b9cc15, /// 0x164
			 0xedb2b3f7, /// 0x168
			 0xa1c28bde, /// 0x16c
			 0x7eb249fb, /// 0x170
			 0xd4c8c694, /// 0x174
			 0x6882c42e, /// 0x178
			 0xd56c6e84, /// 0x17c
			 0x295e4132, /// 0x180
			 0xc2b63b6a, /// 0x184
			 0xd2c33d76, /// 0x188
			 0x6fe47fa2, /// 0x18c
			 0xa62adbf5, /// 0x190
			 0xc0552b1b, /// 0x194
			 0xe5f641e8, /// 0x198
			 0x1691a7f8, /// 0x19c
			 0x3ddeedc5, /// 0x1a0
			 0x3125dfe4, /// 0x1a4
			 0xe9a60610, /// 0x1a8
			 0xaa9d0b28, /// 0x1ac
			 0xe06bfdc4, /// 0x1b0
			 0x76776cde, /// 0x1b4
			 0x3834d6d1, /// 0x1b8
			 0x0fdc4400, /// 0x1bc
			 0xe815e087, /// 0x1c0
			 0x0235fca5, /// 0x1c4
			 0xfd80e477, /// 0x1c8
			 0x1bab70f4, /// 0x1cc
			 0xad453bd0, /// 0x1d0
			 0xcaba293f, /// 0x1d4
			 0xcf5d99e0, /// 0x1d8
			 0x17afea05, /// 0x1dc
			 0xe7ac802f, /// 0x1e0
			 0x6b0d4f8e, /// 0x1e4
			 0x913c4e64, /// 0x1e8
			 0x0d9d945c, /// 0x1ec
			 0x530171c7, /// 0x1f0
			 0x0d57a622, /// 0x1f4
			 0x97303407, /// 0x1f8
			 0x4da57340, /// 0x1fc
			 0xf2c9f81d, /// 0x200
			 0xd328b876, /// 0x204
			 0x15631fee, /// 0x208
			 0x3052bcdb, /// 0x20c
			 0x6ba83375, /// 0x210
			 0x2d86ace5, /// 0x214
			 0x6f5f1e81, /// 0x218
			 0x568a1df4, /// 0x21c
			 0x4c3fdb97, /// 0x220
			 0x51fc6d51, /// 0x224
			 0x7f7c3311, /// 0x228
			 0xf7ab5562, /// 0x22c
			 0xc927823a, /// 0x230
			 0xe0e16f96, /// 0x234
			 0x4e54663d, /// 0x238
			 0xd409c7df, /// 0x23c
			 0x9fbf7fc8, /// 0x240
			 0xe3e870b1, /// 0x244
			 0x32387fae, /// 0x248
			 0x3e6f7571, /// 0x24c
			 0xcd949ba6, /// 0x250
			 0x61d52737, /// 0x254
			 0x5afb02ff, /// 0x258
			 0x6382cb36, /// 0x25c
			 0xfeb11efc, /// 0x260
			 0x5d391ed9, /// 0x264
			 0x313a756c, /// 0x268
			 0x6bcbd2a9, /// 0x26c
			 0x9f617d9a, /// 0x270
			 0x30769b0b, /// 0x274
			 0x1c32cd0b, /// 0x278
			 0x9604f658, /// 0x27c
			 0xe755e1a7, /// 0x280
			 0x7bf9ab97, /// 0x284
			 0x53f5fbc1, /// 0x288
			 0xa63ff019, /// 0x28c
			 0x2ebc189a, /// 0x290
			 0xe25636ff, /// 0x294
			 0x6853b48d, /// 0x298
			 0x3056acd3, /// 0x29c
			 0xc2be362a, /// 0x2a0
			 0x60da53d7, /// 0x2a4
			 0x50b93162, /// 0x2a8
			 0x682b830d, /// 0x2ac
			 0xbc02ada8, /// 0x2b0
			 0xfb9e4080, /// 0x2b4
			 0x73bff8bc, /// 0x2b8
			 0x512ec777, /// 0x2bc
			 0xc21b818d, /// 0x2c0
			 0x08101d82, /// 0x2c4
			 0x56770c20, /// 0x2c8
			 0x4a84b6b7, /// 0x2cc
			 0x6f38f611, /// 0x2d0
			 0xedb2ed95, /// 0x2d4
			 0xbc44f183, /// 0x2d8
			 0x1b6729c4, /// 0x2dc
			 0x0b3369c1, /// 0x2e0
			 0x6b2230ce, /// 0x2e4
			 0x805d06f4, /// 0x2e8
			 0xe84c983f, /// 0x2ec
			 0x6b2b87fe, /// 0x2f0
			 0x538814e8, /// 0x2f4
			 0x74857473, /// 0x2f8
			 0xe45b227d, /// 0x2fc
			 0xd3a0cbce, /// 0x300
			 0xaf09f38c, /// 0x304
			 0xbe795585, /// 0x308
			 0xf243fded, /// 0x30c
			 0xafe44b5b, /// 0x310
			 0xfcb6de60, /// 0x314
			 0x3bd5bf17, /// 0x318
			 0xa2f18c3f, /// 0x31c
			 0xb757829d, /// 0x320
			 0xc9973e1e, /// 0x324
			 0xacdf3f87, /// 0x328
			 0xa1701ce7, /// 0x32c
			 0x3b02cfd2, /// 0x330
			 0x056d00b8, /// 0x334
			 0x4829927e, /// 0x338
			 0xb8073c80, /// 0x33c
			 0x03c8679f, /// 0x340
			 0x575dbd5f, /// 0x344
			 0xad60a01c, /// 0x348
			 0x5d99360e, /// 0x34c
			 0x13920cf1, /// 0x350
			 0x684f2888, /// 0x354
			 0xf2704ce5, /// 0x358
			 0x99a4c269, /// 0x35c
			 0x4d670824, /// 0x360
			 0x1f2dd4aa, /// 0x364
			 0x6faa6dd6, /// 0x368
			 0xd4195813, /// 0x36c
			 0xfda6699b, /// 0x370
			 0x9b62859b, /// 0x374
			 0xe16d9f00, /// 0x378
			 0x690af6e0, /// 0x37c
			 0x68bbeb0b, /// 0x380
			 0xb94e8fec, /// 0x384
			 0x5214bd6f, /// 0x388
			 0x88830c5e, /// 0x38c
			 0xe277b09a, /// 0x390
			 0xe6f02d00, /// 0x394
			 0xc9bc6943, /// 0x398
			 0xf79d42c4, /// 0x39c
			 0x1da83335, /// 0x3a0
			 0xf8d0ed19, /// 0x3a4
			 0xaba19f10, /// 0x3a8
			 0x61455fd2, /// 0x3ac
			 0xdbddda2b, /// 0x3b0
			 0x772ce155, /// 0x3b4
			 0xa57dec3b, /// 0x3b8
			 0x4f5e5fbc, /// 0x3bc
			 0x21f167d2, /// 0x3c0
			 0xce6c926e, /// 0x3c4
			 0xa91066f0, /// 0x3c8
			 0x6849d74e, /// 0x3cc
			 0x427ce522, /// 0x3d0
			 0x4338a138, /// 0x3d4
			 0x9a0f5042, /// 0x3d8
			 0x0a269b88, /// 0x3dc
			 0x9c404af9, /// 0x3e0
			 0x055b2fd5, /// 0x3e4
			 0x791c5a7a, /// 0x3e8
			 0x9b1033a3, /// 0x3ec
			 0x23f9c9cd, /// 0x3f0
			 0x70021e76, /// 0x3f4
			 0x85a977bc, /// 0x3f8
			 0x68e1c05c, /// 0x3fc
			 0x7443e63c, /// 0x400
			 0xb37f8a45, /// 0x404
			 0x55ca7598, /// 0x408
			 0x4156bd68, /// 0x40c
			 0xe7539c3d, /// 0x410
			 0x12e9d4b2, /// 0x414
			 0xa47f14df, /// 0x418
			 0x94379206, /// 0x41c
			 0x250d1090, /// 0x420
			 0x38384ce6, /// 0x424
			 0xaa966372, /// 0x428
			 0xe56559a2, /// 0x42c
			 0xa55062c2, /// 0x430
			 0x072359f6, /// 0x434
			 0x3c919f8d, /// 0x438
			 0x6a988c29, /// 0x43c
			 0x417ef291, /// 0x440
			 0x20d8db2c, /// 0x444
			 0x620ef419, /// 0x448
			 0x7961e6fb, /// 0x44c
			 0x62939ef1, /// 0x450
			 0xa69c9fcd, /// 0x454
			 0x05beb77b, /// 0x458
			 0x3dfa54a1, /// 0x45c
			 0xffdb5a8e, /// 0x460
			 0x32c6a9ab, /// 0x464
			 0xaa712c2d, /// 0x468
			 0x0bef7767, /// 0x46c
			 0x24ea12d8, /// 0x470
			 0xfda9961d, /// 0x474
			 0x22c08ad4, /// 0x478
			 0x60033797, /// 0x47c
			 0xc3d5b7c2, /// 0x480
			 0xa7ffda45, /// 0x484
			 0x3ab5a900, /// 0x488
			 0x3499d2af, /// 0x48c
			 0xbeee4d79, /// 0x490
			 0x59c1add1, /// 0x494
			 0x37ea12c7, /// 0x498
			 0x27d9f16d, /// 0x49c
			 0x850762be, /// 0x4a0
			 0x429a7b9e, /// 0x4a4
			 0x279f4fe3, /// 0x4a8
			 0xd459ef33, /// 0x4ac
			 0xa29a792d, /// 0x4b0
			 0x22d45857, /// 0x4b4
			 0x313428fe, /// 0x4b8
			 0xde2c3614, /// 0x4bc
			 0x924a7767, /// 0x4c0
			 0x5bad9271, /// 0x4c4
			 0x15406412, /// 0x4c8
			 0x6d9cfd71, /// 0x4cc
			 0x56e19548, /// 0x4d0
			 0x52724e1a, /// 0x4d4
			 0xc7e69be6, /// 0x4d8
			 0x77bee010, /// 0x4dc
			 0x01e8f575, /// 0x4e0
			 0x52a7422a, /// 0x4e4
			 0x1d3f18f1, /// 0x4e8
			 0x1566d531, /// 0x4ec
			 0x843d15ba, /// 0x4f0
			 0x138aa413, /// 0x4f4
			 0x6baad8da, /// 0x4f8
			 0x300e4fa4, /// 0x4fc
			 0xb39d4f7a, /// 0x500
			 0xe17479bd, /// 0x504
			 0x8af6a9b4, /// 0x508
			 0x6527fd4c, /// 0x50c
			 0xafddc817, /// 0x510
			 0x8bd49dde, /// 0x514
			 0xf2211e3e, /// 0x518
			 0x8edef18b, /// 0x51c
			 0x1e2e18ab, /// 0x520
			 0x6f78baef, /// 0x524
			 0xc4cc696e, /// 0x528
			 0x901e968e, /// 0x52c
			 0x26a80151, /// 0x530
			 0x238b1a25, /// 0x534
			 0x0d34f11d, /// 0x538
			 0xfd9cef8a, /// 0x53c
			 0x12630d41, /// 0x540
			 0x78ed8bd2, /// 0x544
			 0x7ac6eef3, /// 0x548
			 0x4c846a79, /// 0x54c
			 0x087591d7, /// 0x550
			 0xe4c06fee, /// 0x554
			 0xe818dabc, /// 0x558
			 0xfe38c143, /// 0x55c
			 0xe7394cd5, /// 0x560
			 0x794cfa84, /// 0x564
			 0x717d4066, /// 0x568
			 0x9a5c4861, /// 0x56c
			 0x85a3cb76, /// 0x570
			 0x0c5aa59a, /// 0x574
			 0x16900d4f, /// 0x578
			 0x9e587311, /// 0x57c
			 0xbc5ff4a5, /// 0x580
			 0xed3c3a70, /// 0x584
			 0xf5be6f3b, /// 0x588
			 0xd89a93c9, /// 0x58c
			 0x2e86ede7, /// 0x590
			 0x3a5f1c75, /// 0x594
			 0xf655ad43, /// 0x598
			 0xe3abeeff, /// 0x59c
			 0xbef5a5c5, /// 0x5a0
			 0x285aae89, /// 0x5a4
			 0xf5e41c13, /// 0x5a8
			 0xe8a6357b, /// 0x5ac
			 0x0b2fc8f4, /// 0x5b0
			 0x8c647091, /// 0x5b4
			 0x25c967b6, /// 0x5b8
			 0x6c593ea6, /// 0x5bc
			 0xf0f06f1f, /// 0x5c0
			 0x4e3dc20f, /// 0x5c4
			 0x6c93cbdb, /// 0x5c8
			 0x198d1da4, /// 0x5cc
			 0x720527a2, /// 0x5d0
			 0x5e867a68, /// 0x5d4
			 0xf083ac29, /// 0x5d8
			 0x920aed02, /// 0x5dc
			 0x2fe0b8e1, /// 0x5e0
			 0x7c09028f, /// 0x5e4
			 0xce2400a5, /// 0x5e8
			 0xb301aca5, /// 0x5ec
			 0x6cf78d58, /// 0x5f0
			 0xef579da5, /// 0x5f4
			 0xbbc1b788, /// 0x5f8
			 0x0a9144fa, /// 0x5fc
			 0x00d0a07a, /// 0x600
			 0xc40fc0b2, /// 0x604
			 0x186e6ef0, /// 0x608
			 0x846994b5, /// 0x60c
			 0x1568c25c, /// 0x610
			 0xae9af640, /// 0x614
			 0x17823a3a, /// 0x618
			 0x675b6906, /// 0x61c
			 0x3cbe0c67, /// 0x620
			 0x991352a7, /// 0x624
			 0xcd0f95b0, /// 0x628
			 0xa74ed5e7, /// 0x62c
			 0xcc3450b5, /// 0x630
			 0xf96cd063, /// 0x634
			 0x5960e60c, /// 0x638
			 0x5fbdd5bd, /// 0x63c
			 0xc46d0dab, /// 0x640
			 0xd5c93a97, /// 0x644
			 0x56e67d03, /// 0x648
			 0x335b2524, /// 0x64c
			 0xd55da5e5, /// 0x650
			 0x3e38a47f, /// 0x654
			 0x77c09bbd, /// 0x658
			 0x35747b07, /// 0x65c
			 0xea0bfa13, /// 0x660
			 0x8402f972, /// 0x664
			 0xcb581700, /// 0x668
			 0x079af195, /// 0x66c
			 0x2f283512, /// 0x670
			 0x963fe7be, /// 0x674
			 0xaf34785d, /// 0x678
			 0x5077a5c9, /// 0x67c
			 0x3193fb41, /// 0x680
			 0xfa9e2cbe, /// 0x684
			 0xe84d52d1, /// 0x688
			 0xf7979e5c, /// 0x68c
			 0xc65fe16d, /// 0x690
			 0xe5262cab, /// 0x694
			 0x252db785, /// 0x698
			 0xf5d863a6, /// 0x69c
			 0x08429ecf, /// 0x6a0
			 0xa0a679bc, /// 0x6a4
			 0x9356ef18, /// 0x6a8
			 0x7f68d1d1, /// 0x6ac
			 0xc1c90e38, /// 0x6b0
			 0x0e8da78e, /// 0x6b4
			 0xece1f571, /// 0x6b8
			 0x623b48eb, /// 0x6bc
			 0x5cba558c, /// 0x6c0
			 0xfc443d05, /// 0x6c4
			 0xb4cdd944, /// 0x6c8
			 0x3b345d03, /// 0x6cc
			 0xfc23822b, /// 0x6d0
			 0x1077ffe9, /// 0x6d4
			 0x5eda0800, /// 0x6d8
			 0x8ce1e178, /// 0x6dc
			 0x81bef051, /// 0x6e0
			 0xd8f77e83, /// 0x6e4
			 0x52ede074, /// 0x6e8
			 0xd836206b, /// 0x6ec
			 0x83d2821c, /// 0x6f0
			 0x2ad41ac8, /// 0x6f4
			 0xf05e85f4, /// 0x6f8
			 0x5b00a1bc, /// 0x6fc
			 0xa888baa0, /// 0x700
			 0xe257fcad, /// 0x704
			 0x38a31bbf, /// 0x708
			 0x390f6b3f, /// 0x70c
			 0x6a28c1a9, /// 0x710
			 0xd18ef987, /// 0x714
			 0x871f2c59, /// 0x718
			 0x047c3b7c, /// 0x71c
			 0xa2637064, /// 0x720
			 0x37855202, /// 0x724
			 0x4a245ea1, /// 0x728
			 0xa9a11f58, /// 0x72c
			 0x2ec09616, /// 0x730
			 0x18b3b9db, /// 0x734
			 0x51ba1e03, /// 0x738
			 0xb4478e64, /// 0x73c
			 0x3734213c, /// 0x740
			 0x9e4cf0e9, /// 0x744
			 0x406ab2d9, /// 0x748
			 0x9997a9ca, /// 0x74c
			 0xcb30ed60, /// 0x750
			 0xafb39923, /// 0x754
			 0x79e4fc97, /// 0x758
			 0x62609a38, /// 0x75c
			 0xaf07da0f, /// 0x760
			 0x0cf3b13a, /// 0x764
			 0x2223cd13, /// 0x768
			 0x9d8906fd, /// 0x76c
			 0x889f87c7, /// 0x770
			 0x9a239bde, /// 0x774
			 0xbfa5d3c2, /// 0x778
			 0x91009ca2, /// 0x77c
			 0xcf4820fe, /// 0x780
			 0xcc7e6782, /// 0x784
			 0xaff8f9cc, /// 0x788
			 0x1435eb9c, /// 0x78c
			 0x143d6e2a, /// 0x790
			 0xd540dfc4, /// 0x794
			 0xe3ad7f32, /// 0x798
			 0x8bd912a1, /// 0x79c
			 0x9daf1fac, /// 0x7a0
			 0xaabed5d8, /// 0x7a4
			 0x19186194, /// 0x7a8
			 0xf77cd716, /// 0x7ac
			 0x4e498740, /// 0x7b0
			 0x4e4d8f47, /// 0x7b4
			 0x7bf25fae, /// 0x7b8
			 0x4ad4b28c, /// 0x7bc
			 0x7f06b932, /// 0x7c0
			 0xdf7eb7f8, /// 0x7c4
			 0xf0dbdb85, /// 0x7c8
			 0x2d08e8f2, /// 0x7cc
			 0x556cc7c3, /// 0x7d0
			 0xc3661dab, /// 0x7d4
			 0x7c928499, /// 0x7d8
			 0xbc86216e, /// 0x7dc
			 0x55045653, /// 0x7e0
			 0xbf40be44, /// 0x7e4
			 0xd9234171, /// 0x7e8
			 0x873a5c41, /// 0x7ec
			 0x289ffd70, /// 0x7f0
			 0xf97bfa1a, /// 0x7f4
			 0xe96406f8, /// 0x7f8
			 0x28ad1488, /// 0x7fc
			 0x2f48bb34, /// 0x800
			 0x4220b890, /// 0x804
			 0x05587600, /// 0x808
			 0xd03c93df, /// 0x80c
			 0x9762de5c, /// 0x810
			 0xfc3e4f12, /// 0x814
			 0x116ff2c1, /// 0x818
			 0x9cbd78b5, /// 0x81c
			 0x778409c5, /// 0x820
			 0xe1457213, /// 0x824
			 0xa6c62333, /// 0x828
			 0x368f176a, /// 0x82c
			 0xc93be168, /// 0x830
			 0x91554460, /// 0x834
			 0x7352afe6, /// 0x838
			 0xec288180, /// 0x83c
			 0x5390ac6b, /// 0x840
			 0x524890ca, /// 0x844
			 0x48bb72bc, /// 0x848
			 0x37b47991, /// 0x84c
			 0x890898e1, /// 0x850
			 0x5145edd2, /// 0x854
			 0x43898992, /// 0x858
			 0x1a1c702c, /// 0x85c
			 0xbd9c1438, /// 0x860
			 0xfdf19c37, /// 0x864
			 0xe659a0be, /// 0x868
			 0x1e1339a2, /// 0x86c
			 0x7b167636, /// 0x870
			 0x4fafa873, /// 0x874
			 0x377de4ad, /// 0x878
			 0xe59230b4, /// 0x87c
			 0xdf214af1, /// 0x880
			 0x44b0fce5, /// 0x884
			 0x90ba49af, /// 0x888
			 0x691a1dd1, /// 0x88c
			 0x1807b54b, /// 0x890
			 0x0a4c4aab, /// 0x894
			 0x37daf2dc, /// 0x898
			 0x063ce178, /// 0x89c
			 0x2ae2b175, /// 0x8a0
			 0x803fb17a, /// 0x8a4
			 0xa16a96ec, /// 0x8a8
			 0x715baa2f, /// 0x8ac
			 0x03c2d4be, /// 0x8b0
			 0xa578af2a, /// 0x8b4
			 0x5d4d18c8, /// 0x8b8
			 0xabc871e6, /// 0x8bc
			 0x0ec0a966, /// 0x8c0
			 0xecb166ec, /// 0x8c4
			 0x0efee6eb, /// 0x8c8
			 0xcadf8c4a, /// 0x8cc
			 0x6593f84e, /// 0x8d0
			 0x5df5c819, /// 0x8d4
			 0xf83ff599, /// 0x8d8
			 0xb7700e0f, /// 0x8dc
			 0xc916cec7, /// 0x8e0
			 0xebedf338, /// 0x8e4
			 0xed681af3, /// 0x8e8
			 0xe9183f3d, /// 0x8ec
			 0x0b77db22, /// 0x8f0
			 0xc581c85c, /// 0x8f4
			 0x63cffe99, /// 0x8f8
			 0x97b83e99, /// 0x8fc
			 0x418c13ef, /// 0x900
			 0x079ad57e, /// 0x904
			 0x230b184a, /// 0x908
			 0x4c71337a, /// 0x90c
			 0x18f9e5b8, /// 0x910
			 0xe5e439a3, /// 0x914
			 0xb0aa194b, /// 0x918
			 0x55afd016, /// 0x91c
			 0x4ea10e14, /// 0x920
			 0x16a0e927, /// 0x924
			 0x35d1520e, /// 0x928
			 0xeace4c79, /// 0x92c
			 0x3616e813, /// 0x930
			 0xcb899fa8, /// 0x934
			 0xfebba1f3, /// 0x938
			 0xf92b19da, /// 0x93c
			 0x4748ea53, /// 0x940
			 0xb1ea7107, /// 0x944
			 0x10469de6, /// 0x948
			 0x536966a8, /// 0x94c
			 0xcbeee81b, /// 0x950
			 0x34183a19, /// 0x954
			 0x68750668, /// 0x958
			 0xb7726d2b, /// 0x95c
			 0x8e180cf9, /// 0x960
			 0x10513208, /// 0x964
			 0x3b2a0148, /// 0x968
			 0xa249f81b, /// 0x96c
			 0xaa72b5d9, /// 0x970
			 0x34a0629f, /// 0x974
			 0x8cf717b2, /// 0x978
			 0xa64eb9b4, /// 0x97c
			 0x551a6761, /// 0x980
			 0x81bb66fb, /// 0x984
			 0xdfab35f1, /// 0x988
			 0x86b166d3, /// 0x98c
			 0xc8cee2f7, /// 0x990
			 0x9f578975, /// 0x994
			 0xf265051f, /// 0x998
			 0x0eb5c8b2, /// 0x99c
			 0x2a38f6b0, /// 0x9a0
			 0x8e15bd35, /// 0x9a4
			 0x05abe1ea, /// 0x9a8
			 0x110160c5, /// 0x9ac
			 0x2908e6ed, /// 0x9b0
			 0x81ee11d1, /// 0x9b4
			 0x580bc8bb, /// 0x9b8
			 0xe113fb56, /// 0x9bc
			 0xffded7ba, /// 0x9c0
			 0x28a1a8dc, /// 0x9c4
			 0x00ee4a68, /// 0x9c8
			 0x63877e46, /// 0x9cc
			 0x8e510035, /// 0x9d0
			 0xdff2f3ff, /// 0x9d4
			 0x4b72caee, /// 0x9d8
			 0x308f8db0, /// 0x9dc
			 0xeeb4841a, /// 0x9e0
			 0x78eac853, /// 0x9e4
			 0x7cde78de, /// 0x9e8
			 0xaa2ea087, /// 0x9ec
			 0x2a54d157, /// 0x9f0
			 0x364c8b8e, /// 0x9f4
			 0x55d4bb39, /// 0x9f8
			 0x4f112402, /// 0x9fc
			 0x7350d00c, /// 0xa00
			 0x708bdb35, /// 0xa04
			 0x0f52c245, /// 0xa08
			 0x5c192d0e, /// 0xa0c
			 0x51c4d0e3, /// 0xa10
			 0x6f607e36, /// 0xa14
			 0xc41f8da0, /// 0xa18
			 0x9c383b5f, /// 0xa1c
			 0x0e0f0ab1, /// 0xa20
			 0x57f6db87, /// 0xa24
			 0x921f47b2, /// 0xa28
			 0x7fbdd1dd, /// 0xa2c
			 0x1f0a160a, /// 0xa30
			 0x5211f8a9, /// 0xa34
			 0x35e85100, /// 0xa38
			 0x37f634f6, /// 0xa3c
			 0x1069d844, /// 0xa40
			 0x24094c79, /// 0xa44
			 0xd8c88698, /// 0xa48
			 0xea277265, /// 0xa4c
			 0xf540ebca, /// 0xa50
			 0x3faaeaaa, /// 0xa54
			 0x1ec3063d, /// 0xa58
			 0x7ea410b3, /// 0xa5c
			 0x6337d18d, /// 0xa60
			 0x6eabfc89, /// 0xa64
			 0x0589190b, /// 0xa68
			 0x0e65afb2, /// 0xa6c
			 0x03334a92, /// 0xa70
			 0x862e637b, /// 0xa74
			 0x98767bca, /// 0xa78
			 0xaddf10d4, /// 0xa7c
			 0x10f3a3e6, /// 0xa80
			 0x85f6ec67, /// 0xa84
			 0x26980a3c, /// 0xa88
			 0xccfc071e, /// 0xa8c
			 0xf62384f1, /// 0xa90
			 0x06ff380a, /// 0xa94
			 0x81ac5e26, /// 0xa98
			 0x14473b1b, /// 0xa9c
			 0x2c4b9d21, /// 0xaa0
			 0x870da7ad, /// 0xaa4
			 0xca29081a, /// 0xaa8
			 0xe2dac8f9, /// 0xaac
			 0xeb14bbec, /// 0xab0
			 0xf9a838e7, /// 0xab4
			 0x549b3c9f, /// 0xab8
			 0x3126bdbd, /// 0xabc
			 0x543089ac, /// 0xac0
			 0x5ebbb601, /// 0xac4
			 0xf68adf73, /// 0xac8
			 0x32a7268c, /// 0xacc
			 0x26082c95, /// 0xad0
			 0x28752cae, /// 0xad4
			 0x67f263f6, /// 0xad8
			 0x5b977cfd, /// 0xadc
			 0x94da1483, /// 0xae0
			 0x06d599fa, /// 0xae4
			 0x1c6f64f7, /// 0xae8
			 0x9de30c8b, /// 0xaec
			 0x243bbff4, /// 0xaf0
			 0xbca68895, /// 0xaf4
			 0xc77ce866, /// 0xaf8
			 0x050f6306, /// 0xafc
			 0x1fdb8881, /// 0xb00
			 0x5c39e577, /// 0xb04
			 0xead97898, /// 0xb08
			 0x92ba00af, /// 0xb0c
			 0xe0794d65, /// 0xb10
			 0xe73acb5c, /// 0xb14
			 0xd24554bd, /// 0xb18
			 0x49bf492c, /// 0xb1c
			 0x3ad34aa0, /// 0xb20
			 0xba92fbe1, /// 0xb24
			 0x1c89f3f7, /// 0xb28
			 0x180261e9, /// 0xb2c
			 0xf348927d, /// 0xb30
			 0x8d12cad7, /// 0xb34
			 0x00425256, /// 0xb38
			 0x6ef25f6d, /// 0xb3c
			 0x2c6ff083, /// 0xb40
			 0x04ad550d, /// 0xb44
			 0xd33a5ad0, /// 0xb48
			 0xef8ba117, /// 0xb4c
			 0xac71605b, /// 0xb50
			 0x6a2f3d55, /// 0xb54
			 0x35d65006, /// 0xb58
			 0xd4bbde07, /// 0xb5c
			 0x5f2ac347, /// 0xb60
			 0x8d2ea6e4, /// 0xb64
			 0x9ba02bb0, /// 0xb68
			 0x2b74e735, /// 0xb6c
			 0xdf6707ae, /// 0xb70
			 0xb1868ff9, /// 0xb74
			 0x8c18df9f, /// 0xb78
			 0xed78c761, /// 0xb7c
			 0x08011661, /// 0xb80
			 0x2c106f95, /// 0xb84
			 0x622e2872, /// 0xb88
			 0x593892db, /// 0xb8c
			 0x4b8c1184, /// 0xb90
			 0x05183187, /// 0xb94
			 0xd1d432ba, /// 0xb98
			 0x564d87f8, /// 0xb9c
			 0x7b866b7b, /// 0xba0
			 0x1da05145, /// 0xba4
			 0x9ac725ed, /// 0xba8
			 0x31c3830e, /// 0xbac
			 0xdbd90d39, /// 0xbb0
			 0x6d4ed968, /// 0xbb4
			 0x1af41408, /// 0xbb8
			 0x2dd79523, /// 0xbbc
			 0xc9c6f7f4, /// 0xbc0
			 0x9e491929, /// 0xbc4
			 0x55e6f035, /// 0xbc8
			 0xcab93d5c, /// 0xbcc
			 0xe7fcdd0d, /// 0xbd0
			 0x64d1f9d4, /// 0xbd4
			 0xe6924290, /// 0xbd8
			 0x03df9924, /// 0xbdc
			 0x3790288e, /// 0xbe0
			 0x804928f2, /// 0xbe4
			 0xb0742df1, /// 0xbe8
			 0x96aca085, /// 0xbec
			 0x1edb6e23, /// 0xbf0
			 0x9d1c8699, /// 0xbf4
			 0xb6a11a19, /// 0xbf8
			 0x5123b548, /// 0xbfc
			 0x8f5e8ee6, /// 0xc00
			 0x41bc146b, /// 0xc04
			 0xd396d77d, /// 0xc08
			 0x67bf034f, /// 0xc0c
			 0xbf0c9fbb, /// 0xc10
			 0xe02797eb, /// 0xc14
			 0x31ed712d, /// 0xc18
			 0xf839361c, /// 0xc1c
			 0x92d93579, /// 0xc20
			 0x5fb70dcf, /// 0xc24
			 0xfb5d7c28, /// 0xc28
			 0xd4330085, /// 0xc2c
			 0x6120b42d, /// 0xc30
			 0xf792a660, /// 0xc34
			 0x1448550d, /// 0xc38
			 0xaaa70a57, /// 0xc3c
			 0x8882b370, /// 0xc40
			 0xfaf8f577, /// 0xc44
			 0x2ebae270, /// 0xc48
			 0x6a15b5a6, /// 0xc4c
			 0x52634533, /// 0xc50
			 0x615998e1, /// 0xc54
			 0xafaeda44, /// 0xc58
			 0x77c54ee7, /// 0xc5c
			 0x9298dcf8, /// 0xc60
			 0xabfd5424, /// 0xc64
			 0xe04ff2ff, /// 0xc68
			 0x8b548e10, /// 0xc6c
			 0x4b1f03c4, /// 0xc70
			 0x235e842b, /// 0xc74
			 0xe4fb3008, /// 0xc78
			 0xf026e30d, /// 0xc7c
			 0x516f5cbf, /// 0xc80
			 0x9978de63, /// 0xc84
			 0x1ec16eb5, /// 0xc88
			 0xb27e0bb9, /// 0xc8c
			 0x062a5541, /// 0xc90
			 0xc1d527bd, /// 0xc94
			 0x60f9e880, /// 0xc98
			 0xc3105f86, /// 0xc9c
			 0x87fd06d5, /// 0xca0
			 0xead3b791, /// 0xca4
			 0xc8cff53e, /// 0xca8
			 0x26a61146, /// 0xcac
			 0xf32a6152, /// 0xcb0
			 0x39552b6e, /// 0xcb4
			 0x2ba36176, /// 0xcb8
			 0x3442b9dc, /// 0xcbc
			 0xded17696, /// 0xcc0
			 0x56b38f29, /// 0xcc4
			 0xe21975ff, /// 0xcc8
			 0x93067790, /// 0xccc
			 0xd5989acf, /// 0xcd0
			 0x00da2222, /// 0xcd4
			 0x980b8891, /// 0xcd8
			 0x030e018e, /// 0xcdc
			 0xa6d8accf, /// 0xce0
			 0x2222f0f2, /// 0xce4
			 0xf0fc8006, /// 0xce8
			 0xee6d2450, /// 0xcec
			 0x856a56c2, /// 0xcf0
			 0x34f43647, /// 0xcf4
			 0xe601c5bd, /// 0xcf8
			 0xb4f853ee, /// 0xcfc
			 0xc2cf559a, /// 0xd00
			 0xfe158563, /// 0xd04
			 0xc5893162, /// 0xd08
			 0x9322198c, /// 0xd0c
			 0x3d9b365e, /// 0xd10
			 0x6e6c5a58, /// 0xd14
			 0x7dadda1f, /// 0xd18
			 0x15ba9156, /// 0xd1c
			 0x52107871, /// 0xd20
			 0x126e7543, /// 0xd24
			 0x85dbd5cc, /// 0xd28
			 0x96770004, /// 0xd2c
			 0x38e6cf79, /// 0xd30
			 0xba95d0ae, /// 0xd34
			 0xb4ab57a5, /// 0xd38
			 0x0d5b11d8, /// 0xd3c
			 0x5286625e, /// 0xd40
			 0xf85318d8, /// 0xd44
			 0x5ac3d053, /// 0xd48
			 0xb2e0902b, /// 0xd4c
			 0x4d4418ca, /// 0xd50
			 0x60e0d80f, /// 0xd54
			 0xc21d5c9d, /// 0xd58
			 0xb4d3a826, /// 0xd5c
			 0xb41ba7e6, /// 0xd60
			 0x089358e3, /// 0xd64
			 0x2f321281, /// 0xd68
			 0x585f73dd, /// 0xd6c
			 0x306ac089, /// 0xd70
			 0xcb2d618f, /// 0xd74
			 0xf382a529, /// 0xd78
			 0xa4ca5455, /// 0xd7c
			 0x5f58681b, /// 0xd80
			 0x54d3dfd5, /// 0xd84
			 0x05d97b66, /// 0xd88
			 0x5e6bcd5d, /// 0xd8c
			 0x50f42029, /// 0xd90
			 0xe95b244a, /// 0xd94
			 0xd10555b1, /// 0xd98
			 0x863c0cc0, /// 0xd9c
			 0xd9a74434, /// 0xda0
			 0xf806f75e, /// 0xda4
			 0x593915ca, /// 0xda8
			 0x6d789779, /// 0xdac
			 0x092e5837, /// 0xdb0
			 0x50f26ff8, /// 0xdb4
			 0x52dfe888, /// 0xdb8
			 0x93488d11, /// 0xdbc
			 0x86e7e16d, /// 0xdc0
			 0xbc8d1f50, /// 0xdc4
			 0x25488768, /// 0xdc8
			 0x3c4ee0e3, /// 0xdcc
			 0x118b5032, /// 0xdd0
			 0x4e237ddd, /// 0xdd4
			 0x3e8af82e, /// 0xdd8
			 0x5d6695fb, /// 0xddc
			 0x7edb1890, /// 0xde0
			 0xfcc68991, /// 0xde4
			 0x22416d50, /// 0xde8
			 0x635259e9, /// 0xdec
			 0x2e80bea2, /// 0xdf0
			 0xbb9a8daa, /// 0xdf4
			 0x28fb3750, /// 0xdf8
			 0x88e82b16, /// 0xdfc
			 0x68b222e0, /// 0xe00
			 0xbc410fcb, /// 0xe04
			 0x647f4c87, /// 0xe08
			 0x3ff3be01, /// 0xe0c
			 0x46e551aa, /// 0xe10
			 0xc4eca862, /// 0xe14
			 0x608b1421, /// 0xe18
			 0x41b66f01, /// 0xe1c
			 0xcab782f6, /// 0xe20
			 0x404e72aa, /// 0xe24
			 0xb3df02e9, /// 0xe28
			 0x4f792859, /// 0xe2c
			 0xd73d14c7, /// 0xe30
			 0x4dfb276b, /// 0xe34
			 0x426f0e24, /// 0xe38
			 0xa1e2cf74, /// 0xe3c
			 0x7a798501, /// 0xe40
			 0x40eab8fb, /// 0xe44
			 0x7d47b4b9, /// 0xe48
			 0x5fbd31f9, /// 0xe4c
			 0xcb645547, /// 0xe50
			 0xa6a744c4, /// 0xe54
			 0x5f888681, /// 0xe58
			 0xfd61968e, /// 0xe5c
			 0xbb117ad5, /// 0xe60
			 0xd6476abc, /// 0xe64
			 0x570bf467, /// 0xe68
			 0x0300e655, /// 0xe6c
			 0xa05a9f44, /// 0xe70
			 0xf2fb04ad, /// 0xe74
			 0xe9ad2097, /// 0xe78
			 0x95d94ade, /// 0xe7c
			 0xea394783, /// 0xe80
			 0x7e4304d0, /// 0xe84
			 0x07e924bf, /// 0xe88
			 0xe1010a92, /// 0xe8c
			 0x3a95912f, /// 0xe90
			 0x2e204530, /// 0xe94
			 0x810b6af6, /// 0xe98
			 0x7e83452f, /// 0xe9c
			 0x325a5623, /// 0xea0
			 0x72aacd13, /// 0xea4
			 0x18b60aa6, /// 0xea8
			 0x88899f55, /// 0xeac
			 0x9f523ce9, /// 0xeb0
			 0x6ce7743b, /// 0xeb4
			 0x24274e80, /// 0xeb8
			 0xf7f46bea, /// 0xebc
			 0x3f5532c8, /// 0xec0
			 0x666e85b0, /// 0xec4
			 0xc940e59d, /// 0xec8
			 0xf07820e3, /// 0xecc
			 0x6ab185b0, /// 0xed0
			 0x1d3528e3, /// 0xed4
			 0x20f0d26a, /// 0xed8
			 0x70b1759f, /// 0xedc
			 0x03b39752, /// 0xee0
			 0xc962251c, /// 0xee4
			 0xb75a91aa, /// 0xee8
			 0x2220cbff, /// 0xeec
			 0x96ade4de, /// 0xef0
			 0x57e735f0, /// 0xef4
			 0x94ce0c02, /// 0xef8
			 0xae96d2ba, /// 0xefc
			 0xe765749d, /// 0xf00
			 0x0cd8ab0d, /// 0xf04
			 0x168a76cf, /// 0xf08
			 0x9a8bb4a0, /// 0xf0c
			 0xcf81173d, /// 0xf10
			 0xe137f6f0, /// 0xf14
			 0x562af1b4, /// 0xf18
			 0x729e2afa, /// 0xf1c
			 0x7623d49c, /// 0xf20
			 0x21bcc325, /// 0xf24
			 0x2e66c6f9, /// 0xf28
			 0x8156ff8b, /// 0xf2c
			 0x080eff05, /// 0xf30
			 0x258efab4, /// 0xf34
			 0xe5b6aad3, /// 0xf38
			 0x3653ad57, /// 0xf3c
			 0x1ff9f538, /// 0xf40
			 0x67ddba70, /// 0xf44
			 0xe361f621, /// 0xf48
			 0x7e326430, /// 0xf4c
			 0x34dca8e6, /// 0xf50
			 0x69e4b70b, /// 0xf54
			 0x40036308, /// 0xf58
			 0x007e9d8c, /// 0xf5c
			 0xb12e81f7, /// 0xf60
			 0x075858ff, /// 0xf64
			 0x834dfaf2, /// 0xf68
			 0x9f92d5cf, /// 0xf6c
			 0x3b74344c, /// 0xf70
			 0xc432f933, /// 0xf74
			 0x9af4fe3c, /// 0xf78
			 0xabf2c15b, /// 0xf7c
			 0xb45b68c7, /// 0xf80
			 0xb92385f3, /// 0xf84
			 0xbe4fc0a0, /// 0xf88
			 0xffb20c99, /// 0xf8c
			 0x7cf7cdf8, /// 0xf90
			 0xe6223ea5, /// 0xf94
			 0x4aa4500a, /// 0xf98
			 0x7b627b8d, /// 0xf9c
			 0xcdf80e69, /// 0xfa0
			 0x8fd69821, /// 0xfa4
			 0xd15c148e, /// 0xfa8
			 0x3c52573f, /// 0xfac
			 0x4027b6e1, /// 0xfb0
			 0x0dc30595, /// 0xfb4
			 0xe139a904, /// 0xfb8
			 0x42000625, /// 0xfbc
			 0x5493ce4a, /// 0xfc0
			 0x5dd03bf8, /// 0xfc4
			 0x5c283169, /// 0xfc8
			 0x46249bc3, /// 0xfcc
			 0x47f3b49d, /// 0xfd0
			 0x448ba9e3, /// 0xfd4
			 0x17b82835, /// 0xfd8
			 0x22a4737e, /// 0xfdc
			 0xc10384ca, /// 0xfe0
			 0x547f861f, /// 0xfe4
			 0x6aa77a92, /// 0xfe8
			 0x8defc3d7, /// 0xfec
			 0x6586305b, /// 0xff0
			 0xebfebf97, /// 0xff4
			 0x82fb065d, /// 0xff8
			 0xb307d12c /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00004
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00008
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00014
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00018
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0001c
			 0xcb000000,                                                  // -8388608.0                                  /// 00020
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00028
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00030
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00040
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00044
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00048
			 0x0c700000,                                                  // Leading 1s:                                 /// 0004c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00054
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00058
			 0xffc00001,                                                  // -signaling NaN                              /// 0005c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00060
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00064
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00068
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0006c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00074
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00078
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0007c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00084
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0008c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00090
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00094
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00098
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0009c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000ec
			 0x4b000000,                                                  // 8388608.0                                   /// 000f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00108
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00118
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0012c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00130
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00134
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00138
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0013c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00140
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00148
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00150
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00154
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00158
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0015c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00160
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0016c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00170
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00178
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0017c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00180
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0018c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00194
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00198
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80000000,                                                  // -zero                                       /// 00200
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00210
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0xffc00001,                                                  // -signaling NaN                              /// 00218
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00220
			 0x0c600000,                                                  // Leading 1s:                                 /// 00224
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00234
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00238
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0023c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00240
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00244
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0024c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xbf028f5c,                                                  // -0.51                                       /// 00268
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0026c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00270
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00274
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x7fc00001,                                                  // signaling NaN                               /// 0027c
			 0x00000000,                                                  // zero                                        /// 00280
			 0x7f800000,                                                  // inf                                         /// 00284
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00288
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00294
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0029c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 002c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00300
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x3f028f5c,                                                  // 0.51                                        /// 00314
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0031c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00320
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00338
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0033c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00340
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00344
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00348
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00350
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00354
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00364
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00368
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0036c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00374
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00378
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00380
			 0x55555555,                                                  // 4 random values                             /// 00384
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00388
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x7f800000,                                                  // inf                                         /// 003b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00400
			 0x0c700000,                                                  // Leading 1s:                                 /// 00404
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00408
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0040c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00410
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00414
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00418
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0041c
			 0x7fc00001,                                                  // signaling NaN                               /// 00420
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00424
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00434
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00438
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0043c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00440
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00444
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0044c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0x3f028f5c,                                                  // 0.51                                        /// 00460
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00464
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00468
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0046c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00470
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00474
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00478
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0047c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00480
			 0x0c700000,                                                  // Leading 1s:                                 /// 00484
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x0c700000,                                                  // Leading 1s:                                 /// 0048c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00490
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00498
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a0
			 0x80000000,                                                  // -zero                                       /// 004a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x0e000001,                                                  // Trailing 1s:                                /// 0050c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00514
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00518
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0051c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00520
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00524
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00528
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00530
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00534
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00538
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0053c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00544
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00550
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00554
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00558
			 0x7f800000,                                                  // inf                                         /// 0055c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00570
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00574
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0057c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00580
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00584
			 0x0c700000,                                                  // Leading 1s:                                 /// 00588
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0058c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00590
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0059c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00608
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0060c
			 0x55555555,                                                  // 4 random values                             /// 00610
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00618
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0061c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0062c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00634
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00638
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0063c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00644
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00654
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00658
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0065c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00660
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00668
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00670
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00674
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00678
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x7f800000,                                                  // inf                                         /// 00680
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00688
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0068c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00694
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00698
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d4
			 0x4b000000,                                                  // 8388608.0                                   /// 006d8
			 0xff800000,                                                  // -inf                                        /// 006dc
			 0x3f028f5c,                                                  // 0.51                                        /// 006e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00700
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00704
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00708
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0070c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00720
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00724
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00730
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00734
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00738
			 0x7f800000,                                                  // inf                                         /// 0073c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00744
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00748
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0xcb000000,                                                  // -8388608.0                                  /// 00750
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00754
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0075c
			 0x7f800000,                                                  // inf                                         /// 00760
			 0x3f028f5c,                                                  // 0.51                                        /// 00764
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00770
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00774
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0077c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00780
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00784
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0078c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00790
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00794
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00798
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0079c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00804
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00810
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00818
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0081c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00824
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00828
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0082c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00834
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00838
			 0x33333333,                                                  // 4 random values                             /// 0083c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00840
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00848
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00850
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00858
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00860
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00864
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x33333333,                                                  // 4 random values                             /// 0086c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x0e000001,                                                  // Trailing 1s:                                /// 00874
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00880
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00888
			 0x7f800000,                                                  // inf                                         /// 0088c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0089c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x7f800000,                                                  // inf                                         /// 008ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 008d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x7f800000,                                                  // inf                                         /// 008e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00904
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0090c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00910
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00914
			 0x00000000,                                                  // zero                                        /// 00918
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00924
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0092c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00930
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00934
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00938
			 0xffc00001,                                                  // -signaling NaN                              /// 0093c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00940
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00958
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0095c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00964
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x0c400000,                                                  // Leading 1s:                                 /// 00974
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00978
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0097c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00980
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00984
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00988
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00990
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xcb000000,                                                  // -8388608.0                                  /// 0099c
			 0x55555555,                                                  // 4 random values                             /// 009a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c0
			 0x7fc00001,                                                  // signaling NaN                               /// 009c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0xcb000000,                                                  // -8388608.0                                  /// 00a1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a30
			 0x7fc00001,                                                  // signaling NaN                               /// 00a34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a88
			 0xbf028f5c,                                                  // -0.51                                       /// 00a8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ab4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ac0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00adc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ae8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00af4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00af8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00afc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b44
			 0xffc00001,                                                  // -signaling NaN                              /// 00b48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b68
			 0xff800000,                                                  // -inf                                        /// 00b6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0xffc00001,                                                  // -signaling NaN                              /// 00be0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x55555555,                                                  // 4 random values                             /// 00c40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c44
			 0x4b000000,                                                  // 8388608.0                                   /// 00c48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x7f800000,                                                  // inf                                         /// 00c58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c68
			 0x80000000,                                                  // -zero                                       /// 00c6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cb4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ccc
			 0xcb000000,                                                  // -8388608.0                                  /// 00cd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cdc
			 0x80000000,                                                  // -zero                                       /// 00ce0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x4b000000,                                                  // 8388608.0                                   /// 00d08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7f800000,                                                  // inf                                         /// 00d40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d58
			 0x55555555,                                                  // 4 random values                             /// 00d5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d7c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00da4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00db8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dbc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ddc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00de8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00df0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00df4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00df8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dfc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e00
			 0x80000000,                                                  // -zero                                       /// 00e04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x55555555,                                                  // 4 random values                             /// 00e0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x4b000000,                                                  // 8388608.0                                   /// 00e18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e20
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e28
			 0x33333333,                                                  // 4 random values                             /// 00e2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ea0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ebc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ec4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ec8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ed4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00edc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ee8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ef8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00efc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0xff800000,                                                  // -inf                                        /// 00f20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f30
			 0xff800000,                                                  // -inf                                        /// 00f34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f50
			 0x3f028f5c,                                                  // 0.51                                        /// 00f54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fe4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80000002 // min subnorm + 1 ulp                           // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xf6af2b69, /// 0x0
			 0x5cde184a, /// 0x4
			 0x26b23f27, /// 0x8
			 0xe8b63767, /// 0xc
			 0x25ac850b, /// 0x10
			 0x881235fb, /// 0x14
			 0x73f746f1, /// 0x18
			 0x46c734c5, /// 0x1c
			 0x361656e6, /// 0x20
			 0x30436b85, /// 0x24
			 0x048994a7, /// 0x28
			 0x74e72c22, /// 0x2c
			 0x864f83f6, /// 0x30
			 0x143b03a1, /// 0x34
			 0x27d83fc4, /// 0x38
			 0xa6ba4f2e, /// 0x3c
			 0xea986e9e, /// 0x40
			 0x2e97f507, /// 0x44
			 0xc94b87fc, /// 0x48
			 0x7315f38b, /// 0x4c
			 0xbcabfe28, /// 0x50
			 0x3fc5da86, /// 0x54
			 0x94fc0cff, /// 0x58
			 0x121b648e, /// 0x5c
			 0x2d4d07aa, /// 0x60
			 0xa8e2c130, /// 0x64
			 0x30c939dc, /// 0x68
			 0x4129b1c4, /// 0x6c
			 0xf305d738, /// 0x70
			 0x7e4b0539, /// 0x74
			 0xb755a9ba, /// 0x78
			 0x85ae78aa, /// 0x7c
			 0xbf702914, /// 0x80
			 0x25194f59, /// 0x84
			 0xada72edc, /// 0x88
			 0x3703fd00, /// 0x8c
			 0xc08e2130, /// 0x90
			 0x14819f35, /// 0x94
			 0xcf895c4e, /// 0x98
			 0x6412bb1c, /// 0x9c
			 0x5fe2dcb6, /// 0xa0
			 0x2a16b838, /// 0xa4
			 0x0197a52b, /// 0xa8
			 0x9db868ca, /// 0xac
			 0x5e6b6d63, /// 0xb0
			 0x5d72ac18, /// 0xb4
			 0x1ceaefe6, /// 0xb8
			 0xa72a883b, /// 0xbc
			 0x49724c22, /// 0xc0
			 0xdd214638, /// 0xc4
			 0x903a653c, /// 0xc8
			 0xf3fcd4ca, /// 0xcc
			 0xeada9fdb, /// 0xd0
			 0xec55dca7, /// 0xd4
			 0x8bf5ed4a, /// 0xd8
			 0x6ab7f100, /// 0xdc
			 0x29fa008d, /// 0xe0
			 0x26be0a31, /// 0xe4
			 0x1df17195, /// 0xe8
			 0x6f76c1c9, /// 0xec
			 0x0cb2fdb7, /// 0xf0
			 0xcbe4c85f, /// 0xf4
			 0x5639ffba, /// 0xf8
			 0x95303c89, /// 0xfc
			 0x4194c797, /// 0x100
			 0x8b7bcf43, /// 0x104
			 0x74b53a60, /// 0x108
			 0x43f05d39, /// 0x10c
			 0xd8305037, /// 0x110
			 0x197519a0, /// 0x114
			 0x66683fe5, /// 0x118
			 0xf94d2527, /// 0x11c
			 0x09533c68, /// 0x120
			 0xe0baba60, /// 0x124
			 0xe0b3dbe9, /// 0x128
			 0x4dcba1ad, /// 0x12c
			 0xdab7edb0, /// 0x130
			 0x9e55ba2b, /// 0x134
			 0x21a3dc40, /// 0x138
			 0xae9f3472, /// 0x13c
			 0xc189fbf6, /// 0x140
			 0x5c36b985, /// 0x144
			 0x3e0f38eb, /// 0x148
			 0x5531aae1, /// 0x14c
			 0xc3001c6f, /// 0x150
			 0xcd84ce9e, /// 0x154
			 0x296a1571, /// 0x158
			 0x3a14d258, /// 0x15c
			 0x3d6ed9c7, /// 0x160
			 0xb0d248d8, /// 0x164
			 0x83ad8c32, /// 0x168
			 0x1f1bbc28, /// 0x16c
			 0x7ecc6826, /// 0x170
			 0x1d13935e, /// 0x174
			 0x0c09bcc1, /// 0x178
			 0xf2884f1c, /// 0x17c
			 0x1c506661, /// 0x180
			 0xa72e65b3, /// 0x184
			 0x94116561, /// 0x188
			 0xb23b6a64, /// 0x18c
			 0x52b31e56, /// 0x190
			 0x8158c2b2, /// 0x194
			 0x3b19da18, /// 0x198
			 0xc7767a15, /// 0x19c
			 0x4eee60f7, /// 0x1a0
			 0xb8033975, /// 0x1a4
			 0xcbf6d1c3, /// 0x1a8
			 0x716ae679, /// 0x1ac
			 0x4a943aca, /// 0x1b0
			 0x3e30abb0, /// 0x1b4
			 0x8476a0db, /// 0x1b8
			 0x7f95a076, /// 0x1bc
			 0xd487aeaf, /// 0x1c0
			 0xd8d82594, /// 0x1c4
			 0x443e5197, /// 0x1c8
			 0x7a481a73, /// 0x1cc
			 0x216efc82, /// 0x1d0
			 0xc3203d3f, /// 0x1d4
			 0x7fcdb6cf, /// 0x1d8
			 0xf23b9910, /// 0x1dc
			 0x1c732b2f, /// 0x1e0
			 0xfdea6049, /// 0x1e4
			 0xfbeceb28, /// 0x1e8
			 0x95946640, /// 0x1ec
			 0xddc54ce1, /// 0x1f0
			 0x6dda9449, /// 0x1f4
			 0x7ef94b4a, /// 0x1f8
			 0xa484c905, /// 0x1fc
			 0xc4fca950, /// 0x200
			 0x63cca0eb, /// 0x204
			 0xeeb9dcbe, /// 0x208
			 0xf97ff344, /// 0x20c
			 0xb0cbd1b3, /// 0x210
			 0x565f2265, /// 0x214
			 0x7b4e918e, /// 0x218
			 0x0fad5b89, /// 0x21c
			 0x4ff467d0, /// 0x220
			 0x824f8646, /// 0x224
			 0xef433117, /// 0x228
			 0xfc2eee54, /// 0x22c
			 0xbf3edbfb, /// 0x230
			 0x1b709884, /// 0x234
			 0x422ccef8, /// 0x238
			 0x12f03a19, /// 0x23c
			 0xc08546fa, /// 0x240
			 0xefbb4b2d, /// 0x244
			 0x32bf07ad, /// 0x248
			 0x79106d79, /// 0x24c
			 0xac8a9b68, /// 0x250
			 0xfb728a41, /// 0x254
			 0xdb17d55d, /// 0x258
			 0x742e4ce5, /// 0x25c
			 0xde62aec0, /// 0x260
			 0xa8e21ead, /// 0x264
			 0x56e41f35, /// 0x268
			 0x61765614, /// 0x26c
			 0xcd071202, /// 0x270
			 0x2778d186, /// 0x274
			 0x5a1b4e68, /// 0x278
			 0xed06f440, /// 0x27c
			 0x45ea6bad, /// 0x280
			 0x37c21b11, /// 0x284
			 0x26e1ce26, /// 0x288
			 0xd5c34dec, /// 0x28c
			 0x9509e498, /// 0x290
			 0x4e65f94c, /// 0x294
			 0x98d4f544, /// 0x298
			 0x4d359058, /// 0x29c
			 0xba00d3f3, /// 0x2a0
			 0x8fe70c71, /// 0x2a4
			 0x1d0f7686, /// 0x2a8
			 0xe6a4861f, /// 0x2ac
			 0x63e03b3a, /// 0x2b0
			 0xed88e3bc, /// 0x2b4
			 0x3a323cbf, /// 0x2b8
			 0xbb4b6f07, /// 0x2bc
			 0x5463f53c, /// 0x2c0
			 0xb5fc1eb4, /// 0x2c4
			 0xac09ee12, /// 0x2c8
			 0x1c7bdd90, /// 0x2cc
			 0x99ac9ad1, /// 0x2d0
			 0x2e10b667, /// 0x2d4
			 0x68800e16, /// 0x2d8
			 0xa3ec2a56, /// 0x2dc
			 0xf79b013a, /// 0x2e0
			 0xb472e5f4, /// 0x2e4
			 0x588231bd, /// 0x2e8
			 0x8e2df1de, /// 0x2ec
			 0x84d77767, /// 0x2f0
			 0x88ac05d5, /// 0x2f4
			 0x250a4647, /// 0x2f8
			 0xe8263545, /// 0x2fc
			 0xc48257ed, /// 0x300
			 0x8dec51e0, /// 0x304
			 0xde85a1b9, /// 0x308
			 0x3be864a8, /// 0x30c
			 0xa4698837, /// 0x310
			 0xfc7d3672, /// 0x314
			 0xf76e0cc9, /// 0x318
			 0xb8e32198, /// 0x31c
			 0x1658a32a, /// 0x320
			 0xd3921984, /// 0x324
			 0x59efa152, /// 0x328
			 0x8ccb5733, /// 0x32c
			 0x76882c37, /// 0x330
			 0x99af2693, /// 0x334
			 0x5684bfa7, /// 0x338
			 0xa6873b0c, /// 0x33c
			 0x6eca456c, /// 0x340
			 0xfcb4c5e4, /// 0x344
			 0x5438cde0, /// 0x348
			 0xe06290a3, /// 0x34c
			 0xbd6a707d, /// 0x350
			 0x0240b301, /// 0x354
			 0x5f96fbaa, /// 0x358
			 0x17972f8a, /// 0x35c
			 0xfbaf90f4, /// 0x360
			 0x44a83e9c, /// 0x364
			 0xaeb68525, /// 0x368
			 0xc6928652, /// 0x36c
			 0xaeedf2f9, /// 0x370
			 0xfe039584, /// 0x374
			 0x0af348c5, /// 0x378
			 0xf7b42224, /// 0x37c
			 0x211a4e4d, /// 0x380
			 0xdceb1553, /// 0x384
			 0x749aea59, /// 0x388
			 0xdd07defe, /// 0x38c
			 0x8e301503, /// 0x390
			 0xf83ae0ba, /// 0x394
			 0x704cb013, /// 0x398
			 0xc34a3049, /// 0x39c
			 0xa3fa3e33, /// 0x3a0
			 0xa608411f, /// 0x3a4
			 0xe931ba04, /// 0x3a8
			 0xb5802d2c, /// 0x3ac
			 0x4a9d4182, /// 0x3b0
			 0x121118e8, /// 0x3b4
			 0xdc2c38f4, /// 0x3b8
			 0xbd65a8ff, /// 0x3bc
			 0xe8124cdb, /// 0x3c0
			 0xe1b8b62c, /// 0x3c4
			 0x2aca6cf8, /// 0x3c8
			 0x8f70c3e2, /// 0x3cc
			 0xe649042c, /// 0x3d0
			 0x11111d57, /// 0x3d4
			 0x62ecc73d, /// 0x3d8
			 0x8cbd7c57, /// 0x3dc
			 0x51b30401, /// 0x3e0
			 0x730874e5, /// 0x3e4
			 0x0bd7bc67, /// 0x3e8
			 0xc8a5d225, /// 0x3ec
			 0x07961665, /// 0x3f0
			 0x027d342b, /// 0x3f4
			 0x0a491a46, /// 0x3f8
			 0x9c667746, /// 0x3fc
			 0x6ddfa0fc, /// 0x400
			 0x75bcb5bc, /// 0x404
			 0xda06c377, /// 0x408
			 0x6840945a, /// 0x40c
			 0x66dcd150, /// 0x410
			 0x2adbf115, /// 0x414
			 0x2992bbb9, /// 0x418
			 0x940b0b41, /// 0x41c
			 0x1fe21f24, /// 0x420
			 0x6f3d0794, /// 0x424
			 0x154efdc7, /// 0x428
			 0x09ea1aeb, /// 0x42c
			 0x90e41b68, /// 0x430
			 0xd2bd0ef9, /// 0x434
			 0xf623fa51, /// 0x438
			 0x480cd877, /// 0x43c
			 0xc818fc43, /// 0x440
			 0xb3a393a6, /// 0x444
			 0xdd1ac8c6, /// 0x448
			 0x31d384f8, /// 0x44c
			 0x79ebbe2b, /// 0x450
			 0x745103ad, /// 0x454
			 0x108db111, /// 0x458
			 0x8c617f28, /// 0x45c
			 0xebc10cb9, /// 0x460
			 0x05645f95, /// 0x464
			 0x2590d8cb, /// 0x468
			 0x3f9e842c, /// 0x46c
			 0xbcbc4e02, /// 0x470
			 0x3199ef00, /// 0x474
			 0xf1bab96f, /// 0x478
			 0x6e506d05, /// 0x47c
			 0x6e8b6c6e, /// 0x480
			 0x9c04c155, /// 0x484
			 0xf9d6f187, /// 0x488
			 0xc8d69695, /// 0x48c
			 0x732b8af1, /// 0x490
			 0x824fbbfa, /// 0x494
			 0x64e5c2e4, /// 0x498
			 0xc1413dad, /// 0x49c
			 0xf465183b, /// 0x4a0
			 0x89517469, /// 0x4a4
			 0xece30e21, /// 0x4a8
			 0x2d1e5112, /// 0x4ac
			 0x9e5d0efe, /// 0x4b0
			 0xf9a28f4f, /// 0x4b4
			 0xc3391fe8, /// 0x4b8
			 0x0a289719, /// 0x4bc
			 0x43e10713, /// 0x4c0
			 0xf24e10a0, /// 0x4c4
			 0x369d1110, /// 0x4c8
			 0xf39a5ff4, /// 0x4cc
			 0x8defd9d0, /// 0x4d0
			 0x7fd6bdd7, /// 0x4d4
			 0x0f044d9e, /// 0x4d8
			 0x0473613a, /// 0x4dc
			 0x113fa439, /// 0x4e0
			 0xda5a1fc9, /// 0x4e4
			 0x6b275fee, /// 0x4e8
			 0x3d6d6e0b, /// 0x4ec
			 0x181cff62, /// 0x4f0
			 0x621e3b48, /// 0x4f4
			 0x0d1ab869, /// 0x4f8
			 0x0968ede9, /// 0x4fc
			 0x97cf7dd7, /// 0x500
			 0x006b1142, /// 0x504
			 0xa12089d0, /// 0x508
			 0x1880289b, /// 0x50c
			 0xcd01abd0, /// 0x510
			 0xd7171caf, /// 0x514
			 0xa8a9e3ca, /// 0x518
			 0x2dd5d8f0, /// 0x51c
			 0x52bbc93b, /// 0x520
			 0x3286c2aa, /// 0x524
			 0x6c7f1fa3, /// 0x528
			 0x857d4e46, /// 0x52c
			 0xa5ff44d0, /// 0x530
			 0x4ce9a2ac, /// 0x534
			 0x4ed0eef1, /// 0x538
			 0x74166b19, /// 0x53c
			 0xc9b13af3, /// 0x540
			 0x1f18de3f, /// 0x544
			 0x12a04be4, /// 0x548
			 0x9508c5ec, /// 0x54c
			 0xa12142cd, /// 0x550
			 0x05ff3224, /// 0x554
			 0xdec880c6, /// 0x558
			 0x59ef1a8f, /// 0x55c
			 0x727b88f6, /// 0x560
			 0x4b153b54, /// 0x564
			 0x51bc607f, /// 0x568
			 0xd7399c64, /// 0x56c
			 0x9f2e989e, /// 0x570
			 0xec16cd25, /// 0x574
			 0xbc07fb45, /// 0x578
			 0xe6b87a31, /// 0x57c
			 0x45cc26d2, /// 0x580
			 0xec0f34a6, /// 0x584
			 0xbce6ea25, /// 0x588
			 0x6cd1ab41, /// 0x58c
			 0xdf0e75b5, /// 0x590
			 0xe020a634, /// 0x594
			 0x03c30397, /// 0x598
			 0xc7a85e50, /// 0x59c
			 0xcccab99b, /// 0x5a0
			 0xfde89a75, /// 0x5a4
			 0xdfcf0bc4, /// 0x5a8
			 0xd053d6ce, /// 0x5ac
			 0xcb11f864, /// 0x5b0
			 0x4b418f56, /// 0x5b4
			 0xb264f3d1, /// 0x5b8
			 0x016d7b30, /// 0x5bc
			 0xdd83a2bc, /// 0x5c0
			 0xd742a563, /// 0x5c4
			 0x7cb6483a, /// 0x5c8
			 0x7fcb68bd, /// 0x5cc
			 0x822b8b9e, /// 0x5d0
			 0x1b83e605, /// 0x5d4
			 0xe9e51ac8, /// 0x5d8
			 0xfba23d24, /// 0x5dc
			 0x054dbe9d, /// 0x5e0
			 0xc8760f3a, /// 0x5e4
			 0x94da4ca8, /// 0x5e8
			 0x82594705, /// 0x5ec
			 0x8d4a0cea, /// 0x5f0
			 0xd10ed811, /// 0x5f4
			 0xd553d8e9, /// 0x5f8
			 0xce2ce07e, /// 0x5fc
			 0x75cd801e, /// 0x600
			 0x6d34e759, /// 0x604
			 0xfbedec0c, /// 0x608
			 0x1da00fc9, /// 0x60c
			 0xfa501f3d, /// 0x610
			 0x1a835a39, /// 0x614
			 0x9451bc43, /// 0x618
			 0x0862f271, /// 0x61c
			 0x8d888382, /// 0x620
			 0xd4768fc3, /// 0x624
			 0x7a02b718, /// 0x628
			 0xdf284810, /// 0x62c
			 0x67793b7e, /// 0x630
			 0x2acd63aa, /// 0x634
			 0x51c152e4, /// 0x638
			 0xd934142d, /// 0x63c
			 0x35e4a95c, /// 0x640
			 0x170ddef4, /// 0x644
			 0x9b83bddb, /// 0x648
			 0xf6e549db, /// 0x64c
			 0xe4ddf452, /// 0x650
			 0xdaaa38c7, /// 0x654
			 0x911e5843, /// 0x658
			 0x14e05528, /// 0x65c
			 0xcf9ea775, /// 0x660
			 0x28c1a42f, /// 0x664
			 0x15bb7fe4, /// 0x668
			 0x6732350d, /// 0x66c
			 0x82087463, /// 0x670
			 0x5be5148f, /// 0x674
			 0xcb3bcd11, /// 0x678
			 0x4adc2a44, /// 0x67c
			 0xe9a22b7a, /// 0x680
			 0x4510115b, /// 0x684
			 0x3a1dd0d2, /// 0x688
			 0x700f9f65, /// 0x68c
			 0x4efaae84, /// 0x690
			 0x45b43cc7, /// 0x694
			 0x7cd03387, /// 0x698
			 0x21d1d7d5, /// 0x69c
			 0xb9d3bf16, /// 0x6a0
			 0x4557793c, /// 0x6a4
			 0x177f0e7e, /// 0x6a8
			 0x50e199e9, /// 0x6ac
			 0x46644b6e, /// 0x6b0
			 0xae18e09e, /// 0x6b4
			 0x48e3df7f, /// 0x6b8
			 0xd20ed3ef, /// 0x6bc
			 0x792aba8e, /// 0x6c0
			 0x8af1cdcd, /// 0x6c4
			 0xde419657, /// 0x6c8
			 0x52705260, /// 0x6cc
			 0x53ac7722, /// 0x6d0
			 0xb6b609fc, /// 0x6d4
			 0x484b068c, /// 0x6d8
			 0x53948b7d, /// 0x6dc
			 0xff21530e, /// 0x6e0
			 0xf492ad63, /// 0x6e4
			 0x6cb5f64d, /// 0x6e8
			 0xee7b06a1, /// 0x6ec
			 0x8008c5f6, /// 0x6f0
			 0x8e98914b, /// 0x6f4
			 0x39372b3d, /// 0x6f8
			 0xbec782dd, /// 0x6fc
			 0x0875f01e, /// 0x700
			 0x8d5ebbce, /// 0x704
			 0xf841a1b4, /// 0x708
			 0x40159c56, /// 0x70c
			 0xd493e39b, /// 0x710
			 0xa7b89b2b, /// 0x714
			 0x58bbbe5b, /// 0x718
			 0x64ea1a01, /// 0x71c
			 0xadd098ad, /// 0x720
			 0x9e44741d, /// 0x724
			 0x76c4506b, /// 0x728
			 0xa40a0132, /// 0x72c
			 0x13381c82, /// 0x730
			 0xa35d8589, /// 0x734
			 0x178fff6f, /// 0x738
			 0x211e1489, /// 0x73c
			 0x5a55ef64, /// 0x740
			 0xc6cfe5c8, /// 0x744
			 0x1b1d65b5, /// 0x748
			 0xbf93c149, /// 0x74c
			 0x48884643, /// 0x750
			 0xabc8e1ff, /// 0x754
			 0xf6b98753, /// 0x758
			 0x7f4b0154, /// 0x75c
			 0x31256c44, /// 0x760
			 0xcf582336, /// 0x764
			 0x7556fd4e, /// 0x768
			 0x09576c91, /// 0x76c
			 0x3834b157, /// 0x770
			 0x5adf8c9a, /// 0x774
			 0x6d33b254, /// 0x778
			 0x4c42043e, /// 0x77c
			 0x458efb97, /// 0x780
			 0xb185bc69, /// 0x784
			 0x6205a907, /// 0x788
			 0x9ea079cf, /// 0x78c
			 0xcf8df950, /// 0x790
			 0xe10b2bed, /// 0x794
			 0xa224bb11, /// 0x798
			 0x8edb9f8b, /// 0x79c
			 0x0a456373, /// 0x7a0
			 0x19ede515, /// 0x7a4
			 0xf0be8612, /// 0x7a8
			 0x1e1dc3de, /// 0x7ac
			 0x96e7a821, /// 0x7b0
			 0x68112e5c, /// 0x7b4
			 0x3390abb9, /// 0x7b8
			 0xeae7f304, /// 0x7bc
			 0xf2ee43ee, /// 0x7c0
			 0xe73e3da6, /// 0x7c4
			 0x0fd486a0, /// 0x7c8
			 0x498d5164, /// 0x7cc
			 0x7ad8b6b8, /// 0x7d0
			 0x27b1923b, /// 0x7d4
			 0x8fbdd010, /// 0x7d8
			 0x62ade92c, /// 0x7dc
			 0x5f1eed41, /// 0x7e0
			 0x9e0b38e0, /// 0x7e4
			 0x360c755d, /// 0x7e8
			 0xabee8aa7, /// 0x7ec
			 0xe932c13d, /// 0x7f0
			 0x62fd800b, /// 0x7f4
			 0xcddc4333, /// 0x7f8
			 0xb50b2dea, /// 0x7fc
			 0x9bddad9c, /// 0x800
			 0x471561c4, /// 0x804
			 0xb1d5dc9a, /// 0x808
			 0x5e290cbe, /// 0x80c
			 0x553dd341, /// 0x810
			 0x048a4eae, /// 0x814
			 0x78f6fadc, /// 0x818
			 0x00f0d171, /// 0x81c
			 0xab6b121b, /// 0x820
			 0x2e546d1a, /// 0x824
			 0x3d0d4885, /// 0x828
			 0xe3325e46, /// 0x82c
			 0x69923332, /// 0x830
			 0xb4e343ab, /// 0x834
			 0x5957326c, /// 0x838
			 0x86811645, /// 0x83c
			 0x37d55271, /// 0x840
			 0x161ef298, /// 0x844
			 0x8eafa2f9, /// 0x848
			 0xb5cd1e04, /// 0x84c
			 0x20631c9a, /// 0x850
			 0xd1ed6bb6, /// 0x854
			 0x7ba6eb16, /// 0x858
			 0xd0f3e584, /// 0x85c
			 0x130498b0, /// 0x860
			 0x49ac5029, /// 0x864
			 0xe6c5a088, /// 0x868
			 0x7abfba1d, /// 0x86c
			 0x8752b0ec, /// 0x870
			 0x412eba43, /// 0x874
			 0x6c5294e6, /// 0x878
			 0x40dfc8a6, /// 0x87c
			 0x98d7fde9, /// 0x880
			 0x480e0a45, /// 0x884
			 0x7737d756, /// 0x888
			 0xd0e65d2d, /// 0x88c
			 0x38861980, /// 0x890
			 0xab83a06d, /// 0x894
			 0x3c2d524b, /// 0x898
			 0x073a5f90, /// 0x89c
			 0xa118f815, /// 0x8a0
			 0x0bfd3ee5, /// 0x8a4
			 0x2f30c823, /// 0x8a8
			 0x1fe4dd8b, /// 0x8ac
			 0x54f7c701, /// 0x8b0
			 0x136ec201, /// 0x8b4
			 0x71af8b8e, /// 0x8b8
			 0xf885f925, /// 0x8bc
			 0x645a70d4, /// 0x8c0
			 0xb472d48b, /// 0x8c4
			 0xb2a128b6, /// 0x8c8
			 0xaf04bb24, /// 0x8cc
			 0x60900761, /// 0x8d0
			 0xf2467fba, /// 0x8d4
			 0x076a709e, /// 0x8d8
			 0x31c926ed, /// 0x8dc
			 0x1b2ae5c9, /// 0x8e0
			 0x402e70b6, /// 0x8e4
			 0x697dedd3, /// 0x8e8
			 0xf1d9bb3a, /// 0x8ec
			 0x8f69702f, /// 0x8f0
			 0xd1f0a877, /// 0x8f4
			 0xcff4bb6f, /// 0x8f8
			 0x053f7b2e, /// 0x8fc
			 0x2d5b3502, /// 0x900
			 0x108ed794, /// 0x904
			 0x2f0274be, /// 0x908
			 0xaf5dd63f, /// 0x90c
			 0x394362e5, /// 0x910
			 0x792f68a0, /// 0x914
			 0xe7a3f8ef, /// 0x918
			 0x404edd04, /// 0x91c
			 0x4009db7b, /// 0x920
			 0x59455ae7, /// 0x924
			 0x0ea97a97, /// 0x928
			 0x4c5ac975, /// 0x92c
			 0xc0bfd536, /// 0x930
			 0xdc6232c4, /// 0x934
			 0x23865b60, /// 0x938
			 0xf545193a, /// 0x93c
			 0x49498874, /// 0x940
			 0xb00bf287, /// 0x944
			 0xc01b618e, /// 0x948
			 0xf553d71b, /// 0x94c
			 0xa68ba5d2, /// 0x950
			 0x80a56476, /// 0x954
			 0xb0275b84, /// 0x958
			 0xf72dd048, /// 0x95c
			 0xaf89555f, /// 0x960
			 0x98e975fc, /// 0x964
			 0x399e66f0, /// 0x968
			 0xc9308527, /// 0x96c
			 0x2cddf6a6, /// 0x970
			 0x752f3aa2, /// 0x974
			 0x9bac4fa1, /// 0x978
			 0x311a1546, /// 0x97c
			 0x19eab1aa, /// 0x980
			 0xe3efe06e, /// 0x984
			 0xed6c6b07, /// 0x988
			 0x9f36ef0e, /// 0x98c
			 0xee8861cf, /// 0x990
			 0x91792539, /// 0x994
			 0x7a0bb95c, /// 0x998
			 0xdaf46924, /// 0x99c
			 0xd3ad37c1, /// 0x9a0
			 0x07ef11a3, /// 0x9a4
			 0xe4d1354d, /// 0x9a8
			 0x8e4ed7f8, /// 0x9ac
			 0x6538d22f, /// 0x9b0
			 0xd4ce4eaf, /// 0x9b4
			 0x71493e8a, /// 0x9b8
			 0x3186e65d, /// 0x9bc
			 0x71602207, /// 0x9c0
			 0xf9fa67ad, /// 0x9c4
			 0xc07a3916, /// 0x9c8
			 0xe6745c9f, /// 0x9cc
			 0x1e14e73d, /// 0x9d0
			 0x204f4304, /// 0x9d4
			 0x26d2f133, /// 0x9d8
			 0xccd41188, /// 0x9dc
			 0x3fef0434, /// 0x9e0
			 0x820edbe2, /// 0x9e4
			 0x68ed7920, /// 0x9e8
			 0x144f63f7, /// 0x9ec
			 0x515bf13b, /// 0x9f0
			 0x537fb332, /// 0x9f4
			 0x82cc5711, /// 0x9f8
			 0x51c514b0, /// 0x9fc
			 0x536bfa7f, /// 0xa00
			 0x8fa6ac9e, /// 0xa04
			 0xa8bb339a, /// 0xa08
			 0xd559d27d, /// 0xa0c
			 0x40742e47, /// 0xa10
			 0x1b1ce116, /// 0xa14
			 0x8a680b70, /// 0xa18
			 0x5c5ca6db, /// 0xa1c
			 0x1d3a315c, /// 0xa20
			 0xfc6bdfff, /// 0xa24
			 0x9df64c54, /// 0xa28
			 0x01bdcdb6, /// 0xa2c
			 0xb601b0c4, /// 0xa30
			 0x258ab7de, /// 0xa34
			 0x428e9510, /// 0xa38
			 0x2586aa66, /// 0xa3c
			 0x84074994, /// 0xa40
			 0xc4e4e661, /// 0xa44
			 0x5222efa5, /// 0xa48
			 0x18d9e0e5, /// 0xa4c
			 0x2759d52f, /// 0xa50
			 0x63c990a6, /// 0xa54
			 0xb3f95f39, /// 0xa58
			 0xce55ed50, /// 0xa5c
			 0x97918435, /// 0xa60
			 0x70a33676, /// 0xa64
			 0xca10d9d6, /// 0xa68
			 0x4c8c8fa3, /// 0xa6c
			 0x0bade8a9, /// 0xa70
			 0xbf12c353, /// 0xa74
			 0x679828a4, /// 0xa78
			 0x1f58b820, /// 0xa7c
			 0xb4a71ea9, /// 0xa80
			 0x0c2321dd, /// 0xa84
			 0x67c11b69, /// 0xa88
			 0x0a02ee66, /// 0xa8c
			 0xf01b4874, /// 0xa90
			 0xcaee8fb9, /// 0xa94
			 0x2f5460c7, /// 0xa98
			 0xba8f5edc, /// 0xa9c
			 0x77b329ec, /// 0xaa0
			 0xf2e802b7, /// 0xaa4
			 0xb35495e8, /// 0xaa8
			 0x785b89db, /// 0xaac
			 0xe44f0f7c, /// 0xab0
			 0x12273a34, /// 0xab4
			 0xb9ae1bd8, /// 0xab8
			 0x3c1c9ea3, /// 0xabc
			 0xfc3e2d3d, /// 0xac0
			 0xbdd404e0, /// 0xac4
			 0x798bb057, /// 0xac8
			 0xaa17969f, /// 0xacc
			 0x74792cee, /// 0xad0
			 0x049317d0, /// 0xad4
			 0xb44097b5, /// 0xad8
			 0x8834ec18, /// 0xadc
			 0x1836b33b, /// 0xae0
			 0xc6679942, /// 0xae4
			 0x66807dea, /// 0xae8
			 0xf3c263cb, /// 0xaec
			 0xc4af452c, /// 0xaf0
			 0x7ed509af, /// 0xaf4
			 0xda12da3d, /// 0xaf8
			 0xf106ad01, /// 0xafc
			 0xb8424d11, /// 0xb00
			 0xa640b8b1, /// 0xb04
			 0xe0533629, /// 0xb08
			 0xea150ae5, /// 0xb0c
			 0x7fb7e24c, /// 0xb10
			 0x7726de7d, /// 0xb14
			 0x4b14d587, /// 0xb18
			 0x8ae48587, /// 0xb1c
			 0x666ef84d, /// 0xb20
			 0xef6aec91, /// 0xb24
			 0x68e2ee68, /// 0xb28
			 0xe098a9cc, /// 0xb2c
			 0xee4b6b52, /// 0xb30
			 0xa4a015b2, /// 0xb34
			 0xead0a23c, /// 0xb38
			 0x88e0e594, /// 0xb3c
			 0x8989dfe6, /// 0xb40
			 0x03406f48, /// 0xb44
			 0xde5701b9, /// 0xb48
			 0x814e7280, /// 0xb4c
			 0x941dfe1e, /// 0xb50
			 0xa23826ac, /// 0xb54
			 0xd780be74, /// 0xb58
			 0x88fa7e9f, /// 0xb5c
			 0x503437db, /// 0xb60
			 0xa457f264, /// 0xb64
			 0x261016d0, /// 0xb68
			 0x896d7672, /// 0xb6c
			 0xb061bbcc, /// 0xb70
			 0xedf905f0, /// 0xb74
			 0xbfa105dd, /// 0xb78
			 0xdb52f9f2, /// 0xb7c
			 0x7c87c907, /// 0xb80
			 0x8e7d0c75, /// 0xb84
			 0xbf4f7f9d, /// 0xb88
			 0x79225829, /// 0xb8c
			 0xe62bbeee, /// 0xb90
			 0x0e3354a1, /// 0xb94
			 0x803990ef, /// 0xb98
			 0xd5951a94, /// 0xb9c
			 0xa2a9a6ed, /// 0xba0
			 0x8fc03ce7, /// 0xba4
			 0xfbfdc0c3, /// 0xba8
			 0xfc9ecc67, /// 0xbac
			 0x4a72ba5b, /// 0xbb0
			 0x492d48b5, /// 0xbb4
			 0x684dd076, /// 0xbb8
			 0xb3d601d7, /// 0xbbc
			 0xcffab10e, /// 0xbc0
			 0xac1408f2, /// 0xbc4
			 0x262ae649, /// 0xbc8
			 0x3e675ec9, /// 0xbcc
			 0xf5338dd6, /// 0xbd0
			 0x6512a74b, /// 0xbd4
			 0x1d4448d5, /// 0xbd8
			 0x17809f3a, /// 0xbdc
			 0xa9680982, /// 0xbe0
			 0xd0f63e9c, /// 0xbe4
			 0x97f30f35, /// 0xbe8
			 0x03359828, /// 0xbec
			 0xb39dd6f5, /// 0xbf0
			 0x1cbd3194, /// 0xbf4
			 0xc53425e2, /// 0xbf8
			 0x71d93e2e, /// 0xbfc
			 0x4ae483f4, /// 0xc00
			 0x3bc0acd1, /// 0xc04
			 0xece844cd, /// 0xc08
			 0x4619e63a, /// 0xc0c
			 0x7d06b1b8, /// 0xc10
			 0xcd17cfce, /// 0xc14
			 0xd3411d54, /// 0xc18
			 0x0acbfc0d, /// 0xc1c
			 0xb3f99d4e, /// 0xc20
			 0x63c39ef1, /// 0xc24
			 0x0134d25f, /// 0xc28
			 0x7c840a92, /// 0xc2c
			 0x925844af, /// 0xc30
			 0x2b774e04, /// 0xc34
			 0x59e9ce86, /// 0xc38
			 0x7283a8f0, /// 0xc3c
			 0x40ea7d8c, /// 0xc40
			 0xa2cff236, /// 0xc44
			 0xde9eb534, /// 0xc48
			 0x5c3eb256, /// 0xc4c
			 0x0f30995d, /// 0xc50
			 0xd6444fe0, /// 0xc54
			 0xcdbe1f43, /// 0xc58
			 0x92f204ea, /// 0xc5c
			 0xf2b8ea87, /// 0xc60
			 0xdff3b021, /// 0xc64
			 0xb35da03f, /// 0xc68
			 0x99d7a348, /// 0xc6c
			 0x54b1ec56, /// 0xc70
			 0x53cb741e, /// 0xc74
			 0x1df5890a, /// 0xc78
			 0xe48fd590, /// 0xc7c
			 0x38310d62, /// 0xc80
			 0x6a0ac629, /// 0xc84
			 0xa5f927eb, /// 0xc88
			 0x1a7daa73, /// 0xc8c
			 0x2b22a060, /// 0xc90
			 0x7ca235d1, /// 0xc94
			 0xb41b000e, /// 0xc98
			 0x6b44daed, /// 0xc9c
			 0x1560c66e, /// 0xca0
			 0x2ab24f6c, /// 0xca4
			 0xe1500355, /// 0xca8
			 0x248d35e9, /// 0xcac
			 0x807744ba, /// 0xcb0
			 0x2af19b26, /// 0xcb4
			 0xe69f490a, /// 0xcb8
			 0x05675b84, /// 0xcbc
			 0x0f914f45, /// 0xcc0
			 0x7cfae075, /// 0xcc4
			 0x1aa15a0f, /// 0xcc8
			 0x647339d0, /// 0xccc
			 0x3579eb8d, /// 0xcd0
			 0xfca4977e, /// 0xcd4
			 0x2f2e3c57, /// 0xcd8
			 0xe7c20c09, /// 0xcdc
			 0x560e0a87, /// 0xce0
			 0x3e68d0d8, /// 0xce4
			 0xbe64e755, /// 0xce8
			 0x3a239b5f, /// 0xcec
			 0x82681c2e, /// 0xcf0
			 0x970a959a, /// 0xcf4
			 0x2749398e, /// 0xcf8
			 0x04797ab7, /// 0xcfc
			 0xcfaa2146, /// 0xd00
			 0x57d7c255, /// 0xd04
			 0xe5649f83, /// 0xd08
			 0x8cd7cfb0, /// 0xd0c
			 0xe91a0ad0, /// 0xd10
			 0xb3b16a97, /// 0xd14
			 0xb178cda2, /// 0xd18
			 0x9464af1a, /// 0xd1c
			 0x6c6ec999, /// 0xd20
			 0x1447d7a4, /// 0xd24
			 0xd0e2a8c2, /// 0xd28
			 0x5fbed772, /// 0xd2c
			 0x78f03ad9, /// 0xd30
			 0xfae84fd9, /// 0xd34
			 0x679169f6, /// 0xd38
			 0xb6ca38f6, /// 0xd3c
			 0x6d65d31e, /// 0xd40
			 0xf6b52d88, /// 0xd44
			 0x9f6293ed, /// 0xd48
			 0xca92c4b5, /// 0xd4c
			 0x4e0c2b2b, /// 0xd50
			 0x318ad152, /// 0xd54
			 0xb99b1967, /// 0xd58
			 0xc3485d67, /// 0xd5c
			 0x991fbb4a, /// 0xd60
			 0x3920e602, /// 0xd64
			 0x5345d701, /// 0xd68
			 0x140472f4, /// 0xd6c
			 0x08708097, /// 0xd70
			 0x1d593f5d, /// 0xd74
			 0xd5af5cd2, /// 0xd78
			 0x25954e6c, /// 0xd7c
			 0x1da162ef, /// 0xd80
			 0x530d585c, /// 0xd84
			 0x7f08146f, /// 0xd88
			 0x8bb25966, /// 0xd8c
			 0x35deb3c3, /// 0xd90
			 0x8e7cbe9d, /// 0xd94
			 0x6735e9eb, /// 0xd98
			 0xab14ed0d, /// 0xd9c
			 0xa1d2e5f3, /// 0xda0
			 0xb7d53654, /// 0xda4
			 0x3b1ee662, /// 0xda8
			 0xe312fd6e, /// 0xdac
			 0xa9540b71, /// 0xdb0
			 0x4b1cc26e, /// 0xdb4
			 0x03d2bfde, /// 0xdb8
			 0x78dc0806, /// 0xdbc
			 0x013d4bf4, /// 0xdc0
			 0xc46b6774, /// 0xdc4
			 0xb15a11a9, /// 0xdc8
			 0xda43ece9, /// 0xdcc
			 0x7285cacf, /// 0xdd0
			 0x1af8c672, /// 0xdd4
			 0x8c94cf03, /// 0xdd8
			 0x25b5e3db, /// 0xddc
			 0x06fa6844, /// 0xde0
			 0x50439db4, /// 0xde4
			 0x28278977, /// 0xde8
			 0x98101b2a, /// 0xdec
			 0x9880bb61, /// 0xdf0
			 0x08d30ee2, /// 0xdf4
			 0x8f85aaa9, /// 0xdf8
			 0x219116e4, /// 0xdfc
			 0x55ffd363, /// 0xe00
			 0x736e1c1e, /// 0xe04
			 0x48133dce, /// 0xe08
			 0x2699e75c, /// 0xe0c
			 0x5f01bec2, /// 0xe10
			 0x8a8d27a1, /// 0xe14
			 0xac1b51f0, /// 0xe18
			 0xace3dbae, /// 0xe1c
			 0xe87ddba2, /// 0xe20
			 0x68d10457, /// 0xe24
			 0x6368f7e4, /// 0xe28
			 0x23dbd861, /// 0xe2c
			 0x88eee879, /// 0xe30
			 0x321d53a5, /// 0xe34
			 0xc5f3fd70, /// 0xe38
			 0x54930fa3, /// 0xe3c
			 0xf1d6dcaf, /// 0xe40
			 0x41426c16, /// 0xe44
			 0xb262dbbe, /// 0xe48
			 0x7d8bc9bf, /// 0xe4c
			 0xc11d96cd, /// 0xe50
			 0x16ac7eb0, /// 0xe54
			 0x1b6baf8b, /// 0xe58
			 0x863f8ea6, /// 0xe5c
			 0x36569c27, /// 0xe60
			 0x47fa6f37, /// 0xe64
			 0xfdd8aea1, /// 0xe68
			 0x259902f1, /// 0xe6c
			 0xe749c6de, /// 0xe70
			 0xc6b7280f, /// 0xe74
			 0x7f872696, /// 0xe78
			 0x69caf043, /// 0xe7c
			 0xf445861e, /// 0xe80
			 0xcccbefbe, /// 0xe84
			 0x47646651, /// 0xe88
			 0x5474ebfe, /// 0xe8c
			 0xdd4ec1d5, /// 0xe90
			 0x1c92ccb0, /// 0xe94
			 0x7b1799d6, /// 0xe98
			 0xfc523ef3, /// 0xe9c
			 0x85fb70ff, /// 0xea0
			 0xa4d663a5, /// 0xea4
			 0x1f473ea7, /// 0xea8
			 0x31ee9a45, /// 0xeac
			 0x4cf4ec71, /// 0xeb0
			 0x57d8982a, /// 0xeb4
			 0xa1f83432, /// 0xeb8
			 0xb02daf3e, /// 0xebc
			 0x940dea96, /// 0xec0
			 0x86496ae4, /// 0xec4
			 0x8c24b695, /// 0xec8
			 0x21571edf, /// 0xecc
			 0xaea51811, /// 0xed0
			 0x82bcf167, /// 0xed4
			 0x9feb12df, /// 0xed8
			 0xb899585d, /// 0xedc
			 0xe07ee12a, /// 0xee0
			 0x28409a1d, /// 0xee4
			 0x0a642113, /// 0xee8
			 0x220ebc21, /// 0xeec
			 0x5dd7b0b6, /// 0xef0
			 0xe20df8c2, /// 0xef4
			 0x54ebda9a, /// 0xef8
			 0x89fb9397, /// 0xefc
			 0x0a082c4e, /// 0xf00
			 0x8cd42de4, /// 0xf04
			 0x84c3e1e4, /// 0xf08
			 0x418389c0, /// 0xf0c
			 0xbd099301, /// 0xf10
			 0x451d8f50, /// 0xf14
			 0xcdd96f68, /// 0xf18
			 0x7a23d1e5, /// 0xf1c
			 0x9dd10208, /// 0xf20
			 0x6593a6eb, /// 0xf24
			 0xc12e1e34, /// 0xf28
			 0xb2fb7b68, /// 0xf2c
			 0x3e1bf189, /// 0xf30
			 0x90b4fbee, /// 0xf34
			 0xff9ee996, /// 0xf38
			 0x3af21bfc, /// 0xf3c
			 0xb1024bae, /// 0xf40
			 0x0921cebb, /// 0xf44
			 0x1d68edcf, /// 0xf48
			 0xc3858ad6, /// 0xf4c
			 0x8573e55e, /// 0xf50
			 0xb4be74e5, /// 0xf54
			 0x384ef44a, /// 0xf58
			 0x4c44e69d, /// 0xf5c
			 0x5a68c35d, /// 0xf60
			 0x349e1b58, /// 0xf64
			 0xbfd6b96a, /// 0xf68
			 0xcda56e33, /// 0xf6c
			 0x6de6ad27, /// 0xf70
			 0x024154ac, /// 0xf74
			 0x20f93427, /// 0xf78
			 0x32060100, /// 0xf7c
			 0xfbb07f84, /// 0xf80
			 0x86f52098, /// 0xf84
			 0xd355d025, /// 0xf88
			 0x0c78def6, /// 0xf8c
			 0xcd60c00d, /// 0xf90
			 0x9854165b, /// 0xf94
			 0xeac02ce2, /// 0xf98
			 0x52642195, /// 0xf9c
			 0xa3c589a2, /// 0xfa0
			 0xb1a703e0, /// 0xfa4
			 0xc2eb6579, /// 0xfa8
			 0x2cf4b340, /// 0xfac
			 0xf4d54782, /// 0xfb0
			 0x87dd528e, /// 0xfb4
			 0xc2111ed0, /// 0xfb8
			 0x6ddab7d0, /// 0xfbc
			 0x566051a3, /// 0xfc0
			 0x8121cb06, /// 0xfc4
			 0xcb01cdc8, /// 0xfc8
			 0x3c3a35e0, /// 0xfcc
			 0x1f4bb106, /// 0xfd0
			 0x52c46dff, /// 0xfd4
			 0xee4d0e9c, /// 0xfd8
			 0xdfdbe220, /// 0xfdc
			 0x04259d24, /// 0xfe0
			 0x2b27af97, /// 0xfe4
			 0x7d208215, /// 0xfe8
			 0x794cae69, /// 0xfec
			 0xbf139e2f, /// 0xff0
			 0xf9e2ad57, /// 0xff4
			 0x46373fdd, /// 0xff8
			 0x9603badb /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x646a3418, /// 0x0
			 0xe66c32ee, /// 0x4
			 0x2bff730c, /// 0x8
			 0xefed7489, /// 0xc
			 0x1c25781f, /// 0x10
			 0xbdf8671b, /// 0x14
			 0xca70af32, /// 0x18
			 0xc95caa91, /// 0x1c
			 0xec00a2f9, /// 0x20
			 0x071101a9, /// 0x24
			 0x555e5f23, /// 0x28
			 0x813d6a45, /// 0x2c
			 0xadca6287, /// 0x30
			 0xedb0dffe, /// 0x34
			 0x7e01b727, /// 0x38
			 0x3200d87f, /// 0x3c
			 0x8e366ccf, /// 0x40
			 0x9beb0a39, /// 0x44
			 0xfe80b19d, /// 0x48
			 0xf99a5b2c, /// 0x4c
			 0xaae9f5d2, /// 0x50
			 0xe5325122, /// 0x54
			 0x274107ee, /// 0x58
			 0xaca0e4b9, /// 0x5c
			 0x8cb302d0, /// 0x60
			 0x5e757569, /// 0x64
			 0x7edaa0cc, /// 0x68
			 0xe945db9d, /// 0x6c
			 0xcc39dc82, /// 0x70
			 0x57be65ac, /// 0x74
			 0x64be6557, /// 0x78
			 0x7c76fbdc, /// 0x7c
			 0x11e13ab7, /// 0x80
			 0xf3475f8a, /// 0x84
			 0x36151a5c, /// 0x88
			 0xfbb48b8f, /// 0x8c
			 0x4c56ca92, /// 0x90
			 0x9eaa1aa7, /// 0x94
			 0xdcf1670e, /// 0x98
			 0x4cea95a2, /// 0x9c
			 0x557f9e70, /// 0xa0
			 0x4b319d8a, /// 0xa4
			 0x268b92ac, /// 0xa8
			 0x6cb19c64, /// 0xac
			 0x7818f4e9, /// 0xb0
			 0x2020a3d6, /// 0xb4
			 0x7ebbb876, /// 0xb8
			 0xcedbc39a, /// 0xbc
			 0x7155c9d9, /// 0xc0
			 0xde5eea26, /// 0xc4
			 0x62b843b5, /// 0xc8
			 0x52895220, /// 0xcc
			 0xb6d94d39, /// 0xd0
			 0xea6a4ef6, /// 0xd4
			 0x1be7384f, /// 0xd8
			 0x3f0cd287, /// 0xdc
			 0x824d072d, /// 0xe0
			 0xbc17a5de, /// 0xe4
			 0x1662c69f, /// 0xe8
			 0x1badc694, /// 0xec
			 0x8755ba5f, /// 0xf0
			 0x7a9cd68f, /// 0xf4
			 0x57d7542a, /// 0xf8
			 0x91b15024, /// 0xfc
			 0x743cf78d, /// 0x100
			 0xfce8d4d4, /// 0x104
			 0x4ee92e6d, /// 0x108
			 0x6860efc9, /// 0x10c
			 0x23b38d01, /// 0x110
			 0x1974bdf0, /// 0x114
			 0x12d61da6, /// 0x118
			 0xf7882f2b, /// 0x11c
			 0xd667c79f, /// 0x120
			 0xce07a44c, /// 0x124
			 0x9e8ac26f, /// 0x128
			 0x1c18f695, /// 0x12c
			 0xc5efae41, /// 0x130
			 0x78e8f378, /// 0x134
			 0xfc734fe1, /// 0x138
			 0x6f90810b, /// 0x13c
			 0xbfc049d2, /// 0x140
			 0x39b95143, /// 0x144
			 0x64eacd95, /// 0x148
			 0xd518a6bc, /// 0x14c
			 0xc3bf3093, /// 0x150
			 0xfc6a5678, /// 0x154
			 0xafe8ceda, /// 0x158
			 0x4b1d169c, /// 0x15c
			 0x7cc1c2a1, /// 0x160
			 0x9b4c12ec, /// 0x164
			 0x4f908cbe, /// 0x168
			 0xbb8590cf, /// 0x16c
			 0x1ec8566f, /// 0x170
			 0xb5e511e6, /// 0x174
			 0xde3974fb, /// 0x178
			 0x423828ff, /// 0x17c
			 0x3e44f0a3, /// 0x180
			 0x10127c69, /// 0x184
			 0x482d9ea0, /// 0x188
			 0x70464053, /// 0x18c
			 0x134ef675, /// 0x190
			 0xfbe7356f, /// 0x194
			 0x8ceb7865, /// 0x198
			 0xa73d8279, /// 0x19c
			 0xffc9c2a9, /// 0x1a0
			 0x6d6cce39, /// 0x1a4
			 0xe7b2d6a6, /// 0x1a8
			 0x640ac3ad, /// 0x1ac
			 0xc9f33c28, /// 0x1b0
			 0x34bdef52, /// 0x1b4
			 0x69c756b9, /// 0x1b8
			 0xbdf1c183, /// 0x1bc
			 0x838f8d00, /// 0x1c0
			 0x7a58522a, /// 0x1c4
			 0xec1574ee, /// 0x1c8
			 0x9cde61ca, /// 0x1cc
			 0xe6e9d527, /// 0x1d0
			 0x1602c437, /// 0x1d4
			 0x94a31db0, /// 0x1d8
			 0xcc8b67f8, /// 0x1dc
			 0x16949c43, /// 0x1e0
			 0x9c8b6116, /// 0x1e4
			 0xa7197442, /// 0x1e8
			 0x665b4448, /// 0x1ec
			 0x45bd2640, /// 0x1f0
			 0x110ab3f7, /// 0x1f4
			 0x751195cf, /// 0x1f8
			 0x5a7d6797, /// 0x1fc
			 0xd38383fd, /// 0x200
			 0xa03fb957, /// 0x204
			 0x6d36cbf4, /// 0x208
			 0x136241dc, /// 0x20c
			 0x179aca1b, /// 0x210
			 0xfba63a8b, /// 0x214
			 0x38810173, /// 0x218
			 0x73a5f5f5, /// 0x21c
			 0x96731a37, /// 0x220
			 0x1e7c90f7, /// 0x224
			 0xe4795865, /// 0x228
			 0xd1bf2182, /// 0x22c
			 0x3de8c973, /// 0x230
			 0xf183c6be, /// 0x234
			 0x0cadfdd0, /// 0x238
			 0x81c16467, /// 0x23c
			 0xf0ca62e0, /// 0x240
			 0x7b8bd9db, /// 0x244
			 0xaec98c77, /// 0x248
			 0xe421235a, /// 0x24c
			 0x5b93ea99, /// 0x250
			 0xaf60345e, /// 0x254
			 0xa6fd1e64, /// 0x258
			 0x4aeeffd3, /// 0x25c
			 0xd763c4ac, /// 0x260
			 0xce6aa35e, /// 0x264
			 0xfcb262d0, /// 0x268
			 0xf5524404, /// 0x26c
			 0x23b043c6, /// 0x270
			 0xa299d033, /// 0x274
			 0xa6474db7, /// 0x278
			 0x3aae3fb6, /// 0x27c
			 0x27ce063c, /// 0x280
			 0x82d1cafa, /// 0x284
			 0x490423de, /// 0x288
			 0x771abeab, /// 0x28c
			 0xd1ee2c1c, /// 0x290
			 0xc3940fb6, /// 0x294
			 0xe3b65eb9, /// 0x298
			 0x3be93e14, /// 0x29c
			 0xb0028e43, /// 0x2a0
			 0x4b132631, /// 0x2a4
			 0x1e82a209, /// 0x2a8
			 0x6e823625, /// 0x2ac
			 0x20b76745, /// 0x2b0
			 0x920e1368, /// 0x2b4
			 0xf681d180, /// 0x2b8
			 0x3111e9b1, /// 0x2bc
			 0x205cac87, /// 0x2c0
			 0xf2407dfc, /// 0x2c4
			 0x495533c5, /// 0x2c8
			 0xffb6942a, /// 0x2cc
			 0x88640db9, /// 0x2d0
			 0xfc4589f5, /// 0x2d4
			 0x9dd7f313, /// 0x2d8
			 0xdd28ce47, /// 0x2dc
			 0xf475e6b6, /// 0x2e0
			 0xb0a843f7, /// 0x2e4
			 0x8e1f36aa, /// 0x2e8
			 0x7f9d36cd, /// 0x2ec
			 0x00549960, /// 0x2f0
			 0x8134181f, /// 0x2f4
			 0x46da07a9, /// 0x2f8
			 0x1df1aac2, /// 0x2fc
			 0xded9b8fe, /// 0x300
			 0xb249f77d, /// 0x304
			 0x59ef3a79, /// 0x308
			 0x2bec2c0f, /// 0x30c
			 0x35d9e1d0, /// 0x310
			 0x0efe4b23, /// 0x314
			 0x25ff5df5, /// 0x318
			 0x097f5896, /// 0x31c
			 0x7fc7612c, /// 0x320
			 0xd07be197, /// 0x324
			 0xe13db5ec, /// 0x328
			 0x503fcef2, /// 0x32c
			 0xeb2e6519, /// 0x330
			 0x4476e574, /// 0x334
			 0xfe7464c1, /// 0x338
			 0x22d052ab, /// 0x33c
			 0x9bbccd71, /// 0x340
			 0x1245991b, /// 0x344
			 0xa92fea06, /// 0x348
			 0xf8d00afc, /// 0x34c
			 0xdb053199, /// 0x350
			 0x9c98e787, /// 0x354
			 0x6ad72990, /// 0x358
			 0x749235e4, /// 0x35c
			 0xe706d3cf, /// 0x360
			 0x40040568, /// 0x364
			 0xc8e12a96, /// 0x368
			 0xa44c4a70, /// 0x36c
			 0x8348bcbd, /// 0x370
			 0xbe843bb0, /// 0x374
			 0xba3d0eda, /// 0x378
			 0x87c53a5a, /// 0x37c
			 0x753fddaf, /// 0x380
			 0xa50eb35b, /// 0x384
			 0xf9f64e55, /// 0x388
			 0x15fe875d, /// 0x38c
			 0x9fbd2961, /// 0x390
			 0xc7d38cef, /// 0x394
			 0x0c581cbe, /// 0x398
			 0xcd52e67f, /// 0x39c
			 0xfbe16624, /// 0x3a0
			 0xec4ac321, /// 0x3a4
			 0x6e02913a, /// 0x3a8
			 0xa89c40b5, /// 0x3ac
			 0xf82de554, /// 0x3b0
			 0x652d4923, /// 0x3b4
			 0xbd2d882f, /// 0x3b8
			 0xdae83ad7, /// 0x3bc
			 0xc796fd3e, /// 0x3c0
			 0x3e9d6bd6, /// 0x3c4
			 0x5b1eaeb8, /// 0x3c8
			 0x933ca078, /// 0x3cc
			 0x8eb87133, /// 0x3d0
			 0xfbb32d0e, /// 0x3d4
			 0x835ded7f, /// 0x3d8
			 0xe4711d10, /// 0x3dc
			 0xa96aa9e3, /// 0x3e0
			 0x3b5e845c, /// 0x3e4
			 0x5532d11b, /// 0x3e8
			 0xf597dae0, /// 0x3ec
			 0xc76b53a7, /// 0x3f0
			 0xa6bbb006, /// 0x3f4
			 0x1ff6e0d7, /// 0x3f8
			 0x2773effa, /// 0x3fc
			 0x94c60aab, /// 0x400
			 0x04da83fc, /// 0x404
			 0xd2bb50bb, /// 0x408
			 0xc8b8bf6f, /// 0x40c
			 0x04a9b747, /// 0x410
			 0x7ec6e7ee, /// 0x414
			 0x440db525, /// 0x418
			 0xd6babd6f, /// 0x41c
			 0xaeebd95b, /// 0x420
			 0x0fa7b97c, /// 0x424
			 0x1c110743, /// 0x428
			 0xa85106c7, /// 0x42c
			 0x0d94f90d, /// 0x430
			 0x94c8807f, /// 0x434
			 0x15eb4a8c, /// 0x438
			 0x6de3f7f5, /// 0x43c
			 0xc633e210, /// 0x440
			 0xbdb69fb5, /// 0x444
			 0xbf9482f6, /// 0x448
			 0xad96e070, /// 0x44c
			 0x1d123732, /// 0x450
			 0x7d74c72f, /// 0x454
			 0x3f0d15b8, /// 0x458
			 0x347663fd, /// 0x45c
			 0x6ede1c3f, /// 0x460
			 0x67c59df6, /// 0x464
			 0x3b532e16, /// 0x468
			 0xdf9cf920, /// 0x46c
			 0xf13ddace, /// 0x470
			 0xa27b0ef2, /// 0x474
			 0xe55a0515, /// 0x478
			 0x251dbb68, /// 0x47c
			 0xdb75664a, /// 0x480
			 0x46989108, /// 0x484
			 0xea131b5f, /// 0x488
			 0x141035ee, /// 0x48c
			 0x9c33d310, /// 0x490
			 0x05fa5fc7, /// 0x494
			 0x5173544c, /// 0x498
			 0x7422fedd, /// 0x49c
			 0xbdc4d072, /// 0x4a0
			 0xcf6441c0, /// 0x4a4
			 0x9225e9f1, /// 0x4a8
			 0x648da0e0, /// 0x4ac
			 0xa140e00a, /// 0x4b0
			 0xa82d842a, /// 0x4b4
			 0x357f2736, /// 0x4b8
			 0x6133e7f2, /// 0x4bc
			 0x3e4f4d61, /// 0x4c0
			 0x7f2d50f0, /// 0x4c4
			 0xf556df07, /// 0x4c8
			 0x4d960d00, /// 0x4cc
			 0xb4992b0e, /// 0x4d0
			 0xcdf1a46a, /// 0x4d4
			 0x64431c0d, /// 0x4d8
			 0x8b3ebd01, /// 0x4dc
			 0x716d3892, /// 0x4e0
			 0x42987396, /// 0x4e4
			 0x998adc98, /// 0x4e8
			 0x9a92d925, /// 0x4ec
			 0x87f319e9, /// 0x4f0
			 0xc64e3bd5, /// 0x4f4
			 0x669b6669, /// 0x4f8
			 0x1d41e373, /// 0x4fc
			 0x896978e7, /// 0x500
			 0x6441c6ad, /// 0x504
			 0xc28d7b2f, /// 0x508
			 0x3a516530, /// 0x50c
			 0xfca1080b, /// 0x510
			 0xee23cdab, /// 0x514
			 0x0547ad18, /// 0x518
			 0x074086e7, /// 0x51c
			 0x15646f68, /// 0x520
			 0xb328f244, /// 0x524
			 0x94ff2ab4, /// 0x528
			 0xfed05db5, /// 0x52c
			 0x9b4ba274, /// 0x530
			 0x21b6758b, /// 0x534
			 0xc298e5c1, /// 0x538
			 0xd627763e, /// 0x53c
			 0x17413440, /// 0x540
			 0xf3a77196, /// 0x544
			 0xab41f526, /// 0x548
			 0x76604d89, /// 0x54c
			 0x88eb1bd3, /// 0x550
			 0x7abdd695, /// 0x554
			 0x0a4cf545, /// 0x558
			 0xc6815066, /// 0x55c
			 0x93a7aab9, /// 0x560
			 0x2a39bf74, /// 0x564
			 0x7fbc3c98, /// 0x568
			 0xc930139d, /// 0x56c
			 0x637f1133, /// 0x570
			 0x7186d606, /// 0x574
			 0x29654687, /// 0x578
			 0xc23dd90a, /// 0x57c
			 0x033bc557, /// 0x580
			 0x2e63364c, /// 0x584
			 0xebbabd41, /// 0x588
			 0xb57eb6ba, /// 0x58c
			 0x662e0484, /// 0x590
			 0xc062dd48, /// 0x594
			 0x0e484659, /// 0x598
			 0x357035b0, /// 0x59c
			 0x025e6e04, /// 0x5a0
			 0x805d834a, /// 0x5a4
			 0xb4d38afd, /// 0x5a8
			 0xdcf475e7, /// 0x5ac
			 0x819e253c, /// 0x5b0
			 0x1433b510, /// 0x5b4
			 0x3ed8a3c0, /// 0x5b8
			 0x4954a856, /// 0x5bc
			 0xf523771f, /// 0x5c0
			 0x4240977a, /// 0x5c4
			 0x8c2dc939, /// 0x5c8
			 0x7e0f45f4, /// 0x5cc
			 0x14e5335d, /// 0x5d0
			 0x1c2f09f3, /// 0x5d4
			 0xac89a536, /// 0x5d8
			 0x3dc777b5, /// 0x5dc
			 0xb6f9bcdc, /// 0x5e0
			 0x5cb756b4, /// 0x5e4
			 0xb281622c, /// 0x5e8
			 0xdcc912fd, /// 0x5ec
			 0xb4fc72b1, /// 0x5f0
			 0x262ae423, /// 0x5f4
			 0x5c9ade82, /// 0x5f8
			 0xa1b2dd54, /// 0x5fc
			 0x66c90081, /// 0x600
			 0xa02906d8, /// 0x604
			 0xf9a2b932, /// 0x608
			 0xf81af015, /// 0x60c
			 0x6b8c75a8, /// 0x610
			 0x258d37a7, /// 0x614
			 0x08a6a642, /// 0x618
			 0x7058416a, /// 0x61c
			 0x9fd1ec3c, /// 0x620
			 0x3d213744, /// 0x624
			 0x8ca7554d, /// 0x628
			 0xe3f853ec, /// 0x62c
			 0x74b444a9, /// 0x630
			 0x431db024, /// 0x634
			 0x612a0e70, /// 0x638
			 0x1ad31abd, /// 0x63c
			 0xbcf711cb, /// 0x640
			 0xca29b861, /// 0x644
			 0x33deced8, /// 0x648
			 0xa3a092c7, /// 0x64c
			 0xaa658204, /// 0x650
			 0x310aefa0, /// 0x654
			 0x6554f3d8, /// 0x658
			 0x05e1085c, /// 0x65c
			 0x75bb39e7, /// 0x660
			 0x29f59946, /// 0x664
			 0x9b509769, /// 0x668
			 0xb05f9e22, /// 0x66c
			 0xee1b592c, /// 0x670
			 0xcaf1d03b, /// 0x674
			 0x426bf3c6, /// 0x678
			 0xcd378b24, /// 0x67c
			 0x9422ac7b, /// 0x680
			 0x31d12fda, /// 0x684
			 0x847e56bc, /// 0x688
			 0x708dd4bd, /// 0x68c
			 0x2d6d7332, /// 0x690
			 0x0bad8174, /// 0x694
			 0xb15e254a, /// 0x698
			 0x014aaa90, /// 0x69c
			 0xa0eaf090, /// 0x6a0
			 0x193dc937, /// 0x6a4
			 0xa361bfa3, /// 0x6a8
			 0x8da67d15, /// 0x6ac
			 0x8c669165, /// 0x6b0
			 0x5d9c5229, /// 0x6b4
			 0xd9e239ec, /// 0x6b8
			 0xd9a75301, /// 0x6bc
			 0x4502f12c, /// 0x6c0
			 0xe4087b38, /// 0x6c4
			 0x8573767d, /// 0x6c8
			 0x9b087005, /// 0x6cc
			 0x3697e444, /// 0x6d0
			 0x0e8d31e0, /// 0x6d4
			 0xb97c59b9, /// 0x6d8
			 0xb9927eba, /// 0x6dc
			 0xacd41734, /// 0x6e0
			 0xa2038a2d, /// 0x6e4
			 0x77caf223, /// 0x6e8
			 0x168f6d44, /// 0x6ec
			 0x82a5f332, /// 0x6f0
			 0xace51d1e, /// 0x6f4
			 0x6dbd2ccb, /// 0x6f8
			 0xb1a514e6, /// 0x6fc
			 0x632b3b9f, /// 0x700
			 0x4c84538c, /// 0x704
			 0x83f8033a, /// 0x708
			 0x98c8cb0e, /// 0x70c
			 0xa1b04786, /// 0x710
			 0x441829f8, /// 0x714
			 0x44a2c79d, /// 0x718
			 0x27079bf4, /// 0x71c
			 0x2418eb24, /// 0x720
			 0xd8aed8ce, /// 0x724
			 0xc713516e, /// 0x728
			 0x795a4cd1, /// 0x72c
			 0x5a4f1c03, /// 0x730
			 0xc3ee56dd, /// 0x734
			 0x69d43fb0, /// 0x738
			 0xccdfe2eb, /// 0x73c
			 0x54d582f9, /// 0x740
			 0x6bef2883, /// 0x744
			 0xebde0ddb, /// 0x748
			 0xf11b1f98, /// 0x74c
			 0x86be52b1, /// 0x750
			 0x93660161, /// 0x754
			 0x353e84a0, /// 0x758
			 0x2f8ae02b, /// 0x75c
			 0x68205bc4, /// 0x760
			 0x0c6955d6, /// 0x764
			 0xc390e0f4, /// 0x768
			 0x6ea7f016, /// 0x76c
			 0x0cddd428, /// 0x770
			 0x07143e8d, /// 0x774
			 0x43523e4e, /// 0x778
			 0x0127f0d5, /// 0x77c
			 0xafde270f, /// 0x780
			 0x6ae7b0e8, /// 0x784
			 0xcba7f525, /// 0x788
			 0x1e1cd930, /// 0x78c
			 0xc0e40b46, /// 0x790
			 0x24f63d89, /// 0x794
			 0xf526fc47, /// 0x798
			 0x4ff691ca, /// 0x79c
			 0x52753ebb, /// 0x7a0
			 0x710ba9be, /// 0x7a4
			 0x1be20c58, /// 0x7a8
			 0x85c83962, /// 0x7ac
			 0x21b953e6, /// 0x7b0
			 0xc27c2536, /// 0x7b4
			 0xb06ceba4, /// 0x7b8
			 0x2c640b9c, /// 0x7bc
			 0x9f7440a2, /// 0x7c0
			 0xee0feff1, /// 0x7c4
			 0x050d899c, /// 0x7c8
			 0x18a308ad, /// 0x7cc
			 0x8372549d, /// 0x7d0
			 0x19dae01f, /// 0x7d4
			 0x7dfbaf1c, /// 0x7d8
			 0x82487eef, /// 0x7dc
			 0xf127dac7, /// 0x7e0
			 0xbf4144ef, /// 0x7e4
			 0xc81ba5f2, /// 0x7e8
			 0xa98b9605, /// 0x7ec
			 0xe87b1b66, /// 0x7f0
			 0x27dcab86, /// 0x7f4
			 0xe36a3c3c, /// 0x7f8
			 0x5c672936, /// 0x7fc
			 0xd513a0b2, /// 0x800
			 0x2a79ffba, /// 0x804
			 0x07269fd0, /// 0x808
			 0x016bafde, /// 0x80c
			 0x292cef14, /// 0x810
			 0x54be3fdd, /// 0x814
			 0x24118e19, /// 0x818
			 0xca32aca4, /// 0x81c
			 0xd7736c73, /// 0x820
			 0x58af261d, /// 0x824
			 0xf254b64c, /// 0x828
			 0xfe2c906b, /// 0x82c
			 0x740fc810, /// 0x830
			 0x8e470833, /// 0x834
			 0x026f3845, /// 0x838
			 0x45dfc132, /// 0x83c
			 0x8372d757, /// 0x840
			 0xc2074ee9, /// 0x844
			 0x1de366de, /// 0x848
			 0x02fc7e90, /// 0x84c
			 0x74e05141, /// 0x850
			 0x14fc2752, /// 0x854
			 0x579b748e, /// 0x858
			 0xab3294d7, /// 0x85c
			 0x1260b1c7, /// 0x860
			 0x4f8a479a, /// 0x864
			 0xc620b937, /// 0x868
			 0x759d2b50, /// 0x86c
			 0x2703a2f0, /// 0x870
			 0xab105dd6, /// 0x874
			 0xd0421970, /// 0x878
			 0x74a005fb, /// 0x87c
			 0x7e7c24b3, /// 0x880
			 0x31dc4b7d, /// 0x884
			 0x77ede0d1, /// 0x888
			 0x1262c42a, /// 0x88c
			 0x09619545, /// 0x890
			 0x44ef572d, /// 0x894
			 0xd938c03f, /// 0x898
			 0x616302e6, /// 0x89c
			 0x293437f4, /// 0x8a0
			 0xfe432270, /// 0x8a4
			 0xcfd64223, /// 0x8a8
			 0x880f0613, /// 0x8ac
			 0xf9ef746f, /// 0x8b0
			 0x39ea6ce8, /// 0x8b4
			 0x919ce9d3, /// 0x8b8
			 0xb4d984fe, /// 0x8bc
			 0x31c7262e, /// 0x8c0
			 0x3eaa77c3, /// 0x8c4
			 0x49326e2f, /// 0x8c8
			 0x8120d404, /// 0x8cc
			 0xa4c88a8a, /// 0x8d0
			 0x289c7b97, /// 0x8d4
			 0xcc515bc6, /// 0x8d8
			 0xa6786f53, /// 0x8dc
			 0x7c3677c7, /// 0x8e0
			 0x5dd61b60, /// 0x8e4
			 0x07de41e7, /// 0x8e8
			 0x4aa8553a, /// 0x8ec
			 0x3c568200, /// 0x8f0
			 0x948a52a4, /// 0x8f4
			 0xc9b2771a, /// 0x8f8
			 0xe1b6c24b, /// 0x8fc
			 0xe9b5e480, /// 0x900
			 0x9f0bfd95, /// 0x904
			 0x56320087, /// 0x908
			 0x80f4b1da, /// 0x90c
			 0x53777eda, /// 0x910
			 0x496c30e7, /// 0x914
			 0x4a922be1, /// 0x918
			 0xa21b071f, /// 0x91c
			 0x07ac98c8, /// 0x920
			 0x06a99532, /// 0x924
			 0xa2f2bd96, /// 0x928
			 0xd73c093b, /// 0x92c
			 0xbedb5fc9, /// 0x930
			 0x7f90e47d, /// 0x934
			 0xc3b592af, /// 0x938
			 0x02e037de, /// 0x93c
			 0x5ca491ff, /// 0x940
			 0xb46af19b, /// 0x944
			 0x84b25237, /// 0x948
			 0x643b97e3, /// 0x94c
			 0xb5601ffd, /// 0x950
			 0x208de991, /// 0x954
			 0x0301d362, /// 0x958
			 0xfc3c810b, /// 0x95c
			 0x9012d9d6, /// 0x960
			 0x890f68e6, /// 0x964
			 0x01af2666, /// 0x968
			 0x2f2d5492, /// 0x96c
			 0x20994d90, /// 0x970
			 0x5a25f013, /// 0x974
			 0x9d6ed4c0, /// 0x978
			 0x5f4ecbc5, /// 0x97c
			 0x8efb9096, /// 0x980
			 0x44c5685e, /// 0x984
			 0x87b7d4af, /// 0x988
			 0x645b3590, /// 0x98c
			 0x5c69b110, /// 0x990
			 0x92d271ef, /// 0x994
			 0x93688302, /// 0x998
			 0x3196062d, /// 0x99c
			 0x96bd868c, /// 0x9a0
			 0xa6f8a9b8, /// 0x9a4
			 0x372314cb, /// 0x9a8
			 0x3e48b182, /// 0x9ac
			 0xa3bc3dab, /// 0x9b0
			 0x7e3066c0, /// 0x9b4
			 0xb0a38773, /// 0x9b8
			 0x51cbe27e, /// 0x9bc
			 0x42896660, /// 0x9c0
			 0xd1ce3eef, /// 0x9c4
			 0x86e27f14, /// 0x9c8
			 0x8c4c00dc, /// 0x9cc
			 0x70830713, /// 0x9d0
			 0xe3d6c38d, /// 0x9d4
			 0xd3bd2058, /// 0x9d8
			 0x2eae2f82, /// 0x9dc
			 0x8673af1e, /// 0x9e0
			 0x2cd33cd6, /// 0x9e4
			 0xa463bab0, /// 0x9e8
			 0x088de6b4, /// 0x9ec
			 0xb7a5d04b, /// 0x9f0
			 0x3cc2cf1a, /// 0x9f4
			 0xb474a518, /// 0x9f8
			 0x73dc5951, /// 0x9fc
			 0x2abc145c, /// 0xa00
			 0x071915bd, /// 0xa04
			 0x735eb034, /// 0xa08
			 0x530a209f, /// 0xa0c
			 0xd7f0d629, /// 0xa10
			 0x84cca60b, /// 0xa14
			 0xae6ab5e6, /// 0xa18
			 0x28fd8af6, /// 0xa1c
			 0x98ffb084, /// 0xa20
			 0x9b0b82fd, /// 0xa24
			 0x4a5238d0, /// 0xa28
			 0x5a6e9139, /// 0xa2c
			 0x01e77342, /// 0xa30
			 0xf2ff68df, /// 0xa34
			 0x951e4b66, /// 0xa38
			 0x87ef4942, /// 0xa3c
			 0x63752181, /// 0xa40
			 0x7666484f, /// 0xa44
			 0xb35f4c48, /// 0xa48
			 0xfa2ddd35, /// 0xa4c
			 0x0a0687ea, /// 0xa50
			 0xb0892993, /// 0xa54
			 0xe2362361, /// 0xa58
			 0x2cc83e92, /// 0xa5c
			 0x320d0d1e, /// 0xa60
			 0xa222bfa0, /// 0xa64
			 0x5b142b22, /// 0xa68
			 0x755ad0fe, /// 0xa6c
			 0x97513988, /// 0xa70
			 0x01d7ebd5, /// 0xa74
			 0x9b671632, /// 0xa78
			 0x74907b5a, /// 0xa7c
			 0x1dfffd98, /// 0xa80
			 0x668c6415, /// 0xa84
			 0x16123061, /// 0xa88
			 0xf4ca5576, /// 0xa8c
			 0xc7a2b646, /// 0xa90
			 0x8faece0c, /// 0xa94
			 0xa61a45db, /// 0xa98
			 0x1dd1dcfb, /// 0xa9c
			 0x0bacc4ae, /// 0xaa0
			 0x91d2a7b5, /// 0xaa4
			 0x0ea0d7af, /// 0xaa8
			 0x0c5dddfd, /// 0xaac
			 0xd97d1ce5, /// 0xab0
			 0xe7663fe1, /// 0xab4
			 0x18cd0784, /// 0xab8
			 0x67f80de4, /// 0xabc
			 0x82d49d03, /// 0xac0
			 0xb7eedf5b, /// 0xac4
			 0x8e9d4a6b, /// 0xac8
			 0x2f644e88, /// 0xacc
			 0xfd2bdff7, /// 0xad0
			 0x221c5ae1, /// 0xad4
			 0x8ffa51bd, /// 0xad8
			 0x188fe876, /// 0xadc
			 0x10c18d2c, /// 0xae0
			 0xba48bf3b, /// 0xae4
			 0xcce67e56, /// 0xae8
			 0x17082fb5, /// 0xaec
			 0x078c3a46, /// 0xaf0
			 0x44f22a6a, /// 0xaf4
			 0x5f7aaaf9, /// 0xaf8
			 0xcaa2a5eb, /// 0xafc
			 0xe801769d, /// 0xb00
			 0x6fb8b454, /// 0xb04
			 0xe6f69d8e, /// 0xb08
			 0x6f44d84c, /// 0xb0c
			 0x48725aed, /// 0xb10
			 0xbb85659a, /// 0xb14
			 0x316a8af8, /// 0xb18
			 0xbcbdc463, /// 0xb1c
			 0x0fac69d7, /// 0xb20
			 0x12b5d125, /// 0xb24
			 0x83857e39, /// 0xb28
			 0xf7a85cac, /// 0xb2c
			 0xf892846a, /// 0xb30
			 0xa316bc86, /// 0xb34
			 0xc0f88909, /// 0xb38
			 0xfd3552ba, /// 0xb3c
			 0x671031ff, /// 0xb40
			 0x77c5f0e0, /// 0xb44
			 0x869fa8e9, /// 0xb48
			 0xde7727cb, /// 0xb4c
			 0x1c531953, /// 0xb50
			 0x5c0efe83, /// 0xb54
			 0x360d92af, /// 0xb58
			 0x14fa5e57, /// 0xb5c
			 0x88bd6ffe, /// 0xb60
			 0x6c1159c5, /// 0xb64
			 0x641330e3, /// 0xb68
			 0x913e23e4, /// 0xb6c
			 0x93d207a3, /// 0xb70
			 0x24b0cd6f, /// 0xb74
			 0xc6944cad, /// 0xb78
			 0xee17c7da, /// 0xb7c
			 0x8010eb21, /// 0xb80
			 0x18a86b76, /// 0xb84
			 0x96593d34, /// 0xb88
			 0xeda1363c, /// 0xb8c
			 0x8d7d709f, /// 0xb90
			 0x653f9807, /// 0xb94
			 0x6e1199f9, /// 0xb98
			 0x0e664421, /// 0xb9c
			 0x80c8cf2a, /// 0xba0
			 0xbc2a255b, /// 0xba4
			 0x99fe9969, /// 0xba8
			 0x5de6c308, /// 0xbac
			 0xaf41d9ab, /// 0xbb0
			 0x02d3a6ee, /// 0xbb4
			 0xd089ec5c, /// 0xbb8
			 0xcb2e82a6, /// 0xbbc
			 0x518cd176, /// 0xbc0
			 0x6e6c9f54, /// 0xbc4
			 0xba30c5d4, /// 0xbc8
			 0x8f585874, /// 0xbcc
			 0x624f580c, /// 0xbd0
			 0xaa378b4f, /// 0xbd4
			 0x4f1bc117, /// 0xbd8
			 0x0c17ab78, /// 0xbdc
			 0x2d7c650a, /// 0xbe0
			 0xeb62d43f, /// 0xbe4
			 0x308077fe, /// 0xbe8
			 0xecdbdab1, /// 0xbec
			 0xfc8d65c5, /// 0xbf0
			 0xb6ae5cbc, /// 0xbf4
			 0x0e4205ff, /// 0xbf8
			 0x92683aa9, /// 0xbfc
			 0x7391b0ea, /// 0xc00
			 0x1b7d302f, /// 0xc04
			 0x75c5bb2c, /// 0xc08
			 0xbd38bd00, /// 0xc0c
			 0x70633b99, /// 0xc10
			 0xa588faf5, /// 0xc14
			 0x9c9bafdf, /// 0xc18
			 0x8dd318f1, /// 0xc1c
			 0xb93e9a5f, /// 0xc20
			 0x37b5b63c, /// 0xc24
			 0x0e3e7940, /// 0xc28
			 0x1a948249, /// 0xc2c
			 0x87091f99, /// 0xc30
			 0x5d481382, /// 0xc34
			 0xfc01f455, /// 0xc38
			 0xfd16554b, /// 0xc3c
			 0x36bb30da, /// 0xc40
			 0xfe556115, /// 0xc44
			 0x6e939496, /// 0xc48
			 0x6818adda, /// 0xc4c
			 0xa1183f54, /// 0xc50
			 0x3fafd7ab, /// 0xc54
			 0x80b84afb, /// 0xc58
			 0x87f0d478, /// 0xc5c
			 0x91f4cd4a, /// 0xc60
			 0xda58f806, /// 0xc64
			 0xa3d3ecd2, /// 0xc68
			 0x356f3a9f, /// 0xc6c
			 0xb2e39e77, /// 0xc70
			 0x36abb170, /// 0xc74
			 0x16bd345e, /// 0xc78
			 0x30c674ba, /// 0xc7c
			 0x875bd560, /// 0xc80
			 0xfe8900ae, /// 0xc84
			 0x6589cb29, /// 0xc88
			 0xd1a2126d, /// 0xc8c
			 0xdf5a067e, /// 0xc90
			 0x2b70c3cd, /// 0xc94
			 0xdee6ac0f, /// 0xc98
			 0x45eb295e, /// 0xc9c
			 0x456b7ff5, /// 0xca0
			 0x84c42d2e, /// 0xca4
			 0xf2a3c174, /// 0xca8
			 0xd04ec0eb, /// 0xcac
			 0xdc7e979f, /// 0xcb0
			 0x25cb0b20, /// 0xcb4
			 0x399a87d4, /// 0xcb8
			 0x40552f74, /// 0xcbc
			 0x7fbf5e26, /// 0xcc0
			 0xc890fc0c, /// 0xcc4
			 0x0b4f5814, /// 0xcc8
			 0x48d52ce2, /// 0xccc
			 0xdc83d35c, /// 0xcd0
			 0x73f8ece1, /// 0xcd4
			 0x32628f85, /// 0xcd8
			 0x75f4039e, /// 0xcdc
			 0x4b9eacc7, /// 0xce0
			 0x87560732, /// 0xce4
			 0xd82da43a, /// 0xce8
			 0xe325714b, /// 0xcec
			 0xf4da5726, /// 0xcf0
			 0x47896e41, /// 0xcf4
			 0x86b8a91c, /// 0xcf8
			 0x412b9f92, /// 0xcfc
			 0x352badb7, /// 0xd00
			 0x66849696, /// 0xd04
			 0x147ad967, /// 0xd08
			 0x3423d1ff, /// 0xd0c
			 0xfe4041c9, /// 0xd10
			 0xa346e421, /// 0xd14
			 0x00c1ec6c, /// 0xd18
			 0x52748d23, /// 0xd1c
			 0xbfb7c6fa, /// 0xd20
			 0x508ca61b, /// 0xd24
			 0x7570367f, /// 0xd28
			 0x14406f09, /// 0xd2c
			 0x035cba9d, /// 0xd30
			 0x5b7a1f44, /// 0xd34
			 0xe85eb9d1, /// 0xd38
			 0x0ebe40ca, /// 0xd3c
			 0x25d4b7b1, /// 0xd40
			 0x3d0210af, /// 0xd44
			 0x5460ee1d, /// 0xd48
			 0x3a594fa0, /// 0xd4c
			 0x73de8598, /// 0xd50
			 0xfc48683c, /// 0xd54
			 0x56dcf01c, /// 0xd58
			 0x20840d66, /// 0xd5c
			 0x353afdae, /// 0xd60
			 0x9daaefdc, /// 0xd64
			 0x0f7639e5, /// 0xd68
			 0xf80dac46, /// 0xd6c
			 0xcd8c5ce6, /// 0xd70
			 0x79ca281a, /// 0xd74
			 0x4fdc15f2, /// 0xd78
			 0x7f742b4a, /// 0xd7c
			 0xf0a5e1e3, /// 0xd80
			 0xbc146f28, /// 0xd84
			 0x83a41003, /// 0xd88
			 0x792ed26b, /// 0xd8c
			 0xb271ee83, /// 0xd90
			 0xdde982cd, /// 0xd94
			 0x1f0fdadb, /// 0xd98
			 0xe7b13685, /// 0xd9c
			 0x338fa9e9, /// 0xda0
			 0x5bea32ae, /// 0xda4
			 0x3ed7c174, /// 0xda8
			 0x0b31dc86, /// 0xdac
			 0x7f0561cd, /// 0xdb0
			 0x589b087d, /// 0xdb4
			 0xbf26dde0, /// 0xdb8
			 0x88a987c5, /// 0xdbc
			 0xf18fdfe6, /// 0xdc0
			 0xf3710ad5, /// 0xdc4
			 0x8b6a9d84, /// 0xdc8
			 0x604e0c89, /// 0xdcc
			 0x8df18afc, /// 0xdd0
			 0xbfebeae2, /// 0xdd4
			 0x198337aa, /// 0xdd8
			 0x62c1a58b, /// 0xddc
			 0x9f99841a, /// 0xde0
			 0x4936adb8, /// 0xde4
			 0x2b55a05f, /// 0xde8
			 0xa3483c0e, /// 0xdec
			 0xf1b31429, /// 0xdf0
			 0x7899ca78, /// 0xdf4
			 0x2f01cb93, /// 0xdf8
			 0x12b5e15a, /// 0xdfc
			 0x718fd84c, /// 0xe00
			 0xf9135262, /// 0xe04
			 0xacc92f71, /// 0xe08
			 0xf44bcdcf, /// 0xe0c
			 0xdbe9ee36, /// 0xe10
			 0xf658c80f, /// 0xe14
			 0x0233008a, /// 0xe18
			 0xe74ba292, /// 0xe1c
			 0x636b54a1, /// 0xe20
			 0xb118700b, /// 0xe24
			 0x1fa67a7b, /// 0xe28
			 0xebe072b4, /// 0xe2c
			 0xf2fba792, /// 0xe30
			 0xd7175ee9, /// 0xe34
			 0xe38aa018, /// 0xe38
			 0x5813d853, /// 0xe3c
			 0xbe154ee5, /// 0xe40
			 0xeadef52b, /// 0xe44
			 0xac439eca, /// 0xe48
			 0x4faee301, /// 0xe4c
			 0x18406395, /// 0xe50
			 0x5616a547, /// 0xe54
			 0x7a3c8f0d, /// 0xe58
			 0x2fd68c96, /// 0xe5c
			 0x5f36dd56, /// 0xe60
			 0xaff125a5, /// 0xe64
			 0x3f3411ff, /// 0xe68
			 0x09d3bca0, /// 0xe6c
			 0xb4ac7812, /// 0xe70
			 0xa4417eaa, /// 0xe74
			 0x65e73ec6, /// 0xe78
			 0x19218a52, /// 0xe7c
			 0x07b80005, /// 0xe80
			 0xcb50e7a8, /// 0xe84
			 0x0e8c8d39, /// 0xe88
			 0x3844721d, /// 0xe8c
			 0xe6eb0697, /// 0xe90
			 0x1013a2eb, /// 0xe94
			 0x8e0910f2, /// 0xe98
			 0xfd0bff6b, /// 0xe9c
			 0x7b28f7e4, /// 0xea0
			 0x25757a51, /// 0xea4
			 0x48b2f5a7, /// 0xea8
			 0xa39937a5, /// 0xeac
			 0x5165c8ee, /// 0xeb0
			 0xb953cdbb, /// 0xeb4
			 0xac7d339a, /// 0xeb8
			 0x3e0943f8, /// 0xebc
			 0x083a33bb, /// 0xec0
			 0xcde7077f, /// 0xec4
			 0xcfec5ce3, /// 0xec8
			 0x3907ebcd, /// 0xecc
			 0xf588a985, /// 0xed0
			 0xa1f1b9c2, /// 0xed4
			 0x2cf6a330, /// 0xed8
			 0xbd11010b, /// 0xedc
			 0x72885dfd, /// 0xee0
			 0xa60c2c65, /// 0xee4
			 0x77e6014a, /// 0xee8
			 0x567ccd90, /// 0xeec
			 0xe0552e14, /// 0xef0
			 0xbf1c9a63, /// 0xef4
			 0x43dc7585, /// 0xef8
			 0x9f9c982c, /// 0xefc
			 0x4fbd88c7, /// 0xf00
			 0xca7995ec, /// 0xf04
			 0xf3e252df, /// 0xf08
			 0xc45f1d25, /// 0xf0c
			 0x47d097c6, /// 0xf10
			 0x57af3828, /// 0xf14
			 0xd3bc2ffc, /// 0xf18
			 0xe2c99d8f, /// 0xf1c
			 0xda8b601f, /// 0xf20
			 0x77163933, /// 0xf24
			 0x85b2877b, /// 0xf28
			 0xcd6f9baa, /// 0xf2c
			 0xd58fe5ec, /// 0xf30
			 0x050a8465, /// 0xf34
			 0xf5c60d31, /// 0xf38
			 0xdd4de1e9, /// 0xf3c
			 0x08b0aa97, /// 0xf40
			 0xe628f477, /// 0xf44
			 0xcb2ba1a0, /// 0xf48
			 0x45a67465, /// 0xf4c
			 0x68651e74, /// 0xf50
			 0xac06d392, /// 0xf54
			 0xae0dccba, /// 0xf58
			 0x76ee4b18, /// 0xf5c
			 0xbd87baa4, /// 0xf60
			 0x9122c482, /// 0xf64
			 0x7115d688, /// 0xf68
			 0x9be752ec, /// 0xf6c
			 0x7b5f39f1, /// 0xf70
			 0x31de8238, /// 0xf74
			 0x3842bb40, /// 0xf78
			 0xbb7fed30, /// 0xf7c
			 0xfc16c207, /// 0xf80
			 0xdb279cc0, /// 0xf84
			 0x431e3f24, /// 0xf88
			 0x99242e9d, /// 0xf8c
			 0x049a3d88, /// 0xf90
			 0x6bf4973d, /// 0xf94
			 0x1fb2790e, /// 0xf98
			 0x44b2632e, /// 0xf9c
			 0x8a4ddea7, /// 0xfa0
			 0xcf38c60b, /// 0xfa4
			 0x17877c85, /// 0xfa8
			 0x8db13349, /// 0xfac
			 0x27fac89b, /// 0xfb0
			 0xed3b8349, /// 0xfb4
			 0x7adbad59, /// 0xfb8
			 0xacd6a346, /// 0xfbc
			 0x5d2a1619, /// 0xfc0
			 0x917822fa, /// 0xfc4
			 0x64fb7bfa, /// 0xfc8
			 0x08cd0718, /// 0xfcc
			 0x985ca92c, /// 0xfd0
			 0xe85693ad, /// 0xfd4
			 0xb2d1c1b2, /// 0xfd8
			 0x66942875, /// 0xfdc
			 0x2dbc59f3, /// 0xfe0
			 0xf46a3b9e, /// 0xfe4
			 0x611adfc6, /// 0xfe8
			 0xadd0ab84, /// 0xfec
			 0x9fedea06, /// 0xff0
			 0x3dac440b, /// 0xff4
			 0xaa9953dc, /// 0xff8
			 0x05ff9761 /// last
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
			 0x000003cc,
			 0x0000004c,
			 0x0000038c,
			 0x00000210,
			 0x00000778,
			 0x000000cc,
			 0x00000778,
			 0x000004a0,

			 /// vpu register f2
			 0x40a00000,
			 0x41700000,
			 0x41f00000,
			 0x41300000,
			 0x41e80000,
			 0x40000000,
			 0x41c80000,
			 0x40c00000,

			 /// vpu register f3
			 0x41b80000,
			 0x41e00000,
			 0x41c00000,
			 0x41300000,
			 0x40000000,
			 0x41300000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f4
			 0x42000000,
			 0x41c80000,
			 0x41600000,
			 0x41d00000,
			 0x41f00000,
			 0x41a80000,
			 0x42000000,
			 0x41c80000,

			 /// vpu register f5
			 0x41a80000,
			 0x41b00000,
			 0x3f800000,
			 0x3f800000,
			 0x41000000,
			 0x41400000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f6
			 0x41600000,
			 0x41880000,
			 0x41100000,
			 0x40400000,
			 0x41a00000,
			 0x41200000,
			 0x40400000,
			 0x41a80000,

			 /// vpu register f7
			 0x41100000,
			 0x41400000,
			 0x41c80000,
			 0x41900000,
			 0x41000000,
			 0x41a00000,
			 0x3f800000,
			 0x41b00000,

			 /// vpu register f8
			 0x40800000,
			 0x41b00000,
			 0x3f800000,
			 0x41f00000,
			 0x41900000,
			 0x41d80000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f9
			 0x40800000,
			 0x41500000,
			 0x41a80000,
			 0x41a00000,
			 0x40c00000,
			 0x40a00000,
			 0x41c00000,
			 0x3f800000,

			 /// vpu register f10
			 0x41e80000,
			 0x41e00000,
			 0x41a00000,
			 0x41880000,
			 0x41000000,
			 0x41880000,
			 0x41e00000,
			 0x3f800000,

			 /// vpu register f11
			 0x3f800000,
			 0x41300000,
			 0x3f800000,
			 0x41200000,
			 0x41300000,
			 0x41400000,
			 0x41d00000,
			 0x41200000,

			 /// vpu register f12
			 0x41600000,
			 0x41f80000,
			 0x41b80000,
			 0x3f800000,
			 0x41700000,
			 0x41900000,
			 0x41c80000,
			 0x41e00000,

			 /// vpu register f13
			 0x41e80000,
			 0x41100000,
			 0x42000000,
			 0x40800000,
			 0x40e00000,
			 0x41a00000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f14
			 0x41200000,
			 0x41200000,
			 0x40000000,
			 0x40400000,
			 0x41000000,
			 0x41000000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f15
			 0x42000000,
			 0x41100000,
			 0x41200000,
			 0x41400000,
			 0x41b00000,
			 0x41000000,
			 0x41e00000,
			 0x41980000,

			 /// vpu register f16
			 0x40400000,
			 0x41f80000,
			 0x40e00000,
			 0x41c00000,
			 0x41900000,
			 0x41c00000,
			 0x41100000,
			 0x40000000,

			 /// vpu register f17
			 0x3f800000,
			 0x41900000,
			 0x41100000,
			 0x41d00000,
			 0x41a00000,
			 0x41e80000,
			 0x41d80000,
			 0x41a80000,

			 /// vpu register f18
			 0x41600000,
			 0x41a00000,
			 0x41700000,
			 0x41600000,
			 0x41100000,
			 0x41a80000,
			 0x41b00000,
			 0x40000000,

			 /// vpu register f19
			 0x41300000,
			 0x41a80000,
			 0x41200000,
			 0x41600000,
			 0x41000000,
			 0x41e00000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f20
			 0x41000000,
			 0x41500000,
			 0x41d00000,
			 0x42000000,
			 0x41f00000,
			 0x41b00000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f21
			 0x41b80000,
			 0x41400000,
			 0x41880000,
			 0x40e00000,
			 0x41a80000,
			 0x40a00000,
			 0x41100000,
			 0x41d80000,

			 /// vpu register f22
			 0x41980000,
			 0x41f80000,
			 0x40c00000,
			 0x3f800000,
			 0x41700000,
			 0x41000000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f23
			 0x40400000,
			 0x41a00000,
			 0x3f800000,
			 0x40400000,
			 0x41300000,
			 0x41880000,
			 0x40000000,
			 0x41a00000,

			 /// vpu register f24
			 0x41400000,
			 0x41b00000,
			 0x40e00000,
			 0x41e80000,
			 0x41a00000,
			 0x40800000,
			 0x41000000,
			 0x40800000,

			 /// vpu register f25
			 0x41200000,
			 0x40000000,
			 0x41300000,
			 0x41700000,
			 0x40c00000,
			 0x41000000,
			 0x41980000,
			 0x41100000,

			 /// vpu register f26
			 0x40400000,
			 0x41600000,
			 0x41600000,
			 0x40000000,
			 0x40400000,
			 0x41900000,
			 0x41880000,
			 0x41880000,

			 /// vpu register f27
			 0x3f800000,
			 0x41b80000,
			 0x41200000,
			 0x41200000,
			 0x41200000,
			 0x41c00000,
			 0x41400000,
			 0x41880000,

			 /// vpu register f28
			 0x41c80000,
			 0x41500000,
			 0x41200000,
			 0x40a00000,
			 0x41f00000,
			 0x41a00000,
			 0x41d80000,
			 0x41f80000,

			 /// vpu register f29
			 0x40c00000,
			 0x40a00000,
			 0x40800000,
			 0x41600000,
			 0x40a00000,
			 0x41c80000,
			 0x41400000,
			 0x42000000,

			 /// vpu register f30
			 0x40000000,
			 0x41d80000,
			 0x41700000,
			 0x41200000,
			 0x41800000,
			 0x41a00000,
			 0x41100000,
			 0x41700000,

			 /// vpu register f31
			 0x41200000,
			 0x41e80000,
			 0x41b80000,
			 0x40000000,
			 0x41a80000,
			 0x41b00000,
			 0x40c00000,
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
		"fsgnjx.ps f15, f10, f27\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f28, f26\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f24, f18, f3\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f29, f13, f10\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f15, f12, f19\n"                           ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f22, f0, f4\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f16, f14, f20\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f1, f5\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f3, f11, f10\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f15, f27, f27\n"                           ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f9, f23, f20\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f25, f1, f27\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f15, f19, f16\n"                           ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f20, f28, f18\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f12, f2, f5\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f17, f16\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f3, f12, f17\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f29, f1, f9\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f9, f10\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f2, f4, f21\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f23, f13, f6\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f29, f24, f10\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f5, f11, f10\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f11, f11, f12\n"                           ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f6, f6\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f17, f2, f21\n"                            ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f30, f23, f0\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f20, f4, f19\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f27, f29, f14\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f27, f27, f9\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f28, f4, f1\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f30, f2, f14\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f9, f2, f29\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f5, f28, f3\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f28, f15, f27\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f14, f30, f9\n"                            ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f25, f23\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f30, f23, f0\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f11, f16, f30\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f3, f30\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f18, f14, f9\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f25, f22\n"                            ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f21, f4, f11\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f23, f20, f6\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f13, f7\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f2, f14, f3\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f28, f30, f6\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f21, f17, f16\n"                           ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f23, f20, f23\n"                           ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f18, f0\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f4, f2, f17\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f14, f6, f1\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f3, f5, f13\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f30, f8\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f9, f22, f4\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f30, f30, f29\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f14, f24, f29\n"                           ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f27, f22, f22\n"                           ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f26, f20, f22\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f10, f6, f17\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f18, f27, f24\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f3, f3, f10\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f12, f6, f6\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f8, f30, f9\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f5, f20, f24\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f28, f17\n"                           ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f22, f19, f0\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f19, f1, f29\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f11, f10, f25\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f27, f28, f0\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f20, f4, f1\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f20, f3, f27\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f21, f17, f17\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f26, f13, f17\n"                           ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f22, f7, f10\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f17, f2, f15\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f16, f27, f9\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f11, f21, f15\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f16, f7, f26\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f10, f12, f0\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f11, f6, f8\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f30, f7, f5\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f0, f11\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f25, f17, f22\n"                           ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f7, f21, f8\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f21, f14, f12\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f23, f19, f8\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f26, f17, f15\n"                           ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f29, f18, f23\n"                           ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f21, f17, f19\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f17, f19, f24\n"                           ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f30, f1, f24\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f14, f7, f12\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f26, f14, f9\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f11, f0, f18\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f3, f7, f7\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f13, f2, f1\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f18, f20, f16\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.ps f14, f10, f13\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
