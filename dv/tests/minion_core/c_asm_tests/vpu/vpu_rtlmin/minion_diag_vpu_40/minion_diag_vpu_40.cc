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
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00004
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x33333333,                                                  // 4 random values                             /// 0000c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00010
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0001c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00020
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00028
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0002c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00030
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00038
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0003c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00011111,                                                  // 9.7958E-41                                  /// 00054
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7fc00001,                                                  // signaling NaN                               /// 0005c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00060
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00064
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0006c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00070
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00074
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00078
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0007c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00080
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00088
			 0x00011111,                                                  // 9.7958E-41                                  /// 0008c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00090
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00094
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00098
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0009c
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x4b000000,                                                  // 8388608.0                                   /// 000a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000ec
			 0x4b000000,                                                  // 8388608.0                                   /// 000f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00100
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00104
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00108
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00110
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00118
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x33333333,                                                  // 4 random values                             /// 00124
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00130
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00140
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00144
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00150
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00158
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00160
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00168
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0016c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00178
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0017c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00180
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00190
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00194
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001cc
			 0xcb000000,                                                  // -8388608.0                                  /// 001d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001d8
			 0xff800000,                                                  // -inf                                        /// 001dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00200
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00204
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00208
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00218
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0021c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00220
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00224
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00228
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0022c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00238
			 0xffc00001,                                                  // -signaling NaN                              /// 0023c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00248
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00250
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00254
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00258
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0025c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00260
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00268
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0026c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00270
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00274
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00278
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0027c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00294
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002ac
			 0x4b000000,                                                  // 8388608.0                                   /// 002b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 002d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00304
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00310
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00314
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00318
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0031c
			 0x55555555,                                                  // 4 random values                             /// 00320
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00324
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00328
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0032c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00338
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0033c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00340
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0e000007,                                                  // Trailing 1s:                                /// 00348
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0034c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00350
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00354
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00358
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0035c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00360
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00364
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00368
			 0xffc00001,                                                  // -signaling NaN                              /// 0036c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00378
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00380
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00390
			 0xcb000000,                                                  // -8388608.0                                  /// 00394
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00398
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0039c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0xcb000000,                                                  // -8388608.0                                  /// 003a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000000,                                                  // -zero                                       /// 003c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00400
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00404
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00410
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0e000001,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0041c
			 0x3f028f5c,                                                  // 0.51                                        /// 00420
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00424
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00428
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0042c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00430
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00434
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0043c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00440
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x80011111,                                                  // -9.7958E-41                                 /// 00448
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0044c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00454
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00458
			 0x00000000,                                                  // zero                                        /// 0045c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0046c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00478
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0047c
			 0x80000000,                                                  // -zero                                       /// 00480
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00484
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00490
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00494
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00498
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000000,                                                  // -zero                                       /// 004a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d8
			 0xffc00001,                                                  // -signaling NaN                              /// 004dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00500
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00504
			 0x33333333,                                                  // 4 random values                             /// 00508
			 0x80011111,                                                  // -9.7958E-41                                 /// 0050c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x55555555,                                                  // 4 random values                             /// 00514
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00518
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00520
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0052c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00534
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00538
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00540
			 0x0c400000,                                                  // Leading 1s:                                 /// 00544
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0054c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00550
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80011111,                                                  // -9.7958E-41                                 /// 00558
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0055c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00564
			 0x00000000,                                                  // zero                                        /// 00568
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0056c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00570
			 0xff800000,                                                  // -inf                                        /// 00574
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0057c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00580
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00584
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00588
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0058c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00590
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00598
			 0x4b000000,                                                  // 8388608.0                                   /// 0059c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005bc
			 0xbf028f5c,                                                  // -0.51                                       /// 005c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005cc
			 0x80000000,                                                  // -zero                                       /// 005d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00604
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00608
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00610
			 0x55555555,                                                  // 4 random values                             /// 00614
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00620
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00624
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x3f028f5c,                                                  // 0.51                                        /// 00630
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0063c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00640
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00648
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0064c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00654
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00658
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0xcb000000,                                                  // -8388608.0                                  /// 0067c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00680
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00684
			 0xbf028f5c,                                                  // -0.51                                       /// 00688
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0068c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00690
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00698
			 0x0c400000,                                                  // Leading 1s:                                 /// 0069c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006b4
			 0x55555555,                                                  // 4 random values                             /// 006b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0xff800000,                                                  // -inf                                        /// 006c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00700
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00704
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00708
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00710
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00720
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00724
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00728
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000000,                                                  // -zero                                       /// 00734
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00738
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00744
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00748
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0074c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00750
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0c780000,                                                  // Leading 1s:                                 /// 00758
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00760
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0076c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00770
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00774
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00780
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00784
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00790
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0079c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007d4
			 0xcb000000,                                                  // -8388608.0                                  /// 007d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00800
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00804
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x7fc00001,                                                  // signaling NaN                               /// 00818
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00820
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00824
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00828
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0082c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00834
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00838
			 0xbf028f5c,                                                  // -0.51                                       /// 0083c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00840
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0084c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00850
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00858
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00860
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0086c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00870
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00874
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00878
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0087c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00880
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00884
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00888
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00890
			 0x80011111,                                                  // -9.7958E-41                                 /// 00894
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00898
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0089c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a4
			 0x80000000,                                                  // -zero                                       /// 008a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00900
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00904
			 0x55555555,                                                  // 4 random values                             /// 00908
			 0x0c600000,                                                  // Leading 1s:                                 /// 0090c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00910
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00918
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00920
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00928
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0092c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00934
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0xff800000,                                                  // -inf                                        /// 0093c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00940
			 0x7f800000,                                                  // inf                                         /// 00944
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00950
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00958
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0095c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00968
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00970
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00978
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0097c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00990
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00994
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00998
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c4
			 0x7fc00001,                                                  // signaling NaN                               /// 009c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x7f800000,                                                  // inf                                         /// 009f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a34
			 0x7fc00001,                                                  // signaling NaN                               /// 00a38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a48
			 0x55555555,                                                  // 4 random values                             /// 00a4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00000000,                                                  // zero                                        /// 00a74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aa0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aa4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ab8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00abc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac8
			 0x3f028f5c,                                                  // 0.51                                        /// 00acc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00afc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b74
			 0xffc00001,                                                  // -signaling NaN                              /// 00b78
			 0xffc00001,                                                  // -signaling NaN                              /// 00b7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ba0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ba4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bc4
			 0x7f800000,                                                  // inf                                         /// 00bc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00be4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00be8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c28
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c40
			 0xcb000000,                                                  // -8388608.0                                  /// 00c44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ca0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ca4
			 0xff800000,                                                  // -inf                                        /// 00ca8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ccc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ce4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cf4
			 0x7f800000,                                                  // inf                                         /// 00cf8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d08
			 0xffc00001,                                                  // -signaling NaN                              /// 00d0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00da0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd8
			 0x55555555,                                                  // 4 random values                             /// 00ddc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df4
			 0x00000000,                                                  // zero                                        /// 00df8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e84
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xbf028f5c,                                                  // -0.51                                       /// 00e94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ebc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00edc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ee8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f50
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x3f028f5c,                                                  // 0.51                                        /// 00f88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fe4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ff0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ff4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0e00000f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x4795ca42, /// 0x0
			 0xda3c9fe8, /// 0x4
			 0x48eb6560, /// 0x8
			 0x5fb63d32, /// 0xc
			 0xaca2b750, /// 0x10
			 0x6b46fca6, /// 0x14
			 0x07639c28, /// 0x18
			 0xad7c6e69, /// 0x1c
			 0x701712b5, /// 0x20
			 0x9ab25d72, /// 0x24
			 0xede8ff62, /// 0x28
			 0x0c797c4e, /// 0x2c
			 0xb9ff8c5a, /// 0x30
			 0xdc538c45, /// 0x34
			 0xa6d7b434, /// 0x38
			 0xdbcbc3a5, /// 0x3c
			 0x601781b7, /// 0x40
			 0x55076aef, /// 0x44
			 0xa251d312, /// 0x48
			 0x86db356d, /// 0x4c
			 0x00112754, /// 0x50
			 0xf95f6bd1, /// 0x54
			 0xf31c5843, /// 0x58
			 0xacbe11c2, /// 0x5c
			 0x86db5857, /// 0x60
			 0xaa3611fd, /// 0x64
			 0xba3a33de, /// 0x68
			 0x3a566e1e, /// 0x6c
			 0xc2c3bcbd, /// 0x70
			 0x7a081400, /// 0x74
			 0x53c7e17e, /// 0x78
			 0x0472f53d, /// 0x7c
			 0xcfd79aee, /// 0x80
			 0xbc8ddd78, /// 0x84
			 0x06132d3d, /// 0x88
			 0x66486621, /// 0x8c
			 0x8d73c1f8, /// 0x90
			 0x0447e66c, /// 0x94
			 0x91f14f0b, /// 0x98
			 0xebdea424, /// 0x9c
			 0x9668b993, /// 0xa0
			 0xad00d85c, /// 0xa4
			 0x485ccfea, /// 0xa8
			 0x84bab2c9, /// 0xac
			 0xae4cb565, /// 0xb0
			 0x29239355, /// 0xb4
			 0xa0e3e3d3, /// 0xb8
			 0xd1445d25, /// 0xbc
			 0xc4908325, /// 0xc0
			 0x8129f3b5, /// 0xc4
			 0x0ec77124, /// 0xc8
			 0x12e94242, /// 0xcc
			 0xfa3bd1c7, /// 0xd0
			 0x02834286, /// 0xd4
			 0xeef49302, /// 0xd8
			 0xb7c48233, /// 0xdc
			 0xd70e3912, /// 0xe0
			 0x34772933, /// 0xe4
			 0x49fd42ad, /// 0xe8
			 0x777c4788, /// 0xec
			 0xcacfd8af, /// 0xf0
			 0x34126a01, /// 0xf4
			 0x372ddbe4, /// 0xf8
			 0x1dc819df, /// 0xfc
			 0x933e6894, /// 0x100
			 0x15b7dc31, /// 0x104
			 0xd4c3d473, /// 0x108
			 0x5ca9afca, /// 0x10c
			 0x72f78563, /// 0x110
			 0xb327f57d, /// 0x114
			 0xa1bb1b13, /// 0x118
			 0xe758a41f, /// 0x11c
			 0x9f544d24, /// 0x120
			 0x4cd64a07, /// 0x124
			 0xe4969713, /// 0x128
			 0x3cbeab98, /// 0x12c
			 0x19b56bd7, /// 0x130
			 0x84fe7336, /// 0x134
			 0x73d8b16a, /// 0x138
			 0x88531584, /// 0x13c
			 0xe15ff509, /// 0x140
			 0x000e7847, /// 0x144
			 0x0e997571, /// 0x148
			 0x37a85feb, /// 0x14c
			 0xd5d3925e, /// 0x150
			 0x63132be7, /// 0x154
			 0xa2b52fe9, /// 0x158
			 0x5c5da1a1, /// 0x15c
			 0x0ccddc3c, /// 0x160
			 0x03275734, /// 0x164
			 0xa582742c, /// 0x168
			 0xe06b9bb0, /// 0x16c
			 0xadca8799, /// 0x170
			 0x074f5aee, /// 0x174
			 0xd3b5bf79, /// 0x178
			 0x9b89b194, /// 0x17c
			 0x9ea2f493, /// 0x180
			 0x5a99fe63, /// 0x184
			 0xc2810fa4, /// 0x188
			 0x0e70572d, /// 0x18c
			 0xda56ad64, /// 0x190
			 0x0080f308, /// 0x194
			 0x52225f0b, /// 0x198
			 0xd57c7bb1, /// 0x19c
			 0x06fa4ba5, /// 0x1a0
			 0x19daed4f, /// 0x1a4
			 0x15ac0bdc, /// 0x1a8
			 0xad6784f1, /// 0x1ac
			 0x1e17be6d, /// 0x1b0
			 0x3ddb5319, /// 0x1b4
			 0x5143dfef, /// 0x1b8
			 0x1e988d69, /// 0x1bc
			 0x8f96dbba, /// 0x1c0
			 0xdd31046b, /// 0x1c4
			 0xae6423a5, /// 0x1c8
			 0x5c1c806b, /// 0x1cc
			 0x41d43236, /// 0x1d0
			 0xa32fe6ef, /// 0x1d4
			 0x3c580b42, /// 0x1d8
			 0x97cc7a2a, /// 0x1dc
			 0xec4da20d, /// 0x1e0
			 0xcb04dbed, /// 0x1e4
			 0xcac0770e, /// 0x1e8
			 0xd5c80015, /// 0x1ec
			 0xa10d8535, /// 0x1f0
			 0x959207b2, /// 0x1f4
			 0x04269dfb, /// 0x1f8
			 0x1a8b40c9, /// 0x1fc
			 0x0350462c, /// 0x200
			 0xa83028ed, /// 0x204
			 0x198f5408, /// 0x208
			 0x92b4dd7c, /// 0x20c
			 0x9f113f8f, /// 0x210
			 0x0b8e5373, /// 0x214
			 0x2af1f542, /// 0x218
			 0x341c9b6f, /// 0x21c
			 0x5502c2ee, /// 0x220
			 0x43719820, /// 0x224
			 0x091e538e, /// 0x228
			 0x42bd0f6b, /// 0x22c
			 0x350f9b09, /// 0x230
			 0x32b791c1, /// 0x234
			 0x1c582111, /// 0x238
			 0xc104c48f, /// 0x23c
			 0x7ebe953f, /// 0x240
			 0x8415909a, /// 0x244
			 0x897593a4, /// 0x248
			 0xceaa39a8, /// 0x24c
			 0x18dd4824, /// 0x250
			 0x6a167790, /// 0x254
			 0x203173c7, /// 0x258
			 0x9eb9bf93, /// 0x25c
			 0x6838d7a2, /// 0x260
			 0x09660ad2, /// 0x264
			 0xccdb0c14, /// 0x268
			 0x98efaa27, /// 0x26c
			 0x06e6c958, /// 0x270
			 0xc93a27d8, /// 0x274
			 0x65aaee7b, /// 0x278
			 0x10dc6e9b, /// 0x27c
			 0x78b3bd01, /// 0x280
			 0xc20780d2, /// 0x284
			 0xa2450507, /// 0x288
			 0x5f686278, /// 0x28c
			 0xecf5dbc9, /// 0x290
			 0xa22bae07, /// 0x294
			 0x759cc5c5, /// 0x298
			 0x665bd360, /// 0x29c
			 0x746e528f, /// 0x2a0
			 0x64165242, /// 0x2a4
			 0x8f6c7d8b, /// 0x2a8
			 0xc53780fc, /// 0x2ac
			 0x4a023456, /// 0x2b0
			 0xb919fa10, /// 0x2b4
			 0x198c0d53, /// 0x2b8
			 0xd4949e94, /// 0x2bc
			 0xd48e5e68, /// 0x2c0
			 0x602509c6, /// 0x2c4
			 0x6ba6036b, /// 0x2c8
			 0x54d4f334, /// 0x2cc
			 0x56e7069a, /// 0x2d0
			 0xf50efaab, /// 0x2d4
			 0xe38ff838, /// 0x2d8
			 0xfbef5a81, /// 0x2dc
			 0x0ce36172, /// 0x2e0
			 0x5dd1f553, /// 0x2e4
			 0x65e86560, /// 0x2e8
			 0xa560ea70, /// 0x2ec
			 0x109a1f98, /// 0x2f0
			 0x85a6e1f5, /// 0x2f4
			 0x87093df2, /// 0x2f8
			 0x44facfb9, /// 0x2fc
			 0xecbdee46, /// 0x300
			 0x4636896a, /// 0x304
			 0x60b4aaa2, /// 0x308
			 0xc48366a6, /// 0x30c
			 0x057a1299, /// 0x310
			 0x93cc716f, /// 0x314
			 0xbfc6ea93, /// 0x318
			 0xc94ea56a, /// 0x31c
			 0x13d0cf8b, /// 0x320
			 0x2018fe4e, /// 0x324
			 0x1fb86e48, /// 0x328
			 0x47ff7e49, /// 0x32c
			 0xcb03cba6, /// 0x330
			 0x0e909b07, /// 0x334
			 0xb1bb06f0, /// 0x338
			 0x1dd4666a, /// 0x33c
			 0x2ac28fac, /// 0x340
			 0x4276af8d, /// 0x344
			 0xbba8ee80, /// 0x348
			 0x7342dfff, /// 0x34c
			 0xbad3bf49, /// 0x350
			 0xf2b680f3, /// 0x354
			 0x311155eb, /// 0x358
			 0xe1c399e9, /// 0x35c
			 0x377d55eb, /// 0x360
			 0x5b8960cf, /// 0x364
			 0x3cbd418e, /// 0x368
			 0x68e1e900, /// 0x36c
			 0x51525615, /// 0x370
			 0xd3908b5f, /// 0x374
			 0xc2e1b6ae, /// 0x378
			 0xb89d74bd, /// 0x37c
			 0x52411c64, /// 0x380
			 0x01c6dce8, /// 0x384
			 0x45880bcc, /// 0x388
			 0xb714c717, /// 0x38c
			 0xb8978b7c, /// 0x390
			 0x521f6338, /// 0x394
			 0x698f44b8, /// 0x398
			 0x7a380d81, /// 0x39c
			 0x856cde05, /// 0x3a0
			 0xbf2550d7, /// 0x3a4
			 0xf1464dae, /// 0x3a8
			 0x1c7a5a41, /// 0x3ac
			 0xdba3f06f, /// 0x3b0
			 0xe4e16e42, /// 0x3b4
			 0x6fdbb8a0, /// 0x3b8
			 0x839c0cc5, /// 0x3bc
			 0xa8066412, /// 0x3c0
			 0xf664806d, /// 0x3c4
			 0x0cae57ef, /// 0x3c8
			 0xc7a88e72, /// 0x3cc
			 0xcf7e7a08, /// 0x3d0
			 0x4200db54, /// 0x3d4
			 0x919b0993, /// 0x3d8
			 0xd45c9488, /// 0x3dc
			 0x187d242b, /// 0x3e0
			 0x04db8890, /// 0x3e4
			 0x3d023a58, /// 0x3e8
			 0x49261b8b, /// 0x3ec
			 0x45c0d7d2, /// 0x3f0
			 0x5cb3f28c, /// 0x3f4
			 0x9b9b330d, /// 0x3f8
			 0x787e2024, /// 0x3fc
			 0xb6c68260, /// 0x400
			 0x5dbd05a4, /// 0x404
			 0x48ee3b89, /// 0x408
			 0x294f871a, /// 0x40c
			 0x83b11856, /// 0x410
			 0x2a525dd7, /// 0x414
			 0xd6990f18, /// 0x418
			 0xfeafe50e, /// 0x41c
			 0x832f1421, /// 0x420
			 0x13b747e8, /// 0x424
			 0xc165adee, /// 0x428
			 0x789fd7de, /// 0x42c
			 0x9f483eb6, /// 0x430
			 0x6eb3d717, /// 0x434
			 0x5e610cd8, /// 0x438
			 0x3dd95919, /// 0x43c
			 0x25a3ba1a, /// 0x440
			 0x74178686, /// 0x444
			 0x27760f22, /// 0x448
			 0xc9d84b3e, /// 0x44c
			 0xca52bda4, /// 0x450
			 0x7b93762e, /// 0x454
			 0x50a5c2a6, /// 0x458
			 0x7b9999da, /// 0x45c
			 0x8bd8ebde, /// 0x460
			 0x67a1cb30, /// 0x464
			 0x70343c0c, /// 0x468
			 0x3accf8d9, /// 0x46c
			 0x8cbb49ca, /// 0x470
			 0x2daaece8, /// 0x474
			 0x189fe796, /// 0x478
			 0x3c9bc2bd, /// 0x47c
			 0xb91f50b9, /// 0x480
			 0x339af1a4, /// 0x484
			 0xb40f3380, /// 0x488
			 0x5552c883, /// 0x48c
			 0xd9f32a07, /// 0x490
			 0xa55e2ab5, /// 0x494
			 0xb8f19091, /// 0x498
			 0xb07771f6, /// 0x49c
			 0xd36ceead, /// 0x4a0
			 0xfd6b038a, /// 0x4a4
			 0x021727a6, /// 0x4a8
			 0xb7906ccf, /// 0x4ac
			 0xab4f50e7, /// 0x4b0
			 0xd043e6c6, /// 0x4b4
			 0xc301fdc4, /// 0x4b8
			 0x9b4bcd56, /// 0x4bc
			 0xc9645fcb, /// 0x4c0
			 0x40697946, /// 0x4c4
			 0x2a114e06, /// 0x4c8
			 0xdd43a245, /// 0x4cc
			 0x3f34a2c1, /// 0x4d0
			 0x40f35895, /// 0x4d4
			 0x9c767baf, /// 0x4d8
			 0x616a0c50, /// 0x4dc
			 0xe40182d5, /// 0x4e0
			 0xfd692ede, /// 0x4e4
			 0xe5bdce2b, /// 0x4e8
			 0x67d6ce45, /// 0x4ec
			 0xb3387bcf, /// 0x4f0
			 0xdbda4a6b, /// 0x4f4
			 0x77fd8c81, /// 0x4f8
			 0xcbe42ccc, /// 0x4fc
			 0xd72550ad, /// 0x500
			 0x806821e9, /// 0x504
			 0x95fe0422, /// 0x508
			 0x7385dffa, /// 0x50c
			 0x9ac71386, /// 0x510
			 0x3d8cd66d, /// 0x514
			 0xe0c49a8f, /// 0x518
			 0xca24d634, /// 0x51c
			 0x4fb39319, /// 0x520
			 0x0ff538a8, /// 0x524
			 0x0c637480, /// 0x528
			 0x3e28e3b4, /// 0x52c
			 0x36a2658e, /// 0x530
			 0xaca01c35, /// 0x534
			 0x636d0c92, /// 0x538
			 0xadc84742, /// 0x53c
			 0x129028c4, /// 0x540
			 0x5b9fa6b8, /// 0x544
			 0x823cb485, /// 0x548
			 0xf80962d2, /// 0x54c
			 0x6c517436, /// 0x550
			 0xf05457dd, /// 0x554
			 0x9336215f, /// 0x558
			 0x2e9d1f5f, /// 0x55c
			 0x24177073, /// 0x560
			 0xef2f2d0c, /// 0x564
			 0x7faa10c1, /// 0x568
			 0x1fa7981e, /// 0x56c
			 0xe39c679f, /// 0x570
			 0xae36b117, /// 0x574
			 0xfd73ee46, /// 0x578
			 0x8547c333, /// 0x57c
			 0xece7a268, /// 0x580
			 0xc5dcd99c, /// 0x584
			 0x691ad5e2, /// 0x588
			 0xa65d511c, /// 0x58c
			 0xd23565c6, /// 0x590
			 0x70e2c451, /// 0x594
			 0x44b0f69b, /// 0x598
			 0xab2feed6, /// 0x59c
			 0xca080fba, /// 0x5a0
			 0xeedacef5, /// 0x5a4
			 0xfe3b1555, /// 0x5a8
			 0x6e884f68, /// 0x5ac
			 0xebd3a063, /// 0x5b0
			 0xf258bf30, /// 0x5b4
			 0x9aa2b116, /// 0x5b8
			 0x9ea69029, /// 0x5bc
			 0x368635c2, /// 0x5c0
			 0xa5f8bc68, /// 0x5c4
			 0x4a6041a2, /// 0x5c8
			 0x1815d3ee, /// 0x5cc
			 0x763a20fc, /// 0x5d0
			 0xfdd546f1, /// 0x5d4
			 0x02176dde, /// 0x5d8
			 0xddb75740, /// 0x5dc
			 0xf217da1c, /// 0x5e0
			 0x2580f828, /// 0x5e4
			 0xaa1839d9, /// 0x5e8
			 0xd1156ac8, /// 0x5ec
			 0xca470b5f, /// 0x5f0
			 0xc6167615, /// 0x5f4
			 0x9f7652e7, /// 0x5f8
			 0x0a189102, /// 0x5fc
			 0x98375095, /// 0x600
			 0x737744e7, /// 0x604
			 0x7e5a0fc9, /// 0x608
			 0xd43d331b, /// 0x60c
			 0xbc72f031, /// 0x610
			 0x421bc928, /// 0x614
			 0xa6f9cee9, /// 0x618
			 0xf58323b2, /// 0x61c
			 0xdb9cb1ad, /// 0x620
			 0x3faefccb, /// 0x624
			 0x2548f3b2, /// 0x628
			 0xd7012fe6, /// 0x62c
			 0x5342e889, /// 0x630
			 0x1cd94717, /// 0x634
			 0x2446eaf7, /// 0x638
			 0x18fd794e, /// 0x63c
			 0x220af1d2, /// 0x640
			 0xd0baf5de, /// 0x644
			 0x9d59e798, /// 0x648
			 0x588475fb, /// 0x64c
			 0x96909573, /// 0x650
			 0x09f29885, /// 0x654
			 0x5859976c, /// 0x658
			 0xbbf7d70b, /// 0x65c
			 0x4d87b18a, /// 0x660
			 0xfa89dff0, /// 0x664
			 0x7ab95a62, /// 0x668
			 0xcada2025, /// 0x66c
			 0x571d1a54, /// 0x670
			 0x13d6e81e, /// 0x674
			 0xac1d463f, /// 0x678
			 0x02bb1aa9, /// 0x67c
			 0x5ac3b1be, /// 0x680
			 0xf6eb1039, /// 0x684
			 0x1a5a7e2f, /// 0x688
			 0x629e3834, /// 0x68c
			 0x95a35eca, /// 0x690
			 0xc78d945c, /// 0x694
			 0xff28ead9, /// 0x698
			 0x18644c0a, /// 0x69c
			 0x6fa69697, /// 0x6a0
			 0x6bfc08a8, /// 0x6a4
			 0xa6775e58, /// 0x6a8
			 0x6628a0f0, /// 0x6ac
			 0x000fcb07, /// 0x6b0
			 0xb0c81100, /// 0x6b4
			 0x9e5a9363, /// 0x6b8
			 0x75e9cfed, /// 0x6bc
			 0xd9e32639, /// 0x6c0
			 0x633e6003, /// 0x6c4
			 0xcd057aec, /// 0x6c8
			 0x48746b01, /// 0x6cc
			 0x10bdb794, /// 0x6d0
			 0x392e809e, /// 0x6d4
			 0xd025a0db, /// 0x6d8
			 0x150daa33, /// 0x6dc
			 0x28857f59, /// 0x6e0
			 0x9ea86c06, /// 0x6e4
			 0x950a4926, /// 0x6e8
			 0x50fe9274, /// 0x6ec
			 0x0d06e5c2, /// 0x6f0
			 0xd2a63459, /// 0x6f4
			 0x485759b7, /// 0x6f8
			 0xb56d3020, /// 0x6fc
			 0x16f73741, /// 0x700
			 0x1cb25aa1, /// 0x704
			 0x747bcfae, /// 0x708
			 0x6989054a, /// 0x70c
			 0xc98a9cac, /// 0x710
			 0x3ca6299d, /// 0x714
			 0xdc651300, /// 0x718
			 0xdfbb9cc1, /// 0x71c
			 0x38ecab93, /// 0x720
			 0xc116176c, /// 0x724
			 0x2930642c, /// 0x728
			 0x5ac8de29, /// 0x72c
			 0x6f526eab, /// 0x730
			 0x7b85300c, /// 0x734
			 0x30d32e92, /// 0x738
			 0x63eaf108, /// 0x73c
			 0xcb4f3e58, /// 0x740
			 0xad97f4cc, /// 0x744
			 0xb8ef7ec4, /// 0x748
			 0x96a1cc6d, /// 0x74c
			 0x89356d64, /// 0x750
			 0xc8aaa986, /// 0x754
			 0x63ad9462, /// 0x758
			 0x003023ae, /// 0x75c
			 0xb7b3ed39, /// 0x760
			 0xc1b6a296, /// 0x764
			 0x40f02f0c, /// 0x768
			 0x3c24d5b6, /// 0x76c
			 0xb3ebaffe, /// 0x770
			 0xc8437ae8, /// 0x774
			 0xc5f5f8be, /// 0x778
			 0xe5a6df9d, /// 0x77c
			 0xc1bf2b16, /// 0x780
			 0x0b0559ba, /// 0x784
			 0x9c3a51f5, /// 0x788
			 0xa1488ccd, /// 0x78c
			 0xcdaf585a, /// 0x790
			 0x9a901c74, /// 0x794
			 0x865efb26, /// 0x798
			 0x2a182910, /// 0x79c
			 0x54174f96, /// 0x7a0
			 0x1f160ff8, /// 0x7a4
			 0x47640bf0, /// 0x7a8
			 0x48c39ac1, /// 0x7ac
			 0x1dbb4bd7, /// 0x7b0
			 0x597c4d60, /// 0x7b4
			 0x8858b89e, /// 0x7b8
			 0x2d202fd2, /// 0x7bc
			 0x48de1a53, /// 0x7c0
			 0x889e3a3b, /// 0x7c4
			 0x19df955d, /// 0x7c8
			 0xa7728410, /// 0x7cc
			 0xfe617f71, /// 0x7d0
			 0x8e6611c1, /// 0x7d4
			 0xdb3f2dfe, /// 0x7d8
			 0x9d44fba6, /// 0x7dc
			 0xd7cf46fc, /// 0x7e0
			 0x090f6104, /// 0x7e4
			 0x48585f4a, /// 0x7e8
			 0x81ca9c89, /// 0x7ec
			 0x66c8b2ed, /// 0x7f0
			 0x7e06bf01, /// 0x7f4
			 0xc6f0424e, /// 0x7f8
			 0x157f8f8c, /// 0x7fc
			 0xa5c638a1, /// 0x800
			 0x3d9defc0, /// 0x804
			 0x20101a16, /// 0x808
			 0x3599c59c, /// 0x80c
			 0x50401b3d, /// 0x810
			 0x999eebf5, /// 0x814
			 0x0fa56037, /// 0x818
			 0xafd37423, /// 0x81c
			 0x167a0fe6, /// 0x820
			 0xc766ee75, /// 0x824
			 0x2a9a059a, /// 0x828
			 0x189cec25, /// 0x82c
			 0xeaa176ef, /// 0x830
			 0xd5e9fa05, /// 0x834
			 0xe4ea995f, /// 0x838
			 0x79db3fd3, /// 0x83c
			 0x25d5bda4, /// 0x840
			 0x96aadd5e, /// 0x844
			 0xd8651001, /// 0x848
			 0x941fb994, /// 0x84c
			 0xfd66408e, /// 0x850
			 0x098ca00f, /// 0x854
			 0x970dbd94, /// 0x858
			 0x0c469e60, /// 0x85c
			 0xacb71571, /// 0x860
			 0xb0ac79ce, /// 0x864
			 0xfddfa552, /// 0x868
			 0x8c2f286f, /// 0x86c
			 0x8ce0cba5, /// 0x870
			 0xfbb567ef, /// 0x874
			 0xf40a8ff4, /// 0x878
			 0x09e03366, /// 0x87c
			 0x23168737, /// 0x880
			 0x92f11fb2, /// 0x884
			 0xc03594ef, /// 0x888
			 0xa655ec84, /// 0x88c
			 0x15b64cf9, /// 0x890
			 0x9f08c5e7, /// 0x894
			 0xb0114b93, /// 0x898
			 0x78337673, /// 0x89c
			 0x23c9ad9f, /// 0x8a0
			 0x3e17f413, /// 0x8a4
			 0x2d44a4a7, /// 0x8a8
			 0x2145ff90, /// 0x8ac
			 0xe704b402, /// 0x8b0
			 0xea05f5dd, /// 0x8b4
			 0x5050b69e, /// 0x8b8
			 0x49ee703e, /// 0x8bc
			 0xec637eea, /// 0x8c0
			 0x4dcc3106, /// 0x8c4
			 0x6322480e, /// 0x8c8
			 0xb4e1edf9, /// 0x8cc
			 0x98ff3054, /// 0x8d0
			 0xc6f8ce02, /// 0x8d4
			 0x4205535e, /// 0x8d8
			 0x09f4387c, /// 0x8dc
			 0xc5e6f81e, /// 0x8e0
			 0x5ad30993, /// 0x8e4
			 0x551c142c, /// 0x8e8
			 0x2134e0df, /// 0x8ec
			 0x0cab8ce6, /// 0x8f0
			 0xb830a5a5, /// 0x8f4
			 0x9bb84a84, /// 0x8f8
			 0x239abebc, /// 0x8fc
			 0x9642fb5c, /// 0x900
			 0xf988894e, /// 0x904
			 0x961e0b42, /// 0x908
			 0x8a4759a6, /// 0x90c
			 0xafd7c284, /// 0x910
			 0x88742c42, /// 0x914
			 0x9d371eee, /// 0x918
			 0x9d732ff0, /// 0x91c
			 0xec7cdf20, /// 0x920
			 0x4d1f126c, /// 0x924
			 0xae564393, /// 0x928
			 0x760fe1c7, /// 0x92c
			 0xeb2def01, /// 0x930
			 0xb60bd273, /// 0x934
			 0x2eaaabfd, /// 0x938
			 0x55178e32, /// 0x93c
			 0x65997f7c, /// 0x940
			 0xfe016140, /// 0x944
			 0xf3f244f5, /// 0x948
			 0xf2cc3336, /// 0x94c
			 0x15c59b1e, /// 0x950
			 0xe783743f, /// 0x954
			 0xb47190d3, /// 0x958
			 0x55dcc2d2, /// 0x95c
			 0x86023b1e, /// 0x960
			 0x77781bcf, /// 0x964
			 0x17c6eb15, /// 0x968
			 0x4b5365fa, /// 0x96c
			 0x72129b20, /// 0x970
			 0x8aed2ee5, /// 0x974
			 0xff10aeb7, /// 0x978
			 0x309c7a3f, /// 0x97c
			 0xf79888a7, /// 0x980
			 0x4b45175f, /// 0x984
			 0x06b57cdb, /// 0x988
			 0xc5a60517, /// 0x98c
			 0x747649d4, /// 0x990
			 0x2a07c72d, /// 0x994
			 0x0236d39a, /// 0x998
			 0xd6eb2337, /// 0x99c
			 0x12a34519, /// 0x9a0
			 0xc39f19b3, /// 0x9a4
			 0xf703de82, /// 0x9a8
			 0x2c479ccc, /// 0x9ac
			 0x050a9a66, /// 0x9b0
			 0xe8832cee, /// 0x9b4
			 0xc91cd2b9, /// 0x9b8
			 0x8a7359f3, /// 0x9bc
			 0xfcf3eafa, /// 0x9c0
			 0x8fe10277, /// 0x9c4
			 0xab009d18, /// 0x9c8
			 0xb491bb60, /// 0x9cc
			 0xd0247c9c, /// 0x9d0
			 0x516ad99b, /// 0x9d4
			 0x1b136845, /// 0x9d8
			 0xfe7c386a, /// 0x9dc
			 0x0820c2b2, /// 0x9e0
			 0x064593e0, /// 0x9e4
			 0x124858fd, /// 0x9e8
			 0x1b386f95, /// 0x9ec
			 0x20c801cd, /// 0x9f0
			 0x537cf68b, /// 0x9f4
			 0xcecf86d3, /// 0x9f8
			 0x1cf4a1b1, /// 0x9fc
			 0x1ceb3501, /// 0xa00
			 0x6f372fb6, /// 0xa04
			 0xe2804991, /// 0xa08
			 0x6af8729a, /// 0xa0c
			 0xc747959b, /// 0xa10
			 0x0be064c1, /// 0xa14
			 0xf9597d40, /// 0xa18
			 0x92d9dda6, /// 0xa1c
			 0x9a900478, /// 0xa20
			 0x10ed73ed, /// 0xa24
			 0x43d8d9be, /// 0xa28
			 0x5777cace, /// 0xa2c
			 0xdd657c1d, /// 0xa30
			 0x26a82113, /// 0xa34
			 0x4275694c, /// 0xa38
			 0x771ad29f, /// 0xa3c
			 0x709a298f, /// 0xa40
			 0x46f6164d, /// 0xa44
			 0xbafc980e, /// 0xa48
			 0x2d732604, /// 0xa4c
			 0xd9896dd2, /// 0xa50
			 0xf73c0e5b, /// 0xa54
			 0x2e6d2665, /// 0xa58
			 0x931fbf4c, /// 0xa5c
			 0xfe3f0d3a, /// 0xa60
			 0x4a764e9b, /// 0xa64
			 0xff60edd9, /// 0xa68
			 0xff92e50f, /// 0xa6c
			 0x11ac9f8b, /// 0xa70
			 0x2a1cf5ee, /// 0xa74
			 0x618f6372, /// 0xa78
			 0x35d54cfa, /// 0xa7c
			 0xda60b443, /// 0xa80
			 0xbee62a95, /// 0xa84
			 0xd5f9af0c, /// 0xa88
			 0x4674ff74, /// 0xa8c
			 0x2d97e228, /// 0xa90
			 0x32773ee3, /// 0xa94
			 0xf07edccc, /// 0xa98
			 0x2d73a35b, /// 0xa9c
			 0x2fd8b277, /// 0xaa0
			 0xe4e09c69, /// 0xaa4
			 0xb163143d, /// 0xaa8
			 0xd1e41961, /// 0xaac
			 0x453de1e2, /// 0xab0
			 0x716985ac, /// 0xab4
			 0xf1792294, /// 0xab8
			 0x38d76aba, /// 0xabc
			 0x31782c69, /// 0xac0
			 0x4a2950a0, /// 0xac4
			 0x2ac7f84a, /// 0xac8
			 0x96bf65ef, /// 0xacc
			 0xa41817e2, /// 0xad0
			 0xbc5c235e, /// 0xad4
			 0x30d65838, /// 0xad8
			 0xfc97ef73, /// 0xadc
			 0x0cc561ba, /// 0xae0
			 0xc42120ed, /// 0xae4
			 0x94aaa9b5, /// 0xae8
			 0xa44d31be, /// 0xaec
			 0x76be2e54, /// 0xaf0
			 0x7bfdb09e, /// 0xaf4
			 0xe51a0125, /// 0xaf8
			 0xedad3974, /// 0xafc
			 0xa14f465e, /// 0xb00
			 0x492cc8e8, /// 0xb04
			 0x970f691b, /// 0xb08
			 0xca3d57c4, /// 0xb0c
			 0x10a6ff6d, /// 0xb10
			 0x2d0df62b, /// 0xb14
			 0x081aae91, /// 0xb18
			 0x1fb46aaf, /// 0xb1c
			 0xc824ee5e, /// 0xb20
			 0x029389ae, /// 0xb24
			 0xa6695691, /// 0xb28
			 0x4629fa77, /// 0xb2c
			 0x4e96680c, /// 0xb30
			 0xd3aba020, /// 0xb34
			 0x6ecad432, /// 0xb38
			 0x85c05019, /// 0xb3c
			 0xae0ff7b1, /// 0xb40
			 0xb246914f, /// 0xb44
			 0x441a256e, /// 0xb48
			 0xc685a764, /// 0xb4c
			 0x4fa62101, /// 0xb50
			 0x96a7659c, /// 0xb54
			 0xa1e44f3e, /// 0xb58
			 0x0a67529f, /// 0xb5c
			 0xb74c65a2, /// 0xb60
			 0x97d3dd77, /// 0xb64
			 0x778706ce, /// 0xb68
			 0x9948e7f5, /// 0xb6c
			 0x8acaa0ab, /// 0xb70
			 0xbd782cf0, /// 0xb74
			 0xdb38e249, /// 0xb78
			 0xed6c8279, /// 0xb7c
			 0xa0677849, /// 0xb80
			 0xb4111705, /// 0xb84
			 0xd53c2c6d, /// 0xb88
			 0x0a3337be, /// 0xb8c
			 0x16cb6a4b, /// 0xb90
			 0x187fa793, /// 0xb94
			 0x79ae52fb, /// 0xb98
			 0x3549d8ea, /// 0xb9c
			 0x54cf9960, /// 0xba0
			 0x47a64f24, /// 0xba4
			 0x13b92824, /// 0xba8
			 0xde780116, /// 0xbac
			 0x89b101e3, /// 0xbb0
			 0x32216569, /// 0xbb4
			 0x663aa97e, /// 0xbb8
			 0xa6f07d85, /// 0xbbc
			 0x01b4fad7, /// 0xbc0
			 0x284ace94, /// 0xbc4
			 0x3816e9a1, /// 0xbc8
			 0x44afb95d, /// 0xbcc
			 0x0ba6b3a2, /// 0xbd0
			 0x1e2c90e0, /// 0xbd4
			 0x961330b2, /// 0xbd8
			 0x723b0ebf, /// 0xbdc
			 0x9e21a60c, /// 0xbe0
			 0xb741ae06, /// 0xbe4
			 0xb8ded78c, /// 0xbe8
			 0x6fea8f16, /// 0xbec
			 0xf967001c, /// 0xbf0
			 0xa32c7a14, /// 0xbf4
			 0x735c9c35, /// 0xbf8
			 0xb63ecc6b, /// 0xbfc
			 0x2bacd484, /// 0xc00
			 0x7f7ccd05, /// 0xc04
			 0x54917db1, /// 0xc08
			 0xbeb2b60b, /// 0xc0c
			 0x3b1dc184, /// 0xc10
			 0xa3426fc5, /// 0xc14
			 0x60576c3c, /// 0xc18
			 0xbd9d83d5, /// 0xc1c
			 0xbda0393f, /// 0xc20
			 0xa9bee3d0, /// 0xc24
			 0x760a35de, /// 0xc28
			 0x0b31bbc1, /// 0xc2c
			 0xdb3638fe, /// 0xc30
			 0xbc0e92ab, /// 0xc34
			 0xe93edab2, /// 0xc38
			 0xaafad48e, /// 0xc3c
			 0xdac5fdb4, /// 0xc40
			 0xb6e1cd06, /// 0xc44
			 0xd416ce01, /// 0xc48
			 0xc09b988e, /// 0xc4c
			 0xc1a85801, /// 0xc50
			 0x007ef68e, /// 0xc54
			 0xfed64ba8, /// 0xc58
			 0xbee7ae58, /// 0xc5c
			 0xa6d117bf, /// 0xc60
			 0x73fbda62, /// 0xc64
			 0x6a3883ce, /// 0xc68
			 0x6357f1fa, /// 0xc6c
			 0x54c921c9, /// 0xc70
			 0x1afafb77, /// 0xc74
			 0x47f6a024, /// 0xc78
			 0x82841383, /// 0xc7c
			 0x82761c26, /// 0xc80
			 0xe7077b45, /// 0xc84
			 0x73c08b62, /// 0xc88
			 0xda6a9d82, /// 0xc8c
			 0x1b75fb85, /// 0xc90
			 0xc0988a67, /// 0xc94
			 0xd38f3417, /// 0xc98
			 0x66665d57, /// 0xc9c
			 0xa70addea, /// 0xca0
			 0x5bee7436, /// 0xca4
			 0xc91c71ae, /// 0xca8
			 0x4970f8ad, /// 0xcac
			 0xfdc1d400, /// 0xcb0
			 0xcfe45482, /// 0xcb4
			 0xcad3274b, /// 0xcb8
			 0xb43085c6, /// 0xcbc
			 0x48d5b3f1, /// 0xcc0
			 0x4a30cccd, /// 0xcc4
			 0x8806a7ef, /// 0xcc8
			 0x7e0ad4ce, /// 0xccc
			 0x6c6fe9f2, /// 0xcd0
			 0xdc54d182, /// 0xcd4
			 0x3b33ba48, /// 0xcd8
			 0xc3780e44, /// 0xcdc
			 0x6f20d6bf, /// 0xce0
			 0x98db2f6f, /// 0xce4
			 0x9e6799ff, /// 0xce8
			 0xef4b8698, /// 0xcec
			 0x02dc40f4, /// 0xcf0
			 0x0bb1c7bf, /// 0xcf4
			 0xb66c78d9, /// 0xcf8
			 0x1fde3d09, /// 0xcfc
			 0x50154499, /// 0xd00
			 0x523ec347, /// 0xd04
			 0xf78ac22c, /// 0xd08
			 0x1dd62cce, /// 0xd0c
			 0x702373f1, /// 0xd10
			 0x0cf3d272, /// 0xd14
			 0x94489cdc, /// 0xd18
			 0x4b810a48, /// 0xd1c
			 0xbb0b0eb5, /// 0xd20
			 0xe9970815, /// 0xd24
			 0xb80fbc43, /// 0xd28
			 0xa0c6f535, /// 0xd2c
			 0xef212c96, /// 0xd30
			 0x00252a87, /// 0xd34
			 0x43d58cab, /// 0xd38
			 0x84712125, /// 0xd3c
			 0xdc430868, /// 0xd40
			 0xaf397c43, /// 0xd44
			 0x23c4501e, /// 0xd48
			 0x01f4b99b, /// 0xd4c
			 0xafa3df24, /// 0xd50
			 0xe0909b08, /// 0xd54
			 0x6f2d66a6, /// 0xd58
			 0x245de551, /// 0xd5c
			 0xec6e759a, /// 0xd60
			 0x94bfb25a, /// 0xd64
			 0x49315af7, /// 0xd68
			 0xa8c96e7c, /// 0xd6c
			 0xb73984d6, /// 0xd70
			 0x9df934cf, /// 0xd74
			 0xa28f85e0, /// 0xd78
			 0x47c66af9, /// 0xd7c
			 0xab0090f3, /// 0xd80
			 0xfee7d074, /// 0xd84
			 0x3a1e3591, /// 0xd88
			 0xac28e394, /// 0xd8c
			 0xcc565768, /// 0xd90
			 0x19b166db, /// 0xd94
			 0x8d6fe915, /// 0xd98
			 0x1c7ed813, /// 0xd9c
			 0xf1eed4a7, /// 0xda0
			 0xa4ae671e, /// 0xda4
			 0x30207088, /// 0xda8
			 0x4a52b8e0, /// 0xdac
			 0x6c65eeb2, /// 0xdb0
			 0xf83d1ce3, /// 0xdb4
			 0x95a35ada, /// 0xdb8
			 0x76feaccb, /// 0xdbc
			 0x00335707, /// 0xdc0
			 0x302acd8c, /// 0xdc4
			 0x86323a50, /// 0xdc8
			 0x67c8ba7d, /// 0xdcc
			 0x566222c8, /// 0xdd0
			 0xda654666, /// 0xdd4
			 0xeeaa0a03, /// 0xdd8
			 0x461da9e7, /// 0xddc
			 0xdfce54ed, /// 0xde0
			 0x60881f66, /// 0xde4
			 0x53856f63, /// 0xde8
			 0x2975f1bf, /// 0xdec
			 0x0d43a089, /// 0xdf0
			 0xa57b0c11, /// 0xdf4
			 0x84f8517b, /// 0xdf8
			 0x2debadab, /// 0xdfc
			 0xaab86f36, /// 0xe00
			 0xace5cee8, /// 0xe04
			 0xb7f820b3, /// 0xe08
			 0x2cc90447, /// 0xe0c
			 0xa6a65ce8, /// 0xe10
			 0xb7e2b56f, /// 0xe14
			 0x1a513258, /// 0xe18
			 0x6592fcfe, /// 0xe1c
			 0xa219cd93, /// 0xe20
			 0x2c1a408d, /// 0xe24
			 0x44c6d0ec, /// 0xe28
			 0x0de6081c, /// 0xe2c
			 0xe3cc95fa, /// 0xe30
			 0xbfedbea5, /// 0xe34
			 0xd45443e5, /// 0xe38
			 0x6390a5a6, /// 0xe3c
			 0x2e24fd46, /// 0xe40
			 0xa647ed69, /// 0xe44
			 0x32f7fe07, /// 0xe48
			 0x1cd6548f, /// 0xe4c
			 0x4737937c, /// 0xe50
			 0xb83eab75, /// 0xe54
			 0xff131e1e, /// 0xe58
			 0x7864b713, /// 0xe5c
			 0xee11951d, /// 0xe60
			 0x7000daad, /// 0xe64
			 0xd91631e4, /// 0xe68
			 0x1ebbbb56, /// 0xe6c
			 0xa4e7f2b7, /// 0xe70
			 0x32d6212c, /// 0xe74
			 0x3609ba1b, /// 0xe78
			 0x5cf2d9b3, /// 0xe7c
			 0x12cee5e2, /// 0xe80
			 0x5c6ed823, /// 0xe84
			 0xb6b8779f, /// 0xe88
			 0x08f3f4cb, /// 0xe8c
			 0xedcb2d1d, /// 0xe90
			 0xb7ac6ba2, /// 0xe94
			 0x199e9fa2, /// 0xe98
			 0xc037529e, /// 0xe9c
			 0x947e3733, /// 0xea0
			 0x696e8482, /// 0xea4
			 0xd99123a1, /// 0xea8
			 0x2ddb9806, /// 0xeac
			 0x7d79051d, /// 0xeb0
			 0x92960450, /// 0xeb4
			 0xa96d3890, /// 0xeb8
			 0xc436f054, /// 0xebc
			 0x54136ef1, /// 0xec0
			 0xea24a801, /// 0xec4
			 0x053b0fbf, /// 0xec8
			 0x88d2ca74, /// 0xecc
			 0x02ec7460, /// 0xed0
			 0xbfed7d04, /// 0xed4
			 0xe4759395, /// 0xed8
			 0x001f6b07, /// 0xedc
			 0xb5065ec2, /// 0xee0
			 0x7c4b9a2c, /// 0xee4
			 0x571347c1, /// 0xee8
			 0x73b3443a, /// 0xeec
			 0xde053faa, /// 0xef0
			 0xd690f8ce, /// 0xef4
			 0xf297bcee, /// 0xef8
			 0xe2ce25eb, /// 0xefc
			 0x65315f4b, /// 0xf00
			 0x6233d22e, /// 0xf04
			 0x8851f9e0, /// 0xf08
			 0x7191ec2e, /// 0xf0c
			 0x173ac00f, /// 0xf10
			 0x47566c85, /// 0xf14
			 0xa65c15d7, /// 0xf18
			 0xe7813b89, /// 0xf1c
			 0x30ffcb2c, /// 0xf20
			 0x84148daa, /// 0xf24
			 0xd38be8a7, /// 0xf28
			 0xe5dfa739, /// 0xf2c
			 0x033649f2, /// 0xf30
			 0xa3ecaa47, /// 0xf34
			 0xe6146356, /// 0xf38
			 0xbc72e122, /// 0xf3c
			 0x9b3cc0bb, /// 0xf40
			 0x5bbbf080, /// 0xf44
			 0xaab7f3fe, /// 0xf48
			 0x1e9f0f54, /// 0xf4c
			 0xd3e0759e, /// 0xf50
			 0xac1006d8, /// 0xf54
			 0xd6cf003e, /// 0xf58
			 0xde107808, /// 0xf5c
			 0x20ccad86, /// 0xf60
			 0xcb1b503d, /// 0xf64
			 0xfd68e239, /// 0xf68
			 0x223af159, /// 0xf6c
			 0xa3d6e4e5, /// 0xf70
			 0xf138f03e, /// 0xf74
			 0xdb31e1b1, /// 0xf78
			 0xde28ddf8, /// 0xf7c
			 0x137f16d9, /// 0xf80
			 0x8780dec4, /// 0xf84
			 0x4abedc92, /// 0xf88
			 0x2ed3bea5, /// 0xf8c
			 0xae59976a, /// 0xf90
			 0x9009ffe3, /// 0xf94
			 0xebc6f3b5, /// 0xf98
			 0x4a0c3817, /// 0xf9c
			 0x86e12056, /// 0xfa0
			 0x080d9f7e, /// 0xfa4
			 0xbcb657f6, /// 0xfa8
			 0x86697d67, /// 0xfac
			 0xb42b21b6, /// 0xfb0
			 0xe72b50ae, /// 0xfb4
			 0xd218adf8, /// 0xfb8
			 0x35e0bbfa, /// 0xfbc
			 0x1267c794, /// 0xfc0
			 0x47103ba8, /// 0xfc4
			 0x86619534, /// 0xfc8
			 0x5f200e4e, /// 0xfcc
			 0x77c19e4c, /// 0xfd0
			 0x1190958d, /// 0xfd4
			 0x11362f3c, /// 0xfd8
			 0x1f910f38, /// 0xfdc
			 0x378a00af, /// 0xfe0
			 0xc4898579, /// 0xfe4
			 0xdfa4fd15, /// 0xfe8
			 0x259a10ee, /// 0xfec
			 0x07e0bca3, /// 0xff0
			 0x2d69471a, /// 0xff4
			 0x0d3fa924, /// 0xff8
			 0x4c47e18a /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00000
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00004
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80011111,                                                  // -9.7958E-41                                 /// 0000c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00010
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00020
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00024
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00011111,                                                  // 9.7958E-41                                  /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0003c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00044
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0004c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00054
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00064
			 0xcb000000,                                                  // -8388608.0                                  /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0006c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00070
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00074
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00078
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0007c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00080
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00084
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xbf028f5c,                                                  // -0.51                                       /// 00090
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00094
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00098
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0009c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00000000,                                                  // zero                                        /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000ac
			 0x55555555,                                                  // 4 random values                             /// 000b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000cc
			 0x3f028f5c,                                                  // 0.51                                        /// 000d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 000e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00104
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00108
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0010c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00110
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00114
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00118
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00124
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00128
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00130
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00134
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00138
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00144
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00148
			 0x0c400000,                                                  // Leading 1s:                                 /// 0014c
			 0x00000000,                                                  // zero                                        /// 00150
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0015c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00164
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0016c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00174
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00178
			 0x33333333,                                                  // 4 random values                             /// 0017c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00184
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00188
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0018c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00190
			 0x80000000,                                                  // -zero                                       /// 00194
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00198
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 001d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00200
			 0x0c700000,                                                  // Leading 1s:                                 /// 00204
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00208
			 0xff800000,                                                  // -inf                                        /// 0020c
			 0xcb000000,                                                  // -8388608.0                                  /// 00210
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00218
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0021c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00224
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00228
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x3f028f5c,                                                  // 0.51                                        /// 00230
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00238
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0023c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00240
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00248
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00000000,                                                  // zero                                        /// 00250
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00258
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00264
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00268
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0026c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0027c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00280
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00284
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00288
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00290
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00294
			 0x7f800000,                                                  // inf                                         /// 00298
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00300
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00304
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00308
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0030c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00320
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00324
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00328
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00330
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00338
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0033c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00340
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00344
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00348
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00350
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00358
			 0xbf028f5c,                                                  // -0.51                                       /// 0035c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00360
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0036c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00370
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00378
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00380
			 0xcb000000,                                                  // -8388608.0                                  /// 00384
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00388
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0038c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00390
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00398
			 0x0c600000,                                                  // Leading 1s:                                 /// 0039c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x4b000000,                                                  // 8388608.0                                   /// 003ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x00000000,                                                  // zero                                        /// 003c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0xffc00001,                                                  // -signaling NaN                              /// 003fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00400
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00404
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00410
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00414
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0041c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x3f028f5c,                                                  // 0.51                                        /// 00434
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0044c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00458
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0045c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00460
			 0x7fc00001,                                                  // signaling NaN                               /// 00464
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00468
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0046c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00474
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0xbf028f5c,                                                  // -0.51                                       /// 00480
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x0c400000,                                                  // Leading 1s:                                 /// 00488
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0048c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00490
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00498
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0xff800000,                                                  // -inf                                        /// 004a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004cc
			 0x3f028f5c,                                                  // 0.51                                        /// 004d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004dc
			 0x7fc00001,                                                  // signaling NaN                               /// 004e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 004f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00500
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00508
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0050c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00514
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00518
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00520
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00524
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00528
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0052c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00534
			 0x4b000000,                                                  // 8388608.0                                   /// 00538
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00540
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00544
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00548
			 0xbf028f5c,                                                  // -0.51                                       /// 0054c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00560
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00564
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00568
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00574
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00584
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0058c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00590
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0059c
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x7f800000,                                                  // inf                                         /// 005b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7fc00001,                                                  // signaling NaN                               /// 005c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005d0
			 0xcb000000,                                                  // -8388608.0                                  /// 005d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00600
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00610
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00614
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00618
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00620
			 0x00000000,                                                  // zero                                        /// 00624
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0xbf028f5c,                                                  // -0.51                                       /// 0062c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00638
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0063c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x80000000,                                                  // -zero                                       /// 00648
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00650
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00654
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00658
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0065c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00664
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00668
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0066c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00670
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e000003,                                                  // Trailing 1s:                                /// 00678
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00684
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00688
			 0x80000000,                                                  // -zero                                       /// 0068c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00694
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00698
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0069c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006e4
			 0x33333333,                                                  // 4 random values                             /// 006e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00708
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0c400000,                                                  // Leading 1s:                                 /// 00718
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0071c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00720
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00724
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0c780000,                                                  // Leading 1s:                                 /// 0072c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00730
			 0xcb000000,                                                  // -8388608.0                                  /// 00734
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00738
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0073c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00754
			 0x33333333,                                                  // 4 random values                             /// 00758
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0075c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00760
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00764
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00768
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0076c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00770
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00774
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0077c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00780
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00784
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00788
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0078c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00794
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00798
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0079c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x4b000000,                                                  // 8388608.0                                   /// 00810
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00814
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00820
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00828
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0082c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00830
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00834
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00840
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00844
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00848
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00858
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0085c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00860
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80011111,                                                  // -9.7958E-41                                 /// 00868
			 0x0c600000,                                                  // Leading 1s:                                 /// 0086c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00874
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c400000,                                                  // Leading 1s:                                 /// 0087c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00880
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00884
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0088c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00894
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x55555555,                                                  // 4 random values                             /// 008b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x3f028f5c,                                                  // 0.51                                        /// 008b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c8
			 0x33333333,                                                  // 4 random values                             /// 008cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e0
			 0x7f800000,                                                  // inf                                         /// 008e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00908
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0090c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00910
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e000003,                                                  // Trailing 1s:                                /// 0091c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00924
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00928
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00930
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00934
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0093c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00944
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0c600000,                                                  // Leading 1s:                                 /// 0094c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c600000,                                                  // Leading 1s:                                 /// 00958
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00960
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00964
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00968
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0096c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00970
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x3f028f5c,                                                  // 0.51                                        /// 00980
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0098c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00990
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009b0
			 0x7f800000,                                                  // inf                                         /// 009b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x7f800000,                                                  // inf                                         /// 009e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f8
			 0x55555555,                                                  // 4 random values                             /// 009fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a00
			 0x7fc00001,                                                  // signaling NaN                               /// 00a04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x55555555,                                                  // 4 random values                             /// 00a20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a84
			 0x7f800000,                                                  // inf                                         /// 00a88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a9c
			 0x33333333,                                                  // 4 random values                             /// 00aa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ab8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00acc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x55555555,                                                  // 4 random values                             /// 00ae0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x7f800000,                                                  // inf                                         /// 00ae8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00af0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00afc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b10
			 0x00000000,                                                  // zero                                        /// 00b14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x55555555,                                                  // 4 random values                             /// 00b20
			 0x7fc00001,                                                  // signaling NaN                               /// 00b24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b40
			 0x7f800000,                                                  // inf                                         /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bb8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bc0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bf0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bf4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c20
			 0x80000000,                                                  // -zero                                       /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c70
			 0x00000000,                                                  // zero                                        /// 00c74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cb8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x33333333,                                                  // 4 random values                             /// 00cc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ccc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cdc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ce4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d4c
			 0x7f800000,                                                  // inf                                         /// 00d50
			 0x33333333,                                                  // 4 random values                             /// 00d54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d70
			 0x4b000000,                                                  // 8388608.0                                   /// 00d74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00da8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00db0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dd0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e18
			 0x55555555,                                                  // 4 random values                             /// 00e1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e40
			 0xff800000,                                                  // -inf                                        /// 00e44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x00000000,                                                  // zero                                        /// 00e64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ebc
			 0x33333333,                                                  // 4 random values                             /// 00ec0
			 0x33333333,                                                  // 4 random values                             /// 00ec4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ec8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x33333333,                                                  // 4 random values                             /// 00eec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ef4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00efc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x7f800000,                                                  // inf                                         /// 00f74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fcc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00800000                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00000
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00004
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0000c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00014
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00018
			 0x00000000,                                                  // zero                                        /// 0001c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00028
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00030
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00038
			 0x80011111,                                                  // -9.7958E-41                                 /// 0003c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00040
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00044
			 0x0c600000,                                                  // Leading 1s:                                 /// 00048
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0004c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00050
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00058
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00070
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00074
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00078
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0007c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00080
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00084
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00088
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0008c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00090
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00094
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000b4
			 0xcb000000,                                                  // -8388608.0                                  /// 000b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00104
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0010c
			 0xbf028f5c,                                                  // -0.51                                       /// 00110
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00114
			 0x0c600000,                                                  // Leading 1s:                                 /// 00118
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00120
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00128
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00130
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00138
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0013c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0014c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00160
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00164
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00168
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00174
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00178
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00190
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00198
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0019c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001c0
			 0x7f800000,                                                  // inf                                         /// 001c4
			 0x7fc00001,                                                  // signaling NaN                               /// 001c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001dc
			 0x4b000000,                                                  // 8388608.0                                   /// 001e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e8
			 0x3f028f5c,                                                  // 0.51                                        /// 001ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00204
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00208
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0020c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00210
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00214
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0021c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00220
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x3f028f5c,                                                  // 0.51                                        /// 00228
			 0x4b000000,                                                  // 8388608.0                                   /// 0022c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00230
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00234
			 0x80000000,                                                  // -zero                                       /// 00238
			 0x0e000003,                                                  // Trailing 1s:                                /// 0023c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00240
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00248
			 0x55555555,                                                  // 4 random values                             /// 0024c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00250
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00254
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00258
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0025c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00264
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x00011111,                                                  // 9.7958E-41                                  /// 0026c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00270
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00274
			 0x7fc00001,                                                  // signaling NaN                               /// 00278
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00284
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00294
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00298
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002a8
			 0x33333333,                                                  // 4 random values                             /// 002ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000000,                                                  // zero                                        /// 002c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x7f800000,                                                  // inf                                         /// 002dc
			 0x4b000000,                                                  // 8388608.0                                   /// 002e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002ec
			 0xcb000000,                                                  // -8388608.0                                  /// 002f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00300
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0031c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00320
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00324
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00334
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00338
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0033c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00344
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00348
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00350
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00354
			 0x0e000003,                                                  // Trailing 1s:                                /// 00358
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00364
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00368
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00378
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0037c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00380
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00388
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00390
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00394
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00398
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0039c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0xff800000,                                                  // -inf                                        /// 003bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x3f028f5c,                                                  // 0.51                                        /// 003d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00400
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00408
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0040c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00414
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00418
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00420
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00424
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0042c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0xff800000,                                                  // -inf                                        /// 00438
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00440
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00458
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0045c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00464
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00470
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00474
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00478
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00480
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00484
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00488
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0048c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00494
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0c600000,                                                  // Leading 1s:                                 /// 0049c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 004b0
			 0x33333333,                                                  // 4 random values                             /// 004b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0xcb000000,                                                  // -8388608.0                                  /// 004e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00500
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00504
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0050c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00510
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00514
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00518
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00520
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00524
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0052c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0e000001,                                                  // Trailing 1s:                                /// 00534
			 0x55555555,                                                  // 4 random values                             /// 00538
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0053c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00544
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00548
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0054c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00550
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00554
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0055c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xbf028f5c,                                                  // -0.51                                       /// 00564
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00568
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00570
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00574
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00578
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0057c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00580
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00588
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0058c
			 0x00000000,                                                  // zero                                        /// 00590
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00598
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x00000000,                                                  // zero                                        /// 005a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x80000000,                                                  // -zero                                       /// 005b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000000,                                                  // zero                                        /// 005b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00000000,                                                  // zero                                        /// 005f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00604
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00608
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0060c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00610
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0061c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00628
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00630
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00634
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00638
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0063c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00644
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00648
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0065c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00660
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00664
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00668
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00670
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00674
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00678
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0067c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x7fc00001,                                                  // signaling NaN                               /// 00684
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x55555555,                                                  // 4 random values                             /// 00690
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0069c
			 0x4b000000,                                                  // 8388608.0                                   /// 006a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80000000,                                                  // -zero                                       /// 006b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00700
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0070c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00710
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xff800000,                                                  // -inf                                        /// 00718
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0071c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00720
			 0x80000000,                                                  // -zero                                       /// 00724
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00728
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0072c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00730
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00734
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0073c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00740
			 0x00011111,                                                  // 9.7958E-41                                  /// 00744
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80011111,                                                  // -9.7958E-41                                 /// 0074c
			 0xff800000,                                                  // -inf                                        /// 00750
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00754
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00758
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0075c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00760
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00764
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00768
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0076c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00778
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00780
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00784
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0078c
			 0x00000000,                                                  // zero                                        /// 00790
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00794
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00798
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007b0
			 0x80000000,                                                  // -zero                                       /// 007b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x33333333,                                                  // 4 random values                             /// 007e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00804
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00808
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00810
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00814
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00818
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00820
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00824
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00828
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0082c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00838
			 0x0c780000,                                                  // Leading 1s:                                 /// 0083c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00840
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00844
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00854
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0xcb000000,                                                  // -8388608.0                                  /// 00860
			 0x55555555,                                                  // 4 random values                             /// 00864
			 0x0e000003,                                                  // Trailing 1s:                                /// 00868
			 0x0c600000,                                                  // Leading 1s:                                 /// 0086c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00870
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00874
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00878
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0087c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00880
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0088c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00890
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00898
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0089c
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008f4
			 0x00000000,                                                  // zero                                        /// 008f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00900
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00904
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00908
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0090c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00910
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00914
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0091c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00920
			 0x3f028f5c,                                                  // 0.51                                        /// 00924
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00928
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0092c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00930
			 0xcb000000,                                                  // -8388608.0                                  /// 00934
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0093c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00940
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00944
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00948
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00950
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00954
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0095c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00960
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00964
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00968
			 0x00011111,                                                  // 9.7958E-41                                  /// 0096c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0097c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00980
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00988
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00990
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00994
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00998
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x3f028f5c,                                                  // 0.51                                        /// 009d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a14
			 0x7f800000,                                                  // inf                                         /// 00a18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a30
			 0xbf028f5c,                                                  // -0.51                                       /// 00a34
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ab4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00acc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ad0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ad4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ad8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00adc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ae4
			 0xff800000,                                                  // -inf                                        /// 00ae8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00af8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00afc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b28
			 0x7f800000,                                                  // inf                                         /// 00b2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ba8
			 0x33333333,                                                  // 4 random values                             /// 00bac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bb4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bcc
			 0x80000000,                                                  // -zero                                       /// 00bd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf0
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cbc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cf4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x33333333,                                                  // 4 random values                             /// 00d64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d68
			 0xcb000000,                                                  // -8388608.0                                  /// 00d6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d80
			 0xff800000,                                                  // -inf                                        /// 00d84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00db4
			 0xff800000,                                                  // -inf                                        /// 00db8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc8
			 0x33333333,                                                  // 4 random values                             /// 00dcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ddc
			 0x33333333,                                                  // 4 random values                             /// 00de0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00df8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x3f028f5c,                                                  // 0.51                                        /// 00e3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0xcb000000,                                                  // -8388608.0                                  /// 00e58
			 0x3f028f5c,                                                  // 0.51                                        /// 00e5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ebc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ec0
			 0x80000000,                                                  // -zero                                       /// 00ec4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ed0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ed4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00edc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee8
			 0xcb000000,                                                  // -8388608.0                                  /// 00eec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00efc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f44
			 0x7fc00001,                                                  // signaling NaN                               /// 00f48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x33333333,                                                  // 4 random values                             /// 00f50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fa8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00000000,                                                  // zero                                        /// 00fb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80000200                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00000
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00004
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00008
			 0x00000000,                                                  // zero                                        /// 0000c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00010
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00014
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00018
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0001c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00020
			 0xcb000000,                                                  // -8388608.0                                  /// 00024
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00040
			 0x7fc00001,                                                  // signaling NaN                               /// 00044
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00050
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00054
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00060
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c780000,                                                  // Leading 1s:                                 /// 0006c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00080
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e000003,                                                  // Trailing 1s:                                /// 00088
			 0x00000000,                                                  // zero                                        /// 0008c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00090
			 0xcb000000,                                                  // -8388608.0                                  /// 00094
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0009c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00100
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00110
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00114
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00118
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00120
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00124
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00130
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00134
			 0xff800000,                                                  // -inf                                        /// 00138
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0013c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00140
			 0x33333333,                                                  // 4 random values                             /// 00144
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00150
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00154
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0015c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00164
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0016c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00170
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00174
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00180
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00188
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0018c
			 0x33333333,                                                  // 4 random values                             /// 00190
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00194
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c400000,                                                  // Leading 1s:                                 /// 001a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00208
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0020c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00210
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00214
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00218
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00220
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00224
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0022c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00230
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00238
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0023c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00244
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0024c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00254
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00258
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00260
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0026c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00270
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00278
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0027c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00284
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00288
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0028c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00290
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00294
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00298
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0029c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ac
			 0xff800000,                                                  // -inf                                        /// 002b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002b8
			 0xffc00001,                                                  // -signaling NaN                              /// 002bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002cc
			 0xcb000000,                                                  // -8388608.0                                  /// 002d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00300
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00304
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0030c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00318
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00328
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0032c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00334
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00340
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00344
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00348
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00350
			 0x4b000000,                                                  // 8388608.0                                   /// 00354
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00358
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00360
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00364
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00368
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00374
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0038c
			 0x55555555,                                                  // 4 random values                             /// 00390
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0039c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003cc
			 0xbf028f5c,                                                  // -0.51                                       /// 003d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00011111,                                                  // 9.7958E-41                                  /// 00408
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0040c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00410
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00414
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00418
			 0x80011111,                                                  // -9.7958E-41                                 /// 0041c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00428
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0042c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00430
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x55555555,                                                  // 4 random values                             /// 00438
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0043c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00440
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00444
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00448
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x4b000000,                                                  // 8388608.0                                   /// 00450
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00454
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00458
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00460
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00464
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00468
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00470
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00474
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0047c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00484
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000000,                                                  // -zero                                       /// 0048c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00490
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00498
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x33333333,                                                  // 4 random values                             /// 004b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e8
			 0x4b000000,                                                  // 8388608.0                                   /// 004ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00500
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00504
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0050c
			 0x80000000,                                                  // -zero                                       /// 00510
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00514
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00518
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0051c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00524
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00534
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00538
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0053c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00544
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0054c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00554
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0056c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00570
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00574
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00584
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0058c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00590
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e000001,                                                  // Trailing 1s:                                /// 00598
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000000,                                                  // zero                                        /// 005bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c8
			 0xffc00001,                                                  // -signaling NaN                              /// 005cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00600
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00610
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00618
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0061c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00624
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0062c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00630
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00638
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0063c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00644
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00648
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0064c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00650
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0065c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00660
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00664
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00668
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00670
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00678
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0067c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0xffc00001,                                                  // -signaling NaN                              /// 00688
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0068c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0069c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006bc
			 0x00000000,                                                  // zero                                        /// 006c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 006f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0xcb000000,                                                  // -8388608.0                                  /// 006f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00704
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00708
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0xcb000000,                                                  // -8388608.0                                  /// 00714
			 0x0e000007,                                                  // Trailing 1s:                                /// 00718
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0071c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80011111,                                                  // -9.7958E-41                                 /// 0072c
			 0x33333333,                                                  // 4 random values                             /// 00730
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00734
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00738
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00740
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00744
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0074c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00754
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0075c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00764
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0076c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00774
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00778
			 0xcb000000,                                                  // -8388608.0                                  /// 0077c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00784
			 0x80011111,                                                  // -9.7958E-41                                 /// 00788
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80000000,                                                  // -zero                                       /// 00794
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00798
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0079c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 007e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007e8
			 0xff800000,                                                  // -inf                                        /// 007ec
			 0xbf028f5c,                                                  // -0.51                                       /// 007f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00800
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00804
			 0xff800000,                                                  // -inf                                        /// 00808
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0080c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00810
			 0x0c700000,                                                  // Leading 1s:                                 /// 00814
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00818
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00824
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00828
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00834
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0083c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x55555555,                                                  // 4 random values                             /// 00848
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0084c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00850
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00854
			 0x00000000,                                                  // zero                                        /// 00858
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00860
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00868
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00874
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00878
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0087c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00880
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00884
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x00011111,                                                  // 9.7958E-41                                  /// 0088c
			 0x55555555,                                                  // 4 random values                             /// 00890
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00894
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00898
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0089c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7fc00001,                                                  // signaling NaN                               /// 008ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00900
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00904
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00914
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00918
			 0xffc00001,                                                  // -signaling NaN                              /// 0091c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0xcb000000,                                                  // -8388608.0                                  /// 00928
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0092c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00930
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0093c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00940
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00944
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0094c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00950
			 0x80000000,                                                  // -zero                                       /// 00954
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00958
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0095c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00968
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0096c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00974
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00978
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00984
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00988
			 0x0c780000,                                                  // Leading 1s:                                 /// 0098c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00990
			 0x80000000,                                                  // -zero                                       /// 00994
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00998
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0099c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009b8
			 0x4b000000,                                                  // 8388608.0                                   /// 009bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x4b000000,                                                  // 8388608.0                                   /// 009d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x7f800000,                                                  // inf                                         /// 009ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0xcb000000,                                                  // -8388608.0                                  /// 00a34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x00000000,                                                  // zero                                        /// 00a48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00000000,                                                  // zero                                        /// 00a64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a88
			 0xff800000,                                                  // -inf                                        /// 00a8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00abc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ac0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00adc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ae0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ae4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ae8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00af4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00afc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80000000,                                                  // -zero                                       /// 00b6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x00000000,                                                  // zero                                        /// 00b74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ba0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ba4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00be4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bf4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ccc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cdc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ce8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cf0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0xffc00001,                                                  // -signaling NaN                              /// 00d44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x55555555,                                                  // 4 random values                             /// 00d68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d78
			 0x3f028f5c,                                                  // 0.51                                        /// 00d7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x55555555,                                                  // 4 random values                             /// 00d94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00db0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00df4
			 0x33333333,                                                  // 4 random values                             /// 00df8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eac
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ebc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ec0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ec4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ecc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ed8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00edc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0xff800000,                                                  // -inf                                        /// 00ee4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ee8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ef4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ef8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00efc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f44
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f64
			 0x55555555,                                                  // 4 random values                             /// 00f68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f90
			 0xbf028f5c,                                                  // -0.51                                       /// 00f94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f98
			 0x4b000000,                                                  // 8388608.0                                   /// 00f9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa8
			 0x4b000000,                                                  // 8388608.0                                   /// 00fac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ff8
			 0x007fffff                                                  // 1.1754942E-38                               /// last
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
			 0x00000624,
			 0x00000664,
			 0x000005e8,
			 0x00000128,
			 0x00000000,
			 0x000003e0,
			 0x000002b0,
			 0x00000510,

			 /// vpu register f2
			 0x41e80000,
			 0x41200000,
			 0x41a00000,
			 0x41c80000,
			 0x42000000,
			 0x41d80000,
			 0x40e00000,
			 0x41200000,

			 /// vpu register f3
			 0x41100000,
			 0x41c80000,
			 0x41800000,
			 0x41e00000,
			 0x41e80000,
			 0x41400000,
			 0x41200000,
			 0x42000000,

			 /// vpu register f4
			 0x41a80000,
			 0x41d00000,
			 0x41880000,
			 0x41b00000,
			 0x41f80000,
			 0x41100000,
			 0x41f80000,
			 0x41a80000,

			 /// vpu register f5
			 0x41880000,
			 0x41b00000,
			 0x41900000,
			 0x41300000,
			 0x41b00000,
			 0x41c00000,
			 0x41e80000,
			 0x40c00000,

			 /// vpu register f6
			 0x41700000,
			 0x41b00000,
			 0x41300000,
			 0x41e00000,
			 0x41b80000,
			 0x41a00000,
			 0x41880000,
			 0x41800000,

			 /// vpu register f7
			 0x41e00000,
			 0x42000000,
			 0x40000000,
			 0x41980000,
			 0x41800000,
			 0x3f800000,
			 0x41a80000,
			 0x41a80000,

			 /// vpu register f8
			 0x41000000,
			 0x41b00000,
			 0x41400000,
			 0x42000000,
			 0x41600000,
			 0x40e00000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f9
			 0x41d80000,
			 0x40a00000,
			 0x41c80000,
			 0x41b80000,
			 0x3f800000,
			 0x41f80000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f10
			 0x40e00000,
			 0x41f00000,
			 0x41a00000,
			 0x42000000,
			 0x41b80000,
			 0x41700000,
			 0x41a80000,
			 0x41500000,

			 /// vpu register f11
			 0x40800000,
			 0x41000000,
			 0x41c80000,
			 0x41980000,
			 0x41d00000,
			 0x3f800000,
			 0x3f800000,
			 0x41800000,

			 /// vpu register f12
			 0x41900000,
			 0x41b80000,
			 0x40800000,
			 0x41a00000,
			 0x41880000,
			 0x41880000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f13
			 0x41a00000,
			 0x40e00000,
			 0x41300000,
			 0x40c00000,
			 0x41700000,
			 0x41400000,
			 0x41980000,
			 0x40c00000,

			 /// vpu register f14
			 0x41400000,
			 0x41300000,
			 0x41e00000,
			 0x41900000,
			 0x41a00000,
			 0x41980000,
			 0x41c00000,
			 0x41c80000,

			 /// vpu register f15
			 0x41a80000,
			 0x41600000,
			 0x41e80000,
			 0x41e80000,
			 0x41400000,
			 0x41880000,
			 0x40a00000,
			 0x41e80000,

			 /// vpu register f16
			 0x41e00000,
			 0x40a00000,
			 0x41c80000,
			 0x40800000,
			 0x41000000,
			 0x40800000,
			 0x41d00000,
			 0x40400000,

			 /// vpu register f17
			 0x41a00000,
			 0x41e80000,
			 0x3f800000,
			 0x40800000,
			 0x41f80000,
			 0x41f00000,
			 0x3f800000,
			 0x41980000,

			 /// vpu register f18
			 0x41200000,
			 0x41900000,
			 0x41000000,
			 0x41200000,
			 0x41c80000,
			 0x41800000,
			 0x41200000,
			 0x41c80000,

			 /// vpu register f19
			 0x41d80000,
			 0x40000000,
			 0x40e00000,
			 0x41c00000,
			 0x41b80000,
			 0x41f80000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f20
			 0x41100000,
			 0x41980000,
			 0x41b00000,
			 0x41980000,
			 0x41100000,
			 0x40000000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f21
			 0x3f800000,
			 0x40800000,
			 0x41f00000,
			 0x41f00000,
			 0x40a00000,
			 0x41d80000,
			 0x40e00000,
			 0x41a80000,

			 /// vpu register f22
			 0x40800000,
			 0x41a00000,
			 0x41900000,
			 0x40800000,
			 0x41300000,
			 0x41900000,
			 0x41800000,
			 0x41800000,

			 /// vpu register f23
			 0x41b00000,
			 0x41500000,
			 0x41980000,
			 0x41a00000,
			 0x41b80000,
			 0x41300000,
			 0x41800000,
			 0x41e00000,

			 /// vpu register f24
			 0x41200000,
			 0x41b80000,
			 0x40800000,
			 0x41f80000,
			 0x41800000,
			 0x41500000,
			 0x41c00000,
			 0x41c00000,

			 /// vpu register f25
			 0x41d80000,
			 0x42000000,
			 0x41400000,
			 0x41100000,
			 0x41d00000,
			 0x41d00000,
			 0x41f00000,
			 0x40000000,

			 /// vpu register f26
			 0x40a00000,
			 0x40400000,
			 0x41e80000,
			 0x41000000,
			 0x41100000,
			 0x41880000,
			 0x41e00000,
			 0x41e80000,

			 /// vpu register f27
			 0x41c80000,
			 0x41e80000,
			 0x41200000,
			 0x41880000,
			 0x41000000,
			 0x41e00000,
			 0x41d80000,
			 0x41600000,

			 /// vpu register f28
			 0x41200000,
			 0x41d80000,
			 0x41c80000,
			 0x40000000,
			 0x40800000,
			 0x41980000,
			 0x41b80000,
			 0x3f800000,

			 /// vpu register f29
			 0x41200000,
			 0x41f80000,
			 0x41c80000,
			 0x41c80000,
			 0x41500000,
			 0x41700000,
			 0x41300000,
			 0x41a00000,

			 /// vpu register f30
			 0x40a00000,
			 0x41900000,
			 0x42000000,
			 0x41880000,
			 0x41e80000,
			 0x41700000,
			 0x40a00000,
			 0x42000000,

			 /// vpu register f31
			 0x41000000,
			 0x41100000,
			 0x3f800000,
			 0x40c00000,
			 0x40c00000,
			 0x41a00000,
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
		"fschg.ps f29, f1 (x13)\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f18, f1 (x12)\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f28, f1 (x11)\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f29, f1 (x12)\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f17, f1 (x14)\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f7, f1 (x13)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f17, f1 (x15)\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f22, f1 (x12)\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f2, f1 (x13)\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x12)\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f19, f1 (x13)\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f23, f1 (x15)\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f18, f1 (x11)\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f27, f1 (x12)\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f3, f1 (x15)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f3, f1 (x12)\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f20, f1 (x12)\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f20, f1 (x13)\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f1, f1 (x15)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f6, f1 (x13)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f1, f1 (x15)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f7, f1 (x12)\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f14, f1 (x11)\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f20, f1 (x11)\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f6, f1 (x11)\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x12)\n"                            ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f11, f1 (x11)\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f5, f1 (x11)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f28, f1 (x14)\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f0, f1 (x15)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f16, f1 (x11)\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f13, f1 (x13)\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f23, f1 (x12)\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f27, f1 (x15)\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x14)\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f6, f1 (x11)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x13)\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f5, f1 (x13)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f10, f1 (x11)\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f26, f1 (x12)\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f16, f1 (x13)\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f6, f1 (x12)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f30, f1 (x15)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f24, f1 (x13)\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f10, f1 (x14)\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f3, f1 (x11)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x11)\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f28, f1 (x15)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f2, f1 (x12)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f15, f1 (x14)\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f19, f1 (x14)\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f4, f1 (x13)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f0, f1 (x13)\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f30, f1 (x12)\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f25, f1 (x13)\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f25, f1 (x15)\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f15, f1 (x15)\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f16, f1 (x15)\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x14)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f22, f1 (x14)\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f18, f1 (x13)\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f19, f1 (x14)\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f26, f1 (x15)\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f19, f1 (x15)\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f10, f1 (x12)\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f24, f1 (x13)\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f0, f1 (x13)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f7, f1 (x14)\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f22, f1 (x14)\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f25, f1 (x11)\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f25, f1 (x13)\n"                            ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f30, f1 (x15)\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f14, f1 (x13)\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f27, f1 (x15)\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f22, f1 (x11)\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f18, f1 (x11)\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f17, f1 (x13)\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f26, f1 (x15)\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f10, f1 (x11)\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f16, f1 (x13)\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f8, f1 (x14)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f26, f1 (x11)\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f27, f1 (x15)\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f29, f1 (x11)\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f19, f1 (x12)\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f23, f1 (x13)\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f11, f1 (x14)\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f17, f1 (x14)\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f12, f1 (x11)\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f0, f1 (x12)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f28, f1 (x13)\n"                            ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f16, f1 (x12)\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f8, f1 (x13)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f17, f1 (x12)\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f24, f1 (x12)\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f18, f1 (x11)\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f18, f1 (x11)\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f8, f1 (x11)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschg.ps f15, f1 (x11)\n"                            ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
