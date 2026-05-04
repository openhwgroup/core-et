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
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00004
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00008
			 0x0e000001,                                                  // Trailing 1s:                                /// 0000c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00020
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00024
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00028
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0002c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00030
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00034
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00038
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0003c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00040
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00044
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00048
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00050
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00054
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00058
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00064
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00068
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00078
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00084
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00088
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0008c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00090
			 0xff800000,                                                  // -inf                                        /// 00094
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00098
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0009c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00108
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00110
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0011c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00120
			 0x0c400000,                                                  // Leading 1s:                                 /// 00124
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00128
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0012c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00138
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0013c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00144
			 0x4b000000,                                                  // 8388608.0                                   /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0014c
			 0x7fc00001,                                                  // signaling NaN                               /// 00150
			 0xbf028f5c,                                                  // -0.51                                       /// 00154
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00158
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00160
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00164
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0016c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0017c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00194
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00198
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0019c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001ac
			 0x55555555,                                                  // 4 random values                             /// 001b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00200
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7fc00001,                                                  // signaling NaN                               /// 00208
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00210
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00218
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0021c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00224
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00228
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00230
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00240
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00244
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00248
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00254
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00258
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0025c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00260
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00264
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00270
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00274
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0027c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00280
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00284
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0028c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00294
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00298
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0029c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002c4
			 0x4b000000,                                                  // 8388608.0                                   /// 002c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002cc
			 0x7f800000,                                                  // inf                                         /// 002d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d4
			 0xffc00001,                                                  // -signaling NaN                              /// 002d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00300
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00304
			 0xcb000000,                                                  // -8388608.0                                  /// 00308
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0030c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00314
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00318
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00320
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0032c
			 0x3f028f5c,                                                  // 0.51                                        /// 00330
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00334
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0033c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00340
			 0x0c600000,                                                  // Leading 1s:                                 /// 00344
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00348
			 0x0c780000,                                                  // Leading 1s:                                 /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00354
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00358
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00368
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xff800000,                                                  // -inf                                        /// 00374
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00378
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00384
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00390
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00394
			 0x00011111,                                                  // 9.7958E-41                                  /// 00398
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0xcb000000,                                                  // -8388608.0                                  /// 003bc
			 0x80000000,                                                  // -zero                                       /// 003c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003d8
			 0x7f800000,                                                  // inf                                         /// 003dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00404
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0xff800000,                                                  // -inf                                        /// 0040c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00410
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00414
			 0x55555555,                                                  // 4 random values                             /// 00418
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00420
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00428
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00430
			 0x3f028f5c,                                                  // 0.51                                        /// 00434
			 0x0e000007,                                                  // Trailing 1s:                                /// 00438
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0043c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00444
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00448
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0044c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00450
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00458
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00460
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00464
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00468
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0046c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00470
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00474
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00478
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00484
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00488
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c780000,                                                  // Leading 1s:                                 /// 00494
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0049c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0xbf028f5c,                                                  // -0.51                                       /// 004a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00514
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00528
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00534
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00540
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0054c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00550
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00558
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0055c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00564
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00574
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00578
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0057c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00580
			 0xcb000000,                                                  // -8388608.0                                  /// 00584
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00588
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0058c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00590
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0059c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x55555555,                                                  // 4 random values                             /// 005f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00600
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00608
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0060c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00614
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00618
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0061c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00620
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00624
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00630
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00634
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0063c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00640
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00644
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00648
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0064c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00650
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00654
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00658
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0065c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00660
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00664
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00678
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0067c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00680
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00684
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00688
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0068c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00690
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00694
			 0x3f028f5c,                                                  // 0.51                                        /// 00698
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0069c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00700
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0070c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00714
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00720
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0x0c400000,                                                  // Leading 1s:                                 /// 0072c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00730
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x33333333,                                                  // 4 random values                             /// 00738
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0073c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00740
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00748
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0074c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00750
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00754
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00758
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00760
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00764
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00774
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00780
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00790
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00794
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00798
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0079c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e4
			 0x3f028f5c,                                                  // 0.51                                        /// 007e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00800
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xbf028f5c,                                                  // -0.51                                       /// 0080c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00818
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00820
			 0x00000000,                                                  // zero                                        /// 00824
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00828
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0082c
			 0x55555555,                                                  // 4 random values                             /// 00830
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00834
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00838
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00844
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00848
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x4b000000,                                                  // 8388608.0                                   /// 00850
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00854
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00858
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0085c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00860
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00864
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00870
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00874
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00878
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0087c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00880
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00888
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0088c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00894
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0089c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008c4
			 0x00000000,                                                  // zero                                        /// 008c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008dc
			 0xbf028f5c,                                                  // -0.51                                       /// 008e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ec
			 0x3f028f5c,                                                  // 0.51                                        /// 008f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00900
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00904
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00908
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00920
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0092c
			 0xbf028f5c,                                                  // -0.51                                       /// 00930
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00934
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00938
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0093c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00948
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0xff800000,                                                  // -inf                                        /// 00950
			 0x7f800000,                                                  // inf                                         /// 00954
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0095c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0096c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00970
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00978
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00980
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0098c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00990
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x33333333,                                                  // 4 random values                             /// 009b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x3f028f5c,                                                  // 0.51                                        /// 009d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f8
			 0x7fc00001,                                                  // signaling NaN                               /// 009fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a34
			 0x33333333,                                                  // 4 random values                             /// 00a38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x00000000,                                                  // zero                                        /// 00a74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a98
			 0xbf028f5c,                                                  // -0.51                                       /// 00a9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00aa4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ab0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80000000,                                                  // -zero                                       /// 00ac4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ae8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00aec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x55555555,                                                  // 4 random values                             /// 00b94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0xff800000,                                                  // -inf                                        /// 00bd4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00be0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00be4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bf4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0xbf028f5c,                                                  // -0.51                                       /// 00c28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x7fc00001,                                                  // signaling NaN                               /// 00c58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ca4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cbc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cc0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ccc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cd8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ce4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x7f800000,                                                  // inf                                         /// 00cf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cfc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d10
			 0x00000000,                                                  // zero                                        /// 00d14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00000000,                                                  // zero                                        /// 00d50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0xff800000,                                                  // -inf                                        /// 00d90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00db8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ddc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e38
			 0x3f028f5c,                                                  // 0.51                                        /// 00e3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e58
			 0xffc00001,                                                  // -signaling NaN                              /// 00e5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eb8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ebc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ed4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ee0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00efc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x00000000,                                                  // zero                                        /// 00f1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x3f028f5c,                                                  // 0.51                                        /// 00f4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00f50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xcb000000,                                                  // -8388608.0                                  /// 00f78
			 0xff800000,                                                  // -inf                                        /// 00f7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fa0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00000000,                                                  // zero                                        /// 00fec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0c7fff00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00000
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00004
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00008
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0000c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00010
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00014
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0001c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00020
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00024
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e000003,                                                  // Trailing 1s:                                /// 0002c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00030
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00034
			 0x80011111,                                                  // -9.7958E-41                                 /// 00038
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0003c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00040
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00044
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00048
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00050
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00054
			 0x7f800000,                                                  // inf                                         /// 00058
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0005c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xffc00001,                                                  // -signaling NaN                              /// 00068
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0006c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00070
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xcb000000,                                                  // -8388608.0                                  /// 00078
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0007c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00080
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0008c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xbf028f5c,                                                  // -0.51                                       /// 00094
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00098
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000d4
			 0xbf028f5c,                                                  // -0.51                                       /// 000d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000f4
			 0xcb000000,                                                  // -8388608.0                                  /// 000f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00104
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00108
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00110
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00114
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00128
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0012c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00130
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00134
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00138
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00148
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0014c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00158
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0015c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00164
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0016c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00170
			 0x00000000,                                                  // zero                                        /// 00174
			 0x00011111,                                                  // 9.7958E-41                                  /// 00178
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00180
			 0xcb000000,                                                  // -8388608.0                                  /// 00184
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00194
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00198
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00200
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00208
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00214
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0021c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00220
			 0x33333333,                                                  // 4 random values                             /// 00224
			 0x7f800000,                                                  // inf                                         /// 00228
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0022c
			 0x3f028f5c,                                                  // 0.51                                        /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0023c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00240
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00244
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0024c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00258
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0025c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00260
			 0x3f028f5c,                                                  // 0.51                                        /// 00264
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0026c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00270
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00274
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0027c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00280
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00284
			 0xcb000000,                                                  // -8388608.0                                  /// 00288
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0028c
			 0x4b000000,                                                  // 8388608.0                                   /// 00290
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00294
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00304
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x7fc00001,                                                  // signaling NaN                               /// 0030c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00310
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00314
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00320
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0032c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00334
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00338
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0033c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00344
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00348
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00350
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00358
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0035c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xff800000,                                                  // -inf                                        /// 00364
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00368
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000000,                                                  // zero                                        /// 00374
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00378
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00380
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00388
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00390
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00394
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00398
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d8
			 0xff800000,                                                  // -inf                                        /// 003dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003f0
			 0x7fc00001,                                                  // signaling NaN                               /// 003f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00400
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00410
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00414
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00418
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00420
			 0x33333333,                                                  // 4 random values                             /// 00424
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00428
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0042c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00430
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00434
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00438
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0043c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00440
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0xffc00001,                                                  // -signaling NaN                              /// 0044c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0045c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00470
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00474
			 0x80000000,                                                  // -zero                                       /// 00478
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0047c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00488
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00494
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004cc
			 0x4b000000,                                                  // 8388608.0                                   /// 004d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00500
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00504
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0050c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00514
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00518
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00520
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000000,                                                  // zero                                        /// 00528
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0052c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x80011111,                                                  // -9.7958E-41                                 /// 00534
			 0xcb000000,                                                  // -8388608.0                                  /// 00538
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0053c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00540
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00554
			 0xff800000,                                                  // -inf                                        /// 00558
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00560
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00564
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00568
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0056c
			 0x55555555,                                                  // 4 random values                             /// 00570
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00574
			 0x80011111,                                                  // -9.7958E-41                                 /// 00578
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00584
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00588
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0058c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00590
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00594
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00598
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0059c
			 0x80000000,                                                  // -zero                                       /// 005a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b8
			 0x33333333,                                                  // 4 random values                             /// 005bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c4
			 0xffc00001,                                                  // -signaling NaN                              /// 005c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00600
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00604
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0060c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00610
			 0xcb000000,                                                  // -8388608.0                                  /// 00614
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00618
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00620
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0062c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00630
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00638
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x55555555,                                                  // 4 random values                             /// 00644
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00650
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0065c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00674
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0067c
			 0x3f028f5c,                                                  // 0.51                                        /// 00680
			 0xffc00001,                                                  // -signaling NaN                              /// 00684
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00688
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00690
			 0x0c780000,                                                  // Leading 1s:                                 /// 00694
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00698
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0069c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00700
			 0x0e000001,                                                  // Trailing 1s:                                /// 00704
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00708
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0070c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00710
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00714
			 0x55555555,                                                  // 4 random values                             /// 00718
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0071c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00720
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00724
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00728
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0072c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00730
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00734
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00738
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00740
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00748
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00750
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00758
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0075c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00764
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00768
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00778
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0077c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00784
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00790
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00794
			 0x0e000003,                                                  // Trailing 1s:                                /// 00798
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00800
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00810
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00818
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0081c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00820
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00824
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0082c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00838
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00844
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00848
			 0xffc00001,                                                  // -signaling NaN                              /// 0084c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00850
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00858
			 0x7f800000,                                                  // inf                                         /// 0085c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00860
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00868
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00870
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00874
			 0xcb000000,                                                  // -8388608.0                                  /// 00878
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00880
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00884
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0088c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00890
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00894
			 0x0c400000,                                                  // Leading 1s:                                 /// 00898
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 008fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00900
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00904
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0090c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00910
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00914
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0091c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00924
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00928
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0092c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00930
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00934
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0093c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00940
			 0xff800000,                                                  // -inf                                        /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00948
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00950
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00958
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0095c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00960
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00964
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00968
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0096c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00970
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00974
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00980
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00988
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0098c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00990
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7f800000,                                                  // inf                                         /// 009a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ac
			 0xbf028f5c,                                                  // -0.51                                       /// 009b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009fc
			 0xff800000,                                                  // -inf                                        /// 00a00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a10
			 0x33333333,                                                  // 4 random values                             /// 00a14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a1c
			 0x7f800000,                                                  // inf                                         /// 00a20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x55555555,                                                  // 4 random values                             /// 00a34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a78
			 0xcb000000,                                                  // -8388608.0                                  /// 00a7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aa4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00abc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00acc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ad8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00adc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ae4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00af4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00afc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b70
			 0x4b000000,                                                  // 8388608.0                                   /// 00b74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ba8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x55555555,                                                  // 4 random values                             /// 00bbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be8
			 0x00000000,                                                  // zero                                        /// 00bec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c7c
			 0xff800000,                                                  // -inf                                        /// 00c80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xbf028f5c,                                                  // -0.51                                       /// 00c98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ccc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7fc00001,                                                  // signaling NaN                               /// 00d5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00da0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7fc00001,                                                  // signaling NaN                               /// 00da8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x33333333,                                                  // 4 random values                             /// 00db0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ddc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00df4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x7fc00001,                                                  // signaling NaN                               /// 00e74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ea4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x7f800000,                                                  // inf                                         /// 00eb4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ecc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ed8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00edc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ee8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00eec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ef4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00efc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f5c
			 0xff800000,                                                  // -inf                                        /// 00f60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fa4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fdc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ff0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ff8
			 0x00000400                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x612ff2e0, /// 0x0
			 0x4e7a258b, /// 0x4
			 0x425124de, /// 0x8
			 0x4f5b24bb, /// 0xc
			 0xb841206b, /// 0x10
			 0x15b1fa5d, /// 0x14
			 0xe08a1111, /// 0x18
			 0xbf95d11a, /// 0x1c
			 0xe02ed682, /// 0x20
			 0xd022fc76, /// 0x24
			 0x04485f5b, /// 0x28
			 0x7f32f361, /// 0x2c
			 0x673abae0, /// 0x30
			 0xaf92b1fb, /// 0x34
			 0x6f223fd0, /// 0x38
			 0xb9f72e9a, /// 0x3c
			 0x4976342f, /// 0x40
			 0xa6443d9f, /// 0x44
			 0xbc62bf0a, /// 0x48
			 0x12022b40, /// 0x4c
			 0x82129316, /// 0x50
			 0xd415a4f5, /// 0x54
			 0x5d9f3b1f, /// 0x58
			 0x65a141d8, /// 0x5c
			 0xe0ea7d40, /// 0x60
			 0xbaee8b04, /// 0x64
			 0x1b83e25e, /// 0x68
			 0x41c47108, /// 0x6c
			 0x0097c813, /// 0x70
			 0x4d543062, /// 0x74
			 0x653782cf, /// 0x78
			 0x09d7d918, /// 0x7c
			 0x6f06c9b0, /// 0x80
			 0x2b05b66d, /// 0x84
			 0x046bf251, /// 0x88
			 0x881fcff2, /// 0x8c
			 0x23be5ecf, /// 0x90
			 0x1fa8dbfe, /// 0x94
			 0x5afb3a6f, /// 0x98
			 0x6cfba678, /// 0x9c
			 0xbf711264, /// 0xa0
			 0x4cbb1ea4, /// 0xa4
			 0xadf157da, /// 0xa8
			 0x0f28e1d4, /// 0xac
			 0x48fe748d, /// 0xb0
			 0x9e558cc0, /// 0xb4
			 0xbf07423f, /// 0xb8
			 0xf556b0c8, /// 0xbc
			 0xae5668c6, /// 0xc0
			 0xbcb2930c, /// 0xc4
			 0x1908e2b2, /// 0xc8
			 0x5db85e4a, /// 0xcc
			 0x621a1a2d, /// 0xd0
			 0x86789b14, /// 0xd4
			 0x672fc643, /// 0xd8
			 0x0b3fb468, /// 0xdc
			 0x8ed05126, /// 0xe0
			 0x6d160c73, /// 0xe4
			 0x50f6fd10, /// 0xe8
			 0x54af8aab, /// 0xec
			 0xc878f4af, /// 0xf0
			 0xe2358a0c, /// 0xf4
			 0xb68d4399, /// 0xf8
			 0xebc69b98, /// 0xfc
			 0x64152c80, /// 0x100
			 0x72f51c85, /// 0x104
			 0x9ff1d8ce, /// 0x108
			 0x58dda1ca, /// 0x10c
			 0x120a2e85, /// 0x110
			 0x9c1103e5, /// 0x114
			 0x0c746dae, /// 0x118
			 0x56453640, /// 0x11c
			 0x3ea3e636, /// 0x120
			 0x2b653ff8, /// 0x124
			 0xb12f1718, /// 0x128
			 0x5f7cd158, /// 0x12c
			 0x29a9c306, /// 0x130
			 0x4e45ea84, /// 0x134
			 0x4a1145dd, /// 0x138
			 0x2b59cc56, /// 0x13c
			 0xe9f47eb2, /// 0x140
			 0x0115206a, /// 0x144
			 0x077c40e7, /// 0x148
			 0x748e88e9, /// 0x14c
			 0x3ec45366, /// 0x150
			 0x5719dcf6, /// 0x154
			 0xe6fe2c0c, /// 0x158
			 0x13b7a675, /// 0x15c
			 0x0a5ac670, /// 0x160
			 0xc8868183, /// 0x164
			 0xdefbb52c, /// 0x168
			 0x07f893c5, /// 0x16c
			 0xd9f06533, /// 0x170
			 0xd72da00e, /// 0x174
			 0xb02a3e83, /// 0x178
			 0x4328eea9, /// 0x17c
			 0x7f7f9fea, /// 0x180
			 0xaeea8123, /// 0x184
			 0xd4db766c, /// 0x188
			 0x4e3fdcef, /// 0x18c
			 0x6d96274c, /// 0x190
			 0x1ca8d062, /// 0x194
			 0x57057dae, /// 0x198
			 0xb7fe19c5, /// 0x19c
			 0x30b81f11, /// 0x1a0
			 0x47b998bb, /// 0x1a4
			 0x008c5646, /// 0x1a8
			 0xcfa9e290, /// 0x1ac
			 0xa045c240, /// 0x1b0
			 0x53313575, /// 0x1b4
			 0xade9448d, /// 0x1b8
			 0x36930086, /// 0x1bc
			 0x916ed42e, /// 0x1c0
			 0x4456d263, /// 0x1c4
			 0x539c59e1, /// 0x1c8
			 0xe2c42b44, /// 0x1cc
			 0x278091ae, /// 0x1d0
			 0xbb81ad8b, /// 0x1d4
			 0xe7f629e7, /// 0x1d8
			 0xe8377d10, /// 0x1dc
			 0xb20236b2, /// 0x1e0
			 0x8573a822, /// 0x1e4
			 0xa81f7199, /// 0x1e8
			 0x9044d11b, /// 0x1ec
			 0xdb4d3797, /// 0x1f0
			 0x398d3d59, /// 0x1f4
			 0xa0fda70d, /// 0x1f8
			 0x94897306, /// 0x1fc
			 0xce7c818c, /// 0x200
			 0x0b62affb, /// 0x204
			 0x77a7b06b, /// 0x208
			 0x63ca48dd, /// 0x20c
			 0x13d160f9, /// 0x210
			 0x28fabe33, /// 0x214
			 0xde494a46, /// 0x218
			 0x55f9f532, /// 0x21c
			 0x1d8a1678, /// 0x220
			 0xd7ec99f8, /// 0x224
			 0x24c47556, /// 0x228
			 0x02f3f04e, /// 0x22c
			 0x043fd9a4, /// 0x230
			 0x4fd27840, /// 0x234
			 0xa7838941, /// 0x238
			 0x7fa6204a, /// 0x23c
			 0x4d292439, /// 0x240
			 0x1f8a4bdb, /// 0x244
			 0x099a4ed6, /// 0x248
			 0xd8644fa2, /// 0x24c
			 0x985af44e, /// 0x250
			 0x5980fc67, /// 0x254
			 0xf0ca8da9, /// 0x258
			 0x44f76e34, /// 0x25c
			 0x432dd1d0, /// 0x260
			 0x3401fcd8, /// 0x264
			 0x8beccf78, /// 0x268
			 0xf23ca842, /// 0x26c
			 0x499538f5, /// 0x270
			 0xd28e21f6, /// 0x274
			 0x5c8e3508, /// 0x278
			 0x82f681ac, /// 0x27c
			 0x62a03e3b, /// 0x280
			 0x2d147d25, /// 0x284
			 0x9732fa93, /// 0x288
			 0x7c361fe1, /// 0x28c
			 0x094f99df, /// 0x290
			 0x23aec32d, /// 0x294
			 0xad38c30b, /// 0x298
			 0x31e93f27, /// 0x29c
			 0x31aa49a9, /// 0x2a0
			 0xa5000a09, /// 0x2a4
			 0x035201ca, /// 0x2a8
			 0x6dda337a, /// 0x2ac
			 0x4af070c1, /// 0x2b0
			 0x7027869e, /// 0x2b4
			 0x4b7c64e9, /// 0x2b8
			 0x37789767, /// 0x2bc
			 0x30d571d0, /// 0x2c0
			 0x47466f2c, /// 0x2c4
			 0x75cc0bd1, /// 0x2c8
			 0x3578fc89, /// 0x2cc
			 0x98bf4cc8, /// 0x2d0
			 0x2b8b6938, /// 0x2d4
			 0xcb42850a, /// 0x2d8
			 0xe4ab128d, /// 0x2dc
			 0x06db0538, /// 0x2e0
			 0xc74ca6d9, /// 0x2e4
			 0x5c70e8de, /// 0x2e8
			 0x4e0d5a21, /// 0x2ec
			 0xe8d45370, /// 0x2f0
			 0x025b8e78, /// 0x2f4
			 0x2c56fd44, /// 0x2f8
			 0x998221a3, /// 0x2fc
			 0xf89055d5, /// 0x300
			 0xa14485f6, /// 0x304
			 0x0c01719b, /// 0x308
			 0xd0744d65, /// 0x30c
			 0xdb390930, /// 0x310
			 0xd39f8aa6, /// 0x314
			 0xd1347956, /// 0x318
			 0x3b60c06c, /// 0x31c
			 0xb5f37229, /// 0x320
			 0x94481f00, /// 0x324
			 0xc1544f8d, /// 0x328
			 0xc12b4f60, /// 0x32c
			 0x560e6990, /// 0x330
			 0xa59720e6, /// 0x334
			 0x4b5764f0, /// 0x338
			 0x0396e80f, /// 0x33c
			 0x6ee2e386, /// 0x340
			 0xee76220c, /// 0x344
			 0x8f1b2268, /// 0x348
			 0x6be21ad6, /// 0x34c
			 0xc3f1a6c7, /// 0x350
			 0xad9db516, /// 0x354
			 0x634ce078, /// 0x358
			 0x41aecdd5, /// 0x35c
			 0xe9ee9de2, /// 0x360
			 0xef877d30, /// 0x364
			 0x9c82beb0, /// 0x368
			 0xcb87884b, /// 0x36c
			 0x912428e8, /// 0x370
			 0xb25b997e, /// 0x374
			 0xa5f85288, /// 0x378
			 0x4cf03234, /// 0x37c
			 0xa698fe81, /// 0x380
			 0x8d3e490a, /// 0x384
			 0x83739784, /// 0x388
			 0x0c9443de, /// 0x38c
			 0x20070f80, /// 0x390
			 0xe3d22087, /// 0x394
			 0x13214e10, /// 0x398
			 0x46120fd8, /// 0x39c
			 0x0929aec1, /// 0x3a0
			 0xfcb3903e, /// 0x3a4
			 0x3b08fe03, /// 0x3a8
			 0x330dfb0b, /// 0x3ac
			 0xe5b70880, /// 0x3b0
			 0x4ecb2bc6, /// 0x3b4
			 0x141e07ac, /// 0x3b8
			 0x1a39067d, /// 0x3bc
			 0x01593279, /// 0x3c0
			 0xb7e1a38d, /// 0x3c4
			 0xf52455dd, /// 0x3c8
			 0x805010d4, /// 0x3cc
			 0x81823afb, /// 0x3d0
			 0x34f5f0ed, /// 0x3d4
			 0x7432cbb2, /// 0x3d8
			 0x3df15a26, /// 0x3dc
			 0xb66dc1c5, /// 0x3e0
			 0xd5941a4b, /// 0x3e4
			 0x5257e58e, /// 0x3e8
			 0x9ad8a938, /// 0x3ec
			 0xa01f0296, /// 0x3f0
			 0x45c705a3, /// 0x3f4
			 0xca8e8d37, /// 0x3f8
			 0x2827c276, /// 0x3fc
			 0x45f80494, /// 0x400
			 0x0329a8bc, /// 0x404
			 0x234a2a84, /// 0x408
			 0x39851266, /// 0x40c
			 0x21531e13, /// 0x410
			 0x7e69b8c7, /// 0x414
			 0xe3bcb6de, /// 0x418
			 0x611eea8c, /// 0x41c
			 0xd31e3ae2, /// 0x420
			 0x6b41bbc0, /// 0x424
			 0xe703706d, /// 0x428
			 0x8406dbc8, /// 0x42c
			 0xcf977d8a, /// 0x430
			 0xd11571d4, /// 0x434
			 0x06fa146e, /// 0x438
			 0x8d596551, /// 0x43c
			 0x14838407, /// 0x440
			 0x0eb99cd5, /// 0x444
			 0xc0280ae6, /// 0x448
			 0x662e1258, /// 0x44c
			 0xc2929b25, /// 0x450
			 0xe35bbc5c, /// 0x454
			 0xc9c222ef, /// 0x458
			 0xa4c3890b, /// 0x45c
			 0x80f2f9c9, /// 0x460
			 0x957ea308, /// 0x464
			 0x29223cc0, /// 0x468
			 0x9635cddd, /// 0x46c
			 0x2f498a13, /// 0x470
			 0xe9fce6ef, /// 0x474
			 0xf2844e66, /// 0x478
			 0x09344383, /// 0x47c
			 0x65172ac6, /// 0x480
			 0x4024b6c8, /// 0x484
			 0xa7cdd68b, /// 0x488
			 0x8b797ba5, /// 0x48c
			 0xd2ec324c, /// 0x490
			 0x9eb7caef, /// 0x494
			 0x98f12ca7, /// 0x498
			 0x8ba821e3, /// 0x49c
			 0xf6d6e766, /// 0x4a0
			 0x5c90e65c, /// 0x4a4
			 0x23a2cfb7, /// 0x4a8
			 0x1bce65de, /// 0x4ac
			 0xcf7079df, /// 0x4b0
			 0xd177bba5, /// 0x4b4
			 0xe68a1f73, /// 0x4b8
			 0x4263050d, /// 0x4bc
			 0xab7c8aff, /// 0x4c0
			 0xabb5ff5f, /// 0x4c4
			 0x2cc74444, /// 0x4c8
			 0x6756f0c7, /// 0x4cc
			 0x94917c15, /// 0x4d0
			 0xb6fbb63d, /// 0x4d4
			 0x9d746d1f, /// 0x4d8
			 0xf63d4654, /// 0x4dc
			 0x5ac83804, /// 0x4e0
			 0xb26e673d, /// 0x4e4
			 0xd0e388d7, /// 0x4e8
			 0x50a45c9e, /// 0x4ec
			 0x01c6d6d7, /// 0x4f0
			 0x21d9a3c0, /// 0x4f4
			 0xa6de55c3, /// 0x4f8
			 0x5108277a, /// 0x4fc
			 0xf052f51d, /// 0x500
			 0x9ebcf8da, /// 0x504
			 0x6dac38a8, /// 0x508
			 0xb8e51a51, /// 0x50c
			 0xc91408af, /// 0x510
			 0xc4b1d773, /// 0x514
			 0x9e8cafd5, /// 0x518
			 0xbdfb9779, /// 0x51c
			 0x6c96e419, /// 0x520
			 0x796256ac, /// 0x524
			 0xa9204ed0, /// 0x528
			 0x1ec229ab, /// 0x52c
			 0xc04bddc6, /// 0x530
			 0x10be7db5, /// 0x534
			 0x6f61a64b, /// 0x538
			 0xbc3987c2, /// 0x53c
			 0xe3ef5068, /// 0x540
			 0xc14bed40, /// 0x544
			 0xda246c2f, /// 0x548
			 0x1fe46e6a, /// 0x54c
			 0xaa9bfa04, /// 0x550
			 0x9bd56cb6, /// 0x554
			 0xbda64f2e, /// 0x558
			 0x27125ebe, /// 0x55c
			 0x93aa77a0, /// 0x560
			 0xd5c810af, /// 0x564
			 0x1ec7213c, /// 0x568
			 0xb6c0cd47, /// 0x56c
			 0xfad5fd3d, /// 0x570
			 0xb3bb8d9a, /// 0x574
			 0x8f9f613b, /// 0x578
			 0x95c8be24, /// 0x57c
			 0x33eff84d, /// 0x580
			 0xe2db04ae, /// 0x584
			 0x2b4ae33e, /// 0x588
			 0xd5b451fe, /// 0x58c
			 0x3b5500ff, /// 0x590
			 0x70a6732d, /// 0x594
			 0xd4b49db0, /// 0x598
			 0xc91923b9, /// 0x59c
			 0xc2017b50, /// 0x5a0
			 0x668d6a3e, /// 0x5a4
			 0xdbd99e2b, /// 0x5a8
			 0xfdb32578, /// 0x5ac
			 0x461ff91b, /// 0x5b0
			 0x2f945ae7, /// 0x5b4
			 0x3f554b48, /// 0x5b8
			 0x8be1aa03, /// 0x5bc
			 0x2a77b204, /// 0x5c0
			 0x900770ba, /// 0x5c4
			 0x7797ca0b, /// 0x5c8
			 0x6a720760, /// 0x5cc
			 0x4549463e, /// 0x5d0
			 0x1005551a, /// 0x5d4
			 0x5ac12cf8, /// 0x5d8
			 0xd46c1c19, /// 0x5dc
			 0x4d17c13a, /// 0x5e0
			 0x384c970f, /// 0x5e4
			 0xe55eb204, /// 0x5e8
			 0x2dbeb016, /// 0x5ec
			 0xcf873692, /// 0x5f0
			 0x37c35a93, /// 0x5f4
			 0x107574be, /// 0x5f8
			 0x6b27732e, /// 0x5fc
			 0xb2e7b512, /// 0x600
			 0x126509e1, /// 0x604
			 0x1ba65b60, /// 0x608
			 0x5987b758, /// 0x60c
			 0x2aaeda13, /// 0x610
			 0xcce2958c, /// 0x614
			 0x4c39ebc3, /// 0x618
			 0xf9bd4dee, /// 0x61c
			 0x82374a3b, /// 0x620
			 0xbdeab072, /// 0x624
			 0xc55e9811, /// 0x628
			 0xa1f8ca68, /// 0x62c
			 0xa86ef3ef, /// 0x630
			 0x5c32fa30, /// 0x634
			 0xc2a2f81c, /// 0x638
			 0x01fb34ab, /// 0x63c
			 0xe4488072, /// 0x640
			 0xd5408349, /// 0x644
			 0xed28b41f, /// 0x648
			 0xa0aeb1a3, /// 0x64c
			 0x54ad92d1, /// 0x650
			 0x1c76bfc3, /// 0x654
			 0x82c085fa, /// 0x658
			 0x9d3a0a0d, /// 0x65c
			 0x9727ab56, /// 0x660
			 0x17a0f225, /// 0x664
			 0x4b20c877, /// 0x668
			 0x0d136a28, /// 0x66c
			 0x122cad10, /// 0x670
			 0x748ce139, /// 0x674
			 0x628bf6f0, /// 0x678
			 0x7d48fa6f, /// 0x67c
			 0xa28aaab7, /// 0x680
			 0xf42bcaff, /// 0x684
			 0x6fe0413b, /// 0x688
			 0x39226d28, /// 0x68c
			 0xee95d8c7, /// 0x690
			 0xd47f37a1, /// 0x694
			 0x1a4c2455, /// 0x698
			 0x2bbc813f, /// 0x69c
			 0x1cb0c529, /// 0x6a0
			 0x9868959e, /// 0x6a4
			 0x2646c0c1, /// 0x6a8
			 0xe14f11f6, /// 0x6ac
			 0x8367cac6, /// 0x6b0
			 0x868cb423, /// 0x6b4
			 0xb153b89a, /// 0x6b8
			 0xd29078c6, /// 0x6bc
			 0xfa4d6cc1, /// 0x6c0
			 0xb4753583, /// 0x6c4
			 0x5f36ac8a, /// 0x6c8
			 0x05cbcb60, /// 0x6cc
			 0x53ec6eec, /// 0x6d0
			 0xbe2a73ec, /// 0x6d4
			 0x39c50ef5, /// 0x6d8
			 0x74f08483, /// 0x6dc
			 0x39a3d4f2, /// 0x6e0
			 0xd0f73da4, /// 0x6e4
			 0x641818d4, /// 0x6e8
			 0x256be98b, /// 0x6ec
			 0xba68baee, /// 0x6f0
			 0x415f93e0, /// 0x6f4
			 0xb3b13ac2, /// 0x6f8
			 0x8ebbd25c, /// 0x6fc
			 0xaa973efe, /// 0x700
			 0x98f5fb03, /// 0x704
			 0xb77f231d, /// 0x708
			 0x1d0ff28c, /// 0x70c
			 0x93df6e33, /// 0x710
			 0x95676671, /// 0x714
			 0xc39c0e8d, /// 0x718
			 0x402fe3fb, /// 0x71c
			 0xc4d38d88, /// 0x720
			 0xb1af98ef, /// 0x724
			 0x47192df2, /// 0x728
			 0xdf09981b, /// 0x72c
			 0x3dda9813, /// 0x730
			 0x96190bce, /// 0x734
			 0x10503559, /// 0x738
			 0xaa30b865, /// 0x73c
			 0x51df0d30, /// 0x740
			 0x90960bb7, /// 0x744
			 0xd45a7406, /// 0x748
			 0xd6252dd5, /// 0x74c
			 0x3749c240, /// 0x750
			 0x6bea6d4a, /// 0x754
			 0x9f9cf93d, /// 0x758
			 0xdae25f0d, /// 0x75c
			 0x2d545e57, /// 0x760
			 0xfbf72884, /// 0x764
			 0x4f616235, /// 0x768
			 0x2b2969cb, /// 0x76c
			 0xeb206038, /// 0x770
			 0x26a9aa4b, /// 0x774
			 0x7363ad0d, /// 0x778
			 0x788ae19d, /// 0x77c
			 0xd40bff64, /// 0x780
			 0xc29d8591, /// 0x784
			 0x13121564, /// 0x788
			 0x1b638fd6, /// 0x78c
			 0xf2cbc3fe, /// 0x790
			 0xa533ca56, /// 0x794
			 0xee42f3cf, /// 0x798
			 0x08cdee84, /// 0x79c
			 0x4fef60ab, /// 0x7a0
			 0x4010f160, /// 0x7a4
			 0x7425f74a, /// 0x7a8
			 0xa0c88c0b, /// 0x7ac
			 0x8c263677, /// 0x7b0
			 0xfc4113f6, /// 0x7b4
			 0x7c3b4b3c, /// 0x7b8
			 0x91e913f2, /// 0x7bc
			 0x6abd4cd4, /// 0x7c0
			 0xc7757f8b, /// 0x7c4
			 0xd19b1cbe, /// 0x7c8
			 0x3cc73a7a, /// 0x7cc
			 0xe56bb573, /// 0x7d0
			 0xa5d0583e, /// 0x7d4
			 0x2cfad3ab, /// 0x7d8
			 0xd50af865, /// 0x7dc
			 0x08933b56, /// 0x7e0
			 0x6bb63897, /// 0x7e4
			 0x85a1c484, /// 0x7e8
			 0x1d6ce51c, /// 0x7ec
			 0x1b8acd25, /// 0x7f0
			 0x0c8c68e9, /// 0x7f4
			 0x100e6603, /// 0x7f8
			 0x0f31f92e, /// 0x7fc
			 0x539ae9ed, /// 0x800
			 0x5adb7750, /// 0x804
			 0x70de6952, /// 0x808
			 0xf68166e2, /// 0x80c
			 0x3b975de5, /// 0x810
			 0xdc9fd47b, /// 0x814
			 0x45ec31be, /// 0x818
			 0x0b62c544, /// 0x81c
			 0xf5d097b8, /// 0x820
			 0x875383df, /// 0x824
			 0x571d0f18, /// 0x828
			 0xc928bdd4, /// 0x82c
			 0x54baff2b, /// 0x830
			 0x67b0d6d8, /// 0x834
			 0x446a041e, /// 0x838
			 0x2c437304, /// 0x83c
			 0x1b1335f4, /// 0x840
			 0xd3c53e87, /// 0x844
			 0x1186eb5b, /// 0x848
			 0x6f9d089f, /// 0x84c
			 0x4a77a87a, /// 0x850
			 0x786c7ac8, /// 0x854
			 0xb80c470e, /// 0x858
			 0x0475a31f, /// 0x85c
			 0x083d617b, /// 0x860
			 0x48dde46f, /// 0x864
			 0x4a803330, /// 0x868
			 0xbfc558ff, /// 0x86c
			 0x1a73bd19, /// 0x870
			 0x4912b89a, /// 0x874
			 0x1f03bd69, /// 0x878
			 0x83dafe9b, /// 0x87c
			 0x074566f2, /// 0x880
			 0xf7641712, /// 0x884
			 0x1387aefd, /// 0x888
			 0xa1e8c3e0, /// 0x88c
			 0x61041318, /// 0x890
			 0xa3456150, /// 0x894
			 0x82f67979, /// 0x898
			 0x17c513e8, /// 0x89c
			 0x90490c07, /// 0x8a0
			 0x7a7d92a0, /// 0x8a4
			 0xa47ff582, /// 0x8a8
			 0x235d31c0, /// 0x8ac
			 0xa37b3dfc, /// 0x8b0
			 0x7490e6f5, /// 0x8b4
			 0x4bfe698a, /// 0x8b8
			 0xde0bbe02, /// 0x8bc
			 0x0c415b02, /// 0x8c0
			 0x4ef71bff, /// 0x8c4
			 0x15d1ab8d, /// 0x8c8
			 0x3a23027a, /// 0x8cc
			 0xba64d32f, /// 0x8d0
			 0x3ccb4048, /// 0x8d4
			 0xef1f846c, /// 0x8d8
			 0x57e6761a, /// 0x8dc
			 0xf8340a7b, /// 0x8e0
			 0xa5b1ea6d, /// 0x8e4
			 0xaa92afd6, /// 0x8e8
			 0x06085329, /// 0x8ec
			 0x1bf27058, /// 0x8f0
			 0x7ff84d97, /// 0x8f4
			 0x97eb332d, /// 0x8f8
			 0x9a5ac93d, /// 0x8fc
			 0x391d37ee, /// 0x900
			 0xf33091cb, /// 0x904
			 0x7552acd8, /// 0x908
			 0x13c0232f, /// 0x90c
			 0x8f297551, /// 0x910
			 0x6a98ecf0, /// 0x914
			 0x0519d0a4, /// 0x918
			 0x7df55d55, /// 0x91c
			 0xb6679e2f, /// 0x920
			 0xb87c5dca, /// 0x924
			 0xf8067e42, /// 0x928
			 0x6174aa40, /// 0x92c
			 0xc487f300, /// 0x930
			 0x11994176, /// 0x934
			 0x67ea7d2c, /// 0x938
			 0x5992bc57, /// 0x93c
			 0xea506fe3, /// 0x940
			 0x1c93b979, /// 0x944
			 0x05acffde, /// 0x948
			 0xbb69572e, /// 0x94c
			 0x4bb7c64c, /// 0x950
			 0x05023b65, /// 0x954
			 0xc93d16ae, /// 0x958
			 0xeec370a5, /// 0x95c
			 0xcc2166e2, /// 0x960
			 0xc66d55f2, /// 0x964
			 0x0f3f40bb, /// 0x968
			 0x830180b1, /// 0x96c
			 0xd54387e7, /// 0x970
			 0xf67b471c, /// 0x974
			 0xfeb72212, /// 0x978
			 0xa6eebabf, /// 0x97c
			 0xea0964b0, /// 0x980
			 0x23a40b7d, /// 0x984
			 0x69d57618, /// 0x988
			 0x1dea3d7a, /// 0x98c
			 0xd248ad8b, /// 0x990
			 0x5572e49d, /// 0x994
			 0xa1b705c3, /// 0x998
			 0x88682a5a, /// 0x99c
			 0xb121233d, /// 0x9a0
			 0x0a4295c8, /// 0x9a4
			 0x98f3576c, /// 0x9a8
			 0xd6271dee, /// 0x9ac
			 0x144b7006, /// 0x9b0
			 0xf4860f91, /// 0x9b4
			 0xb4d9cebc, /// 0x9b8
			 0x699f056d, /// 0x9bc
			 0x82f16380, /// 0x9c0
			 0x027fc9ae, /// 0x9c4
			 0xe6990dbb, /// 0x9c8
			 0x4f1eb5d2, /// 0x9cc
			 0x32a7ddb7, /// 0x9d0
			 0xce2f02cf, /// 0x9d4
			 0xa952e28e, /// 0x9d8
			 0x9fb364b4, /// 0x9dc
			 0xe35adf8e, /// 0x9e0
			 0xad813946, /// 0x9e4
			 0x7b815bad, /// 0x9e8
			 0xf2aea69e, /// 0x9ec
			 0xb20b91ea, /// 0x9f0
			 0x7cb9152f, /// 0x9f4
			 0x907e4742, /// 0x9f8
			 0x44aae834, /// 0x9fc
			 0x3684b407, /// 0xa00
			 0x4c55c9cc, /// 0xa04
			 0xc0be2d2a, /// 0xa08
			 0x5d32afa2, /// 0xa0c
			 0x88bae2af, /// 0xa10
			 0xe9a709c8, /// 0xa14
			 0x46e58db8, /// 0xa18
			 0x0230e4fa, /// 0xa1c
			 0xfca9df67, /// 0xa20
			 0x934405c1, /// 0xa24
			 0xc8e94e7d, /// 0xa28
			 0x2f79c557, /// 0xa2c
			 0x8d8118ab, /// 0xa30
			 0xc4c46ed9, /// 0xa34
			 0xa26007c8, /// 0xa38
			 0x596f193c, /// 0xa3c
			 0x6bae1637, /// 0xa40
			 0x7d9e0981, /// 0xa44
			 0x7f2ff97a, /// 0xa48
			 0x267760ad, /// 0xa4c
			 0x10d715f4, /// 0xa50
			 0x9f5dab47, /// 0xa54
			 0x8c5e1512, /// 0xa58
			 0x2301acf9, /// 0xa5c
			 0x5e2c51fc, /// 0xa60
			 0xb1f7b9a7, /// 0xa64
			 0x77ef397c, /// 0xa68
			 0x52bb6677, /// 0xa6c
			 0x509993a4, /// 0xa70
			 0x610fe6a7, /// 0xa74
			 0x74f4e4ea, /// 0xa78
			 0xd40a8486, /// 0xa7c
			 0xe325cb0c, /// 0xa80
			 0x0c22331d, /// 0xa84
			 0x5e290449, /// 0xa88
			 0xedbfb59e, /// 0xa8c
			 0xa2d6021a, /// 0xa90
			 0x62edf14d, /// 0xa94
			 0x8213b4be, /// 0xa98
			 0x517f8bb7, /// 0xa9c
			 0x7fb2c2ad, /// 0xaa0
			 0xcbaf500c, /// 0xaa4
			 0xabf2ce7d, /// 0xaa8
			 0xb171c411, /// 0xaac
			 0x755c8ab3, /// 0xab0
			 0xb4295f3d, /// 0xab4
			 0x24b58af9, /// 0xab8
			 0xfe24789d, /// 0xabc
			 0x8262f814, /// 0xac0
			 0x79bd83f1, /// 0xac4
			 0xaf7f476d, /// 0xac8
			 0x0b7767df, /// 0xacc
			 0x0a366309, /// 0xad0
			 0x18c0ecc9, /// 0xad4
			 0x22148fb2, /// 0xad8
			 0x8302a739, /// 0xadc
			 0xf9f92062, /// 0xae0
			 0x6c3596e9, /// 0xae4
			 0x6707af96, /// 0xae8
			 0x712e1789, /// 0xaec
			 0x7ca00287, /// 0xaf0
			 0x30936aa3, /// 0xaf4
			 0x87cbdb3d, /// 0xaf8
			 0x3345d13e, /// 0xafc
			 0x5c4c3c7d, /// 0xb00
			 0x5131bf23, /// 0xb04
			 0x0dcd8e8a, /// 0xb08
			 0x9820edd0, /// 0xb0c
			 0xfa53affa, /// 0xb10
			 0xa80ea03a, /// 0xb14
			 0x802bacf3, /// 0xb18
			 0x23a2b210, /// 0xb1c
			 0x8c24bcef, /// 0xb20
			 0x8960e930, /// 0xb24
			 0xe0725249, /// 0xb28
			 0x37c45dfc, /// 0xb2c
			 0xfa2852ea, /// 0xb30
			 0x91a35bf1, /// 0xb34
			 0x9cd526ad, /// 0xb38
			 0x75b233df, /// 0xb3c
			 0xb4531b5a, /// 0xb40
			 0xc929ed55, /// 0xb44
			 0x34778f61, /// 0xb48
			 0xd805a749, /// 0xb4c
			 0xb53516ad, /// 0xb50
			 0x8efad750, /// 0xb54
			 0x4fcd8861, /// 0xb58
			 0xfa75a11f, /// 0xb5c
			 0x0a022cce, /// 0xb60
			 0x4a4d7d2c, /// 0xb64
			 0x4199998a, /// 0xb68
			 0x7202b06e, /// 0xb6c
			 0xa5e505eb, /// 0xb70
			 0x53bff207, /// 0xb74
			 0x8b83001f, /// 0xb78
			 0x51a26132, /// 0xb7c
			 0x77c62106, /// 0xb80
			 0x0ac2af35, /// 0xb84
			 0xd41b3cc6, /// 0xb88
			 0xf11b30a9, /// 0xb8c
			 0xb3fbd93b, /// 0xb90
			 0x09c4de42, /// 0xb94
			 0xfc5cb812, /// 0xb98
			 0x734b22b3, /// 0xb9c
			 0x88cc4d74, /// 0xba0
			 0xdaab4ab7, /// 0xba4
			 0x74962a32, /// 0xba8
			 0x08978c52, /// 0xbac
			 0xa5d88ba9, /// 0xbb0
			 0xe11f3c16, /// 0xbb4
			 0xe1817a81, /// 0xbb8
			 0xa5de3231, /// 0xbbc
			 0xe3897205, /// 0xbc0
			 0x9ac1ffa5, /// 0xbc4
			 0xded8f2d5, /// 0xbc8
			 0x919e26bf, /// 0xbcc
			 0xdcec4c8a, /// 0xbd0
			 0x7ea0ccb5, /// 0xbd4
			 0xa84aa7e3, /// 0xbd8
			 0x8b1e07b1, /// 0xbdc
			 0x6635e83f, /// 0xbe0
			 0xe478f981, /// 0xbe4
			 0x40d70867, /// 0xbe8
			 0x4d8cc0bc, /// 0xbec
			 0xea17217c, /// 0xbf0
			 0xecf8e454, /// 0xbf4
			 0x427ca65c, /// 0xbf8
			 0x655a392f, /// 0xbfc
			 0x052d7a34, /// 0xc00
			 0xda862eb1, /// 0xc04
			 0xf9ad0bb1, /// 0xc08
			 0xc8fce65f, /// 0xc0c
			 0xdab28982, /// 0xc10
			 0xc68d6e61, /// 0xc14
			 0x5a170616, /// 0xc18
			 0x13ecfbd9, /// 0xc1c
			 0x788c7dba, /// 0xc20
			 0xe7444a39, /// 0xc24
			 0x0b00a3e2, /// 0xc28
			 0xfbd2a3ff, /// 0xc2c
			 0x5b9e149b, /// 0xc30
			 0x7d3b4d10, /// 0xc34
			 0xc2b6dc64, /// 0xc38
			 0xbc208379, /// 0xc3c
			 0xec04f742, /// 0xc40
			 0xb4da2c32, /// 0xc44
			 0xccb5c2d5, /// 0xc48
			 0x8f882f8a, /// 0xc4c
			 0x95068cc6, /// 0xc50
			 0x6490eeab, /// 0xc54
			 0x0689101a, /// 0xc58
			 0xcc9c6c2e, /// 0xc5c
			 0x053c907f, /// 0xc60
			 0xc1b3818a, /// 0xc64
			 0x6cb89630, /// 0xc68
			 0x54cba7ad, /// 0xc6c
			 0x9db708a9, /// 0xc70
			 0xee2fd431, /// 0xc74
			 0x03a12e29, /// 0xc78
			 0x7ee3b731, /// 0xc7c
			 0xcb5eb95d, /// 0xc80
			 0x02eac4ce, /// 0xc84
			 0x374b4f6c, /// 0xc88
			 0xea92cc0c, /// 0xc8c
			 0x2d72a552, /// 0xc90
			 0x2825c658, /// 0xc94
			 0xf8e080ee, /// 0xc98
			 0x7fb49821, /// 0xc9c
			 0x4f5d6d98, /// 0xca0
			 0xebccc9ff, /// 0xca4
			 0xa008d23f, /// 0xca8
			 0xcfbca43a, /// 0xcac
			 0x7af0ee86, /// 0xcb0
			 0x42a09745, /// 0xcb4
			 0x4f647249, /// 0xcb8
			 0xbf4c9b2a, /// 0xcbc
			 0x03dc7a18, /// 0xcc0
			 0x6548f714, /// 0xcc4
			 0xcda6a155, /// 0xcc8
			 0x9801f09e, /// 0xccc
			 0xcf1e7161, /// 0xcd0
			 0xed85be48, /// 0xcd4
			 0x6949727c, /// 0xcd8
			 0x5c8295c9, /// 0xcdc
			 0xaa66ca13, /// 0xce0
			 0x60016006, /// 0xce4
			 0xe677da96, /// 0xce8
			 0x7d114960, /// 0xcec
			 0x82ad729b, /// 0xcf0
			 0x50dc88fe, /// 0xcf4
			 0xd1fb8acf, /// 0xcf8
			 0xed3e98e2, /// 0xcfc
			 0x9b197680, /// 0xd00
			 0xa8e0e436, /// 0xd04
			 0x52960b6a, /// 0xd08
			 0x960814af, /// 0xd0c
			 0xc80fdaad, /// 0xd10
			 0x7bac67f3, /// 0xd14
			 0x9a5f4384, /// 0xd18
			 0x1393ac04, /// 0xd1c
			 0x80026181, /// 0xd20
			 0x34261e12, /// 0xd24
			 0x241fcceb, /// 0xd28
			 0x3a8e25a9, /// 0xd2c
			 0x8cea4fcd, /// 0xd30
			 0xb2603322, /// 0xd34
			 0xcbfd1787, /// 0xd38
			 0x0331c350, /// 0xd3c
			 0xf84fe33e, /// 0xd40
			 0xba4d3da8, /// 0xd44
			 0xb2d0cf26, /// 0xd48
			 0x4c3e2f44, /// 0xd4c
			 0x98d1d1d8, /// 0xd50
			 0x3cb08da6, /// 0xd54
			 0xe188326c, /// 0xd58
			 0x41689ec1, /// 0xd5c
			 0xc416b6d8, /// 0xd60
			 0xed0c9f3f, /// 0xd64
			 0x6e023997, /// 0xd68
			 0x4df6d0bf, /// 0xd6c
			 0xba159516, /// 0xd70
			 0xb9d3220c, /// 0xd74
			 0xe2242b8a, /// 0xd78
			 0x97ff68e8, /// 0xd7c
			 0x740699b6, /// 0xd80
			 0xbe4a6dfd, /// 0xd84
			 0x5f829b94, /// 0xd88
			 0x486c635b, /// 0xd8c
			 0xc5ab134c, /// 0xd90
			 0xb43ba5c1, /// 0xd94
			 0xd9853f81, /// 0xd98
			 0x423de2e1, /// 0xd9c
			 0x1c03aae5, /// 0xda0
			 0xf5f4615b, /// 0xda4
			 0x33d6e0f2, /// 0xda8
			 0x5583cede, /// 0xdac
			 0x138d64aa, /// 0xdb0
			 0x21037823, /// 0xdb4
			 0xcaca84b0, /// 0xdb8
			 0x94a48957, /// 0xdbc
			 0xaa572102, /// 0xdc0
			 0x82ff9350, /// 0xdc4
			 0x4164b596, /// 0xdc8
			 0xeae3f8f6, /// 0xdcc
			 0xfb48c895, /// 0xdd0
			 0xe0200674, /// 0xdd4
			 0xa93bd9d3, /// 0xdd8
			 0xba6b4ae8, /// 0xddc
			 0xa1f2522d, /// 0xde0
			 0x690e9bc3, /// 0xde4
			 0x83e39650, /// 0xde8
			 0xcd38efbe, /// 0xdec
			 0x3b3308eb, /// 0xdf0
			 0x039fa3c4, /// 0xdf4
			 0x467257cf, /// 0xdf8
			 0x893fb5b2, /// 0xdfc
			 0xcb722e14, /// 0xe00
			 0x9a54b2be, /// 0xe04
			 0x32537a13, /// 0xe08
			 0xcb3d632f, /// 0xe0c
			 0xdac4c7da, /// 0xe10
			 0xead85fde, /// 0xe14
			 0xd85e6805, /// 0xe18
			 0xbd78c309, /// 0xe1c
			 0x8756ff8d, /// 0xe20
			 0x0f6b29c6, /// 0xe24
			 0xde38a618, /// 0xe28
			 0xad511b39, /// 0xe2c
			 0x6cbbd7b3, /// 0xe30
			 0xfddbd41b, /// 0xe34
			 0x7c8fbf43, /// 0xe38
			 0xd6602466, /// 0xe3c
			 0x18e271b0, /// 0xe40
			 0x9eece0fb, /// 0xe44
			 0x9dccde9e, /// 0xe48
			 0x0ebf1b65, /// 0xe4c
			 0x6fb96239, /// 0xe50
			 0x4835969e, /// 0xe54
			 0x23794da9, /// 0xe58
			 0x9a955aa7, /// 0xe5c
			 0x0f958edf, /// 0xe60
			 0x2fe38341, /// 0xe64
			 0x91f45677, /// 0xe68
			 0x8d75eacc, /// 0xe6c
			 0x3e8a005d, /// 0xe70
			 0x213d41f9, /// 0xe74
			 0x4a531867, /// 0xe78
			 0xde5099aa, /// 0xe7c
			 0x40d5d863, /// 0xe80
			 0x767267c0, /// 0xe84
			 0xfbe60a86, /// 0xe88
			 0x19a63ce9, /// 0xe8c
			 0xcd1064b2, /// 0xe90
			 0x16dbd46e, /// 0xe94
			 0x77e41b4a, /// 0xe98
			 0x75f8d4db, /// 0xe9c
			 0x3858b490, /// 0xea0
			 0x02e2239b, /// 0xea4
			 0xeb8ac5f9, /// 0xea8
			 0xfc9f09f5, /// 0xeac
			 0x0ab65ad7, /// 0xeb0
			 0x2e81b2ec, /// 0xeb4
			 0xb659da2a, /// 0xeb8
			 0x5a046cb7, /// 0xebc
			 0x79b8506d, /// 0xec0
			 0xd21c368f, /// 0xec4
			 0x87063a02, /// 0xec8
			 0xa912030b, /// 0xecc
			 0xe72f261f, /// 0xed0
			 0xab1376fc, /// 0xed4
			 0x0f5d7886, /// 0xed8
			 0xffbe1d3d, /// 0xedc
			 0x9869d2f8, /// 0xee0
			 0x565964c9, /// 0xee4
			 0x1510c697, /// 0xee8
			 0x6151b032, /// 0xeec
			 0xdd984d25, /// 0xef0
			 0x9123b83a, /// 0xef4
			 0x76c686a6, /// 0xef8
			 0xb5e80666, /// 0xefc
			 0x9222d5c8, /// 0xf00
			 0xc4566e85, /// 0xf04
			 0x11ec02f8, /// 0xf08
			 0x429de9a5, /// 0xf0c
			 0x7983ec36, /// 0xf10
			 0xb5b84ccc, /// 0xf14
			 0x8e6282e7, /// 0xf18
			 0x6fad8b37, /// 0xf1c
			 0x72110430, /// 0xf20
			 0x536fd57c, /// 0xf24
			 0x48b31115, /// 0xf28
			 0x82494b28, /// 0xf2c
			 0xd6919de2, /// 0xf30
			 0x633bbea8, /// 0xf34
			 0xc6ba35e8, /// 0xf38
			 0xd3cd78a6, /// 0xf3c
			 0x06f931fc, /// 0xf40
			 0x4fcc9b04, /// 0xf44
			 0x2b3d03bf, /// 0xf48
			 0x28a3f758, /// 0xf4c
			 0xb604fedc, /// 0xf50
			 0xb3b4dee2, /// 0xf54
			 0x9fcfa6f1, /// 0xf58
			 0xd2996bb1, /// 0xf5c
			 0x39557a68, /// 0xf60
			 0xb9fccf00, /// 0xf64
			 0x45920316, /// 0xf68
			 0x45f6cfd6, /// 0xf6c
			 0x9fb5330a, /// 0xf70
			 0x9d21e0dc, /// 0xf74
			 0xde7f02ab, /// 0xf78
			 0xeb14d161, /// 0xf7c
			 0x02eda699, /// 0xf80
			 0x5db864f4, /// 0xf84
			 0xe2b5ea2a, /// 0xf88
			 0x1d52496f, /// 0xf8c
			 0x43f1bc8d, /// 0xf90
			 0x45596e2e, /// 0xf94
			 0x9471c41a, /// 0xf98
			 0x2224f14f, /// 0xf9c
			 0x3a61c4d1, /// 0xfa0
			 0x2e08ea70, /// 0xfa4
			 0x9b86967e, /// 0xfa8
			 0x0d003f8d, /// 0xfac
			 0xa2cbd14b, /// 0xfb0
			 0x34f1cdb4, /// 0xfb4
			 0x33ece4d7, /// 0xfb8
			 0x17b10f99, /// 0xfbc
			 0x21b156be, /// 0xfc0
			 0x476df6b1, /// 0xfc4
			 0x57df1e4c, /// 0xfc8
			 0x4cae21b9, /// 0xfcc
			 0x8827e98b, /// 0xfd0
			 0x2a51c081, /// 0xfd4
			 0x59d0a27a, /// 0xfd8
			 0xb85bdc58, /// 0xfdc
			 0xd1c44b2f, /// 0xfe0
			 0xd6958560, /// 0xfe4
			 0xba0980fe, /// 0xfe8
			 0x896deaee, /// 0xfec
			 0xdaae90d7, /// 0xff0
			 0xa6118f7e, /// 0xff4
			 0x4dff3edc, /// 0xff8
			 0x75d376ba /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0c700000,                                                  // Leading 1s:                                 /// 0000c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00014
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00018
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0xbf028f5c,                                                  // -0.51                                       /// 00020
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00024
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00028
			 0x00011111,                                                  // 9.7958E-41                                  /// 0002c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00030
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00038
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0003c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00040
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00044
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0004c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00060
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00064
			 0x3f028f5c,                                                  // 0.51                                        /// 00068
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00070
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00078
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0007c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00080
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00084
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00088
			 0x00000000,                                                  // zero                                        /// 0008c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00090
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00098
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0009c
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00100
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00108
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00114
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00118
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00128
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0012c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00130
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00134
			 0x33333333,                                                  // 4 random values                             /// 00138
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0013c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x0c700000,                                                  // Leading 1s:                                 /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00148
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0014c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00150
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00154
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00158
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0015c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00160
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00164
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00174
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00178
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00180
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00194
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00198
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0019c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00200
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00204
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00208
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0020c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00210
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00214
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00218
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0021c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00224
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00228
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0022c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00238
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00240
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00244
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00250
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00254
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00258
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0x0e000003,                                                  // Trailing 1s:                                /// 00264
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x7fc00001,                                                  // signaling NaN                               /// 0026c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00274
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0027c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00284
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00288
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0028c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00290
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00294
			 0x33333333,                                                  // 4 random values                             /// 00298
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0029c
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002ec
			 0xffc00001,                                                  // -signaling NaN                              /// 002f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00300
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x7fc00001,                                                  // signaling NaN                               /// 00308
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0030c
			 0x80000000,                                                  // -zero                                       /// 00310
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00318
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0031c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00320
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00324
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00328
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0032c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00330
			 0x00011111,                                                  // 9.7958E-41                                  /// 00334
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00338
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0033c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00340
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00344
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00348
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0034c
			 0xffc00001,                                                  // -signaling NaN                              /// 00350
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00358
			 0x0c600000,                                                  // Leading 1s:                                 /// 0035c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00360
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00011111,                                                  // 9.7958E-41                                  /// 0036c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00378
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0037c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00380
			 0xbf028f5c,                                                  // -0.51                                       /// 00384
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00388
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00390
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0039c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003d8
			 0x33333333,                                                  // 4 random values                             /// 003dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00400
			 0x0e000007,                                                  // Trailing 1s:                                /// 00404
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0040c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00414
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00418
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00420
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00424
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00430
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00434
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00438
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00454
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00468
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00470
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00474
			 0xcb000000,                                                  // -8388608.0                                  /// 00478
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00484
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00490
			 0x00011111,                                                  // 9.7958E-41                                  /// 00494
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00498
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x7fc00001,                                                  // signaling NaN                               /// 004c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00500
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00504
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00508
			 0x7f800000,                                                  // inf                                         /// 0050c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00510
			 0x00000000,                                                  // zero                                        /// 00514
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00518
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x80000000,                                                  // -zero                                       /// 00520
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00524
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0052c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00530
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00534
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00538
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00544
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00554
			 0x55555555,                                                  // 4 random values                             /// 00558
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0055c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00560
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x7f800000,                                                  // inf                                         /// 00568
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0056c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00570
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00574
			 0x0c780000,                                                  // Leading 1s:                                 /// 00578
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0057c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00580
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00590
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00598
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a0
			 0x4b000000,                                                  // 8388608.0                                   /// 005a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0xffc00001,                                                  // -signaling NaN                              /// 005d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00600
			 0x33333333,                                                  // 4 random values                             /// 00604
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00608
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00610
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00614
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00624
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00630
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00634
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00640
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00644
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00648
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0064c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00650
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00658
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00660
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00664
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00668
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0066c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80000000,                                                  // -zero                                       /// 00678
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0067c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00680
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00684
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00688
			 0x0c700000,                                                  // Leading 1s:                                 /// 0068c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00698
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x4b000000,                                                  // 8388608.0                                   /// 006a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80000000,                                                  // -zero                                       /// 006b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x55555555,                                                  // 4 random values                             /// 006e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00704
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0070c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00714
			 0x55555555,                                                  // 4 random values                             /// 00718
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0071c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00734
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00738
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00744
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00748
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00750
			 0x33333333,                                                  // 4 random values                             /// 00754
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00760
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00764
			 0x3f028f5c,                                                  // 0.51                                        /// 00768
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0076c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00774
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00778
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0077c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00780
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00784
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x00000000,                                                  // zero                                        /// 00790
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0079c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007a4
			 0x55555555,                                                  // 4 random values                             /// 007a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00800
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00808
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0080c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00814
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00818
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0081c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00820
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00824
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00828
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0082c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00830
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00838
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0083c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00840
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00850
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00854
			 0xcb000000,                                                  // -8388608.0                                  /// 00858
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0085c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00860
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00870
			 0x0c780000,                                                  // Leading 1s:                                 /// 00874
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00878
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0087c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00888
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00890
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00898
			 0x0c700000,                                                  // Leading 1s:                                 /// 0089c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008bc
			 0x33333333,                                                  // 4 random values                             /// 008c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008dc
			 0xff800000,                                                  // -inf                                        /// 008e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00904
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0090c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00920
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00924
			 0x55555555,                                                  // 4 random values                             /// 00928
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00934
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0093c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00948
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0094c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00954
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00958
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00964
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0096c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00970
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00978
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00980
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00984
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0098c
			 0xffc00001,                                                  // -signaling NaN                              /// 00990
			 0xbf028f5c,                                                  // -0.51                                       /// 00994
			 0x3f028f5c,                                                  // 0.51                                        /// 00998
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a0c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a24
			 0xbf028f5c,                                                  // -0.51                                       /// 00a28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x4b000000,                                                  // 8388608.0                                   /// 00a54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a70
			 0x4b000000,                                                  // 8388608.0                                   /// 00a74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aa4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ab4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ac4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00adc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00af0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b00
			 0x4b000000,                                                  // 8388608.0                                   /// 00b04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b64
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ba4
			 0x80000000,                                                  // -zero                                       /// 00ba8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0xffc00001,                                                  // -signaling NaN                              /// 00bd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bd4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bf4
			 0xff800000,                                                  // -inf                                        /// 00bf8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c70
			 0xff800000,                                                  // -inf                                        /// 00c74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c78
			 0xcb000000,                                                  // -8388608.0                                  /// 00c7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c80
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x7fc00001,                                                  // signaling NaN                               /// 00c88
			 0xffc00001,                                                  // -signaling NaN                              /// 00c8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ccc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x55555555,                                                  // 4 random values                             /// 00cd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ce0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cf0
			 0x00000000,                                                  // zero                                        /// 00cf4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cfc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d50
			 0x4b000000,                                                  // 8388608.0                                   /// 00d54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d78
			 0x7fc00001,                                                  // signaling NaN                               /// 00d7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d8c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00da0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00db8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de4
			 0x7f800000,                                                  // inf                                         /// 00de8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00df0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x33333333,                                                  // 4 random values                             /// 00e24
			 0x7f800000,                                                  // inf                                         /// 00e28
			 0x4b000000,                                                  // 8388608.0                                   /// 00e2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e50
			 0x7fc00001,                                                  // signaling NaN                               /// 00e54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e68
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0x3f028f5c,                                                  // 0.51                                        /// 00e84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e90
			 0x33333333,                                                  // 4 random values                             /// 00e94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ea8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eb4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80000000,                                                  // -zero                                       /// 00ec0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x33333333,                                                  // 4 random values                             /// 00ef0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000000,                                                  // -zero                                       /// 00f04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f08
			 0xff800000,                                                  // -inf                                        /// 00f0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7f800000,                                                  // inf                                         /// 00f60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xbf028f5c,                                                  // -0.51                                       /// 00f84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa0
			 0x80000000,                                                  // -zero                                       /// 00fa4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fcc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff4
			 0x55555555,                                                  // 4 random values                             /// 00ff8
			 0x00000010                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x6bdf2fdb, /// 0x0
			 0x277985ca, /// 0x4
			 0xf641b187, /// 0x8
			 0x895e23df, /// 0xc
			 0xec3f4679, /// 0x10
			 0x53a0e2db, /// 0x14
			 0x37720caf, /// 0x18
			 0xd72a7841, /// 0x1c
			 0x6c7a6d1e, /// 0x20
			 0xbd3b4fa2, /// 0x24
			 0xb08a4143, /// 0x28
			 0xfb49608a, /// 0x2c
			 0xb6f5bb4e, /// 0x30
			 0x4accdc16, /// 0x34
			 0x068f8597, /// 0x38
			 0x7aa3e137, /// 0x3c
			 0xc75ffacd, /// 0x40
			 0x82366c92, /// 0x44
			 0x83f3ecf4, /// 0x48
			 0x6eb70350, /// 0x4c
			 0xdf0c26a6, /// 0x50
			 0x1b878a16, /// 0x54
			 0xe50dcc0b, /// 0x58
			 0xc87b0679, /// 0x5c
			 0xf389fd66, /// 0x60
			 0x9464bce2, /// 0x64
			 0x1dbe07dc, /// 0x68
			 0xa65f769b, /// 0x6c
			 0xfb17b881, /// 0x70
			 0x94833867, /// 0x74
			 0x26c4fc8e, /// 0x78
			 0xe294b8e4, /// 0x7c
			 0x4392cdf7, /// 0x80
			 0xe1aab46b, /// 0x84
			 0x11701cfa, /// 0x88
			 0x77d34005, /// 0x8c
			 0x61fdef31, /// 0x90
			 0x39a7b87f, /// 0x94
			 0x299ac389, /// 0x98
			 0x12e09cfb, /// 0x9c
			 0x5233bec4, /// 0xa0
			 0x779c4799, /// 0xa4
			 0x4f8dcad4, /// 0xa8
			 0xe8c12c22, /// 0xac
			 0xab320a42, /// 0xb0
			 0xf0c7608c, /// 0xb4
			 0x2bb036e9, /// 0xb8
			 0x11ffe3b9, /// 0xbc
			 0x4b2f2999, /// 0xc0
			 0x384b137f, /// 0xc4
			 0xa69a4b38, /// 0xc8
			 0x7e9b57ce, /// 0xcc
			 0x85e71d8f, /// 0xd0
			 0x1d670350, /// 0xd4
			 0xeac4b720, /// 0xd8
			 0x933b520e, /// 0xdc
			 0xd97d688a, /// 0xe0
			 0x85861b41, /// 0xe4
			 0x476d5110, /// 0xe8
			 0x86058e91, /// 0xec
			 0xda15e4a9, /// 0xf0
			 0xbf361c14, /// 0xf4
			 0xedb423b9, /// 0xf8
			 0x26aa2510, /// 0xfc
			 0xa2a9ce8a, /// 0x100
			 0x10d3b385, /// 0x104
			 0x73d89041, /// 0x108
			 0xe475c921, /// 0x10c
			 0xdc0cd7ff, /// 0x110
			 0x2593b56a, /// 0x114
			 0x22137955, /// 0x118
			 0x34dc4d9a, /// 0x11c
			 0xb8bec7cf, /// 0x120
			 0x99ee5bba, /// 0x124
			 0x9a594cb5, /// 0x128
			 0x927b34f2, /// 0x12c
			 0x6362782b, /// 0x130
			 0xd0a58bdf, /// 0x134
			 0x84240ea9, /// 0x138
			 0xe37d74ca, /// 0x13c
			 0x5bed4b4b, /// 0x140
			 0x74ce737e, /// 0x144
			 0xa3ea0ef9, /// 0x148
			 0xa10dc77d, /// 0x14c
			 0x302ad371, /// 0x150
			 0x4f7ceee3, /// 0x154
			 0xe64c432a, /// 0x158
			 0x5736c30d, /// 0x15c
			 0x19b066b8, /// 0x160
			 0xb9d2a98d, /// 0x164
			 0x79d301b8, /// 0x168
			 0x67a999ea, /// 0x16c
			 0xfb2bd028, /// 0x170
			 0x6374159e, /// 0x174
			 0xf8e04723, /// 0x178
			 0x4cf541d9, /// 0x17c
			 0x4474cfc6, /// 0x180
			 0xdd947af1, /// 0x184
			 0xc19f5e87, /// 0x188
			 0x4436d44e, /// 0x18c
			 0x214d40a9, /// 0x190
			 0xbba19fc1, /// 0x194
			 0x4fc10898, /// 0x198
			 0x2c60b3f7, /// 0x19c
			 0xedf11f0d, /// 0x1a0
			 0x87952aa0, /// 0x1a4
			 0x026fe98c, /// 0x1a8
			 0xc1385e4b, /// 0x1ac
			 0xccc75253, /// 0x1b0
			 0x774c2f90, /// 0x1b4
			 0x735df263, /// 0x1b8
			 0x9ae1c196, /// 0x1bc
			 0x3d4928af, /// 0x1c0
			 0x7baa18ce, /// 0x1c4
			 0x46301e87, /// 0x1c8
			 0x8c01e329, /// 0x1cc
			 0xf0180331, /// 0x1d0
			 0x62ba276f, /// 0x1d4
			 0xb17474a8, /// 0x1d8
			 0xaf9fcc40, /// 0x1dc
			 0xd8405e53, /// 0x1e0
			 0x840ae4e6, /// 0x1e4
			 0x7bf7a9ee, /// 0x1e8
			 0x7afb8d12, /// 0x1ec
			 0x9b39d257, /// 0x1f0
			 0xdb873f4b, /// 0x1f4
			 0x0d71c8b4, /// 0x1f8
			 0xfddc8ca5, /// 0x1fc
			 0x23361b2f, /// 0x200
			 0x30689f7a, /// 0x204
			 0x20b1c03a, /// 0x208
			 0x262d8141, /// 0x20c
			 0x2672c60a, /// 0x210
			 0xc4665799, /// 0x214
			 0xf8dd2e7b, /// 0x218
			 0xb5a830b3, /// 0x21c
			 0x91ca714b, /// 0x220
			 0xff0db533, /// 0x224
			 0x9e8a8ca1, /// 0x228
			 0xce878e65, /// 0x22c
			 0x688ac93f, /// 0x230
			 0xd178263d, /// 0x234
			 0xe7354b07, /// 0x238
			 0xa431ccb2, /// 0x23c
			 0x1d2ab6a1, /// 0x240
			 0x7f1b5d5c, /// 0x244
			 0x65cd842e, /// 0x248
			 0x413892a2, /// 0x24c
			 0x6a80578e, /// 0x250
			 0xa88d5f3c, /// 0x254
			 0xeab5319c, /// 0x258
			 0xc7e627da, /// 0x25c
			 0xb7c949b9, /// 0x260
			 0xf9a8becf, /// 0x264
			 0x59c1673a, /// 0x268
			 0xf6b49a85, /// 0x26c
			 0x30c05542, /// 0x270
			 0x75b1c543, /// 0x274
			 0xd3fed529, /// 0x278
			 0x2329939d, /// 0x27c
			 0xb3ef071e, /// 0x280
			 0xfc7adc54, /// 0x284
			 0x0294dcc9, /// 0x288
			 0x2f7d7749, /// 0x28c
			 0xd98d07d3, /// 0x290
			 0x609b8822, /// 0x294
			 0xf0f55b44, /// 0x298
			 0x38a9b449, /// 0x29c
			 0xc4957cb5, /// 0x2a0
			 0xf49b3e02, /// 0x2a4
			 0x5b947f99, /// 0x2a8
			 0xc027df74, /// 0x2ac
			 0xd15cfce5, /// 0x2b0
			 0xcf412d5f, /// 0x2b4
			 0x5d397063, /// 0x2b8
			 0x7deed0f8, /// 0x2bc
			 0x899b0ca8, /// 0x2c0
			 0x0a1ae10f, /// 0x2c4
			 0xf15a65bd, /// 0x2c8
			 0x1c86e9cf, /// 0x2cc
			 0x6eeda795, /// 0x2d0
			 0x0e950f2d, /// 0x2d4
			 0x1f489602, /// 0x2d8
			 0x3c9fc7e4, /// 0x2dc
			 0x9aae49e3, /// 0x2e0
			 0xa175c5f4, /// 0x2e4
			 0xf50b2f65, /// 0x2e8
			 0xb8e21162, /// 0x2ec
			 0x58fc6916, /// 0x2f0
			 0x287b1555, /// 0x2f4
			 0x67eab4d3, /// 0x2f8
			 0x1a0d9e51, /// 0x2fc
			 0xd9d7b345, /// 0x300
			 0xdde89ba9, /// 0x304
			 0x7f62d1aa, /// 0x308
			 0x3f6708e1, /// 0x30c
			 0x430193f9, /// 0x310
			 0xe5729d5c, /// 0x314
			 0x5c86dc95, /// 0x318
			 0x2744309c, /// 0x31c
			 0x72d92bab, /// 0x320
			 0x8e5fc868, /// 0x324
			 0xef1c49ed, /// 0x328
			 0xed39c37c, /// 0x32c
			 0x2f90d394, /// 0x330
			 0xe4870d40, /// 0x334
			 0x97537f40, /// 0x338
			 0x7035e79d, /// 0x33c
			 0xf533e0a1, /// 0x340
			 0x92d0aa65, /// 0x344
			 0xdf3710f1, /// 0x348
			 0x3b275c01, /// 0x34c
			 0x608c00e0, /// 0x350
			 0xd8ae8f26, /// 0x354
			 0xd22e317c, /// 0x358
			 0x01d629fd, /// 0x35c
			 0x28b1b5d0, /// 0x360
			 0x538c60eb, /// 0x364
			 0x6c0b6dd1, /// 0x368
			 0x29ca54dd, /// 0x36c
			 0xd416f0c5, /// 0x370
			 0x7fc79f24, /// 0x374
			 0xba048be8, /// 0x378
			 0xc66d766b, /// 0x37c
			 0x04929d01, /// 0x380
			 0xef60338f, /// 0x384
			 0x1fef3220, /// 0x388
			 0xf7f3cdda, /// 0x38c
			 0x425a87e8, /// 0x390
			 0x54c81fb5, /// 0x394
			 0xe39a2591, /// 0x398
			 0xe32c4bf9, /// 0x39c
			 0xfc4eea9d, /// 0x3a0
			 0xed7ebcc2, /// 0x3a4
			 0x52f0b0eb, /// 0x3a8
			 0x3b28f78b, /// 0x3ac
			 0x6239bdb9, /// 0x3b0
			 0x492fc81f, /// 0x3b4
			 0x924e5805, /// 0x3b8
			 0x501f0559, /// 0x3bc
			 0x5e733cbe, /// 0x3c0
			 0x368ebaee, /// 0x3c4
			 0xf29cd02a, /// 0x3c8
			 0xc37aa109, /// 0x3cc
			 0x0f0a58b8, /// 0x3d0
			 0xfdb8febd, /// 0x3d4
			 0x95d6f666, /// 0x3d8
			 0x0c54ffba, /// 0x3dc
			 0x95899e95, /// 0x3e0
			 0x17011cbe, /// 0x3e4
			 0xb6393a67, /// 0x3e8
			 0x33d9d5c1, /// 0x3ec
			 0x97b68b55, /// 0x3f0
			 0xa96bd381, /// 0x3f4
			 0xec9ccd21, /// 0x3f8
			 0xc3f9e904, /// 0x3fc
			 0x03f0b134, /// 0x400
			 0xc556c952, /// 0x404
			 0xbcb4b64a, /// 0x408
			 0xd32a541c, /// 0x40c
			 0x90a0ca0d, /// 0x410
			 0xf03c753b, /// 0x414
			 0x213fbb98, /// 0x418
			 0xee20c2d0, /// 0x41c
			 0x4cf45d24, /// 0x420
			 0x4b3721bc, /// 0x424
			 0x76b81458, /// 0x428
			 0x7cad6a4e, /// 0x42c
			 0x7f08dced, /// 0x430
			 0xd3ba36e4, /// 0x434
			 0x4802bee7, /// 0x438
			 0x379c414f, /// 0x43c
			 0xaae36ec6, /// 0x440
			 0x76dff51a, /// 0x444
			 0x821a27e2, /// 0x448
			 0x4683bbd7, /// 0x44c
			 0x92fe69cf, /// 0x450
			 0xfd038334, /// 0x454
			 0x1a9e12df, /// 0x458
			 0xb1190fec, /// 0x45c
			 0x8ad561e9, /// 0x460
			 0x21754cea, /// 0x464
			 0x68892548, /// 0x468
			 0xf6643af4, /// 0x46c
			 0xcd22d342, /// 0x470
			 0x371595db, /// 0x474
			 0x59a70e1c, /// 0x478
			 0x9edfe924, /// 0x47c
			 0x159a2094, /// 0x480
			 0x43a92c66, /// 0x484
			 0xa30c955b, /// 0x488
			 0xaf9422c7, /// 0x48c
			 0x9c9a5247, /// 0x490
			 0xce46288d, /// 0x494
			 0xebce8e44, /// 0x498
			 0x61c9ef0b, /// 0x49c
			 0x7c0e51fd, /// 0x4a0
			 0xd9f4a40b, /// 0x4a4
			 0xd2be7a3f, /// 0x4a8
			 0xb0a4909b, /// 0x4ac
			 0x62ac0dcc, /// 0x4b0
			 0x4409aeef, /// 0x4b4
			 0xb1f5d63b, /// 0x4b8
			 0xf994ba5f, /// 0x4bc
			 0x39dbf905, /// 0x4c0
			 0xc039b1f2, /// 0x4c4
			 0x28530294, /// 0x4c8
			 0xb860b2cd, /// 0x4cc
			 0x7b186b4f, /// 0x4d0
			 0x3486d0a3, /// 0x4d4
			 0x46332b04, /// 0x4d8
			 0x5ce0acd0, /// 0x4dc
			 0x70796ad2, /// 0x4e0
			 0xe7c00cb1, /// 0x4e4
			 0x916ceb07, /// 0x4e8
			 0x07d60c0c, /// 0x4ec
			 0x204fb383, /// 0x4f0
			 0x90efeb83, /// 0x4f4
			 0xb3f826ec, /// 0x4f8
			 0x64376d60, /// 0x4fc
			 0xd9915f81, /// 0x500
			 0x8ec01b70, /// 0x504
			 0xa9a3c412, /// 0x508
			 0x885fa1ca, /// 0x50c
			 0xf726e30d, /// 0x510
			 0x0d411d94, /// 0x514
			 0x1e50fd3c, /// 0x518
			 0x26b7e7be, /// 0x51c
			 0x58ec9558, /// 0x520
			 0x5140462a, /// 0x524
			 0x205b27c9, /// 0x528
			 0x34a35817, /// 0x52c
			 0xb2eb7558, /// 0x530
			 0xd881c107, /// 0x534
			 0x248a7314, /// 0x538
			 0xb3a9920f, /// 0x53c
			 0x6efea984, /// 0x540
			 0xac67342a, /// 0x544
			 0xa152c4ea, /// 0x548
			 0xee4aaed6, /// 0x54c
			 0x6e02005a, /// 0x550
			 0x571989f5, /// 0x554
			 0x5560f49e, /// 0x558
			 0xb49acea8, /// 0x55c
			 0x38f9fc0f, /// 0x560
			 0x4efb7726, /// 0x564
			 0x6008c02b, /// 0x568
			 0x57227a1f, /// 0x56c
			 0xab479b0a, /// 0x570
			 0xed101eab, /// 0x574
			 0x824da71c, /// 0x578
			 0x334a7d56, /// 0x57c
			 0x4e7f1550, /// 0x580
			 0x1abe4246, /// 0x584
			 0x428244de, /// 0x588
			 0x8599b811, /// 0x58c
			 0x3fd13f69, /// 0x590
			 0x70011980, /// 0x594
			 0xd2c7801f, /// 0x598
			 0x3010f98d, /// 0x59c
			 0x86130f1d, /// 0x5a0
			 0x431cdc9f, /// 0x5a4
			 0xe85b05a4, /// 0x5a8
			 0x7d8b8256, /// 0x5ac
			 0xa847f148, /// 0x5b0
			 0x7a68fc9b, /// 0x5b4
			 0xc0fcd718, /// 0x5b8
			 0xebec51f9, /// 0x5bc
			 0x5f7b0235, /// 0x5c0
			 0x7b819af2, /// 0x5c4
			 0x3ae40a9e, /// 0x5c8
			 0xb6d0e3f2, /// 0x5cc
			 0x1c485d1a, /// 0x5d0
			 0x6c03f736, /// 0x5d4
			 0x1f937d1c, /// 0x5d8
			 0x64b1043f, /// 0x5dc
			 0xc438deac, /// 0x5e0
			 0xe9e8659e, /// 0x5e4
			 0x98b46e57, /// 0x5e8
			 0x04d95131, /// 0x5ec
			 0x42aa50ef, /// 0x5f0
			 0xf7199ed3, /// 0x5f4
			 0xba153a51, /// 0x5f8
			 0xab68f5d1, /// 0x5fc
			 0x67e7f527, /// 0x600
			 0x1f522d24, /// 0x604
			 0x8417a3d4, /// 0x608
			 0x3e41306c, /// 0x60c
			 0xdcf1627a, /// 0x610
			 0x8c851a5a, /// 0x614
			 0x91359dba, /// 0x618
			 0x7b61b259, /// 0x61c
			 0xdb5a6777, /// 0x620
			 0x3f8aee32, /// 0x624
			 0x3ff6a582, /// 0x628
			 0x1ac404d8, /// 0x62c
			 0x37fad146, /// 0x630
			 0x0f4b0d79, /// 0x634
			 0xdf81f77a, /// 0x638
			 0xbc4bbe09, /// 0x63c
			 0xbd296324, /// 0x640
			 0x69e32df8, /// 0x644
			 0xc1049e5a, /// 0x648
			 0xc777c2de, /// 0x64c
			 0xbf787380, /// 0x650
			 0xe4beda01, /// 0x654
			 0x3320855a, /// 0x658
			 0x08b21d08, /// 0x65c
			 0x60dff01b, /// 0x660
			 0x5b81326a, /// 0x664
			 0x9c8f50ca, /// 0x668
			 0x292454c0, /// 0x66c
			 0x1c67b2d0, /// 0x670
			 0x89629da2, /// 0x674
			 0x82927c39, /// 0x678
			 0x2c9ec8e6, /// 0x67c
			 0x0e13c93f, /// 0x680
			 0x9464815f, /// 0x684
			 0xc7bed352, /// 0x688
			 0x257a7b8a, /// 0x68c
			 0x8f3125f8, /// 0x690
			 0x79096ef4, /// 0x694
			 0x11447be1, /// 0x698
			 0xe4c99f02, /// 0x69c
			 0x95038ebb, /// 0x6a0
			 0x01393b3d, /// 0x6a4
			 0xff398a00, /// 0x6a8
			 0xc5822dc6, /// 0x6ac
			 0xef8d4630, /// 0x6b0
			 0xc300d069, /// 0x6b4
			 0x402cb641, /// 0x6b8
			 0xf95f8cbe, /// 0x6bc
			 0xea0ea322, /// 0x6c0
			 0x4487f77c, /// 0x6c4
			 0x4b9aa169, /// 0x6c8
			 0x5ab8e74d, /// 0x6cc
			 0x5f045919, /// 0x6d0
			 0x9588307e, /// 0x6d4
			 0x0b479f84, /// 0x6d8
			 0xbf7f6cba, /// 0x6dc
			 0x6c1cab1a, /// 0x6e0
			 0x8485c418, /// 0x6e4
			 0x098cac9f, /// 0x6e8
			 0xe88bf497, /// 0x6ec
			 0xc58debb9, /// 0x6f0
			 0xc3b1d63a, /// 0x6f4
			 0x9e25b525, /// 0x6f8
			 0x0eb50683, /// 0x6fc
			 0x5bd6a0da, /// 0x700
			 0x49294ea3, /// 0x704
			 0x8e16b00a, /// 0x708
			 0x0b72c4fa, /// 0x70c
			 0x85e66ad5, /// 0x710
			 0x6ed02640, /// 0x714
			 0x6b93ce2f, /// 0x718
			 0x5d0ea154, /// 0x71c
			 0xf9e78133, /// 0x720
			 0x8488a9a5, /// 0x724
			 0x2712c825, /// 0x728
			 0x074f7da6, /// 0x72c
			 0x7078c29c, /// 0x730
			 0x97601430, /// 0x734
			 0xbf7bbed6, /// 0x738
			 0x06ecd9ed, /// 0x73c
			 0x60b43082, /// 0x740
			 0xc0565c3a, /// 0x744
			 0x337cf8f5, /// 0x748
			 0xb89c855a, /// 0x74c
			 0x93155877, /// 0x750
			 0xa3669afe, /// 0x754
			 0xa5557dec, /// 0x758
			 0x4252b347, /// 0x75c
			 0x3b741061, /// 0x760
			 0xd4b40a47, /// 0x764
			 0x372f7523, /// 0x768
			 0x4cb36866, /// 0x76c
			 0xf8cabdde, /// 0x770
			 0xdadda70f, /// 0x774
			 0x9c260987, /// 0x778
			 0x51f0037a, /// 0x77c
			 0xead2c727, /// 0x780
			 0x10445562, /// 0x784
			 0xb507acaf, /// 0x788
			 0x17921a9b, /// 0x78c
			 0x1db9fdca, /// 0x790
			 0xa584eb96, /// 0x794
			 0x8812b153, /// 0x798
			 0x1bb4dba6, /// 0x79c
			 0xeb97d1e2, /// 0x7a0
			 0xbf9e2dc8, /// 0x7a4
			 0x254c66de, /// 0x7a8
			 0xf07f8594, /// 0x7ac
			 0xab49b865, /// 0x7b0
			 0x4645396e, /// 0x7b4
			 0x75d6cf52, /// 0x7b8
			 0x2d5aa954, /// 0x7bc
			 0x2399c778, /// 0x7c0
			 0x2138a252, /// 0x7c4
			 0x8980a624, /// 0x7c8
			 0x2b5a2486, /// 0x7cc
			 0x421daca2, /// 0x7d0
			 0x9bbad5a9, /// 0x7d4
			 0xf9839da7, /// 0x7d8
			 0xa467ea0a, /// 0x7dc
			 0xdb47a47c, /// 0x7e0
			 0x98678cd0, /// 0x7e4
			 0x50a0843f, /// 0x7e8
			 0x0b7f57ac, /// 0x7ec
			 0x56efde08, /// 0x7f0
			 0x453590a9, /// 0x7f4
			 0x7e224fd2, /// 0x7f8
			 0x3fc6718e, /// 0x7fc
			 0xd91db125, /// 0x800
			 0x565991f8, /// 0x804
			 0x21bdc333, /// 0x808
			 0x5a63f49a, /// 0x80c
			 0x6adb0c0b, /// 0x810
			 0x16a8ed5a, /// 0x814
			 0x808c1025, /// 0x818
			 0x3b397cad, /// 0x81c
			 0x4237cce1, /// 0x820
			 0x8506305c, /// 0x824
			 0x709ca4d4, /// 0x828
			 0x1f9a59c8, /// 0x82c
			 0x235a1445, /// 0x830
			 0x2c37dd1b, /// 0x834
			 0x329f7c0b, /// 0x838
			 0xae991eab, /// 0x83c
			 0x0e2918cf, /// 0x840
			 0x10ae6645, /// 0x844
			 0xbefe7303, /// 0x848
			 0xe6fa7bff, /// 0x84c
			 0xe572a6b4, /// 0x850
			 0x055f610c, /// 0x854
			 0x9ded3b5f, /// 0x858
			 0xfd96a04d, /// 0x85c
			 0xc214e5b7, /// 0x860
			 0x1051cade, /// 0x864
			 0x6ba2e02f, /// 0x868
			 0x8e444576, /// 0x86c
			 0xbafb39de, /// 0x870
			 0x8c6824da, /// 0x874
			 0x0715f1fb, /// 0x878
			 0x4aee8bff, /// 0x87c
			 0x40d24ad9, /// 0x880
			 0xdc81a02f, /// 0x884
			 0x13dcf7ff, /// 0x888
			 0x07c35d9d, /// 0x88c
			 0x1f07140a, /// 0x890
			 0x1658a89d, /// 0x894
			 0x586ffed3, /// 0x898
			 0x2d03163d, /// 0x89c
			 0x97cd637e, /// 0x8a0
			 0x8be0496f, /// 0x8a4
			 0x01c38f4d, /// 0x8a8
			 0x19f7a297, /// 0x8ac
			 0xad6167f1, /// 0x8b0
			 0x16698ec2, /// 0x8b4
			 0x7bc7c890, /// 0x8b8
			 0x7ff96a87, /// 0x8bc
			 0x54fa3e7b, /// 0x8c0
			 0x82f1ef5f, /// 0x8c4
			 0xe30ecc67, /// 0x8c8
			 0x7d0560b1, /// 0x8cc
			 0x19a9f637, /// 0x8d0
			 0x5ab55445, /// 0x8d4
			 0xe9cb9d75, /// 0x8d8
			 0x746d60a3, /// 0x8dc
			 0x5500b78a, /// 0x8e0
			 0x52864174, /// 0x8e4
			 0xbb72d81f, /// 0x8e8
			 0x64024e1b, /// 0x8ec
			 0x4a5798ab, /// 0x8f0
			 0x1e5b19a0, /// 0x8f4
			 0xc3703889, /// 0x8f8
			 0x4c21a181, /// 0x8fc
			 0x0819556e, /// 0x900
			 0x3bdc41c2, /// 0x904
			 0xee93be9e, /// 0x908
			 0x50837047, /// 0x90c
			 0x7ad99034, /// 0x910
			 0xfc27bf5f, /// 0x914
			 0x443695ee, /// 0x918
			 0x700bbcaf, /// 0x91c
			 0x5dc3f407, /// 0x920
			 0x7a1d4a39, /// 0x924
			 0x58ee0afe, /// 0x928
			 0x5078bf18, /// 0x92c
			 0xbdce80e0, /// 0x930
			 0x55c6da13, /// 0x934
			 0xeaac0c00, /// 0x938
			 0x72a10731, /// 0x93c
			 0xc21b718b, /// 0x940
			 0x916bfdeb, /// 0x944
			 0xaee5192b, /// 0x948
			 0x3eed1831, /// 0x94c
			 0xe4ca951e, /// 0x950
			 0xb1c59c22, /// 0x954
			 0xa9cbd863, /// 0x958
			 0x650526fd, /// 0x95c
			 0xd9b75e5b, /// 0x960
			 0x3de1a21d, /// 0x964
			 0x1ffeef1b, /// 0x968
			 0x760e7b76, /// 0x96c
			 0xee1468b6, /// 0x970
			 0xdbef812d, /// 0x974
			 0xefdbfa1b, /// 0x978
			 0xbc1b6219, /// 0x97c
			 0xa1b0a97c, /// 0x980
			 0xff9a0ce0, /// 0x984
			 0xab5e4952, /// 0x988
			 0xea092bbd, /// 0x98c
			 0xf8b7cea8, /// 0x990
			 0xb2b0c4b7, /// 0x994
			 0x57819498, /// 0x998
			 0xe4e76a8a, /// 0x99c
			 0xddda224f, /// 0x9a0
			 0xa5b0c45c, /// 0x9a4
			 0xfb4cd0a7, /// 0x9a8
			 0x96e47d58, /// 0x9ac
			 0xeecffde9, /// 0x9b0
			 0xe422774a, /// 0x9b4
			 0x821aad29, /// 0x9b8
			 0x4193ee5a, /// 0x9bc
			 0x17bae25a, /// 0x9c0
			 0x56385998, /// 0x9c4
			 0xa02dbca8, /// 0x9c8
			 0xab3a293a, /// 0x9cc
			 0x9d983b3d, /// 0x9d0
			 0x17fa49a6, /// 0x9d4
			 0x78c744a0, /// 0x9d8
			 0x107fefd3, /// 0x9dc
			 0x14b02460, /// 0x9e0
			 0xfe648b15, /// 0x9e4
			 0xd16eaf41, /// 0x9e8
			 0x1298bf08, /// 0x9ec
			 0xd6316bda, /// 0x9f0
			 0x1cf46374, /// 0x9f4
			 0xb91a2b71, /// 0x9f8
			 0x7a29fa69, /// 0x9fc
			 0xf25e3293, /// 0xa00
			 0xf84a6d47, /// 0xa04
			 0xcfdbe6ee, /// 0xa08
			 0x4ef20d6c, /// 0xa0c
			 0xb44c200b, /// 0xa10
			 0xb47f1492, /// 0xa14
			 0xdf919475, /// 0xa18
			 0xbf29061a, /// 0xa1c
			 0x41017be8, /// 0xa20
			 0xd8c4c298, /// 0xa24
			 0xc97630cc, /// 0xa28
			 0xfc480315, /// 0xa2c
			 0xf3d07790, /// 0xa30
			 0x925b01ba, /// 0xa34
			 0xdd119eeb, /// 0xa38
			 0xd4e87f5b, /// 0xa3c
			 0x4eed392e, /// 0xa40
			 0x1519a830, /// 0xa44
			 0xa22191ef, /// 0xa48
			 0xbcc479a2, /// 0xa4c
			 0x5cf21558, /// 0xa50
			 0x8ed788bd, /// 0xa54
			 0x431f2df3, /// 0xa58
			 0xf366d43e, /// 0xa5c
			 0xf0471940, /// 0xa60
			 0xe2dacb39, /// 0xa64
			 0x4687bab2, /// 0xa68
			 0x61835858, /// 0xa6c
			 0x54105f0d, /// 0xa70
			 0x97d7e19f, /// 0xa74
			 0x7ef7fc03, /// 0xa78
			 0x7a4551ce, /// 0xa7c
			 0x55f49874, /// 0xa80
			 0x6545ac28, /// 0xa84
			 0x96e4bcd0, /// 0xa88
			 0x936a4d36, /// 0xa8c
			 0x0597625e, /// 0xa90
			 0x55beddf3, /// 0xa94
			 0xfbc12b02, /// 0xa98
			 0x945ecb5d, /// 0xa9c
			 0xc09b6988, /// 0xaa0
			 0xa976ee59, /// 0xaa4
			 0x0c7914de, /// 0xaa8
			 0x0d825d37, /// 0xaac
			 0xcfa37eda, /// 0xab0
			 0x22e628ce, /// 0xab4
			 0x32f037df, /// 0xab8
			 0x2e29166a, /// 0xabc
			 0xb7b688f5, /// 0xac0
			 0xb6541283, /// 0xac4
			 0xce0a2b8b, /// 0xac8
			 0xe3c71d34, /// 0xacc
			 0x7f4d753e, /// 0xad0
			 0xf21ca9ab, /// 0xad4
			 0x179c2127, /// 0xad8
			 0x8de4f316, /// 0xadc
			 0xb1cc5b21, /// 0xae0
			 0x579affc2, /// 0xae4
			 0xfe1180e0, /// 0xae8
			 0xa1fa200c, /// 0xaec
			 0x15f19c81, /// 0xaf0
			 0x5cf796fb, /// 0xaf4
			 0x8f70f33f, /// 0xaf8
			 0xeac77459, /// 0xafc
			 0x3fcc6d69, /// 0xb00
			 0xb70a9713, /// 0xb04
			 0x0abb396a, /// 0xb08
			 0x2da8c9e9, /// 0xb0c
			 0x3b1ee77d, /// 0xb10
			 0xb5f25ab9, /// 0xb14
			 0x4a732c02, /// 0xb18
			 0x6446d83b, /// 0xb1c
			 0xdeb18c0a, /// 0xb20
			 0xcd762ad5, /// 0xb24
			 0x9101960f, /// 0xb28
			 0x51ba4037, /// 0xb2c
			 0x2002ffb7, /// 0xb30
			 0x283a5270, /// 0xb34
			 0x698861cd, /// 0xb38
			 0x19d6effa, /// 0xb3c
			 0xfedc03b2, /// 0xb40
			 0xfc954218, /// 0xb44
			 0xd12c8f78, /// 0xb48
			 0x88be52ff, /// 0xb4c
			 0xe92639b9, /// 0xb50
			 0xefd6216d, /// 0xb54
			 0x68655c8d, /// 0xb58
			 0x1225b274, /// 0xb5c
			 0xc43d00d2, /// 0xb60
			 0x480240db, /// 0xb64
			 0x8d6f0ac6, /// 0xb68
			 0xd3f32bae, /// 0xb6c
			 0x02796b77, /// 0xb70
			 0xe2b071a4, /// 0xb74
			 0x2c4fcba1, /// 0xb78
			 0x59d961d6, /// 0xb7c
			 0x02959402, /// 0xb80
			 0x9555ee9c, /// 0xb84
			 0x8dbe507b, /// 0xb88
			 0xba9e6a82, /// 0xb8c
			 0x321dfa1d, /// 0xb90
			 0x0096be74, /// 0xb94
			 0xad57332e, /// 0xb98
			 0xe172e200, /// 0xb9c
			 0xe673e13f, /// 0xba0
			 0x295b073f, /// 0xba4
			 0x38813c62, /// 0xba8
			 0x82467a27, /// 0xbac
			 0x2a9af349, /// 0xbb0
			 0x3d18ed75, /// 0xbb4
			 0x56c1135a, /// 0xbb8
			 0x87e1d277, /// 0xbbc
			 0xfb150e46, /// 0xbc0
			 0xe67d4a9a, /// 0xbc4
			 0x8a79451d, /// 0xbc8
			 0x57c7edf3, /// 0xbcc
			 0x031a7223, /// 0xbd0
			 0x2f1f3e5b, /// 0xbd4
			 0x06a71f06, /// 0xbd8
			 0x8cee25c8, /// 0xbdc
			 0x74726b79, /// 0xbe0
			 0xd9c560f4, /// 0xbe4
			 0xb347a799, /// 0xbe8
			 0x3dd4314f, /// 0xbec
			 0xb381ce4d, /// 0xbf0
			 0xc4947002, /// 0xbf4
			 0x8c33d7d8, /// 0xbf8
			 0x8897502c, /// 0xbfc
			 0x9f903ca2, /// 0xc00
			 0x32ff2a18, /// 0xc04
			 0x069abddc, /// 0xc08
			 0xb497df4b, /// 0xc0c
			 0xcb66e8bc, /// 0xc10
			 0x8216c62f, /// 0xc14
			 0x07705fe4, /// 0xc18
			 0x875ad1ff, /// 0xc1c
			 0x50ec1f60, /// 0xc20
			 0x6889eb38, /// 0xc24
			 0xaeffbf6b, /// 0xc28
			 0xeeadb608, /// 0xc2c
			 0x27128b8d, /// 0xc30
			 0x3b104d60, /// 0xc34
			 0xa77834de, /// 0xc38
			 0x850674e2, /// 0xc3c
			 0xe2a53f0c, /// 0xc40
			 0x91ce55d9, /// 0xc44
			 0x535e4466, /// 0xc48
			 0xc68ecb1b, /// 0xc4c
			 0x8e90219c, /// 0xc50
			 0x15c44b99, /// 0xc54
			 0x0e1a6311, /// 0xc58
			 0x945f1a21, /// 0xc5c
			 0x03611bbd, /// 0xc60
			 0xf4628a41, /// 0xc64
			 0x63421763, /// 0xc68
			 0x4e80a2d9, /// 0xc6c
			 0x1f074634, /// 0xc70
			 0xe01b418c, /// 0xc74
			 0x49ec2cb7, /// 0xc78
			 0x73823674, /// 0xc7c
			 0x2d4397ab, /// 0xc80
			 0xe6c1e41b, /// 0xc84
			 0x768e51fd, /// 0xc88
			 0x1cc316d6, /// 0xc8c
			 0xbebd653e, /// 0xc90
			 0xfa7a1086, /// 0xc94
			 0x0ffc6484, /// 0xc98
			 0x9fb7bcec, /// 0xc9c
			 0x9ad4f29d, /// 0xca0
			 0x17e51a39, /// 0xca4
			 0xb4f1aeaf, /// 0xca8
			 0xa9456515, /// 0xcac
			 0x0e605079, /// 0xcb0
			 0x85a2d7fb, /// 0xcb4
			 0x805110e6, /// 0xcb8
			 0x38548e47, /// 0xcbc
			 0xa012984a, /// 0xcc0
			 0x3d9e0c35, /// 0xcc4
			 0x4674c70a, /// 0xcc8
			 0x9125436e, /// 0xccc
			 0x6e42fe39, /// 0xcd0
			 0x8ab9741d, /// 0xcd4
			 0xc2aee296, /// 0xcd8
			 0xfb9de7e9, /// 0xcdc
			 0xc743c291, /// 0xce0
			 0xc2d6b737, /// 0xce4
			 0x39940cdb, /// 0xce8
			 0xa5669ef3, /// 0xcec
			 0xcdbaff31, /// 0xcf0
			 0x08b7dadb, /// 0xcf4
			 0xe9e6feec, /// 0xcf8
			 0xba049cee, /// 0xcfc
			 0x0ead56d0, /// 0xd00
			 0xb2ae4932, /// 0xd04
			 0x6ecb017c, /// 0xd08
			 0x3bfba9b5, /// 0xd0c
			 0x29732c39, /// 0xd10
			 0x2b5d152e, /// 0xd14
			 0x7a5680da, /// 0xd18
			 0x0ffc24dd, /// 0xd1c
			 0x514467a4, /// 0xd20
			 0x8c1a7db3, /// 0xd24
			 0xc1a1d960, /// 0xd28
			 0x4b4b801a, /// 0xd2c
			 0x5b751f8d, /// 0xd30
			 0xc4079ff9, /// 0xd34
			 0x7f2d2b6c, /// 0xd38
			 0x52ec4f70, /// 0xd3c
			 0xe3323aa1, /// 0xd40
			 0x7dd12ee8, /// 0xd44
			 0x491a783b, /// 0xd48
			 0xf9ee3c6d, /// 0xd4c
			 0xf702e13e, /// 0xd50
			 0x309aeb3c, /// 0xd54
			 0x93cb6bfb, /// 0xd58
			 0x1d006489, /// 0xd5c
			 0x0558e4a1, /// 0xd60
			 0xda11b187, /// 0xd64
			 0x12af53da, /// 0xd68
			 0x01ce9f99, /// 0xd6c
			 0xdf7a599a, /// 0xd70
			 0x500f2f5f, /// 0xd74
			 0xbab131e8, /// 0xd78
			 0xf1f306da, /// 0xd7c
			 0x4d11e14d, /// 0xd80
			 0xbfb5116d, /// 0xd84
			 0x0b90eb84, /// 0xd88
			 0x5a01da6a, /// 0xd8c
			 0x5e4d3ca5, /// 0xd90
			 0x79a952fa, /// 0xd94
			 0x6190af53, /// 0xd98
			 0x15711d72, /// 0xd9c
			 0x63fa7e4d, /// 0xda0
			 0x2a08b0ed, /// 0xda4
			 0xddcaa4a1, /// 0xda8
			 0x498633f6, /// 0xdac
			 0x7330db3b, /// 0xdb0
			 0x4fb16d14, /// 0xdb4
			 0x42dbf6c3, /// 0xdb8
			 0x52726b09, /// 0xdbc
			 0x7a72976c, /// 0xdc0
			 0x0f770e1f, /// 0xdc4
			 0xb263da6a, /// 0xdc8
			 0xc57b714c, /// 0xdcc
			 0xcb6feabf, /// 0xdd0
			 0xbdb418df, /// 0xdd4
			 0x7e1125d4, /// 0xdd8
			 0xc3548457, /// 0xddc
			 0x7c0f430a, /// 0xde0
			 0xd4f5733f, /// 0xde4
			 0x4e57a15b, /// 0xde8
			 0xda309957, /// 0xdec
			 0x5435e487, /// 0xdf0
			 0x6556f1b5, /// 0xdf4
			 0xae575544, /// 0xdf8
			 0xb70b1abc, /// 0xdfc
			 0xae464a8b, /// 0xe00
			 0x29e2f4a0, /// 0xe04
			 0x2166b825, /// 0xe08
			 0xfa0fe3f9, /// 0xe0c
			 0x4a12d8b5, /// 0xe10
			 0x9d3ae5fc, /// 0xe14
			 0xf494c887, /// 0xe18
			 0x2cfaeeee, /// 0xe1c
			 0x9f3ef063, /// 0xe20
			 0xeebc7932, /// 0xe24
			 0x4a8dced0, /// 0xe28
			 0x977560fd, /// 0xe2c
			 0xb3587c78, /// 0xe30
			 0x5fa65346, /// 0xe34
			 0x7918672d, /// 0xe38
			 0x08097b00, /// 0xe3c
			 0xa28007f0, /// 0xe40
			 0xaf6ea2e1, /// 0xe44
			 0x6355e9b6, /// 0xe48
			 0x4cc63d35, /// 0xe4c
			 0x47b4f2de, /// 0xe50
			 0xead1de75, /// 0xe54
			 0xe84b8a3e, /// 0xe58
			 0xee2f717d, /// 0xe5c
			 0xde5351ac, /// 0xe60
			 0xfc2477b3, /// 0xe64
			 0x79996507, /// 0xe68
			 0x1197cf4c, /// 0xe6c
			 0x869d6f28, /// 0xe70
			 0xe7497e87, /// 0xe74
			 0x01e7b5da, /// 0xe78
			 0x9bf12188, /// 0xe7c
			 0x30f46dad, /// 0xe80
			 0xb9ff433a, /// 0xe84
			 0x9f5432c2, /// 0xe88
			 0xdd025ec6, /// 0xe8c
			 0x75e8932f, /// 0xe90
			 0x8f3fa0fd, /// 0xe94
			 0xe845ba8a, /// 0xe98
			 0x6190ed7e, /// 0xe9c
			 0xb0b4ed5b, /// 0xea0
			 0xf913005e, /// 0xea4
			 0x7af65bf9, /// 0xea8
			 0x89d7d04a, /// 0xeac
			 0x3ed1afa2, /// 0xeb0
			 0xbf4e61fc, /// 0xeb4
			 0x115e926b, /// 0xeb8
			 0x3b9513a6, /// 0xebc
			 0xc5229c55, /// 0xec0
			 0xbf07dcb8, /// 0xec4
			 0xdbce4ce9, /// 0xec8
			 0x2825a1b3, /// 0xecc
			 0x82da3da6, /// 0xed0
			 0xcf8cddae, /// 0xed4
			 0x1ec7cb50, /// 0xed8
			 0x2158418c, /// 0xedc
			 0xad27372c, /// 0xee0
			 0x81e473e4, /// 0xee4
			 0x3f9d0372, /// 0xee8
			 0x8f176581, /// 0xeec
			 0x4e1765df, /// 0xef0
			 0x1a055e4e, /// 0xef4
			 0x3cbc76bb, /// 0xef8
			 0x360638b3, /// 0xefc
			 0xa36c3f2b, /// 0xf00
			 0xf443d095, /// 0xf04
			 0x34211a0b, /// 0xf08
			 0x80c3a4a9, /// 0xf0c
			 0x888ecf5c, /// 0xf10
			 0x48258959, /// 0xf14
			 0x1f73b600, /// 0xf18
			 0xecf05b54, /// 0xf1c
			 0x12dfb401, /// 0xf20
			 0x06fba22c, /// 0xf24
			 0xe1c07d5e, /// 0xf28
			 0xd33ca73c, /// 0xf2c
			 0x62bb77e1, /// 0xf30
			 0x4b594b51, /// 0xf34
			 0x05a56144, /// 0xf38
			 0x83cebd12, /// 0xf3c
			 0x10132222, /// 0xf40
			 0x05c8a78e, /// 0xf44
			 0x0f1868cb, /// 0xf48
			 0x0338af4f, /// 0xf4c
			 0x115b7812, /// 0xf50
			 0x352356ae, /// 0xf54
			 0xedbffd05, /// 0xf58
			 0x91a92541, /// 0xf5c
			 0xdef2513d, /// 0xf60
			 0x249f0faf, /// 0xf64
			 0xbbdae09a, /// 0xf68
			 0xa8c608e0, /// 0xf6c
			 0x3cdd427b, /// 0xf70
			 0x27d3d960, /// 0xf74
			 0x9a5a2a9e, /// 0xf78
			 0x29e0568b, /// 0xf7c
			 0xec54dda2, /// 0xf80
			 0x989b25fa, /// 0xf84
			 0x786179de, /// 0xf88
			 0x92b96b31, /// 0xf8c
			 0x2b7e696e, /// 0xf90
			 0xe379b3fe, /// 0xf94
			 0xa60d0ceb, /// 0xf98
			 0x9f1ba4d3, /// 0xf9c
			 0x9af9b7a8, /// 0xfa0
			 0x0399dc53, /// 0xfa4
			 0xd2008bcb, /// 0xfa8
			 0x384a9d1d, /// 0xfac
			 0xca44f1b4, /// 0xfb0
			 0x6f592711, /// 0xfb4
			 0x1f93a7bd, /// 0xfb8
			 0x681bb7ea, /// 0xfbc
			 0x9fbee17f, /// 0xfc0
			 0x1add32c7, /// 0xfc4
			 0xb0366fcf, /// 0xfc8
			 0xbc2cea50, /// 0xfcc
			 0x1d8f56fe, /// 0xfd0
			 0x92460ea4, /// 0xfd4
			 0x93b7588c, /// 0xfd8
			 0xb337c207, /// 0xfdc
			 0x93bf4120, /// 0xfe0
			 0xef335dcb, /// 0xfe4
			 0x5de405f7, /// 0xfe8
			 0x800bd539, /// 0xfec
			 0xcfe1d917, /// 0xff0
			 0x9172832e, /// 0xff4
			 0x12a6fda1, /// 0xff8
			 0x119a9db4 /// last
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
			 0x00000164,
			 0x00000304,
			 0x000005a0,
			 0x000004f8,
			 0x00000574,
			 0x000000cc,
			 0x00000354,
			 0x000001e0,

			 /// vpu register f2
			 0x41600000,
			 0x41f80000,
			 0x40c00000,
			 0x41100000,
			 0x41c00000,
			 0x41b80000,
			 0x41b80000,
			 0x41000000,

			 /// vpu register f3
			 0x41500000,
			 0x41d00000,
			 0x41c80000,
			 0x41b00000,
			 0x41000000,
			 0x41600000,
			 0x41f80000,
			 0x3f800000,

			 /// vpu register f4
			 0x41880000,
			 0x41800000,
			 0x41900000,
			 0x41b80000,
			 0x40000000,
			 0x41e00000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f5
			 0x41900000,
			 0x41300000,
			 0x40a00000,
			 0x41e80000,
			 0x41000000,
			 0x41700000,
			 0x41200000,
			 0x41c00000,

			 /// vpu register f6
			 0x41900000,
			 0x41600000,
			 0x41700000,
			 0x41500000,
			 0x41600000,
			 0x41400000,
			 0x41d00000,
			 0x41400000,

			 /// vpu register f7
			 0x40c00000,
			 0x40a00000,
			 0x41500000,
			 0x41a00000,
			 0x41b00000,
			 0x42000000,
			 0x41a00000,
			 0x40000000,

			 /// vpu register f8
			 0x41600000,
			 0x41880000,
			 0x41300000,
			 0x41a80000,
			 0x40000000,
			 0x41900000,
			 0x41a00000,
			 0x40800000,

			 /// vpu register f9
			 0x41a80000,
			 0x41b80000,
			 0x41d80000,
			 0x41400000,
			 0x40c00000,
			 0x41000000,
			 0x41e80000,
			 0x41800000,

			 /// vpu register f10
			 0x41800000,
			 0x41880000,
			 0x41900000,
			 0x41e80000,
			 0x41600000,
			 0x41500000,
			 0x41700000,
			 0x40a00000,

			 /// vpu register f11
			 0x41500000,
			 0x41e80000,
			 0x41800000,
			 0x41f80000,
			 0x41b00000,
			 0x40800000,
			 0x41000000,
			 0x41c80000,

			 /// vpu register f12
			 0x41c00000,
			 0x41400000,
			 0x41d80000,
			 0x40400000,
			 0x41700000,
			 0x40400000,
			 0x41000000,
			 0x3f800000,

			 /// vpu register f13
			 0x41500000,
			 0x41a80000,
			 0x41100000,
			 0x41880000,
			 0x42000000,
			 0x41f80000,
			 0x41d80000,
			 0x41e80000,

			 /// vpu register f14
			 0x41700000,
			 0x41d80000,
			 0x40e00000,
			 0x41a80000,
			 0x3f800000,
			 0x41880000,
			 0x40c00000,
			 0x41b00000,

			 /// vpu register f15
			 0x41980000,
			 0x41b00000,
			 0x41800000,
			 0x41c00000,
			 0x41600000,
			 0x40400000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f16
			 0x41980000,
			 0x41700000,
			 0x41980000,
			 0x41980000,
			 0x40e00000,
			 0x42000000,
			 0x41980000,
			 0x41200000,

			 /// vpu register f17
			 0x3f800000,
			 0x40000000,
			 0x41f00000,
			 0x40c00000,
			 0x40c00000,
			 0x41a00000,
			 0x40a00000,
			 0x40e00000,

			 /// vpu register f18
			 0x41400000,
			 0x41880000,
			 0x41000000,
			 0x40400000,
			 0x40000000,
			 0x40e00000,
			 0x40000000,
			 0x41800000,

			 /// vpu register f19
			 0x41f80000,
			 0x40a00000,
			 0x41980000,
			 0x42000000,
			 0x40800000,
			 0x40400000,
			 0x41d00000,
			 0x41980000,

			 /// vpu register f20
			 0x41d00000,
			 0x40800000,
			 0x41c80000,
			 0x41f00000,
			 0x41a80000,
			 0x41200000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f21
			 0x41f00000,
			 0x41600000,
			 0x41800000,
			 0x41900000,
			 0x40c00000,
			 0x41a80000,
			 0x41a80000,
			 0x41000000,

			 /// vpu register f22
			 0x41d80000,
			 0x41000000,
			 0x41c00000,
			 0x41a80000,
			 0x41000000,
			 0x41d80000,
			 0x40e00000,
			 0x42000000,

			 /// vpu register f23
			 0x41400000,
			 0x41900000,
			 0x40c00000,
			 0x41100000,
			 0x41400000,
			 0x41a80000,
			 0x41d00000,
			 0x42000000,

			 /// vpu register f24
			 0x41d80000,
			 0x41000000,
			 0x41980000,
			 0x40c00000,
			 0x41800000,
			 0x42000000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f25
			 0x41f00000,
			 0x40400000,
			 0x41f00000,
			 0x41400000,
			 0x41e80000,
			 0x41100000,
			 0x41b80000,
			 0x41100000,

			 /// vpu register f26
			 0x41e80000,
			 0x40000000,
			 0x41000000,
			 0x41880000,
			 0x40e00000,
			 0x41f00000,
			 0x40c00000,
			 0x41980000,

			 /// vpu register f27
			 0x41b80000,
			 0x41900000,
			 0x41a00000,
			 0x41a00000,
			 0x41f80000,
			 0x41880000,
			 0x40c00000,
			 0x41500000,

			 /// vpu register f28
			 0x41b00000,
			 0x40c00000,
			 0x41000000,
			 0x41e80000,
			 0x41e80000,
			 0x3f800000,
			 0x41100000,
			 0x41b00000,

			 /// vpu register f29
			 0x40800000,
			 0x41a80000,
			 0x41d00000,
			 0x41f00000,
			 0x40c00000,
			 0x41900000,
			 0x40800000,
			 0x41a00000,

			 /// vpu register f30
			 0x3f800000,
			 0x40e00000,
			 0x41c80000,
			 0x40800000,
			 0x42000000,
			 0x41980000,
			 0x41b80000,
			 0x41900000,

			 /// vpu register f31
			 0x41980000,
			 0x41d80000,
			 0x41d80000,
			 0x41400000,
			 0x3f800000,
			 0x41f80000,
			 0x40000000,
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
		"frcp_fix.rast f19, f7, f18\n"                        ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f10, f14, f6\n"                        ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f27, f30, f13\n"                       ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f20, f15, f8\n"                        ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f8, f16, f24\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f18, f22\n"                       ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f8, f20, f27\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f19, f19\n"                       ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f26, f9, f25\n"                        ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f29, f13, f23\n"                       ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f19, f30, f3\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f3, f19, f1\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f11, f27, f0\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f27, f11, f1\n"                        ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f10, f20\n"                       ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f24, f16, f21\n"                       ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f19, f22, f21\n"                       ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f26, f12, f10\n"                       ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f10, f11, f23\n"                       ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f4, f12, f23\n"                        ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f29, f19\n"                       ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f19, f5, f24\n"                        ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f4, f7, f15\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f27, f18, f25\n"                       ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f14, f24, f20\n"                       ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f18, f29, f4\n"                        ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f24, f20, f3\n"                        ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f11, f9, f20\n"                        ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f24, f21, f16\n"                       ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f8, f12, f11\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f2, f3\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f24, f11, f15\n"                       ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f22, f20\n"                       ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f29, f17\n"                       ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f7, f16, f12\n"                        ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f20, f12, f0\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f20, f12, f19\n"                       ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f26, f13, f8\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f13, f10, f30\n"                       ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f11, f3, f16\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f18, f2, f23\n"                        ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f2, f0, f6\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f8, f3\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f9, f22, f30\n"                        ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f2, f19, f26\n"                        ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f16, f10, f29\n"                       ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f20, f27, f22\n"                       ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f20, f27, f21\n"                       ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f18, f1, f19\n"                        ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f4, f30, f12\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f20, f19, f13\n"                       ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f29, f17, f0\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f22, f25, f3\n"                        ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f8, f2\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f16, f23, f22\n"                       ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f17, f3, f20\n"                        ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f29, f6, f3\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f16, f29, f14\n"                       ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f3, f23, f21\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f5, f17, f16\n"                        ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f26, f12, f18\n"                       ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f6, f3\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f3, f1, f26\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f21, f1, f18\n"                        ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f6, f10\n"                        ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f13, f19, f6\n"                        ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f29, f2, f8\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f7, f27, f18\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f8, f18, f23\n"                        ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f18, f12, f2\n"                        ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f12, f29, f29\n"                       ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f27, f20, f2\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f17, f11, f6\n"                        ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f5, f28, f3\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f5, f23, f15\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f11, f7\n"                        ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f15, f29, f20\n"                       ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f6, f17, f13\n"                        ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f8, f26, f24\n"                        ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f28, f28, f26\n"                       ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f14, f11, f15\n"                       ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f19, f19, f0\n"                        ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f23, f12, f6\n"                        ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f4, f19, f0\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f5, f24, f9\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f23, f27, f16\n"                       ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f7, f10, f13\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f10, f16, f0\n"                        ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f29, f9, f19\n"                        ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f21, f25, f15\n"                       ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f25, f14, f6\n"                        ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f5, f25, f25\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f2, f5, f24\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f22, f10, f26\n"                       ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f6, f4, f25\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f22, f6, f7\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f8, f21, f7\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f18, f8, f27\n"                        ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"frcp_fix.rast f30, f4, f9\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
